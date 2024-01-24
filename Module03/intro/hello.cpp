//
// File:   hello.cpp
// Author: adam.lewis@athens.edu
// Purpose:
// The most basic threaded application: Hello, World!
//
#include <iostream>
#include <thread>
using namespace std;
void hello()
{
  cout << "Hello, World!" << endl;
}

int main(int argc, char *argv[])
{
  thread t(hello);
  t.join();
  return 0;
}
