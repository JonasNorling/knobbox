/* -*- c++ -*- */
#pragma once
#include "device.h"
#include "TCircularBuffer.h"

#ifdef HOST
#include <functional>
#endif

/**
 * Enqueue MIDI data and send it off. A queue is implemented and clock
 * ticks are sent immediately (they can interrupt ongoing commands
 * without messing up the signalling).
 *
 * Status: Not tested with real device. MIDI IN not implemented.
 */
class TMidi
{
private:
  static const int QueueLen = 32;
  static const uint32_t USART = USART2;

public:
  static const uint8_t MIDI_NOTE_OFF = 0x80;
  static const uint8_t MIDI_NOTE_ON = 0x90;
  static const uint8_t MIDI_CC = 0xb0;
  static const uint8_t MIDI_CLOCK_TICK = 0xf8;

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

  /// Called from ISR, in interrupt context.
  void GotInterrupt()
  {
    DisableTxInterrupt();
    if (SendTick) {
      SendTick = false;
      usart_send(USART, MIDI_CLOCK_TICK);
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
