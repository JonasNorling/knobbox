/* -*- c++ -*- */
#pragma once

#include <cstdint>

/*
 * Thoughts about a memory map for Atmel AT25DF081A.
 *
 * The Flash ROM is 8Mbits (1MiB). Erase block size is
 * 4KiB. Individual bytes can be programmed, max page size is 256
 * bytes. There are 256 erase blocks on the chip.
 */

const size_t NAMELEN = 12;

struct TProductionData
{
  uint32_t Magic;
  uint32_t BoardModel;
  uint32_t BoardRevision;
  uint32_t SerialNumber;
};

struct TGlobalSettings
{
  uint32_t Magic;
  uint8_t CurrentParameterScene;
};

struct TInstrument
{
  uint32_t Magic;
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
  uint32_t Magic;
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
  } Knobs[16];
};

struct TSequencerScene
{
  uint32_t Magic;
  char Name[NAMELEN];
  uint8_t Instrument;
  uint8_t Key;
  uint8_t TriggerKey;
  uint8_t DataOfSomeKind[713];
};

class TMemory
{
public:
  TMemory() : CachedBlockNo(0) {}
  void GetVariable();
  void GetIntSetting();

  void FetchBlock(uint8_t n) { (void)n; }

  static const uint8_t BLOCK_PRODPARAM = 0;
  static const uint8_t BLOCK_SETTINGS = 1;
  static const uint8_t BLOCK_NAMES = 2;
  static const uint8_t BLOCK_FIRST_INSTRUMENT = 16;
  static const uint8_t BLOCK_FIRST_PARAM_SCENE = 32;
  static const uint8_t BLOCK_FIRST_SEQ_SCENE = 64;

private:
  static const size_t BlockSize = 4096;
  static const size_t CacheSize = 3000;
  static_assert(sizeof(TInstrument) < CacheSize, "Too big for block");
  static_assert(sizeof(TSequencerScene) < CacheSize, "Too big for block");
  static_assert(sizeof(TParamScene) < CacheSize, "Too big for block");


  uint8_t CachedBlockNo;
  uint8_t CachedBlock[BlockSize];
};

extern TMemory Memory;
