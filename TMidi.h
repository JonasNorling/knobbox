/* -*- c++ -*- */
#pragma once
#include "device.h"
#include "TCircularBuffer.h"

#ifdef HOST
#include <algorithm>
#endif

class TMidi
{
private:
  static const int QueueLen = 32;
  static const uint32_t USART = USART2;

public:
  TMidi() : SendTick(false)  { }

#ifdef HOST
  void SetUartSend(std::function<void(uint32_t, uint8_t)> fn)
  {
    usart_send = fn;
  }
#endif

  void SendClockTick()
  {
    Pin_led_g.Set();
    SendTick = true;
    EnableTxInterrupt();
  }

  bool EnqueueByte(uint8_t d)
  {
    Pin_led_g.Set();
    // Disable the interrupt when modifying the queue
    DisableTxInterrupt();
    bool status = Queue.Add(d);
    EnableTxInterrupt();
    return status;
  }

  void GotInterrupt() __attribute__((optimize (0)))
  {
    DisableTxInterrupt();
    if (SendTick) {
      SendTick = false;
      usart_send(USART, 0xf8);
    }
    else {
      usart_send(USART, Queue.First());
      Queue.Remove();
    }
    if (!Queue.Empty()) {
      EnableTxInterrupt();
    }
    Pin_led_g.Clear();
  }

private:
#ifndef HOST
  void usart_send(uint32_t usart, uint8_t data)
  {
    USART_DR(usart) = (data & USART_DR_MASK);
  }
#else
  std::function<void(uint32_t, uint8_t)> usart_send;
#endif

  void EnableTxInterrupt()
  {
#ifndef HOST
    USART_CR1(USART) |= USART_CR1_TXEIE;
#endif
  }

  void DisableTxInterrupt()
  {
#ifndef HOST
    USART_CR1(USART) &= ~USART_CR1_TXEIE;
#endif
  }

  TCircularBuffer<uint8_t, 32> Queue;
  volatile bool SendTick;
};

extern TMidi Midi;
