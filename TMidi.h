/* -*- c++ -*- */
#pragma once
#include "IMidi.h"
#include "device.h"
#include "TCircularBuffer.h"

#ifdef HOST
#include <functional>
#endif

struct TMidiEvent
{
    static const uint8_t MIDI_NOTE_OFF = 0x80;
    static const uint8_t MIDI_NOTE_ON = 0x90;
    static const uint8_t MIDI_CC = 0xb0;
    static const uint8_t MIDI_REALTIME = 0xf8;
    static const uint8_t MIDI_CLOCK_TICK = 0xf8;
    static const uint8_t MIDI_NOTE_C0 = 12;
    static const uint8_t MIDI_NOTE_MIN = MIDI_NOTE_C0;
    static const uint8_t MIDI_NOTE_MAX = 127;

    uint8_t GetType() const { return Data[0] & 0xf0; }
    uint8_t GetChannel() const { return Data[0] & 0x0f; }

    /* Note events */
    uint8_t GetNote() const { return Data[1]; }
    uint8_t GetVelocity() const { return Data[2]; }

    /* CC events */
    uint8_t GetCC() const { return Data[1]; }
    uint8_t GetValue() const { return Data[2]; }

    uint8_t Data[3];
};

/**
 * Enqueue MIDI data and send it off. A queue is implemented and clock
 * ticks are sent immediately (they can interrupt ongoing commands
 * without messing up the signaling).
 *
 * Status: Works.
 */
class TMidi : public IMidi
{
private:
    static const int QueueLen = 32;
    static const uint32_t USART = USART2;

public:
    TCircularBuffer<uint8_t, 4> InQueue;

    TMidi() : SendTick(false) { }

    void EnableRxInterrupt()
    {
#ifndef HOST
        USART_CR1(USART) |= USART_CR1_RXNEIE;
#endif
    }

#ifdef HOST
    void SetUartSend(std::function<void(uint32_t, uint8_t)> fn)
    {
        usart_send = fn;
    }
#endif

    void SendClockTick()
    {
        SendTick = true;
        EnableTxInterrupt();
    }

    void SendEvent(const TMidiEvent& event);
    bool EnqueueByte(uint8_t d);

    /// Called from ISR, in interrupt context.
    void GotInterrupt()
    {
#ifndef HOST
        if ((USART_CR1(USART) & USART_CR1_TXEIE) && (USART_SR(USART) & USART_SR_TXE)) {
#else
        if (true) {
#endif
            // We got here because there is space in the TX register
            DisableTxInterrupt();
            if (SendTick) {
                SendTick = false;
                usart_send(USART, TMidiEvent::MIDI_CLOCK_TICK);
            }
            else {
                usart_send(USART, OutQueue.First());
                OutQueue.Remove();
            }
            if (!OutQueue.Empty()) {
                EnableTxInterrupt();
            }
        }
#ifndef HOST
        if ((USART_CR1(USART) & USART_CR1_RXNEIE) && (USART_SR(USART) & USART_SR_RXNE)) {
#else
        if (false) {
#endif
            InQueue.Add(usart_receive(USART));
        }
    }

private:
#ifndef HOST
    void usart_send(uint32_t usart, uint8_t data)
    {
        USART_DR(usart) = (data & USART_DR_MASK);
    }

    uint8_t usart_receive(uint32_t usart)
    {
        return USART_DR(usart) & 0xff;
    }
#else
    std::function<void(uint32_t, uint8_t)> usart_send;
    std::function<uint8_t(uint32_t)> usart_receive;
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

    TCircularBuffer<uint8_t, 32> OutQueue;
    volatile bool SendTick;
};

extern TMidi Midi;
