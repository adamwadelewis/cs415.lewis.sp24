//
// File:   fnpvslamc.cpp
// Author: Adam.Lewis@athens.edu
// Purpose:
// Compare the use of a function pointer, function object (functor), and lambda
// expression to define thread functions in C++
//
#include<iostream>
#include<thread>
#include<vector>

// Let's be lazy and not have to keep typing std:: all the time
using namespace std;
// void foo(int limit)
//
// Sample function to demonstrate function pointers as way to denote thread
// function.
void foo(int limit)
{
  thread::id thisThreadId = this_thread::get_id();
    cout << "Thread "<< thisThreadId
         << "was started via fcn pointer"
         << endl;
  for (int i = 0; i < limit; i++) {
    cout << "Thread "<< thisThreadId << "doing work... funny, funny, function"
         << endl;
  }
}
// class FooFunctor
//
// Now we do the same thing by overriding the function call operator in a class
//
class FooFunctor {
public:
  void operator()(int limit)
    {
      thread::id thisThreadId = this_thread::get_id();
        cout << "Thread "<< thisThreadId << "was started via functor"
             << endl;
      for (int i = 0; i < limit; i++) {
        cout << "Thread "<< thisThreadId << "doing work... dooby, dobby, do"
             << endl;
      }
    }
};
//
// int main()
//
// And we do the fun stuff here
int main()
{
  int limit = 4;
  // First, using the function pointer
  thread t1(foo,limit);
  // And then the function object
  thread t2(FooFunctor(), limit);
  // Let's use a lambda expression, or what is also called an anonymous
  // function. Note here how we are using functions as a a data type.  
  auto foolamb = [](int limit) {
                   thread::id thisThreadId = this_thread::get_id();
                   cout << "Thread "<< thisThreadId
                        << "was started using an anonymous function"
                        << endl;
                   for (int i = 0; i < limit; i++) {
                     cout << "Thread "<< thisThreadId
                          << "doing work... la, la, lambda! Olay!"
                          << endl;
                   }
                 };

  thread t3(foolamb, limit);
  // And now we must wait in turn for each of the threads to finish.  Do this
  // by doing a join
  t1.join();
  t2.join();
  t3.join();
  return 0;
}
