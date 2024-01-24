//
// File:     fork_ex.cpp
// Author:   Adam.Lewis@athens.edu
// Purpose:
// Demo code for CS415 Lecture 02 showing fork and exec
//
#include <cstdlib>
#include <iostream>
#include <string>
// Required by fork routine
#include <sys/types.h>
#include <unistd.h>
using namespace std;
int globalVariable = 2;
int main(int argc, char **argv)
{
   string sIdentifier;
   int    iStackVariable = 20;

   pid_t pID = fork();
   if (pID == 0)                // child
   {
      // Code only executed by child process

      sIdentifier = "Child Process: ";
      globalVariable++;
      iStackVariable++;
    }
    else if (pID < 0)            // failed to fork
    {
        cerr << "Failed to fork" << endl;
        exit(1);
        // Throw exception
    }
    else                                   // parent
    {
      // Code only executed by parent process

      sIdentifier = "Parent Process:";
    }

    // Code executed by both parent and child.

    cout << sIdentifier;
    cout << " Global variable: " << globalVariable;
    cout << " Stack variable: "  << iStackVariable << endl;
}
