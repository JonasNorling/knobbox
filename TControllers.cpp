#include "TControllers.h"

/// The procedure is to pull in the requested scene from flash
/// directly to Scene, and then populate the instrument and
/// parameter names and data from each referenced instrument by
/// reading the instruments to the TMemory cache page in turn.
///
/// Reading is event based, so we have to keep a state and wait for
/// callbacks to proceed.
void TControllers::Load()
{
  Scene.Magic = Scene.MAGIC;
  Scene.Name[0] = 'A';
  Scene.Name[1] = ' ';
  Scene.Name[2] = 't';
  Scene.Name[3] = 'e';
  Scene.Name[4] = 's';
  Scene.Name[5] = 't';
  Scene.Name[6] = ' ';
  Scene.Name[7] = 's';
  Scene.Name[8] = 'c';
  Scene.Name[9] = 'e';
  Scene.Name[10] = 'n';
  Scene.Name[11] = 'e';
  for (int i = 0; i < KNOBS; i++) {
    Scene.Knobs[i].Instrument = 0;
    Scene.Knobs[i].Channel = 0;
    Scene.Knobs[i].Param = i;
    cheap_strcpy(Scene.Knobs[i].Name, "Attack time");
    cheap_strcpy(Scene.Knobs[i].InstrumentName, "Blofeld");
  }

  Scene.Knobs[0].Param = 1;
  cheap_strcpy(Scene.Knobs[0].Name, "Modwheel");
  Scene.Knobs[1].Param = 16;
  cheap_strcpy(Scene.Knobs[1].Name, "LFO1 speed");
  Scene.Knobs[2].Param = 93;
  cheap_strcpy(Scene.Knobs[2].Name, "FX1 mix");
  Scene.Knobs[3].Param = 94;
  cheap_strcpy(Scene.Knobs[3].Name, "FX2 mix");

  for (int i = 0; i < 4; i++) {
    SetValue(i, i*32);
  }
}
