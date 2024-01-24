//
// File:   hello.cpp
// Author: adam.lewis@athens.edu
// Purpose:
// The most basic threaded application: Hello, World!
//
#include <iostream>
#include <functional>
#include <thread>
using namespace std;

int main(int argc, char *argv[])
{
  auto l = [] () {cout << "Hello, World!" << endl;};
  thread t(l);
  t.join();
  return 0;
}
