//
// File:   matmult.cpp
// Author: Adam.Lewis@athens.edu
// Purpose:
// Use matrix multiply as a more complex example of using threads in C++
//

#include <iostream>
#include <random>
#include <chrono>
#include <thread>
#include <functional>

// Configure some useful namespaces for dealing with time stuff
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;

// A few useful constants
static const long MATRIX_SIZE = 100;
static const int THREADS_NUMBER = 4;
static const long NEXECUTIONS = 1e3;

// We need something to store our matrices.  The first reaction is to use an
// 2-d array declared as a global or something allocated on a stack.  There are
// some funky implementation issues with threads we have to address.  Remember
// that each thread gets it's stack while all threads share the heap.  If we
// are dealing with large arrays (which is quite realistic in most
// applications), it is better to use the heap instead
//
// We add a few help methods for getting a matrix configured.

struct Matrix {
  float **elements;
  // void initializeZero()
  //
  // Initialize the matrix and fill with zero.
  // Note how this operates by allocating each row in the heap and then set all
  // values to 0.0f.
  //
  void initalizeZero() {
    elements = new float*[MATRIX_SIZE];
    for (int i = 0; i < MATRIX_SIZE; ++i) {
      elements[i] = new float[MATRIX_SIZE];
      for (int j = 0; j < MATRIX_SIZE; ++j) {
        elements[i][j] = 0.0f;
      }
    }
  }

  // void initializeRandom()
  //
  // Initialize the matrix and fill with random values.   Observe the use of
  // the C++ STL classes for generating random numbers.  This is preferred way
  // to do this with modern C++.
  //
  void initalizeRandom() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> dist(-1e9, 1e9);
    auto random = std::bind(dist, mt);
    elements = new float*[MATRIX_SIZE];
    for (int i = 0; i < MATRIX_SIZE; ++i) {
      elements[i] = new float[MATRIX_SIZE];
      for (int j = 0; j < MATRIX_SIZE; ++j) {
        elements[i][j] = random();
      }
    }
  }

  // void print()
  //
  // And we do have print things.
  //
  void print() {
    std::cout << std::endl;
    for (int i = 0; i < MATRIX_SIZE; ++i) {
      std::cout << "|\t";

      for (int j = 0; j < MATRIX_SIZE; ++j) {
        std::cout << elements[i][j] << "\t";
      }
      std::cout << "|" << std::endl;
    }
  }
};

//
// Let's define some forward declarations so we can go top down in
// in the code.
//
void multiply(Matrix& r, const Matrix& m1, const Matrix& m2);
void singleExecution(Matrix& r,
                     long long& elapsed_time,
                     const Matrix& m1,
                     const Matrix& m2);
void multithreadingExecution(Matrix& r,
                              long long& elapsed_time,
                              const Matrix& m1,
                              const Matrix& m2);
void multiplyThreading(Matrix& result,
                       const int thread_number,
                       const Matrix& m1,
                       const Matrix& m2);
void benchmarkExecution(
  void(*executionFunction)(Matrix& r,
                            long long& elapsed_time,
                            const Matrix& m1,
                            const Matrix& m2));
long long millisecondsNow();

// int main(int argc, char **argv)
//
int main(int argc, char**argv)
{
  std::cout << "Single execution" << std::endl;
  benchmarkExecution(singleExecution);
  std::cout << "Multi thread execution" << std::endl;
  benchmarkExecution(multithreadingExecution);
  std::cout << "End of program" << std::endl;
  return 0;
}

//
// void benchmarkExecution()
//
// This is the driver function that runs our little test.  It creates and fills
// the two test arrays and, via a function pointer, calls the processing
// function for each algorithm type.
//
// Note the signature of the function defines a single argument, a pointer to
// the function that does the actual work.
//
void benchmarkExecution(
  void(*executionFunction)(
    Matrix& r,
    long long& elapsed_time,
    const Matrix& m1,
    const Matrix& m2))
{
  Matrix m1, m2, r;

  long long total_time = 0.0;
  for (int i = 0; i < NEXECUTIONS; ++i) {
    long long elapsed_time = 0.0;
    m1.initalizeRandom();
    m2.initalizeRandom();
    r.initalizeZero();

    executionFunction(r, elapsed_time, m1, m2);
    total_time += elapsed_time;
  }
  std::cout << "\tAverage execution took\t" << (double) total_time / NEXECUTIONS << " ms" << std::endl;
}

