/*	BASIC INTERRUPT VECTOR TABLE FOR STM8 devices
 *	Copyright (c) 2007 STMicroelectronics
 */
extern @far @interrupt void trap_int(void);
extern @far @interrupt void Tim4_int(void);
extern @far @interrupt void uart1_int(void);

typedef void @far (*interrupt_handler_t)(void);

struct interrupt_vector {
	unsigned char interrupt_instruction;
	interrupt_handler_t interrupt_handler;
};

@far @interrupt void NonHandledInterrupt (void)
{
	/* in order to detect unexpected events during development, 
	   it is recommended to set a breakpoint on the following instruction
	*/
	return;
}

extern void _stext();     /* startup routine */

struct interrupt_vector const _vectab[] = {
	{0x82, (interrupt_handler_t)_stext}, /* reset */
	{0x82, trap_int}, /* trap  Software interrupt*/
	{0x82, NonHandledInterrupt}, /* irq0  TLI External top level interrupt*/
	{0x82, NonHandledInterrupt}, /* irq1  AWU Auto wake up from halt*/
	{0x82, NonHandledInterrupt}, /* irq2  CLK Clock controller*/
	{0x82, NonHandledInterrupt}, /* irq3  EXTI0 Port A external interrupts*/
	{0x82, NonHandledInterrupt}, /* irq4  EXTI1 Port B external interrupts*/
	{0x82, NonHandledInterrupt}, /* irq5  EXTI2 Port C external interrupts*/
	{0x82, NonHandledInterrupt}, /* irq6  EXTI3 Port D external interrupts*/
	{0x82, NonHandledInterrupt}, /* irq7  EXTI4 Port E external interrupts*/
	{0x82, NonHandledInterrupt}, /* irq8  */
	{0x82, NonHandledInterrupt}, /* irq9  */
	{0x82, NonHandledInterrupt}, /* irq10 SPI End of transfer*/
	{0x82, NonHandledInterrupt}, /* irq11 TIM1 update/ overflow/underflow/ trigger/ break*/
	{0x82, NonHandledInterrupt}, /* irq12 TIM1 capture/ compare*/
	{0x82, NonHandledInterrupt}, /* irq13 TIM update/ overflow*/
	{0x82, NonHandledInterrupt}, /* irq14 TIM capture/ compare*/
	{0x82, NonHandledInterrupt}, /* irq15 TIM3 Update/ overflow*/
	{0x82, NonHandledInterrupt}, /* irq16 TIM3 Capture/ compare*/
	{0x82, NonHandledInterrupt}, /* irq17 */
	{0x82, uart1_int}, /* irq18 UART1 Ω” ’÷–∂œ*/
	{0x82, NonHandledInterrupt}, /* irq19 I2C I2C interrupt*/
	{0x82, NonHandledInterrupt}, /* irq20 UART2 Tx complete*/
	{0x82, NonHandledInterrupt}, /* irq21 UART2 Receive register DATA FULL*/
	{0x82, NonHandledInterrupt}, /* irq22 ADC1 end of conversion/analog watchdog interrupt*/
	{0x82, NonHandledInterrupt}, //Tim4_int}, /* irq23 TIM update/ overflow*/
	{0x82, NonHandledInterrupt}, /* irq24 Flash EOP/ WR_PG_DIS*/
	{0x82, NonHandledInterrupt}, /* irq25 */
	{0x82, NonHandledInterrupt}, /* irq26 */
	{0x82, NonHandledInterrupt}, /* irq27 */
	{0x82, NonHandledInterrupt}, /* irq28 */
	{0x82, NonHandledInterrupt}, /* irq29 */
};
