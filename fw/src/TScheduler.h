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
#else
#include <pthread.h>
#endif
#include <cstring>

class TScheduler
{
private:
    /**
     * Constant information about the system's tasks.
     * The system can contain a number of tasks with their own stacks, plus a main
     * task that runs on the MSP (interrupt) stack. The main task always has to be
     * runnable (it can't block) and in that sense looks like an idle task.
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
        bool Runnable; ///< Task is runnable, i.e. not blocked waiting for something
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

#ifdef HOST
    static pthread_mutex_t Mutex;
#endif

public:
    friend void pend_sv_handler(void);
    friend void* createThread(void*);

    static void ReturnFromTask()
    {
        while (true);
    }

    /**
     * Initialize the scheduler data structures. All tasks with stacks have
     * their stacks cleared and their stack pointers initialized.
     * Task number 0 is marked as currently executing, all others as runnable.
     */
    static void Init();
    static void Switch();

    static uint8_t GetCurrentTaskId() {
        return CurrentTask;
    }

    /**
     * Switch out the current task, letting other tasks run.
     */
#ifndef HOST
    static inline void Yield()
    {
        // Trigger PendSV
        SCB_ICSR |= SCB_ICSR_PENDSVSET;
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
        __asm__("nop");
    }
#else
    static void Yield();
#endif

    /**
     * Switch out the current task, marking it as not runnable.
     */
    static inline void Block()
    {
        Tcbs[CurrentTask].Runnable = false;
        Yield();
    }

    /**
     * Wait until a condition turns true. We need to do this in an atomic manner,
     * because the condition may be changed by an ISR after it has been checked but
     * before setting Runnable to false.
     */
    static inline void BlockUntil(volatile bool* condition)
    {
        critical_section();
        while (*condition == false) {
            Tcbs[CurrentTask].Runnable = false;
            end_critical_section();
            Yield();
        }
        end_critical_section();
    }

    /**
     * Mark a task as runnable.
     */
    static inline void Wake(uint8_t taskId)
    {
        Tcbs[taskId].Runnable = true;
    }
};
