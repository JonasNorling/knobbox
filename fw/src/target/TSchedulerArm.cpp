/*
 * TScheduler.cpp
 *
 *  Created on: Dec 2, 2012
 *      Author: jonas
 */

#include "device.h"
#include "TScheduler.h"
#include "utils.h"

void TScheduler::Init()
{
    for (int i = 0; i < SCHEDULER_NUM_TASKS; i++) {
        const TTask& task = Tasks[i];
        uint8_t* sp = reinterpret_cast<uint8_t*>(Tcbs[i].StackPointer);

        // Set stack pointer to beginning of stack
        sp = (uint8_t*)task.Stack + task.StackSize;

        // If this isn't the main thread (so it runs with the PSP), fill stack
        if (task.Stack != 0) {
            memset(task.Stack, StackFill, task.StackSize);

            // Push the same thing that a PendSV would push on the task's stack
            sp -= sizeof(TNvicFrame);
            reinterpret_cast<TNvicFrame*>(sp)->R0 = 0xff00ff00;
            reinterpret_cast<TNvicFrame*>(sp)->R1 = 0xff00ff01;
            reinterpret_cast<TNvicFrame*>(sp)->R2 = 0xff00ff02;
            reinterpret_cast<TNvicFrame*>(sp)->R3 = 0xff00ff03;
            reinterpret_cast<TNvicFrame*>(sp)->R12 = 0xff00ff0c;
            reinterpret_cast<TNvicFrame*>(sp)->Lr = reinterpret_cast<void*>(ReturnFromTask);
            reinterpret_cast<TNvicFrame*>(sp)->Pc = reinterpret_cast<void*>(task.EntryPoint);
            reinterpret_cast<TNvicFrame*>(sp)->Psr = 0x21000000; // Default, allegedly
            sp -= sizeof(TSoftwareFrame);
            reinterpret_cast<TSoftwareFrame*>(sp)->Registers[0] = 0xff00ff04;
            reinterpret_cast<TSoftwareFrame*>(sp)->Registers[1] = 0xff00ff05;
            reinterpret_cast<TSoftwareFrame*>(sp)->Registers[2] = 0xff00ff06;
            reinterpret_cast<TSoftwareFrame*>(sp)->Registers[3] = 0xff00ff07;
            reinterpret_cast<TSoftwareFrame*>(sp)->Registers[4] = 0xff00ff08;
            reinterpret_cast<TSoftwareFrame*>(sp)->Registers[5] = 0xff00ff09;
            reinterpret_cast<TSoftwareFrame*>(sp)->Registers[6] = 0xff00ff0a;
            reinterpret_cast<TSoftwareFrame*>(sp)->Registers[7] = 0xff00ff0b;
        }

        Tcbs[i].StackPointer = sp;
        Tcbs[i].Runnable = true;
    }
    CurrentTask = 0;
}

void TScheduler::Switch()
{
    if (Tasks[CurrentTask].Stack != 0) {
        // Current task is not the "main" task (which runs on the MSP stack)
        assert(Tcbs[CurrentTask].StackPointer >= Tasks[CurrentTask].Stack);
        assert(((uint8_t*)Tasks[CurrentTask].Stack)[0] == StackFill);
    }

    // Find the next runnable task
    do {
        CurrentTask = (CurrentTask + 1) % SCHEDULER_NUM_TASKS;
    } while (!Tcbs[CurrentTask].Runnable);
}

/**
 * The interrupt can come from a task that uses the main or program stack pointer (MSP or PSP).
 * In the latter case we are free to do whatever we want here, because the task's stack won't
 * be affected. In the former case, we need to take some care with which registers (R4..R11) the
 * compiler pushes for its own use.
 */
extern "C" {
void __attribute__((naked)) pend_sv_handler(void)
{
        const uint32_t RETURN_ON_PSP = 0xfffffffd;
        const uint32_t RETURN_ON_MSP = 0xfffffff9; // Documentation tells us to use this, but it hardfaults.
        //const uint32_t RETURN_ON_MSP_HANDLER = 0xfffffff1;

        // 0. NVIC has already pushed some registers on the program/main stack.
        // We are free to modify R0..R3 and R12 without saving them again, and additionally the compiler
        // may choose to use R4..R11 in this function. If it does so, the naked attribute will prevent
        // it from saving the register on the stack, so we'll just have to hope that it doesn't do
        // anything with it before our stm or after our ldm.

        // Read the link register
        uint32_t lr;
        __asm__("MOV %0, lr"
                        : "=r" (lr));

        if (lr & 0x4) {
                // This PendSV call was made from a task using the PSP

                // 1. Push all other registers (R4..R11) on the program stack
                void* psp;
                __asm__("MRS %0, psp\n"          // Load PSP to a temporary register
                                 "STMDB %0!, {r4-r11}\n"  // Push context relative temporary register, update register
                                 "MSR psp, %0\n"          // Put back the new stack pointer in PSP (isn't this pointless?)
                                : "=r" (psp));

                // 2. Store that PSP in the current TCB
                TScheduler::Tcbs[TScheduler::CurrentTask].StackPointer = psp;
        }
        else {
                // This PendSV call was made from a task using the MSP. We don't store
                // the MSP value in the TCB, since there is only one such task.

                // 1. Push all other registers (R4..R11) on the main stack
                __asm__("STMDB SP!, {r4-r11}");  // Push context on main stack
        }

        // 3. Call context switch function, changes current TCB
        TScheduler::Switch();

        if (TScheduler::Tasks[TScheduler::CurrentTask].Stack != 0) {
                // New task uses the PSP

                // 4. Load PSP from TCB
                // 5. Pop R4..R11 from the program stack
                void* psp = TScheduler::Tcbs[TScheduler::CurrentTask].StackPointer;
                __asm__("LDMFD %0!, {r4-r11}\n"  // Pop context relative temporary register, update register
                                 "MSR psp, %0\n"          // Put back the stack pointer in PSP
                                : : "r" (psp));

                // 6. Return. NVIC will pop registers and find the PC to use there.
                __asm__("bx %0" : : "r"(RETURN_ON_PSP));
        }
        else {
                // New task uses the MSP

                // 5. Pop R4..R11 from the main stack
                __asm__("LDMFD SP!, {r4-r11}");  // Pop context from main stack;

                // 6. Return. NVIC will pop registers and find the PC to use there.
                __asm__("bx %0" : : "r"(RETURN_ON_MSP));
        }
}
}
