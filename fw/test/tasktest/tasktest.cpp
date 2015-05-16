/*
 * tasktest.c
 *
 *  Created on: Dec 1, 2012
 *      Author: jonas
 */

#define SCHEDULER_NUM_TASKS 3

#include "../device.h"
#include "../TScheduler.h"

void assertFail()
{
    while (1);
}

void hard_fault_handler(void)
{
	while (1);
}

void mem_manage_handler(void)
{
	while (1);
}

void bus_fault_handler(void)
{
	while (1);
}

void usage_fault_handler(void)
{
	while (1);
}

static void task1(void);
static void task2(void);

uint8_t TScheduler::CurrentTask;
TScheduler::TTaskControlBlock TScheduler::Tcbs[SCHEDULER_NUM_TASKS];

static uint8_t __attribute__((aligned(8))) guard1[16];
static uint8_t __attribute__((aligned(8))) task1_stack[256];
static uint8_t __attribute__((aligned(8))) task2_stack[256];
static uint8_t __attribute__((aligned(8))) guard2[16];

const TScheduler::TTask TScheduler::Tasks[SCHEDULER_NUM_TASKS] = {
		{ "main", 0, 0, 0 },
		{ "task1", task1, task1_stack, sizeof(task1_stack) },
		{ "task2", task2, task2_stack, sizeof(task2_stack) }
};

// Called with the program stack
static void task1(void)
{
	while (true) {
		for (int i = 0; i < 1000000; i++) __asm__("nop");
		Pin_vpullup.Toggle();
		TScheduler::Yield();
	}
}

static void task2(void)
{
	while (true) {
		for (int i = 0; i < 1000000; i++) __asm__("nop");
		Pin_vpullup.Toggle();
		TScheduler::Yield();
	}
}

int main()
{
	// Enable can't-happen faults so we don't just get a hard fault
	nvic_enable_irq(NVIC_NMI_IRQ);
	nvic_enable_irq(NVIC_HARD_FAULT_IRQ);
	nvic_enable_irq(NVIC_MEM_MANAGE_IRQ);
	nvic_enable_irq(NVIC_BUS_FAULT_IRQ);
	nvic_enable_irq(NVIC_USAGE_FAULT_IRQ);

	nvic_enable_irq(NVIC_PENDSV_IRQ);

	rcc_peripheral_enable_clock(&RCC_APB2ENR, RCC_APB2ENR_IOPBEN); // GPIOB
	Pin_vpullup.SetOutput();

	memset(guard1, 0x77, sizeof(guard1));
	memset(guard2, 0x77, sizeof(guard2));

	// We are running in privileged thread mode on the main stack here
	TScheduler::Init();

	while (true) {
		for (int i = 0; i < 1000000; i++) __asm__("nop");
		Pin_vpullup.Toggle();
		TScheduler::Yield();

		for (size_t i = 0; i < sizeof(guard1); i++) {
			if (guard1[i] != 0x77) {
				while (1);
			}
		}

		for (size_t i = 0; i < sizeof(guard2); i++) {
			if (guard2[i] != 0x77) {
				while (1);
			}
		}
	}
}
