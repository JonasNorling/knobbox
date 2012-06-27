#include "TSequencer.h"

int TestSequencer()
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
