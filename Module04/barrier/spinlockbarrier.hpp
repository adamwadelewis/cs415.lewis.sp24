//  
// File:   spinlockbarrier.hpp
// Author: Your Glorious Instructor
// Purpose:
// Build a better performing version of a barrier by using atomic
// variables to spin-lock rather than waiting on a condition variable.
//
#include <thread>
#include <stdexcept>

class spinlock_barrier
{
public:
  spinlock_barrier(const spinlock_barrier&) = delete;
  spinlock_barrier& operator=(const spinlock_barrier&) = delete;

  explicit spinlock_barrier(unsigned int count) :
    m_count(check_counter(count)), m_generation(0), 
    m_count_reset_value(count)
  { }

  void count_down_and_wait() {
    unsigned int gen = m_generation.load();

    if (--m_count == 0)
    {
      if (m_generation.compare_exchange_weak(gen, gen + 1))
      {
        m_count = m_count_reset_value;
      }
      return;
    }

    while ((gen == m_generation) && (m_count != 0))
      std::this_thread::yield();
  }

private:
  std::atomic<unsigned int> m_count;
  std::atomic<unsigned int> m_generation;
  unsigned int m_count_reset_value;
  int check_counter(int value ) {
    if (value <= 0) {
      throw std::invalid_argument("Barrier counter can not be <= 0");
    }
    else { return value; }
  }
};


