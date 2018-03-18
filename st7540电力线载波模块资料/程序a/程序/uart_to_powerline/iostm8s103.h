/*	IO DEFINITIONS FOR STM8 S103 covers F and K family
 *	Copyright (c) 2010 by COSMIC Software
 */

/*	PORTS section
 */
/*	Port A
 */
volatile char PA_ODR        @0x5000;	/* Data Output Latch reg */
volatile char PA_IDR        @0x5001;	/* Input Pin Value reg */
volatile char PA_DDR        @0x5002;	/* Data Direction */
volatile char PA_CR1        @0x5003;	/* Control register 1 */
volatile char PA_CR2        @0x5004;	/* Control register 2 */

/*	Port B
 */
volatile char PB_ODR        @0x5005;	/* Data Output Latch reg */
volatile char PB_IDR        @0x5006;	/* Input Pin Value reg */
volatile char PB_DDR        @0x5007;	/* Data Direction */
volatile char PB_CR1        @0x5008;	/* Control register 1 */
volatile char PB_CR2        @0x5009;	/* Control register 2 */

/*	Port C
 */
volatile char PC_ODR        @0x500a;	/* Data Output Latch reg */
volatile char PC_IDR        @0x500b;	/* Input Pin Value reg */
volatile char PC_DDR        @0x500c;	/* Data Direction */
volatile char PC_CR1        @0x500d;	/* Control register 1 */
volatile char PC_CR2        @0x500e;	/* Control register 2 */

/*	Port D
 */
volatile char PD_ODR        @0x500f;	/* Data Output Latch reg */
volatile char PD_IDR        @0x5010;	/* Input Pin Value reg */
volatile char PD_DDR        @0x5011;	/* Data Direction */
volatile char PD_CR1        @0x5012;	/* Control register 1 */
volatile char PD_CR2        @0x5013;	/* Control register 2 */

/*	Port E
 */
volatile char PE_ODR        @0x5014;	/* Data Output Latch reg */
volatile char PE_IDR        @0x5015;	/* Input Pin Value reg */
volatile char PE_DDR        @0x5016;	/* Data Direction */
volatile char PE_CR1        @0x5017;	/* Control register 1 */
volatile char PE_CR2        @0x5018;	/* Control register 2 */

/*	Port F
 */
volatile char PF_ODR        @0x5019;	/* Data Output Latch reg */
volatile char PF_IDR        @0x501a;	/* Input Pin Value reg */
volatile char PF_DDR        @0x501b;	/* Data Direction */
volatile char PF_CR1        @0x501c;	/* Control register 1 */
volatile char PF_CR2        @0x501d;	/* Control register 2 */

/*	FLASH section
 */
volatile char FLASH_CR1     @0x505a;	/* Flash Control Register 1 */
volatile char FLASH_CR2     @0x505b;	/* Flash Control Register 2 */
volatile char FLASH_NCR2    @0x505c;	/* Flash Complementary Control Reg 2 */
volatile char FLASH_FPR     @0x505d;	/* Flash Protection reg */
volatile char FLASH_NFPR    @0x505e;	/* Flash Complementary Protection reg */
volatile char FLASH_IAPSR   @0x505f;	/* Flash in-appl Prog. Status reg */
volatile char FLASH_PUKR    @0x5062;	/* Flash Program memory unprotection reg */
volatile char FLASH_DUKR    @0x5064;	/* Data EEPROM unprotection reg */

/*	External Interrupt Controller section
 */
volatile char EXTI_CR1      @0x50a0;	/* Ext Int Ctrl reg 1 */
volatile char EXTI_CR2      @0x50a1;	/* Ext Int Ctrl reg 2 */

/*	RSTC section
 */
volatile char RST_SR        @0x50b3;	/* Reset Status register */

/*	CLOCK section
 */
volatile char CLK_ICKCR     @0x50c0;	/* Internal Clock Control reg */
volatile char CLK_ECKCR     @0x50c1;	/* External Clock Control reg */
volatile char CLK_CMSR      @0x50c3;	/* Master Status reg */
volatile char CLK_SWR       @0x50c4;	/* Master Switch reg */
volatile char CLK_SWCR      @0x50c5;	/* Switch Control reg */
volatile char CLK_CKDIVR    @0x50c6;	/* Divider register */
volatile char CLK_PCKENR1   @0x50c7;	/* Peripheral Clock Gating reg 1 */
volatile char CLK_CSSR      @0x50c8;	/* Security System register */
volatile char CLK_CCOR      @0x50c9;	/* Configurable Clock Ctrl reg */
volatile char CLK_PCKENR2   @0x50ca;	/* Peripheral Clock Gating reg 2 */
volatile char CLK_CANCCR    @0x50cb;	/* Can Clock Control reg */
volatile char CLK_HSITRIMR  @0x50cc;	/* HSI Calibration Trimming reg */
volatile char CLK_SWIMCCR   @0x50cd;	/* SWIM Clock Control reg */

