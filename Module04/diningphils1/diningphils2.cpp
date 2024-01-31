//
// File:   phil.cpp
// Author: Adam.Lewis@athens.edu
//
// Purpose:
// Illustrate how to solve the dining philosophers problem using
// the C++ thread library
//
// Reference:
// https://mariusbancila.ro/blog/2017/01/16/dining-philosophers-in-cpp11/
#include <array>
#include <mutex>
#include <thread>
#include <atomic>
#include <chrono>
#include <iostream>
#include <string>
#include <random>
#include <iomanip>

// Need a mutex to protect the printing process
// as the standard cout code is not thread-save
std::mutex g_lockprint;

constexpr  int no_of_philosophers = 5;

//
// The fork structure represents a fork used in the process.
// This structure contains only a mutex that is locked when
// one of your glorious instructres picks up a fork and unlocked
// whenever said instructor is finished eating.
//
struct fork
{
  std::mutex mutex;
};

//
// This is class that represents the circular dining table. It has an
// array of forks but aslo an atomic boolean value that indicates when
// the table is open.
//
struct table
{
  std::atomic<bool>                    ready{ false };
  std::array<fork, no_of_philosophers> forks;
};

//
// Here's the class that is the model of your Glorious Instructors in
// all of our lovely EVIIIILLLLL.  Each philosophizing professor has a
// name and references to the forks on their left and right.
//
// The hard work goes on here.  Upon instantiation of a philosopher
// object, a new thread is started and the thread is joined upon
// destruction of the object.   The thread function runs a loop of
// thinking and eating until the table's ready variable is signaled by
// setting it's value to false.
//
struct philosopher
{
private:
  std::string const name;
  table const &     dinnertable;
  fork&             left_fork;
  fork&             right_fork;
  std::thread       lifethread;
  std::mt19937      rng{ std::random_device{}() };
public:
  philosopher(std::string n, table const & t, fork & l, fork & r) :
    name(n),
    dinnertable(t),
    left_fork(l),
    right_fork(r),
    lifethread(&philosopher::dine, this)
  {
  }

  //
  // Note how the thread is joined on destruction of a philosopher
  // object
  ~philosopher()
  {
    lifethread.join();
  }

  //
  // In this method, the philosopher waits by busy looping under the
  // table signals it's ready.  At which point the philosopher dines
  // until the table signals it's closed by setting its ready flag to
  // false.
  //
  void dine()
  {
    while (!dinnertable.ready);
    do
      {
        think();
        eat();
      } while (dinnertable.ready);
  }

  //
  // Don't forget that output isn't thread safe, so a mutex in a
  // lock_guard to prevent more than one thread from printing
  //
  void print(std::string text)
  {
    std::lock_guard<std::mutex> cout_lock(g_lockprint);
    std::cout
      << std::left << std::setw(10) << std::setfill(' ')
      << name << text << std::endl;
  }

  //
  // We're simulating the eating process by acquring the locks on the
  // left and right forks by using the standard lock and then adopting
  // the locks into a lock_guard.  Then we eat by waiting for a bit.
  //
  // Note how RAII means we corectly release the locks upon exit from
  // the function.
  //
  void eat()
  {
    std::lock(left_fork.mutex, right_fork.mutex);
 
    std::lock_guard<std::mutex> left_lock(left_fork.mutex,   std::adopt_lock);
    std::lock_guard<std::mutex> right_lock(right_fork.mutex, std::adopt_lock);
 
    print(" started eating.");
 
    static thread_local std::uniform_int_distribution<> dist(1, 6);
    std::this_thread::sleep_for(std::chrono::milliseconds(dist(rng) * 50));
 
    print(" finished eating.");
  }

  //
  // To think, we just wait for a bit.
  //
  void think()
  {
    static thread_local std::uniform_int_distribution<> wait(1, 6);
    std::this_thread::sleep_for(std::chrono::milliseconds(wait(rng) * 150));
 
    print(" is thinking ");
  }
};

//
// And we drive the simulation using the dine() function.  We setup a
// STL array philosopher objects.   Turn the table on, let things run
// for a bit, and then turn the table off.   So... set things ready and
// then y'all better look as it's time to eat!
//
void dine()
{
  std::this_thread::sleep_for(std::chrono::seconds(1));
  std::cout << "Dinner started!" << std::endl;
 
  {
    table table;
    std::array<philosopher, no_of_philosophers> philosophers
      {
       {
        { "Lewis", table, table.forks[0], table.forks[1] },
        { "Chen",    table, table.forks[1], table.forks[2] },
        { "Fitzsimmons", table, table.forks[2], table.forks[3] },
        { "Fadlalla",      table, table.forks[3], table.forks[4] },
        { "Maxwell", table, table.forks[4], table.forks[0] },
       }
      };
 
    table.ready = true;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    table.ready = false;
  }
 
  std::cout << "Dinner done!" << std::endl;
}

//
// Move along, not a lot to see here.
//
int main()
{   
  dine();
  return 0;
}

