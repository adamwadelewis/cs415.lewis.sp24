//
// File:   barriers.cpp
// Author: Your Glorious Instructor
// Purpose:
// Illustrate the use of the C++ thread library's barrier construct
//
#include <barrier>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

int main() {
  const auto workers = { "bigbird", "burt", "ernie", "elmo" };

  auto on_completion = []() noexcept { 
    // locking not needed here
    static auto phase = "... done\n" "Cleaning up...\n";
    std::cout << phase;
    phase = "... done\n";
  };
  std::barrier sync_point(std::ssize(workers), on_completion);

  auto work = [&](std::string name) {
    std::string product = "  " + name + " worked\n";
    std::cout << product;  // ok, op<< call is atomic
    sync_point.arrive_and_wait();

    product = "  " + name + " cleaned\n";
    std::cout << product;
    sync_point.arrive_and_wait();
  };

  std::cout << "Starting...\n";
  std::vector<std::thread> threads;
  for (auto const& worker : workers) {
    threads.emplace_back(work, worker);
  }
  for (auto& thread : threads) {
    thread.join();
  }
}
