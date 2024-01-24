//
// File:   getpidex.cpp
// Author: Your Glorious Instructor
// Purpose:
// Illustrate use of getpid() and getppid() system calls
//

#include <cstdlib>
#include <unistd.h>
#include <iostream>
using namespace std;
int main() {
  pid_t pid = getpid();
  pid_t ppid = getppid();

  cout << "   My PID is " <<  pid << endl;
  cout << "   I'm the child of PID " << ppid << ".\n";
  return 0;
}
