#pragma once
#include <stddef.h>

/**
 * @brief https://blog.stratifylabs.dev/device/2022-03-02-Simple-Moving-Average-and-Exponential-Moving-Average-in-embedded-Cpp/
 * 
 * @tparam Type 
 * @tparam Count 
 */
template <typename Type, size_t Count>
class SimpleRingBuffer {
private:
  size_t m_head = 0;
  Type m_buffer[Count];

public:
  SimpleRingBuffer() { flush(); }

  constexpr size_t count() const { return Count; }

  SimpleRingBuffer &push(Type value) {
    m_buffer[m_head] = value;
    m_head++;
    if (m_head == Count) {
      m_head = 0;
    }
    return *this;
  }

  Type back() const { return m_buffer[m_head]; }
  Type front() const { return at(0); }

  Type at(size_t offset) {
    const auto adjusted_offset
      = m_head > offset ? m_head - offset - 1 : Count + m_head - offset - 1;
    return m_buffer[adjusted_offset];
  }

  SimpleRingBuffer &flush() {
    for (auto &value : m_buffer) {
      value = {};
    }
    m_head = 0;
    return *this;
  }
};

template <typename Type, size_t Count>
class SimpleMovingAverage {
public:
  constexpr size_t count() const { return Count; }

  SimpleMovingAverage &calculate(Type input) {
    sum += input;
    sum -= m_buffer.back();
    m_buffer.push(input);
    return *this;
  }

  Type present_value() const { return sum / Count; }

private:
  SimpleRingBuffer<Type, Count> m_buffer;
  Type sum = {};
};
