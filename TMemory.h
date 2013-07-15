/* -*- c++ -*- */
#pragma once

#include "TSpiDmaJob.h"
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
const uint32_t FLASH_BLOCK_SIZE = 4096;

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

struct TNameList
{
    static const uint32_t MAGIC = 0xf00d0002;
    typedef char TName[NAMELEN+1]; // Additional pos for a \0

    uint32_t Magic;
    uint32_t StartAddress;
    uint32_t Steplen;
    uint8_t Len;
    uint8_t Count;
    uint8_t FetchedCount;
    TName Names[128];
};
static_assert(sizeof(TNameList) <= FLASH_BLOCK_SIZE,
        "Too big struct");

class IMemoryCallback;

/**
 * This class talks to the SPI flash memory (Atmel AT45DB081D).
 *
 * TMemory has space for an entire cached 4k block. Reads and writes
 * can be made against this cache. The SPI memory chip allows reads of
 * only 128 bytes at a time, so transactions have to be chained
 * together. When writing: a full 4k block is cleared, wait 4 ms, 128
 * byte writes are made until finished.
 */
class TMemory : public IDmaCallback
{
public:
    enum OperationType {
        OP_NONE = 0,
        OP_IDENTIFY,
        OP_READ,
        OP_WRITE,
        OP_READ_NAMELIST
    };

    bool FetchBlock(uint8_t n, IMemoryCallback* cb);
    /// Fetch names scattered in flash. A TNameList will be available in
    /// CachedBlock when this is finished.
    bool FetchNames(uint32_t startaddr, uint8_t len,
            uint8_t count, uint32_t steplen,
            IMemoryCallback* cb);
    bool WriteBlock(uint8_t n, IMemoryCallback* cb);
    uint8_t* GetCachedBlock() { return CachedBlock; }

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
    static const uint32_t BlockSize = FLASH_BLOCK_SIZE;
    static const uint8_t DmaChunkSize = 128;

    /* IDmaCallback */
    virtual void DmaFinished(void* context);

private:
    static const uint32_t CommandSize = 4;

    struct TOperation {
        TOperation() : Type(OP_NONE), BlockNo(0),
                State(STATE_START_WRITE), Callback(0) {}
        enum TWriteState {
            STATE_START_WRITE = 0,
            STATE_WAIT_TO_UNPROTECT,
            STATE_UNPROTECT_WE_SENT,
            STATE_UNPROTECT_SENT,
            STATE_WAIT_TO_ERASE,
            STATE_ERASE_WE_SENT,
            STATE_ERASE_SENT,
            STATE_WAIT_TO_WRITE_DATA,
            STATE_CHUNK_WE_SENT,
            STATE_CHUNK_SENT,
            STATE_LAST_CHUNK_SENT
        };

        OperationType Type;
        uint8_t BlockNo;   ///< Requested block number
        uint8_t NextChunk; ///< Read/write DmaChunkSize sized chunks
        TWriteState State;
        IMemoryCallback* Callback;
    };
    TOperation CurrentOperation;

    // CommandBuffer is for outgoing commands, but status register data
    // is read into it from the SPI bus too.
    uint8_t CommandBuffer[CommandSize];
    uint8_t CachedBlock[BlockSize];

    void FetchNextChunk();
    void FetchNextName();
    void NudgeWriteMachine(); ///< Next hop in the write state machine
    void ReadStatus();
    void Identify();
    void SendWriteEnable();
    void SendUnprotectSector();
    void SendErasePage();
    void SendWriteData();
};

class IMemoryCallback
{
public:
    virtual void MemoryOperationFinished(TMemory::OperationType type,
            uint8_t block) = 0;
};

extern TMemory Memory;
