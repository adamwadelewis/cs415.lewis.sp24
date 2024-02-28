/*
 * File:   shm-client.c
 * Author: Adam.Lewis@athens.edu
 * Purpose:
 * Client-side portion of the demo code for Lab 05.
 */
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>

/*
 * Recall that we set the size of the buffer to this value in the
 * sever side of the lab.
 */
#define SHMSZ     27

/*
 * int main(int, char *[])
 * Here's where we do all of the work.
 */
int main(int argc, char *argv[])
{
  /*
   * Step 1:  Define an intger shmid, and key_t named key.
   */
  int shmid;
  key_t key;
  /*
   * Step 2:  Define two character points shm and s that we will
   *          use as buffers in our program.
   */
  char *shm, *s;

  /*
   * Step 3: We need to get the segment named "5678", created by the server.
   */
  key = 5678;

  /*
   * Step 4: Use shmget() to locate the segment.
   */
  if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
    perror("shmget");
    return(1);
  }

  /*
   * Step 5: Now we attach the segment to our data space using shmat()
   */
  if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
    perror("shmat");
    return(1);
  }

  /*
   * Step 6: Now read what the server put in the memory.
   */
  for (s = shm; *s != NULL; s++)
    putchar(*s);
  putchar('\n');

  /*
   * Step 7: Finally, change the first character of the segment to '*',
   * indicating we have read the segment.
   */
  *shm = '*';

  return(0);
}
