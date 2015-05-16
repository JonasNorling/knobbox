/* -*- c++ -*- */
#pragma once

#include "TFlash.h"
#include <cstdint>

/*
 * Thoughts about a memory map for Atmel AT45DB081D.
 *
 * The Flash ROM is 8Mbits (1MiB). Erase block size is 256 or 264 bytes.
 * Individual bytes can be programmed, up to one page at a time.
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
    static const uint32_t FLAG_DIRTY   = 0x00000001;
    static const uint32_t FLAG_ENABLED = 0x00000002;

    uint32_t Magic;
    uint32_t Flags;
    uint8_t SyncStartTo; ///< To seq1..4, global, free running???
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

    struct TData {
        static const uint8_t FLAG_ON = 0x01;
        static const uint8_t FLAG_SOUNDING = 0x02;
        static const uint8_t FLAG_SAVE_MASK = FLAG_ON;

        uint8_t Flags;
        uint8_t Note;
        uint8_t Len; ///< In 48'ths of a step (48 fills whole step)
        uint8_t Velocity;
        int8_t Offset; ///< In 48'ths of a step (+48 delays one step)
        uint8_t Cc;
        uint16_t Action;
    } Data[SEQLEN];
};

class TMemory : public IFlashCallback
{
public:
    static const int BlockSize = 1024;
    static const uint8_t BLOCK_PRODPARAM = 0;
    static const uint8_t BLOCK_SETTINGS = 1;
    static const uint8_t BLOCK_NAMES = 2;
    static const uint8_t BLOCK_FIRST_INSTRUMENT = 16;
    static const uint8_t INSTRUMENT_COUNT = 16;
    static const uint8_t BLOCK_FIRST_PARAM_SCENE = 32;
    static const uint8_t PARAM_SCENE_COUNT = 32;
    static const uint8_t BLOCK_FIRST_SEQ_SCENE = 64;
    static const uint8_t SEQ_SCENE_COUNT = 32; // Low for now...
    static const uint8_t BLOCK_DEVICE_ID = 0xfe;
    static const uint8_t BLOCK_STATUS_REG = 0xff;

    bool ReadBlocking(uint32_t address, const TBuffer& buffer);

    void FlashJobFinished(TFlashJob& job);

private:
    bool JobFinished;
    uint8_t TaskId;
};

extern TMemory Memory;
