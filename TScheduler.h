#pragma once

/*
 * ABI: r0..r3 are caller-saved (scratch registers), R4..R12 are callee-saved. It is appropriate
 * to use R12 for a system call opcode (saved by NVIC). The stack pointer points to the
 * current extent of the stack -- it is decremented before being used as index in a store.
 * The stack grows downwards, to lower addresses.
 * When an interrupt is processed, 8 registers are stored. LR is set to a special value
 * that makes an ordinary function return into a return from interrupt. The LR value
 * indicates which stack is going to be used (process or main) and can be modified before return.
 *
 *                     ____________________
 *              Stack |                    |
 *                    |                    |
 *    higher          |        R4          | <-- SP saved in TCB (64 bytes of context)
 *  addresses         |        R5          |   ^
 *      |     ^       |        R6          |   |
 *      |     |       |        R7          |   | 8 registers pushed by handler:
 *      |     |       |        R8          |   | R4..R11
 *      |     |       |        R9          |   | The task's complete context is now stored
 *      V     |       |        R10         |   |
 *            |       |        R11         |   |
 *        direction   |        R0          | <-- SP when SVC handler starts to run
 *        of growth   |        R1          |   ^
 *                    |        R2          |   |
 *                    |        R3          |   |
 *                    |        R12         |   | 8 registers pushed by hardware (NVIC):
 *                    |        LR (R14)    |   | xPSR, PC, LR, R12, R3..R0
 *                    |        PC (R15)    |   |
 *                    |       xPSR         |   |
 *                    |                    | <-- SP before SVC
 *                    |      (stuff)       |
 *                    |                    |
 *  Stack + StackSize |____________________|
 *
 */

#ifndef HOST
#include <libopencm3/stm32/f1/scb.h>
#endif
#include <cstring>

class TScheduler
{
private:
    /**
     * Constant information about the system's tasks.
     */
    struct TTask
    {
        const char* Name;
        void (*EntryPoint)(void);
        void* Stack; //< Pointer to stack memory, or 0 if it uses the main stack with MSP.
        int StackSize;
    };

    /**
     * Current state of a task
     */
    struct TTaskControlBlock
    {
        void* StackPointer;
    };

    /**
     * Registers pushed on stack by NVIC (hardware)
     */
    struct TNvicFrame
    {
        uint32_t R0;
        uint32_t R1;
        uint32_t R2;
        uint32_t R3;
        uint32_t R12;
        void* Lr;
        void* Pc;
        uint32_t Psr;
    };

    struct TSoftwareFrame
    {
        uint32_t Registers[8];
    };

    static uint8_t CurrentTask;
    static TTaskControlBlock Tcbs[SCHEDULER_NUM_TASKS];
    static const TTask Tasks[SCHEDULER_NUM_TASKS];
    static const uint8_t StackFill = 0xa5;

public:
    friend void pend_sv_handler(void);

    static void ReturnFromTask()
    {
        while (true);
    }

    /**
     * Initialize the scheduler data structures. All tasks with stacks have
     * their stacks cleared and their stack pointers initialized.
     * Task number 0 is marked as currently executing.
     */
    static void Init();
    static void Switch();

    static inline void Yield()
    {
#ifndef HOST
        // Trigger PendSV
        SCB_ICSR |= SCB_ICSR_PENDSVSET;
        __asm__("nop");
        __asm__("nop");
#endif
    }
};
