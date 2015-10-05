/**
 * @file
 * @brief Interrupt library for PULPino.
 * 
 * Contains various interrupt manipulating functions.
 *
 * @author Florian Zaruba
 *
 * @version 1.0
 *
 * @date 11/9/2015
 *
 */

#ifndef _INT_H_
#define _INT_H_

/* Number of interrupt handlers - really depends on PIC width in OR1200*/
#define MAX_INT_HANDLERS	32

/* Handler entry */
struct ihnd {
	void 	(*handler)(void *);
	void	*arg;
};

/** 
 * @brief Add interrupt handler. 
 *
 * Adds a new interrupt handler on position vect.
 * @param unsigned long vect - interrupt address to which interrupt handler is added.
 * @param void (* handler)(void *) - pointer to function that is called when the ISR. is executed
 * @param void* arg - arguments for the ISR.
 *
 * @return 0 on error, 1 otherwise
 */ 
int int_add(unsigned long vect, void (* handler)(void *), void *arg);


/**
 * @brief Interrupt initialization routine 
 *
 * Initializes an empty interrupt service table,
 * capable to handle 32 interrupts.
 *
 */
void int_init(void);

/**
 * @brief Main interrupt service routine.
 * 
 * Calls the appropiate ISR.
 *
 */
void int_main();

/** 
 * \brief Disables interrupts globally.
 * \param void
 * \return void
 *
 * By writing 1 to the ie (interruptenable) bit 
 * interrupts are globally disable. 
 */
static inline void int_disable(void) {
	// read-modify-write
	int mstatus;
	asm volatile ("csrr %0, mstatus": "=r" (mstatus));
	mstatus &= 0xFFFFFFFE;
	asm volatile ("csrw mstatus, %0" : /* no output */ : "r" (mstatus));
}

/** 
 * \brief Enables interrupts globally.
 * \param void
 * \return void
 *
 * By writing 1 to the ie (interruptenable) bit 
 * interrupts are globally enabled. 
 */
static inline void int_enable(void) {
	// read-modify-write
	int mstatus;
	asm volatile ("csrr %0, mstatus": "=r" (mstatus));
	mstatus |= 0x01;
	asm volatile ("csrw mstatus, %0" : /* no output */ : "r" (mstatus));
}

/** 
 * \brief Timer compare interrupt service routine.
 * \param void
 * \return void
 *
 * Interrupt service routine triggered when
 * 32 bit mtimecmp and mtime match. Only triggered
 * when interrupts are enabled and mtimecmp is not 0x00.
 *
 * Can be redefined by user software.
 */
void int_time_cmp(void);

/** 
 * \brief Timer compare interrupt service routine.
 * \param void
 * \return void
 *
 * Interrupt service routine for level-triggerd
 * external irq pin.
 *
 * Can be redefined by user software.
 */
 void int_main(void);
#endif // _INT_H_
