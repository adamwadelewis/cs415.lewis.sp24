//
// File:    diningphil.cpp
// Author:  Adam.Lewis@athens.edu
// Purpose:
// Dining Philospher's simulation.
//
// This code is based upon the code found in the following article
// from GitHub:
// https://howardhinnant.github.io/dining_philosophers.html
//
// This code represents the chopsticks in the problem as a vector of mutexes.
// The class Philosopher represents a single philosopher and will reference two
// of the mutexes in the table.   Then the set of philosophers that is dining
// represented as a vector of philsophers.
//
// The DP problem shows really interesting behavior when the the philosophers
// are really hungry: they tend to eat more and think less.  This means that
// they spend more time contending for forks.   This implementation is
// attempting to model this situation where you have a more real-world
// high-contention use case.
//
// Therefore in the tests included herein:
//
// 1. The diners do not think at all. They only wish to eat.
// 2. The diners can only eat for at most 10 milliseconds at a time,
//    before they have to relinquish their forks while they chew.
//    This is sometimes as little as 1 millisecond. The exact duration is
//    chosen randomly each time a diner obtains the forks.
// 3. The diner randomly chooses what order to request the forks
//    in. I.e. what order the forks are given to the ::lock() algorithm. The
//    actual order the forks are obtained is encapsulated within the ::lock()
//    algorithm.
// 4. The meal is considered complete when each diner has eaten for
//    a cumulative total of 30 seconds.  5. The efficiency of the algorithm is
//    measured by measuring the wall clock time elapsed from the beginning of
//    the meal to the end of the meal (shorter is better).
//
#include <chrono>
#include <mutex>
#include <random>
#include <array>
#include <vector>
#include <thread>
#include <iostream>

using namespace std;

//
// This version of lock() implements the ordered solution orginally proposed by
// Edsger Dijkstra.  You assign an order to the chopsticks (represented in this
// code by their position in the vector) and have each Philosopher lock
// lower-numbered mutex, then lock the higher-numberd mutex.
//
template <class L0>
void lock (L0& l0, L0& l1)
{
  if (l0.mutex() < l1.mutex())
  {
    unique_lock<L0> u0(l0);
    l1.lock();
    u0.release();
  }
  else
  {
    unique_lock<L0> u1(l1);
    l0.lock();
    u1.release();
  }
}

//
// A class to represent a single Philosopher.
//
class Philosopher {
  mt19937_64 eng{random_device{}()};
  mutex& leftFork;
  mutex& rightFork;
  chrono::milliseconds eatTime{0};
  static constexpr chrono::seconds full{30};
public:
  Philosopher(mutex& left, mutex& right);
  void dine();
private:
  void eat();
  bool flipCoin();
  chrono::milliseconds getEatDuration();
};

//
// Global variable to keep track of how long it
// takes for a philosopher to eat their fill.
constexpr chrono::seconds Philosopher::full;

//
// Philosopher::Philosopher(mutex&, mutex&)
// Create a new philosopher, with mutexes for the
// left and right chopsticks.
//
Philosopher::Philosopher(mutex& left, mutex& right)
  : leftFork(left), rightFork(right)
  {}

//
// void Philosopher::dine()
// Thread function for a philosopher.  Keep eating
// until you are full.
void Philosopher::dine()
{
  while (eatTime < full)
    eat();
}

//
// void Philopsopher::eat()
// Implement the eat operation, with the call to Lock()
// representing the pickup of a chopstick.   Randomly
// decide whether you are going to decide to first pick up
// the left chopstick or the right chopstick.
//
void Philosopher::eat()
{
  using Lock = unique_lock<mutex>;
  Lock first;
  Lock second;
  if (flipCoin())
  {
    first = Lock(leftFork, defer_lock);
    second = Lock(rightFork, defer_lock);
  }
  else
  {
    first = Lock(rightFork, defer_lock);
    second = Lock(leftFork, defer_lock);
  }
  auto d = getEatDuration();
  ::lock(first,second);
  auto end = chrono::steady_clock::now() + d;
  while (chrono::steady_clock::now() < end)
    ;
  eatTime += d;
}

//
// bool Philosopher::flipCoin()
// Simulate the flipping of a coin using the Bernoulli
// distribution.
//
bool Philosopher::flipCoin()
{
  bernoulli_distribution d;
  return d(eng);
}

//
// chono::miliseconds Philosopher::getEatDuration()
// Randomly geneate the amount time required for a philosoper
// to eat.
//
chrono::milliseconds Philosopher::getEatDuration()
{
  uniform_int_distribution<> ms(1,10);
  return min(chrono::milliseconds(ms(eng)), full - eatTime);
}

//
// int main():
// Do multiple runs of the Dining Philosopher's problem for 2 to
// 8 philosophers, keeping track of the amount of the elasped time
//
int main()
{
  cout << "Dining Philosophers Simulation" << endl;
  for (unsigned nt = 2; nt <= 4; ++nt)
  {
    vector<mutex> table(nt);
    vector<Philosopher> diners;
    for (unsigned i = 0; i < table.size(); ++i)
    {
      int j = i;
      int k = 0;
      if (j < (table.size() - 1))
      {
        k = j+1;
      }
      diners.push_back(Philosopher(table[j], table[k]));
    }
    vector<thread> threads(diners.size());
    unsigned i = 0;
    auto timeStart = chrono::high_resolution_clock::now();
    cout << "nt = " << nt << " :  start" << endl;
    for (auto& t : threads)
    {
      t = thread(&Philosopher::dine, diners[i]);
      ++i;
    }
    for (auto& t : threads)
      t.join();
    auto timeEnd = chrono::high_resolution_clock::now();
    cout << "nt = " << nt << " : end" << endl;
    using secs = chrono::duration<float>;
    cout << "nt = " << nt << " : " << secs(timeEnd - timeStart).count() << endl;
  }
  return 0;
}


