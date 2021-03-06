#include "TSequencer.h"

#include <list>
#include <cstring>

int TestPosition()
{
  const int halfstep = TPosition::MinorsPerStep / 2; // probably 96

  TPosition z({0, 0});
  test_assert(z == z);
  test_assert(z == TPosition({0, 0}));
  test_assert(!(z == TPosition({0, 1})));

  TPosition one(z);
  one.AddMinorsAndWrap(TPosition::MinorsPerStep, 10);
  test_assert(one == TPosition({1, 0}));

  { // Small positive addition
    TPosition p({1, TPosition::MinorsPerStep - 10});
    p.AddMinorsAndWrap(13, 4);
    test_assert(p == TPosition({2, 3}));
  }

  { // Wrapping positive whole steps
    TPosition p(one);
    p.AddMinorsAndWrap(4 * TPosition::MinorsPerStep, 4);
    test_assert(p == TPosition({1, 0}));
  }

  { // Wrapping positive fractional steps
    TPosition p(one);
    p.AddMinorsAndWrap(4.5 * TPosition::MinorsPerStep, 4);
    test_assert(p == TPosition({1, halfstep}));
  }

  { // Wrapping a small positive addition
    TPosition p({3, TPosition::MinorsPerStep - 10});
    p.AddMinorsAndWrap(13, 4);
    test_assert(p == TPosition({0, 3}));
  }

  { // Wrapping negative steps
    TPosition p(one);
    p.AddMinorsAndWrap(-3 * TPosition::MinorsPerStep, 4);
    test_assert(p == TPosition({2, 0}));
  }

  { // Wrapping negative fractional steps
    TPosition p(one);
    p.AddMinorsAndWrap(-3.75 * TPosition::MinorsPerStep, 4);
    test_assert(p == TPosition({1, TPosition::MinorsPerStep / 4}));
  }

  return 0;
}

class TMidiBuffer : public IMidi
{
public:
  int Ticks;
  std::list<TMidiEvent> Events;

  TMidiBuffer() : Ticks(0) {}
  void SendClockTick() {
    Ticks++;
  }
  void SendEvent(const TMidiEvent& event) {
    Events.push_back(event);
  }
};

int TestSequence()
{
  LOG("Running sequence test\n");

  TMidiBuffer buffer;
  TSequencer s(buffer);
  s.Load();

  memset(s.Scenes, 0, sizeof(s.Scenes));
  s.Scenes[0].Flags = TSequencerScene::FLAG_ENABLED;
  s.Scenes[0].StepLength = 4;
  s.Scenes[0].Steps = 4;
  s.Scenes[0].Data[0].Flags = TSequencerScene::TData::FLAG_ON;
  s.Scenes[0].Data[0].Note = 60;
  s.Scenes[0].Data[0].Len = 24;
  s.Scenes[0].Data[0].Velocity = 100;

  s.CalculateSchedule(0);

  test_assert(s.GetStepLength(0) == 4);

  // 48 TicksPerBeat, 24 MidiTicksPerBeat
  s.Start();
  s.Step();
  test_assert(buffer.Ticks == 0);
  test_assert(!buffer.Events.empty());
  test_assert(buffer.Events.front().Data[0] == 0x90); // Note on
  buffer.Events.pop_front();

  s.Step();
  test_assert(buffer.Ticks == 1);
  s.Step();
  test_assert(buffer.Ticks == 1);
  s.Step();
  test_assert(buffer.Ticks == 2);

  for (int i = 3; i < 25; i++) s.Step();
  test_assert(buffer.Ticks == 13);
  test_assert(!buffer.Events.empty());
  test_assert(buffer.Events.front().Data[0] == 0x80); // Note off
  buffer.Events.pop_front();
  test_assert(buffer.Events.empty());

  return 0;
}

