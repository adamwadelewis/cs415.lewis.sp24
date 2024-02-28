//
// File:    sch_server.c
// Author:  Adam.Lewis@athens.edu
// Purpose:
// Server component of the shared memory demo in Lab 05
//
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <unistd.h>

// The size of data segment
#define SHMSZ     27

//
// int main(int, char *[])
// The bulk of the work occurs here.
int main(int argc, char *argv[])
{
  /*
   * Step 1: Define three variables: a character c,  an integer shmid,
   *         and key_t key
   */
  char c;
  int shmid;
  key_t key;
  /*
   * Step 2: Define two character pointers we will use as buffers
   *         for shared memory segements.
   */
  char *shm, *s;

  /*
   * Step 3:  Set the value of the key to 5678.  We will use this as the
   *          identifer for the shared memory segment.
   */
  key = 5678;

  /*
   * Step 4:  Now use shmget() to create the segment.
   */
  if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
    perror("shmget");
    return(1);
  }
  /*
   * Step 5:  Now we attach the segment to our data space using the shmat()
   *          function.
   */
  if ((shm = shmat(shmid, NULL, 0)) == (char *) -1) {
    perror("shmat");
    return(1);
  }

  /*
   * Step 6: Now put some things into the memory for the 
   *         other process to read.
   */
  s = shm;

  for (c = 'a'; c <= 'z'; c++)
    *s++ = c;
  *s = '\0';

  /*
   * Finally, we wait until the other process changes the first character of
   * our memory to '*', indicating that it has read what we put there.
   */
  while (*shm != '*')
    sleep(1);

  return(0);
}