/*	WATCHDOG section
 */
volatile char WWDG_CR       @0x50d1;	/* WWDG Control register */
volatile char WWDG_WR       @0x50d2;	/* WWDG Window register */
volatile char IWDG_KR       @0x50e0;	/* IWDG Key register */
volatile char IWDG_PR       @0x50e1;	/* IWDG Prescaler register */
volatile char IWDG_RLR      @0x50e2;	/* IWDG Reload register */

/*	AWU section
 */
volatile char AWU_CSR1      @0x50f0;	/* AWU Control/Status reg 1 */
volatile char AWU_APR       @0x50f1;	/* AWU Async Prescale Buffer reg */
volatile char AWU_TBR       @0x50f2;	/* AWU Timebase selection reg */
volatile char BEEP_CSR      @0x50f3;	/* BEEP control/status reg */

/*	SPI section
 */
volatile char SPI_CR1       @0x5200;	/* SPI Control register 1 */
volatile char SPI_CR2       @0x5201;	/* SPI Control register 2 */
volatile char SPI_ICR       @0x5202;	/* SPI Interrupt/Ctrl reg */
volatile char SPI_SR        @0x5203;	/* SPI Status register */
volatile char SPI_DR        @0x5204;	/* SPI Data I/O reg */
volatile char SPI_CRCPR     @0x5205;	/* SPI CRC Polynomial reg */
volatile char SPI_RXCRCR    @0x5206;	/* SPI Rx CRC register */
volatile char SPI_TXCRCR    @0x5207;	/* SPI Tx CRC register */

/*	I2C section
 */
volatile char I2C_CR1       @0x5210;	/* Control register 1 */
volatile char I2C_CR2       @0x5211;	/* Control register 2 */
volatile char I2C_FREQR     @0x5212;	/* Frequency register */
volatile char I2C_OARL      @0x5213;	/* Own Address reg low */
volatile char I2C_OARH      @0x5214;	/* Own Address reg high */
volatile char I2C_DR        @0x5216;	/* Data Register */
volatile char I2C_SR1       @0x5217;	/* Status Register 1 */
volatile char I2C_SR2       @0x5218;	/* Status Register 2 */
volatile char I2C_SR3       @0x5219;	/* Status Register 3 */
volatile char I2C_ITR       @0x521a;	/* Interrupt Control reg */
volatile char I2C_CCRL      @0x521b;	/* Clock Control reg low */
volatile char I2C_CCRH      @0x521c;	/* Clock Control reg high */
volatile char I2C_TRISER    @0x521d;	/* Trise reg */
volatile char I2C_PECR      @0x521e;	/* Packet Error Checking reg */

/*	UART1 section
 */
volatile char UART1_SR      @0x5230;	/* Status register */
volatile char UART1_DR      @0x5231;	/* Data register */
volatile char UART1_BRR1    @0x5232;	/* Baud Rate reg 1 */
volatile char UART1_BRR2    @0x5233;	/* Baud Rate reg 2 */
volatile char UART1_CR1     @0x5234;	/* Control register 1 */
volatile char UART1_CR2     @0x5235;	/* Control register 2 */
volatile char UART1_CR3     @0x5236;	/* Control register 3 */
volatile char UART1_CR4     @0x5237;	/* Control register 4 */
volatile char UART1_CR5     @0x5238;	/* Control register 5 */
volatile char UART1_GTR     @0x5239;	/* Guard Time register */
volatile char UART1_PSCR    @0x523a;	/* Prescaler register */

/*	TIMER 1 section
 */
