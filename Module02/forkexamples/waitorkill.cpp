//
// File:     waitorkill.cpp
// Author:   Adam.Lewis@athens.edu
// Purpose:
//
// Demo code for CS415 Lecture 2 illustrating wait() and kill()
//
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
using namespace std;

int main(){
   pid_t pid;
   int status, died;
   switch(pid=fork()){
   case -1: cout << "can't fork\n";
            exit(-1);
   case 0 : cout << "   I'm the child of PID " << getppid() << ".\n";
            cout << "   My PID is " <<  getpid() << endl;
	    sleep(2);
            exit(3);
   default: cout << "I'm the parent.\n";
            cout << "My PID is " <<  getpid() << endl;
            // kill the child in 50% of runs
            if (pid & 1)
               kill(pid,SIGKILL);
            died= wait(&status);
            if(WIFEXITED(status))
               cout << "The child, pid=" << pid << ", has returned " 
                    << WEXITSTATUS(status) << endl;
            else
 	       cout << "The child process was sent a " 
                    << WTERMSIG(status) << " signal\n";
  }
}