int TestSequenceQuick()
{
  LOG("Running quick sequence test\n");

  TMidiBuffer buffer;
  TSequencer s(buffer);
  s.Load();

  memset(s.Scenes, 0, sizeof(s.Scenes));
  s.Scenes[0].Flags = TSequencerScene::FLAG_ENABLED;
  s.Scenes[0].StepLength = 5;
  s.Scenes[0].Steps = 3;
  s.Scenes[0].Data[0].Flags = TSequencerScene::TData::FLAG_ON;
  s.Scenes[0].Data[0].Note = 60;
  s.Scenes[0].Data[0].Len = 24;
  s.Scenes[0].Data[0].Velocity = 100;
  s.Scenes[0].Data[1].Flags = TSequencerScene::TData::FLAG_ON;
  s.Scenes[0].Data[1].Note = 61;
  s.Scenes[0].Data[1].Len = 24;
  s.Scenes[0].Data[1].Velocity = 100;
  s.Scenes[0].Data[2].Flags = TSequencerScene::TData::FLAG_ON;
  s.Scenes[0].Data[2].Note = 62;
  s.Scenes[0].Data[2].Len = 24;
  s.Scenes[0].Data[2].Velocity = 100;
  s.Scenes[0].Data[3].Flags = TSequencerScene::TData::FLAG_ON;
  s.Scenes[0].Data[3].Note = 63;
  s.Scenes[0].Data[3].Len = 24;
  s.Scenes[0].Data[3].Velocity = 100;

  s.CalculateSchedule(0);

  s.Start();
  for (int i = 0; i < 4 * TicksPerBeat; i++) s.Step();
  test_assert(buffer.Ticks == 4 * MidiTicksPerBeat);
  test_assert(buffer.Events.size() == s.Scenes[0].StepLength * 2 + 1);

  test_assert(buffer.Events.front().Data[0] == 0x90); // Note on
  test_assert(buffer.Events.front().Data[1] == 60);
  buffer.Events.pop_front();
  test_assert(!buffer.Events.empty());
  test_assert(buffer.Events.front().Data[0] == 0x80); // Note off
  test_assert(buffer.Events.front().Data[1] == 60);
  buffer.Events.pop_front();
  test_assert(!buffer.Events.empty());
  test_assert(buffer.Events.front().Data[0] == 0x90); // Note on
  test_assert(buffer.Events.front().Data[1] == 61);
  buffer.Events.pop_front();
  test_assert(!buffer.Events.empty());
  test_assert(buffer.Events.front().Data[0] == 0x80); // Note off
  test_assert(buffer.Events.front().Data[1] == 61);
  buffer.Events.pop_front();
  test_assert(!buffer.Events.empty());
  test_assert(buffer.Events.front().Data[0] == 0x90); // Note on
  test_assert(buffer.Events.front().Data[1] == 62);
  buffer.Events.pop_front();
  test_assert(!buffer.Events.empty());
  test_assert(buffer.Events.front().Data[0] == 0x80); // Note off
  test_assert(buffer.Events.front().Data[1] == 62);
  buffer.Events.pop_front();
  test_assert(!buffer.Events.empty());
  test_assert(buffer.Events.front().Data[0] == 0x90); // Note on
  test_assert(buffer.Events.front().Data[1] == 60);
  buffer.Events.pop_front();
  test_assert(!buffer.Events.empty());
  test_assert(buffer.Events.front().Data[0] == 0x80); // Note off
  test_assert(buffer.Events.front().Data[1] == 60);
  buffer.Events.pop_front();

  return 0;
}

int TestSequenceComplex()
{
  LOG("Running complex sequence test\n");

  TMidiBuffer buffer;
  TSequencer s(buffer);
  s.Load();

  memset(s.Scenes, 0, sizeof(s.Scenes));
  s.Scenes[0].Flags = TSequencerScene::FLAG_ENABLED;
  s.Scenes[0].StepLength = 4;
  s.Scenes[0].Steps = 4;
  s.Scenes[0].Data[0].Flags = TSequencerScene::TData::FLAG_ON;
  s.Scenes[0].Data[0].Note = 60;
  s.Scenes[0].Data[0].Len = 2;
  s.Scenes[0].Data[1].Flags = TSequencerScene::TData::FLAG_ON;
  s.Scenes[0].Data[1].Note = 61;
  s.Scenes[0].Data[1].Len = 53;
  s.Scenes[0].Data[2].Flags = TSequencerScene::TData::FLAG_ON;
  s.Scenes[0].Data[2].Note = 62;
  s.Scenes[0].Data[2].Len = 24;
  s.Scenes[0].Data[3].Flags = TSequencerScene::TData::FLAG_ON;
  s.Scenes[0].Data[3].Note = 63;
  s.Scenes[0].Data[3].Len = 100;
  s.Scenes[0].Data[3].Offset = -30;

  s.CalculateSchedule(0);

  s.Start();
  for (int i = 0; i < 4 * TicksPerBeat - 1; i++) s.Step();
  test_assert(buffer.Ticks == 4 * MidiTicksPerBeat - 1);
  // Expect an on and off event per note, but the off for [3] will be suppressed
  // because it is not sounding.
  test_assert(buffer.Events.size() == s.Scenes[0].StepLength * 2 - 1);

  test_assert(buffer.Events.front().Data[0] == 0x90); // Note on
  test_assert(buffer.Events.front().Data[1] == 60);
  buffer.Events.pop_front(); test_assert(!buffer.Events.empty());
  test_assert(buffer.Events.front().Data[0] == 0x80); // Note off
  test_assert(buffer.Events.front().Data[1] == 60);
  buffer.Events.pop_front(); test_assert(!buffer.Events.empty());
  test_assert(buffer.Events.front().Data[0] == 0x90); // Note on
  test_assert(buffer.Events.front().Data[1] == 61);
  buffer.Events.pop_front(); test_assert(!buffer.Events.empty());
  test_assert(buffer.Events.front().Data[0] == 0x90); // Note on
  test_assert(buffer.Events.front().Data[1] == 62);
  buffer.Events.pop_front(); test_assert(!buffer.Events.empty());
  test_assert(buffer.Events.front().Data[0] == 0x80); // Note off
  test_assert(buffer.Events.front().Data[1] == 61);
  buffer.Events.pop_front(); test_assert(!buffer.Events.empty());
  test_assert(buffer.Events.front().Data[0] == 0x90); // Note on
  test_assert(buffer.Events.front().Data[1] == 63);
  buffer.Events.pop_front(); test_assert(!buffer.Events.empty());
  test_assert(buffer.Events.front().Data[0] == 0x80); // Note off
  test_assert(buffer.Events.front().Data[1] == 62);
  buffer.Events.pop_front();
  test_assert(buffer.Events.empty());

  return 0;
}

int TestSequencer()
{
  return TestPosition() +
    TestSequence() +
    TestSequenceQuick() +
    TestSequenceComplex();
}