volatile char TIM1_CR1      @0x5250;	/* Control register 1 */
volatile char TIM1_CR2      @0x5251;	/* Control register 2 */
volatile char TIM1_SMCR     @0x5252;	/* Slave Mode Control reg */
volatile char TIM1_ETR      @0x5253;	/* External Trigger reg */
volatile char TIM1_IER      @0x5254;	/* Interrupt Enable reg */
volatile char TIM1_SR1      @0x5255;	/* Status register 1 */
volatile char TIM1_SR2      @0x5256;	/* Status register 2 */
volatile char TIM1_EGR      @0x5257;	/* Event Generation reg */
volatile char TIM1_CCMR1    @0x5258;	/* Capture/Compare Mode reg 1 */
volatile char TIM1_CCMR2    @0x5259;	/* Capture/Compare Mode reg 2 */
volatile char TIM1_CCMR3    @0x525a;	/* Capture/Compare Mode reg 3 */
volatile char TIM1_CCMR4    @0x525b;	/* Capture/Compare Mode reg 4 */
volatile char TIM1_CCER1    @0x525c;	/* Capture/Compare Enable reg 1 */
volatile char TIM1_CCER2    @0x525d;	/* Capture/Compare Enable reg 2 */
volatile char TIM1_CNTRH    @0x525e;	/* Counter reg High */
volatile char TIM1_CNTRL    @0x525f;	/* Counter reg Low */
volatile char TIM1_PSCRH    @0x5260;	/* Prescaler reg High */
volatile char TIM1_PSCRL    @0x5261;	/* Prescaler reg Low */
volatile char TIM1_ARRH     @0x5262;	/* Auto-reload reg High */
volatile char TIM1_ARRL     @0x5263;	/* Auto-reload reg Low */
volatile char TIM1_RCR      @0x5264;	/* Repetition Counter reg */
volatile char TIM1_CCR1H    @0x5265;	/* Capture/Compare reg 1 High */
volatile char TIM1_CCR1L    @0x5266;	/* Capture/Compare reg 1 Low */
volatile char TIM1_CCR2H    @0x5267;	/* Capture/Compare reg 2 High */
volatile char TIM1_CCR2L    @0x5268;	/* Capture/Compare reg 2 Low */
volatile char TIM1_CCR3H    @0x5269;	/* Capture/Compare reg 3 High */
volatile char TIM1_CCR3L    @0x526a;	/* Capture/Compare reg 3 Low */
volatile char TIM1_CCR4H    @0x526b;	/* Capture/Compare reg 4 High */
volatile char TIM1_CCR4L    @0x526c;	/* Capture/Compare reg 4 Low */
volatile char TIM1_BKR      @0x526d;	/* Break register */
volatile char TIM1_DTR      @0x526e;	/* Dead Time register */
volatile char TIM1_OISR     @0x526f;	/* Output Idle State reg */

/*	TIMER 2 section
 */
volatile char TIM2_CR1      @0x5300;	/* Control register 1 */
volatile char TIM2_IER      @0x5303;	/* Interrupt Enable reg */
volatile char TIM2_SR1      @0x5304;	/* Status Register 1 */
volatile char TIM2_SR2      @0x5305;	/* Status Register 2 */
volatile char TIM2_EGR      @0x5306;	/* Event Generation reg */
volatile char TIM2_CCMR1    @0x5307;	/* Capture/Compare Mode reg 1 */
volatile char TIM2_CCMR2    @0x5308;	/* Capture/Compare Mode reg 2 */
volatile char TIM2_CCMR3    @0x5309;	/* Capture/Compare Mode reg 3 */
volatile char TIM2_CCER1    @0x530a;	/* Capture/Compare Enable reg 1 */
volatile char TIM2_CCER2    @0x530b;	/* Capture/Compare Enable reg 2 */
volatile char TIM2_CNTRH    @0x530c;	/* Counter reg High */
volatile char TIM2_CNTRL    @0x530d;	/* Counter reg Low */
volatile char TIM2_PSCR     @0x530e;	/* Prescaler register */
volatile char TIM2_ARRH     @0x530f;	/* Auto-reload reg High */
volatile char TIM2_ARRL     @0x5310;	/* Auto-reload reg Low */
volatile char TIM2_CCR1H    @0x5311;	/* Capture/Compare Reg 1 High */
volatile char TIM2_CCR1L    @0x5312;	/* Capture/Compare Reg 1 Low */
volatile char TIM2_CCR2H    @0x5313;	/* Capture/Compare Reg 2 High */
volatile char TIM2_CCR2L    @0x5314;	/* Capture/Compare Reg 2 Low */
volatile char TIM2_CCR3H    @0x5315;	/* Capture/Compare Reg 3 High */
volatile char TIM2_CCR3L    @0x5316;	/* Capture/Compare Reg 3 Low */