// void multiply(Marix &r, const Matrix &m1, const Matrix& m2)
//
// This function implements the algorithm lfor multipling two
// matrices.   It's single threaded, and just walks through the array
// doing the dot-product of each row and column.   Note that this is O(n^3)
// algorithm (can you explain why?) and so will not perform well for the large
// arrays we're using in this program.  Be preapred to wait.
//
void multiply(Matrix& r, const Matrix& m1, const Matrix& m2) {
  for (int i = 0; i < MATRIX_SIZE; ++i) {
    for (int j = 0; j < MATRIX_SIZE; ++j) {
      float result = 0.0f;
      for (int k = 0; k < MATRIX_SIZE; ++k) {
        const float e1 = m1.elements[i][k];
        const float e2 = m2.elements[k][j];
        result += e1 * e2;
      }
      r.elements[i][j] = result;
    }
  }
}

//
// void singleExecution()
//
// We're looking at simple process for dealing with the single threaded version
// of matrix multiply.   We just note the start time,  do the work, and then
// figure out how the proces took by noting the end time and subtracting.
//
void singleExecution(Matrix& r,
                     long long& elapsed_time,
                     const Matrix& m1,
                     const Matrix& m2) {
  //std::cout << "Starting single thread execution..." << std::endl;
  long long start_time = millisecondsNow();

  //std::cout << "Calculating...." << std::endl;
  multiply(r, m1, m2);

  long long end_time = millisecondsNow();
  //std::cout << "Finishing single thread execution..." << std::endl;

  elapsed_time = end_time - start_time;
}

//
// void multithreadingExecution()
//
// Threre is some setup required to fire-up the different threads.
// We'll track the different threads in an array, start them by traversing that
// array, and then joining in turn.
//
void multithreadingExecution(Matrix& r,
                             long long& elapsed_time,
                             const Matrix& m1,
                             const Matrix& m2) {
  //std::cout << "Starting multithreading execution..." << std::endl;
  long long startTime = millisecondsNow();

  std::thread threads[THREADS_NUMBER];

  for (int i = 0; i < THREADS_NUMBER; ++i) {
    //std::cout << "Starting thread " << i << std::endl;
    threads[i] = std::thread(multiplyThreading,
                             std::ref(r),
                             i,
                             std::ref(m1),
                             std::ref(m2));
  }

  //std::cout << "Calculating...." << std::endl;

  for (int i = 0; i < THREADS_NUMBER; ++i) {
    //std::cout << "Joining thread " << i << std::endl;
    threads[i].join();
  }

  long long endTime = millisecondsNow();
  //std::cout << "Finishing multithreading execution..." << std::endl;

  elapsed_time = endTime - startTime;
}

//
// void multiplyThreading(Matrix& result, const int thread_number, const
//                              Matrix& m1, const Matrix& m2)
//
// The hard problem for doing matrix multiply in parallel is making certain
// that threads don't try to work on the same piece of data at the same time.
// We can arrange the threads to avoid this problem by taking into account the
// total number of 
// operations divided by the number of threads.  This tells you the amount of
// work that you need to do.
//
// Thus, we need to figure out where in the array each thread will be
// operating. We partition the array by treated it as a really long
// one-dimensional array to simplify the partitioning.  And we then assign work
// starting with the first thread and then assigning the next thread to the
// following portion.   Any overhead work that needs to be done will be
// performed by the first thread.
//
// The actual multiplication is performed by taking the partitioning of the
// array assigned to the thread, remapping into a 2-d array and doing the
// single-threaded algorithm for that "sub-chunk" of the larger array.
//
void multiplyThreading(Matrix& result,
                        const int thread_number,
                        const Matrix& m1,
                        const Matrix& m2) {
  // Calculate workload
  const int n_elements = (MATRIX_SIZE * MATRIX_SIZE);
  const int n_operations = n_elements / THREADS_NUMBER;
  const int rest_operations = n_elements % THREADS_NUMBER;

  int start_op, end_op;

  if (thread_number == 0) {
    // First thread does more job
    start_op = n_operations * thread_number;
    end_op = (n_operations * (thread_number + 1)) + rest_operations;
  }
  else {
    start_op = n_operations * thread_number + rest_operations;
    end_op = (n_operations * (thread_number + 1)) + rest_operations;
  }

  for (int op = start_op; op < end_op; ++op) {
    const int row = op % MATRIX_SIZE;
    const int col = op / MATRIX_SIZE;
    float r = 0.0f;
    for (int i = 0; i < MATRIX_SIZE; ++i) {
      const float e1 = m1.elements[row][i];
      const float e2 = m2.elements[i][col];
      r += e1 * e2;
    }

    result.elements[row][col] = r;
  }
}

// long long millisecondsNow()
//
// The C++ STL defines classes for processing system time.   Here we use the
// system_clock::now() funtion to return the number of milliseconds since
// Jan. 1, 1970 (the start of time according to the UNIX gods).
//
long long millisecondsNow() {
  auto msSinceEpoch = duration_cast<milliseconds>(
                             system_clock::now().time_since_epoch()).count();
  return (long long) msSinceEpoch;
};
