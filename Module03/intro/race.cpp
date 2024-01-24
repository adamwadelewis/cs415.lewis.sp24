//
// File:   race.cpp
// Author: Adam.Lewis@athens.edu
// Purpose:
// Illustrate race conditions.
//
#include <iostream>
#include <vector>
#include <thread>
using namespace std;
// So, we want to compute the sum of all squares up and including 20.  We
// iterate up to 20 and assign a thread to do the computation.  Following which
// we pull it all together.
//
// We keep the accumulator as a global.   This will be a problem.
// 
int accum = 0;
//
// void square(int x)
//
// So here's the function where we do the square.  This will be thread
// function.

void square(int x) {
    accum += x * x;
}

//
// int main()
//
// Note that we need make certain all threads have rejoined before we print the
// accumulator to make certain we get good numbers.
//
// Try running this function in a shell with the command (asuming the exeutable
// is named "race":
// for i in {1..1000}; do ./race; done | sort | uniq -c
//
// Notice how we get inconsisent answers?!  The problem is that accumulator is
// global and we have multiple threads trying to hit that variable over time.
// The result is what we call a "race condition".  We're going to have to
// figure out how to address this situation.
//
int main() {
    vector<thread> ths;
    for (int i = 1; i <= 20; i++) {
        ths.push_back(thread(&square, i));
    }

    for (auto& th : ths) {
        th.join();
    }
    cout << "accum = " << accum << endl;
    return 0;
}
