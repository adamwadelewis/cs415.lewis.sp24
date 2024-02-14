// 
// File:   deadlock.cpp
// Author: Your Glorious Instructor
// Purpose:
// Think about the causes of a deadlock.
// Based upon: https://www.bogotobogo.com/cplusplus/C11/9_C11_DeadLock.php
//
#include <iostream>
#include <mutex>
#include <thread>
#include <mutex>

using namespace std;
const int SIZE = 10;

mutex myMutex, myMutex1, myMutex2;

void shared_cout_thread_even(int i)
{
    lock_guard<mutex> g1(myMutex1);
    lock_guard<mutex> g2(myMutex2);
    cout << " " << i << " ";
}

void shared_cout_thread_odd(int i)
{
    lock_guard<mutex> g2(myMutex2);
    lock_guard<mutex> g1(myMutex1);
    cout << " " << i << " ";
}

void shared_cout_main(int i)
{
    lock_guard<mutex> g(myMutex);
    cout << " " << i << " ";
}

void f(int n)
{
    for(int i = SIZE*(n-1); i < SIZE*n ; i++) {
        if(n % 2 == 0)
            shared_cout_thread_even(i);
        else
            shared_cout_thread_odd(i);
    }
}

int main()
{
    thread t1(f, 1);  // 0-9
    thread t2(f, 2);  // 10-19
    thread t3(f, 3);  // 20-29
    thread t4(f, 4);  // 30-39
    thread t5(f, 5);  // 40-49

    for(int i = 0; i > -SIZE; i--)
        shared_cout_main(i);  // (0, -49)

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    return 0;
}