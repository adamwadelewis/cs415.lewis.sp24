//
// File:    wait_ex.cpp
// Author:  Adam.Lewis@athens.edu
// Purpose:
// 	Illustrate use of wait() system call
//
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>
using namespace std;
int main(){
  pid_t pid;
  int status, died;
     switch(pid=fork())
     {
     case -1:
       cout << "can't fork\n";
       exit(-1); // why is there no break here?
     case 0 :
       sleep(2); // this is the code the child runs
       exit(3);  // why is there no break here?
     default:
       died= wait(&status); // this is the code the parent runs 
     }
}

