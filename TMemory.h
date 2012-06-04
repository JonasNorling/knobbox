/* -*- c++ -*- */
#pragma once

#include "TSpiDmaJob.h"
#include <cstdint>

/*
 * Thoughts about a memory map for Atmel AT25DF081A.
 *
 * The Flash ROM is 8Mbits (1MiB). Erase block size is
 * 4KiB. Individual bytes can be programmed, max page size is 256
 * bytes. There are 256 erase blocks on the chip.
 */

const uint32_t NAMELEN = 12;
const int KNOBS = 16;
const int SEQLEN = 16;

struct TProductionData
{
  uint32_t Magic;
  uint32_t Flags;
  uint32_t BoardModel;
  uint32_t BoardRevision;
  uint32_t SerialNumber;
};

struct TGlobalSettings
{
  uint32_t Magic;
  uint32_t Flags;
  uint8_t CurrentParameterScene;
};

struct TInstrument
{
  uint32_t Magic;
  uint32_t Flags;
  char Name[NAMELEN];
  char CCName[128][NAMELEN];
  // Param mode: sysex or NPRN
  // Sysex send framing
  // Sysex receive framing
  struct {
    uint32_t Param;
    uint32_t Range;
    char Name[NAMELEN];
  } Params[64];
};

struct TParamScene
{
  static const uint32_t MAGIC = 0xf00d0001;
  static const uint32_t FLAG_DIRTY = 0x00000001;

  uint32_t Magic;
  uint32_t Flags;
  char Name[NAMELEN];
  struct {
    uint8_t Instrument;
    uint8_t Channel;
    uint8_t Param; // 0..127 is CC, 127.. is NPRN
    char Name[NAMELEN];

    /* Populated when loading */
    char InstrumentName[NAMELEN];
    uint8_t Nprn[2];
    uint16_t NprnRange[2];
  } Knobs[KNOBS];
};

struct TSequencerScene
{
  static const uint32_t MAGIC = 0xf00d0002;
  static const uint32_t FLAG_DIRTY = 0x00000001;

  uint32_t Magic;
  uint32_t Flags;
  char Name[NAMELEN];
  uint8_t Instrument;
  uint8_t Channel;
  uint8_t Key;
  uint8_t TriggerKey;
  uint8_t Mode;
  /// Number of minor ticks to advance per beat clock event. 1 ->
  /// whole note, 2 -> half note, 3 -> dotted quarter, 4 -> quarter, ...
  uint8_t StepLength;
  uint8_t Steps;
  uint8_t Cc;

  struct {
    static const uint32_t FLAG_ON = 0x01;

    uint8_t Flags;
    uint8_t Note;
    uint8_t Len; ///< In 48'ths of StepLength or something...
    uint8_t Velocity;
    uint8_t Offset; ///< In 48'ths of StepLength or something...
    uint8_t Cc;
    uint8_t Action;
    uint8_t Reserved1;
  } Data[SEQLEN];
};

/**
 * This class talks to the SPI flash memory.
 *
 * Status: Just a dummy.
 */
class TMemory : public IDmaCallback
{
public:
  TMemory() : CachedBlockNo(0) {}
  void GetVariable();
  void GetIntSetting();

  void FetchBlock(uint8_t n);
  uint8_t* Data() { return CachedBlock; }

  static const uint8_t BLOCK_PRODPARAM = 0;
  static const uint8_t BLOCK_SETTINGS = 1;
  static const uint8_t BLOCK_NAMES = 2;
  static const uint8_t BLOCK_FIRST_INSTRUMENT = 16;
  static const uint8_t BLOCK_FIRST_PARAM_SCENE = 32;
  static const uint8_t BLOCK_FIRST_SEQ_SCENE = 64;

  /* IDmaCallback */

  virtual void DmaFinished(void* context);

private:
  static const uint32_t BlockSize = 4096;
  static const uint32_t CommandSize = 6;

  uint8_t CachedBlock[BlockSize];
  uint8_t CachedBlockNo;
  uint8_t CommandBuffer[CommandSize];
};

extern TMemory Memory;
