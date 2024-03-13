//
// File:    memmgtunit.hpp
// Author:  Your Glorious Instructor
// Purpose:
// Simulation of a memory management unit where the memory is a 
// dynamically allocated array of integers, with the added "feature" that
// the amount of time required to access that memory has a delay that emulates
// have to go out to secondary storage to get the data.
// 
// This is the starter code for a CS415 assignment where you will need 
// to implement a caching scheme to speed up access to the "memory".  The 
// cache is simulated (well, be simulated by you) using an array of 16 "cache" 
// lines.  Each line will be a C++ tuple containing the following:
// (address, value, invalid flag, dirty flag).
// 
// You will need to adjust this code to cache the most recently accessed 
// items from "memory".   You will need to adjust the code to hit the
// cache first, bring in a new item if there is a cache miss, and decide 
// which line gets flushed if the cache is full.   Note that you will need 
// to make certain your cache remains "coherent": updates to the value in the
// cache get flushed to memory and a line stays in sync with the value in 
// memory.

#include <iostream>
#include <tuple>

#include <array>
#include <memory>
#include <optional>
#include <random>
#include <cmath>
#include <thread>

const int MEMSIZE = 65535;

class MemMgtUnit
{
private:
    int *memory = new int[MEMSIZE];
    using CacheLine = std::tuple<std::size_t, int, bool, bool>;
    std::array<CacheLine, 16> cache;
    int determineDelay();
public:

    std::optional<int> get(std::size_t index);
    void set(std::size_t index, int value);
    int operator[](std::size_t index);
};

//
// int determineDelay()
// Use the normal distribution support in the STL random library to
// give us an idea of the amount of delay it takes to read something from 
// our simulated offline storage.   This is normally distributed with a mean of 
// 750ms and std. deviation of 300ms. 
//
int MemMgtUnit::determineDelay()
{
    const double mean = 750;
    const double stddev = 300;
    std::random_device rd{};
    std::mt19937 gen{rd()};
    std::normal_distribution nd(mean, stddev);
    // Use nd to get normally distributed number, multiply by 100,
    // and then round to get integer;
    return std::round(nd(gen));
}

//
// std::optional<int> MemMgtUnit::get(std::size_t)
// Get a value from the store, in the form of a C++ optional value, with
// the value at the index if things worked or the std::nullopt if 
// there was an error.
//
std::optional<int> MemMgtUnit::get(std::size_t index) 
{
    std::optional<int> value = std::nullopt;
    if ((index >= 0) && (index < MEMSIZE))
    {
        // Delay for random amount of time to simulate time
        // required to get to secondary storage.
        int delayTime = determineDelay();
        std::this_thread::sleep_for(std::chrono::milliseconds(delayTime));
        value = memory[index];
    }
    return value;
}
//
// void MemMgtUnit::set(std::size_t, int)
// Set a value in the store, displaying an error if an 
// invalid address is passed to us.
//
void MemMgtUnit::set(std::size_t index, int value)
{
    if ((index >= 0) && (index < MEMSIZE))
    {
        memory[index] = value;
    }
    else
    {
        std::cerr << "Bad address passed to MemMgtUnit\n";
    }
}
//
// int MemMgtUnit::operator[](std::size_t index)
// Provide an overload of the array index operator, with 
// a program termination if an invalid address is provided to use
//
int MemMgtUnit::operator[](std::size_t index)
{
    std::optional<int> value = get(index);
    if (value.has_value())
    {
        return *value;
    }
    else
    {
        std::cerr << "MemMgtUnit: Index out of bounds\n";
        exit(-1);
    }
}
