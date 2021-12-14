/*
 * test_cbfifo.c - test the cbfifo implementation
 * 
 * Author: Howdy Pierce, howdy.pierce@colorado.edu
 * 
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "test_cbfifo.h"
#include "cbfifo.h"
#include "UART.h"


cbfifo_t Q;

void test_cbfifo()
{
  char *str ="To be, or not to be: that is the question:\n"
    "Whether 'tis nobler in the mind to suffer\n"
    "The slings and arrows of outrageous fortune,\n"
    "Or to take arms against a sea of troubles,\n"
    "And by opposing end them? To die, to sleep--\n"
    "No more--and by a sleep to say we end\n"
    "The heart-ache and the thousand natural shocks\n"
    "That flesh is heir to, 'tis a consummation\n"
    "Devoutly to be wish'd. To die, to sleep;\n"
    "To sleep: perchance to dream: ay, there's the rub;\n"
    "For in that sleep of death what dreams may come\n"
    "When we have shuffled off this mortal coil,\n"
    "Must give us pause.";

  char buf[1024];
  const int cap = cbfifo_capacity(&Q);

  // asserts in following 2 lines -- this is not testing the student,
  // it's validating that the test is correct
  assert(strlen(str) >= cap*2);
  assert(sizeof(buf) > cap);
  assert(cap == 256 || cap == 255);

  cbfifo_init(&Q);

  assert(cbfifo_length(&Q) == 0);
  assert(cbfifo_dequeue(&Q,buf, cap) == 0);
  assert(cbfifo_dequeue(&Q,buf, 1) == 0);

  // enqueue 10 bytes, then dequeue same amt 
  assert(cbfifo_enqueue(&Q,str, 10) == 10);
  assert(cbfifo_length(&Q) == 10);
  assert(cbfifo_dequeue(&Q,buf, 10) == 10);
  assert(strncmp(buf, str, 10) == 0);
  assert(cbfifo_length(&Q) == 0);

  // enqueue 20 bytes;  dequeue 5, then another 20
  assert(cbfifo_enqueue(&Q,str, 20) == 20);
  assert(cbfifo_length(&Q) == 20);
  assert(cbfifo_dequeue(&Q,buf, 5) == 5);
  assert(cbfifo_length(&Q) == 15);
  assert(cbfifo_dequeue(&Q,buf+5, 20) == 15);
  assert(cbfifo_length(&Q) == 0);
  assert(strncmp(buf, str, 20) == 0);

  // fill buffer and then read it back out
  assert(cbfifo_enqueue(&Q,str, cap) == cap);
  assert(cbfifo_length(&Q) == cap);
  assert(cbfifo_enqueue(&Q,str, 1) == 0);
  assert(cbfifo_dequeue(&Q,buf, cap) == cap);
  assert(cbfifo_length(&Q) == 0);
  assert(strncmp(buf, str, cap) == 0);

  // Add 20 bytes and pull out 18
  assert(cbfifo_enqueue(&Q,str, 20) == 20);
  assert(cbfifo_length(&Q) == 20);
  assert(cbfifo_dequeue(&Q,buf, 18) == 18);
  assert(cbfifo_length(&Q) == 2);
  assert(strncmp(buf, str, 18) == 0);

  // Now add a bunch of data in 4 chunks
  int chunk_size = (cap-2) / 4;
  for (int i=0; i<4; i++) {
    assert(cbfifo_enqueue(&Q,str+i*chunk_size, chunk_size) == chunk_size);
    assert(cbfifo_length(&Q) == (i+1)*chunk_size + 2);
  }
  assert(cbfifo_length(&Q) == 4*chunk_size + 2);

  // Take out the 2 remaining bytes from above
  assert(cbfifo_dequeue(&Q,buf, 2) == 2);
  assert(strncmp(buf, str+18, 2) == 0);

  // now read those chunks out a byte at a time
  for (int i=0; i<chunk_size*4; i++) {
    assert(cbfifo_dequeue(&Q,buf+i, 1) == 1);
    assert(cbfifo_length(&Q) == chunk_size*4 - i - 1);
  }
  assert(strncmp(buf, str, chunk_size*4) == 0);
  
  // write more than capacity
  assert(cbfifo_enqueue(&Q,str, 65) == 65);
  assert(cbfifo_enqueue(&Q,str+65, cap) == cap-65);
  assert(cbfifo_length(&Q) == cap);
  assert(cbfifo_dequeue(&Q,buf, cap) == cap);
  assert(cbfifo_length(&Q) == 0);
  assert(strncmp(buf, str, cap) == 0);

  // write zero bytes
  assert(cbfifo_enqueue(&Q,str, 0) == 0);
  assert(cbfifo_length(&Q) == 0);

  // Exercise the following conditions:
  //    enqueue when read < write:
  //        bytes < CAP-write  (1)
  //        bytes exactly CAP-write  (2)
  //        bytes > CAP-write but < space available (3)
  //        bytes exactly the space available (4)
  //        bytes > space available (5)
  assert(cbfifo_enqueue(&Q,str, 32) == 32);  // advance so that read < write
  assert(cbfifo_length(&Q) == 32);
  assert(cbfifo_dequeue(&Q,buf, 16) == 16);
  assert(cbfifo_length(&Q) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(&Q,str+32, 32) == 32);  // (1)
  assert(cbfifo_length(&Q) == 48);
  assert(cbfifo_enqueue(&Q,str+64, cap-64) == cap-64);  // (2)
  assert(cbfifo_length(&Q) == cap-16);
  assert(cbfifo_dequeue(&Q,buf+16, cap-16) == cap-16);
  assert(strncmp(buf, str, cap) == 0);
  
  assert(cbfifo_enqueue(&Q,str, 32) == 32);  // advance so that read < write
  assert(cbfifo_length(&Q) == 32);
  assert(cbfifo_dequeue(&Q,buf, 16) == 16);
  assert(cbfifo_length(&Q) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(&Q,str+32, cap-20) == cap-20);  // (3)
  assert(cbfifo_length(&Q) == cap-4);
  assert(cbfifo_dequeue(&Q,buf, cap-8) == cap-8);
  assert(strncmp(buf, str+16, cap-8) == 0);
  assert(cbfifo_length(&Q) == 4);
  assert(cbfifo_dequeue(&Q,buf, 8) == 4);
  assert(strncmp(buf, str+16+cap-8, 4) == 0);
  assert(cbfifo_length(&Q) == 0);

  assert(cbfifo_enqueue(&Q,str, 49) == 49);  // advance so that read < write
  assert(cbfifo_length(&Q) == 49);
  assert(cbfifo_dequeue(&Q,buf, 16) == 16);
  assert(cbfifo_length(&Q) == 33);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(&Q,str+49, cap-33) == cap-33);  // (4)
  assert(cbfifo_length(&Q) == cap);
  assert(cbfifo_dequeue(&Q,buf, cap) == cap);
  assert(cbfifo_length(&Q) == 0);
  assert(strncmp(buf, str+16, cap) == 0);

  assert(cbfifo_enqueue(&Q,str, 32) == 32);  // advance so that read < write
  assert(cbfifo_length(&Q) == 32);
  assert(cbfifo_dequeue(&Q,buf, 16) == 16);
  assert(cbfifo_length(&Q) == 16);
  assert(strncmp(buf, str, 16) == 0);

  assert(cbfifo_enqueue(&Q,str+32, cap) == cap-16);  // (5)
  assert(cbfifo_dequeue(&Q,buf, 1) == 1);
  assert(cbfifo_length(&Q) == cap-1);
  assert(cbfifo_dequeue(&Q,buf+1, cap-1) == cap-1);
  assert(cbfifo_length(&Q) == 0);
  assert(strncmp(buf, str+16, cap) == 0);

  //    enqueue when write < read:
  //        bytes < read-write (6)
  //        bytes exactly read-write (= the space available) (7)
  //        bytes > space available (8)

  int wpos=0, rpos=0;
  assert(cbfifo_enqueue(&Q,str, cap-4) == cap-4);
  wpos += cap-4;
  assert(cbfifo_length(&Q) == cap-4);
  assert(cbfifo_dequeue(&Q,buf, 32) == 32);
  rpos += 32;
  assert(cbfifo_length(&Q) == cap-36);
  assert(strncmp(buf, str, 32) == 0);
  assert(cbfifo_enqueue(&Q,str+wpos, 12) == 12);
  wpos += 12;
  assert(cbfifo_length(&Q) == cap-24);

  assert(cbfifo_enqueue(&Q,str+wpos, 16) == 16);  // (6)
  assert(cbfifo_length(&Q) == cap-8);
  assert(cbfifo_dequeue(&Q,buf, cap) == cap-8);
  assert(cbfifo_length(&Q) == 0);
  assert(strncmp(buf, str+rpos, cap-8) == 0);

  // reset
  wpos=0;
  rpos=0;
  assert(cbfifo_enqueue(&Q,str, cap-4) == cap-4);
  wpos += cap-4;
  assert(cbfifo_length(&Q) == cap-4);
  assert(cbfifo_dequeue(&Q,buf, 32) == 32);
  rpos += 32;
  assert(cbfifo_length(&Q) == cap-36);
  assert(strncmp(buf, str, 32) == 0);
  assert(cbfifo_enqueue(&Q,str+wpos, 12) == 12);
  wpos += 12;
  assert(cbfifo_length(&Q) == cap-24);

  assert(cbfifo_enqueue(&Q,str+wpos, 24) == 24);  // (7)
  assert(cbfifo_length(&Q) == cap);
  assert(cbfifo_dequeue(&Q,buf, cap) == cap);
  assert(cbfifo_length(&Q) == 0);
  assert(strncmp(buf, str+rpos, cap) == 0);

  // reset
  wpos=0;
  rpos=0;
  assert(cbfifo_enqueue(&Q,str, cap-4) == cap-4);
  wpos += cap-4;
  assert(cbfifo_length(&Q) == cap-4);
  assert(cbfifo_dequeue(&Q,buf, 32) == 32);
  rpos += 32;
  assert(cbfifo_length(&Q) == cap-36);
  assert(strncmp(buf, str, 32) == 0);
  assert(cbfifo_enqueue(&Q,str+wpos, 12) == 12);
  wpos += 12;
  assert(cbfifo_length(&Q) == cap-24);

  assert(cbfifo_enqueue(&Q,str+wpos, 64) == 24);  // (8)
  assert(cbfifo_length(&Q) == cap);
  assert(cbfifo_dequeue(&Q,buf, cap) == cap);
  assert(cbfifo_length(&Q) == 0);
  assert(strncmp(buf, str+rpos, cap) == 0);

  printf("%s: \n\rpassed all test cases\n\r", __FUNCTION__);
}