/*	TIMER 4 section
 */
volatile char TIM4_CR1      @0x5340;	/* Control register 1 */
volatile char TIM4_IER      @0x5343;	/* Interrupt enable reg */
volatile char TIM4_SR       @0x5344;	/* Status register */
volatile char TIM4_EGR      @0x5345;	/* Event Generation reg */
volatile char TIM4_CNTR     @0x5346;	/* Counter register */
volatile char TIM4_PSCR     @0x5347;	/* Prescaler register */
volatile char TIM4_ARR      @0x5348;	/* Auto-reload register */

/*	ADC section
 */
volatile char ADC_DBR[19]   @0x53e0;	/* data buffer registers */
volatile char ADC_CSR       @0x5400;	/* Control/Status reg */
volatile char ADC_CR1       @0x5401;	/* Configuration reg 1 */
volatile char ADC_CR2       @0x5402;	/* Configuration reg 2 */
volatile char ADC_CR3       @0x5403;	/* Configuration reg 3 */
volatile char ADC_DRH       @0x5404;	/* Data reg high */
volatile char ADC_DRL       @0x5405;	/* Data reg low */
volatile char ADC_TDRH      @0x5406;	/* Schmitt Trigger Disable reg high */
volatile char ADC_TDRL      @0x5407;	/* Schmitt Trigger Disable reg low */
volatile char ADC_HTRH      @0x5408;	/* high threshold reg high */
volatile char ADC_HTRL      @0x5409;	/* high threshold reg low */
volatile char ADC_LTRH      @0x540a;	/* low threshold reg high */
volatile char ADC_LTRL      @0x540b;	/* low threshold reg low */
volatile char ADC_AWSRH     @0x540c;	/* analog watchdog status reg high */
volatile char ADC_AWSRL     @0x540d;	/* analog watchdog status reg low */
volatile char ADC_AWCRH     @0x540e;	/* analog watchdog control reg high */
volatile char ADC_AWCRL     @0x540f;	/* analog watchdog control reg low */

/*	CFG section
 */
volatile char CFG_GCR       @0x7f60;	/* Global Configuration register */

/*	ITC section
 */
volatile char ITC_SPR1      @0x7f70;	/* Interrupt Software Priority Reg 1 */
volatile char ITC_SPR2      @0x7f71;	/* Interrupt Software Priority Reg 2 */
volatile char ITC_SPR3      @0x7f72;	/* Interrupt Software Priority Reg 3 */
volatile char ITC_SPR4      @0x7f73;	/* Interrupt Software Priority Reg 4 */
volatile char ITC_SPR5      @0x7f74;	/* Interrupt Software Priority Reg 5 */
volatile char ITC_SPR6      @0x7f75;	/* Interrupt Software Priority Reg 6 */
volatile char ITC_SPR7      @0x7f76;	/* Interrupt Software Priority Reg 7 */
volatile char ITC_SPR8      @0x7f77;	/* Interrupt Software Priority Reg 8 */

/*	SWIM section
 */
volatile char SWIM_CSR      @0x7f80;	/* SWIM Control Status Register */

/*	In-Circuit Debugging section 
 */                                            
volatile char DM_BK1RE      @0x7f90;	/* IC Debugging Breakpoint 1 ESB Reg */
volatile char DM_BK1RH      @0x7f91;	/* IC Debugging Breakpoint 1 High */
volatile char DM_BK1RL      @0x7f92;	/* IC Debugging Breakpoint 1 Low */
volatile char DM_BK2RE      @0x7f93;	/* IC Debugging Breakpoint 2 ESB Reg */
volatile char DM_BK2RH      @0x7f94;	/* IC Debugging Breakpoint 2 High */
volatile char DM_BK2RL      @0x7f95;	/* IC Debugging Breakpoint 2 Low */
volatile char DM_CR1        @0x7f96;	/* IC Debugging Control register 1 */
volatile char DM_CR2        @0x7f97;	/* IC Debugging Control register 2 */
volatile char DM_CSR1       @0x7f98;	/* IC Debugging Control/Status reg 1 */
volatile char DM_CSR2       @0x7f99;	/* IC Debugging Control/Status reg 2 */
volatile char DM_ENFCTR     @0x7f9a;	/* IC Debugging Function Enable Reg 2 */
