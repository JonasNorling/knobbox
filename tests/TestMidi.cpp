#include "TMidi.h"
#include <vector>

int TestMidi()
{
  std::vector<uint8_t> data;
  TMidi midi;
  midi.SetUartSend([&data](uint32_t, uint8_t d) {
      data.push_back(d);
    });

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
