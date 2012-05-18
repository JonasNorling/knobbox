#include "TCircularBuffer.h"

int TestCircularBuffer()
{
  TCircularBuffer<int, 7> buf;
  test_assert(buf.Empty());
  test_assert(buf.Add(1));
  test_assert(!buf.Empty());
  test_assert(buf.Add(2));
  test_assert(buf.Add(3));
  test_assert(buf.Add(4));
  test_assert(buf.Add(5));
  test_assert(buf.Add(6));
  test_assert(buf.Add(7));

  test_assert(!buf.Empty());
  test_assert(buf.First() == 1);
  test_assert(buf.First() == 1);
  buf.Remove();
  test_assert(buf.First() == 2);
  buf.Remove();
  buf.Remove();
  buf.Remove();
  buf.Remove();
  test_assert(buf.First() == 6);
  test_assert(!buf.Empty());

  test_assert(buf.Add(8));
  buf.Remove();
  test_assert(buf.First() == 7);
  test_assert(!buf.Empty());
  buf.Remove();
  test_assert(buf.First() == 8);
  test_assert(!buf.Empty());
  buf.Remove();
  test_assert(buf.Empty());

  test_assert(buf.Add(9));
  test_assert(!buf.Empty());
  test_assert(buf.First() == 9);
  buf.Remove();
  test_assert(buf.Empty());

  return 0;
}
