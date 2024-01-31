/*
 * File:     devsim.cpp
 * Author:   Adam.Lewis@athens.edu
 *
 * This program is a solution to the CS415 device simulation threading problem.
 * This code serves multiple purposes: (1) provide a more extensive example of
 * using threads, (2) demo how to use to C++ STL random classes for generating
 * random numbers, and (3) provide more practice with the STL container and
 * iterator classes.
 *
 */

#include <iostream>
#include <fstream>
#include <random>
#include <thread>
#include <mutex>
#include <string>
#include <sstream>
#include <deque>
#include <chrono>

// I am being lazy here... the common preference is to not do a using
// statement. 
using namespace std;

// We will be using this deque as a shared global between two threads: a thread
// that simulates the reading of a set of devices and the main thread that is
// extracting values from the queue and printing them.
deque<string> inputQueue;

// Two mutexes to protect the queue, one that protects reading an entry
// form the queue and the other to protect writing to the queue.
mutex readMTX;
mutex writeMTX;

//
// void simulateInput(int rate)
//
// This function simulates a set of devices generating input to the
// program. Here's where you get to use some of those things that we learn from
// our Prob. and Stats. class.   The devices generate new input at a predefined
// rate.  For example, we expect input at a rate of 3 samples/second.
//
// Data from the devices is simulated using a normal distribution with a mean
// of 5.0 and standard deviation of 3.0.   We simulate device id by randomly
// selecting a uniformly distributed integer (this is what we
// commonly think of when we do random numbers in a program) between 0 and 10.
//
// We build the data for the queue using a string stream.  The randomly
// generated data is written to the stream and we then push the backing string
// onto the queue.
//

void simulateInput(int rate) 
{
  random_device rd{};
  mt19937 gen{rd()};
  int count = 0;

  poisson_distribution<> pD(rate);
  normal_distribution<> nD(5.0, 3.0);
  uniform_int_distribution<int> uD(0,10);

  while(true)
  {
    int numberEvents = pD(gen);
    if (numberEvents > 0)
    {
      for (int i = 0; i < numberEvents; ++i)
      {
        stringstream sampleStream;
        int deviceNumber = uD(gen);
        double sample = nD(gen);
        count++;
        // Put the lock guard with a block so that it will
        // release when the guard is destroyed on completion of the
        // the block. Mutex unlocks when writeLock goes out of scope.
        {
          std::lock_guard<std::mutex> writeLock(writeMTX);
          sampleStream << count << " " << deviceNumber << " " << sample;
          //cout << count << " " << deviceNumber <<  " " << sample << endl;
          inputQueue.push_front(sampleStream.str());
        }
      }
    }
    this_thread::sleep_for(chrono::milliseconds(10));
  }
}

// void processData(int devNum)
//
// This function checks to see if no is writing to the queue, and if so locks
// the queue for writing, and then looks to see if needs to lock the queue for
// reading.  Once it has exclusive access to the queue,  it checks to see if
// the tail of queue contains input for it to process.   If not, then it then
// unlocks both mutexes.   If it does, it pops the queue, processes the data,
// and then unlocks the queues.

void processData(int devNum)
{
  fstream dataStream;
  bool keepProcessing = true;
  string fileName = "device" + to_string(devNum);
  int count;
  int device;
  float value;
  dataStream.open(fileName);
  while(keepProcessing)
  {
    if (! inputQueue.empty())
    {
      // Use a lock_guard object to lock the queue.  Things are a bit tricky
      // here as we have to loock the write mutex to keep the writer out of the
      // critical section and the read mutex to keep the readers out of things.
      // This means we need to use a lock, and adopt it to make certain we
      // do something to avoid get us into a deadlock.
      //
      // Note that this happens enough that C++17 and later supports a scoped
      // lock which you can intiialize with both mutexes:
      // std::scoped_lock bothLocks{readMTX, writeMTX}
      // This does the three steps below as part of the scoped_lock class.
      std::lock(readMTX,writeMTX);
      std::lock_guard<std::mutex> writeLock(writeMTX, std::adopt_lock);
      std::lock_guard<std::mutex> readLock(readMTX, std::adopt_lock);
      string sample = inputQueue.back();
      stringstream sampleStringStream(sample);
      sampleStringStream >> count >> device >> value;
      if (device != 999) {
        cout << "Thread " << devNum << " sees sample " << count;
        cout << " for device " << device << " with value " << value << endl;
        if (devNum == device) {
          inputQueue.pop_back();
          dataStream << sample << endl;
        }
      }
      else {
        cout << "Device " << devNum << "has seen exit message" << endl;
        keepProcessing = false;
      }
    }
  }
  // Close file
  dataStream.close();
}

//
// void fiveMinuteTimer()
//
// At the end of five minutes, lock the reader and writer mutexes and write
// an EXIT message to the queue.
//
void fiveMinuteTimer()
{
  bool keepgoing = true;
  int counter = 0;
  cout << "Five Minute Alarm: " << counter << "minutes have passed." << endl;
  while (keepgoing) {
    this_thread::sleep_for(chrono::seconds(1));
    ++counter;
    cout << "Five Minute Alarm: " << counter << "seconds have passed." << endl;
    if (counter == 5) {
      keepgoing = false;
    }
  }
  {
    std::lock_guard<std::mutex> writeLock(writeMTX);
    cout << "Shutdown message inserted into queue\n";
    inputQueue.push_front("999 999 999");
  }
}

//
// int main(int argc, char *argv[])
//
// We test our input simulator by creating two threads: our main application
// thread (created when we start our program) and a thread that executes the
// simulateInput() program.   Then, we go into an infinite loop reading and
// printing the data values from the input queue.
//
// NOTES:
// You must manually terminate this program as both threads intentionally
// has infinite loops.
//
// TODO:
// For the moment, we use a constant to define the average sample rate passed
// to the simulateInput() function.  This needs to be changed to get that value
// from the command-line arguments.
//

const int SAMPLE_ARRIVAL_RATE = 3;
int main(int argc, char *argv[])
{
  vector<thread> dataThreads;
  inputQueue.push_front("0 0 0.0  ");
  // Build and lanuch the input simulator thread
  cout << "Starting writer thread" << endl;
  thread inputThread(simulateInput, SAMPLE_ARRIVAL_RATE);
  inputThread.detach();
  // Build and launch the thread we're using to do a five minute timer.
  cout << "Starting simulation timer thread" << endl;
  thread timerThread(fiveMinuteTimer);
  timerThread.detach();
  // Pause three seconds
  cout << "Pausing three seconds for station identification" << endl;
  this_thread::sleep_for(chrono::milliseconds(3000));
  // Now start printing stuff from the queue
  for (int i=0; i <= 10; ++i)
  {
    cout << "Starting reader number " << i << endl;
    dataThreads.push_back(thread(processData,i));
  }
  // Wait for readers
  for (auto &thread: dataThreads)
  {
    thread.join();
  }
}
