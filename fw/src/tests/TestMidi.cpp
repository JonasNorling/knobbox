#include "TMidi.h"
#include "TMidiParser.h"
#include <vector>

static std::vector<uint8_t> data;
void push_data(uint32_t, uint8_t d)
{
  data.push_back(d);
}

int TestMidi()
{
  TMidi midi;
  midi.SetUartSend(push_data);

  midi.EnqueueByte(1);
  midi.EnqueueByte(2);
  test_assert(data.size() == 0);
  midi.GotInterrupt();
  midi.GotInterrupt();

  test_assert(data.size() == 2);
  test_assert(data[0] == 1);
  test_assert(data[1] == 2);

  midi.EnqueueByte(3);
  midi.EnqueueByte(4);
  midi.SendClockTick();
  midi.EnqueueByte(5);
  midi.GotInterrupt();
  midi.GotInterrupt();
  midi.EnqueueByte(6);
  midi.EnqueueByte(7);
  midi.GotInterrupt();
  midi.GotInterrupt();
  midi.GotInterrupt();
  midi.GotInterrupt();

  test_assert(data.size() == 8);
  test_assert(data[0] == 1);
  test_assert(data[1] == 2);
  test_assert(data[2] == 0xf8);
  test_assert(data[3] == 3);
  test_assert(data[4] == 4);
  test_assert(data[5] == 5);
  test_assert(data[6] == 6);
  test_assert(data[7] == 7);

  return 0;
}

int TestMidiParser()
{
    TMidiParser p;
    test_assert(p.Feed(0x90) == TMidiParser::NEED_MORE_FOOD);
    test_assert(p.Feed(0x45) == TMidiParser::NEED_MORE_FOOD);
    test_assert(p.Feed(0x40) == TMidiParser::GOT_EVENT);

    TMidiEvent e = p.GetEvent();
    test_assert(e.Data[0] == 0x90);
    test_assert(e.GetType() == TMidiEvent::MIDI_NOTE_ON);
    test_assert(e.GetNote() == 0x45);
    test_assert(e.GetVelocity() == 0x40);


    test_assert(p.Feed(0x80) == TMidiParser::NEED_MORE_FOOD);
    test_assert(p.Feed(0x43) == TMidiParser::NEED_MORE_FOOD);
    test_assert(p.Feed(0x00) == TMidiParser::GOT_EVENT);

    e = p.GetEvent();
    test_assert(e.Data[0] == 0x80);
    test_assert(e.GetType() == TMidiEvent::MIDI_NOTE_OFF);
    test_assert(e.GetNote() == 0x43);
    test_assert(e.GetVelocity() == 0x00);


    test_assert(p.Feed(0xb3) == TMidiParser::NEED_MORE_FOOD);
    test_assert(p.Feed(0xf8) == TMidiParser::GOT_REALTIME);
    test_assert(p.Feed(  13) == TMidiParser::NEED_MORE_FOOD);
    test_assert(p.Feed(0xf8) == TMidiParser::GOT_REALTIME);
    test_assert(p.Feed(  42) == TMidiParser::GOT_EVENT);

    e = p.GetEvent();
    test_assert(e.Data[0] == 0xb3);
    test_assert(e.GetType() == TMidiEvent::MIDI_CC);
    test_assert(e.GetChannel() == 3);
    test_assert(e.GetCC() == 13);
    test_assert(e.GetValue() == 42);

    // Running status
    test_assert(p.Feed(  14) == TMidiParser::NEED_MORE_FOOD);
    test_assert(p.Feed(  43) == TMidiParser::GOT_EVENT);

    return 0;
}
