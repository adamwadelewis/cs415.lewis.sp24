//
// File:   mmusim.cpp
// Author: Your Glorious Instructor
// Purpose:
// Test cases for our simulation of a memory management unit.  These tests
// are built using Google Test and provide a starting point for testing
// the cache you are adding to this simulation.
//

#include <gtest/gtest.h>
#include <optional>
#include <algorithm>
#include <numeric>
#include <utility>
#include <chrono>
#include <vector>
#include <cmath>
#include "memmgtunit.hpp"

class MMUSimTest: public ::testing::Test {
    void SetUp() override {}
    void TearDown() override {}
};

// Test: Address out of range causes return of std::nullopt optional
// Precondition: MMU is empty
// Postcondition:  MMU remains empty, option DOES not have value
TEST_F(MMUSimTest, BadAddr)
{
    MemMgtUnit testUnit;
    unsigned outOfBoundsIdx = MEMSIZE + 1;
    std::optional<int> returnedValue = testUnit.get(outOfBoundsIdx);
    EXPECT_FALSE(returnedValue.has_value());
    outOfBoundsIdx = -1;
    returnedValue = testUnit.get(outOfBoundsIdx);
    EXPECT_FALSE(returnedValue.has_value());
}

// Test: Set places correct value in "memory"
// Precondition: MMU is empty
// Postcondition: The first 10 entries in the MMU are set to expected values
TEST_F(MMUSimTest, CorrectSet)
{
    MemMgtUnit testUnit;
    int startingValue = 42;
    for (std::size_t idx = 0; idx <= 10; ++idx)
    {
        testUnit.set(idx, startingValue);
        startingValue++;
    }
    // Now test post condition:
    int storedValue = 42;
    for (std::size_t idx = 0; idx <= 10; ++idx)
    {
        std::optional<int> returnedValue = testUnit.get(idx);
        EXPECT_TRUE(returnedValue.has_value());
        EXPECT_TRUE(*returnedValue == storedValue);
        storedValue++;
    }
}

//
// template<typename T> T computeStats(const std::vector<T> &) 
// Compute the basic stats of the mean and standard deviation 
// of a data set to confirm it matches the expected values for
// that data set.
//
template<typename T>
std::pair<T, T> computeStats(const std::vector<T> &vec) {
    const size_t sz = vec.size();
    if (sz <= 1) {
        return std::make_pair(0.0, 0.0);
    }

    // Calculate the mean
    const T mean = std::accumulate(vec.begin(), vec.end(), 0.0) / sz;

    // Now calculate the variance
    auto variance_func = [&mean, &sz](T accumulator, const T& val) {
        return accumulator + ((val - mean)*(val - mean) / (sz - 1));
    };

    const T variance = std::accumulate(vec.begin(), vec.end(), 0.0, variance_func);
    return std::make_pair(mean,sqrt(variance));
}

// Test: Confirm that access times have expected mean and std.dev.
// Precondition: Data in memory
// Postcondition: Time takes appears to be normally distributed
TEST_F(MMUSimTest, NormalTimings) {
    std::vector<double> timings;
    MemMgtUnit testUnit;
    for (int i = 0; i <= 10000; ++i ){
        testUnit.set(i, i);
    }
    std::optional<int> aValue;
    for (int i = 0; i <= 10; ++i) {
        auto start = std::chrono::steady_clock::now();
        aValue = testUnit.get(i);
        auto end = std::chrono::steady_clock::now();
        auto aDuration = std::chrono::duration_cast<std::chrono::milliseconds> (end-start);
        timings.push_back(aDuration.count());
    }
    std::pair<double,double> stats = computeStats(timings);
    std::cout << "Mean: " << stats.first << " Std. Dev: " << stats.second << std::endl;
}