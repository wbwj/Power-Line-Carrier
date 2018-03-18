   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
2161                     	switch	.data
2162  0000               _sys_time:
2163  0000 0000          	dc.w	0
2164  0002               _SYS_task:
2165  0002 00            	dc.b	0
2197                     ; 15 void CLK_ini(void)
2197                     ; 16 {
2199                     	switch	.text
2200  0000               _CLK_ini:
2204                     ; 17 	*(u8*)0x4807 |= (1<<3);
2206  0000 72164807      	bset	18439,#3
2207                     ; 19 	CLK_SWR=0xb4;//切换为外部震荡;E1:HSI;D2:LSI
2209  0004 35b450c4      	mov	_CLK_SWR,#180
2210                     ; 20 	CLK_SWCR|=2;//开始切换
2212  0008 721250c5      	bset	_CLK_SWCR,#1
2213                     ; 21 	CLK_CKDIVR=0x0;//不分频
2215  000c 725f50c6      	clr	_CLK_CKDIVR
2216                     ; 25 }
2219  0010 81            	ret
2248                     ; 26 void TIM4_ini(void)
2248                     ; 27 {
2249                     	switch	.text
2250  0011               _TIM4_ini:
2254                     ; 28 	TIM4_IER = 0;//禁止中断
2256  0011 725f5343      	clr	_TIM4_IER
2257                     ; 29 	TIM4_EGR = 1;//允许产生更新事件
2259  0015 35015345      	mov	_TIM4_EGR,#1
2260                     ; 30 	TIM4_PSCR= 7;//分频
2262  0019 35075347      	mov	_TIM4_PSCR,#7
2263                     ; 31 	TIM4_ARR = 255;
2265  001d 35ff5348      	mov	_TIM4_ARR,#255
2266                     ; 32 	TIM4_CNTR= 255;
2268  0021 35ff5346      	mov	_TIM4_CNTR,#255
2269                     ; 33 	TIM4_CR1 =0x01; //允许计数工作
2271  0025 35015340      	mov	_TIM4_CR1,#1
2272                     ; 34 	TIM4_IER = 1;//禁止中断
2274  0029 35015343      	mov	_TIM4_IER,#1
2275                     ; 37 	TIM4_EGR = 1;
2277  002d 35015345      	mov	_TIM4_EGR,#1
2278                     ; 38 }
2281  0031 81            	ret
2320                     ; 40 void GPIO_init(void)
2320                     ; 41 {
2321                     	switch	.text
2322  0032               _GPIO_init:
2326                     ; 43 	PA_DDR &= ~BIT3;//输入
2328  0032 72175002      	bres	_PA_DDR,#3
2329                     ; 44 	PA_CR1 &= ~BIT3;//float
2331  0036 72175003      	bres	_PA_CR1,#3
2332                     ; 45 	PA_CR2 &= ~BIT3;//无中断
2334  003a 72175004      	bres	_PA_CR2,#3
2335                     ; 47 	PB_DDR &= ~BIT4;//输入
2337  003e 72195007      	bres	_PB_DDR,#4
2338                     ; 48 	PB_CR1 &= ~BIT4;//float
2340  0042 72195008      	bres	_PB_CR1,#4
2341                     ; 49 	PB_CR2 &= ~BIT4;//无中断
2343  0046 72195009      	bres	_PB_CR2,#4
2344                     ; 51 	PB_DDR |= BIT5;//输出
2346  004a 721a5007      	bset	_PB_DDR,#5
2347                     ; 52 	PB_CR1 |= BIT5;//推挽
2349  004e 721a5008      	bset	_PB_CR1,#5
2350                     ; 53 	PB_CR2 &= ~BIT5;//低速
2352  0052 721b5009      	bres	_PB_CR2,#5
2353                     ; 54 	PB_ODR &= ~BIT5;//输出
2355  0056 721b5005      	bres	_PB_ODR,#5
2356                     ; 57 	PC_DDR |= BIT3;//输出
2358  005a 7216500c      	bset	_PC_DDR,#3
2359                     ; 58 	PC_CR1 |= BIT3;//推挽
2361  005e 7216500d      	bset	_PC_CR1,#3
2362                     ; 59 	PC_CR2 &= ~BIT3;//低速
2364  0062 7217500e      	bres	_PC_CR2,#3
2365                     ; 60   PC_ODR |= BIT3;//输出
2367  0066 7216500a      	bset	_PC_ODR,#3
2368                     ; 62 	PC_DDR |= BIT4;//输出
2370  006a 7218500c      	bset	_PC_DDR,#4
2371                     ; 63 	PC_CR1 |= BIT4;//推挽
2373  006e 7218500d      	bset	_PC_CR1,#4
2374                     ; 64 	PC_CR2 &= ~BIT4;//低速
2376  0072 7219500e      	bres	_PC_CR2,#4
2377                     ; 65 	PC_ODR &= ~BIT4;//输出
2379  0076 7219500a      	bres	_PC_ODR,#4
2380                     ; 67 	PC_DDR &= ~BIT5;//输入
2382  007a 721b500c      	bres	_PC_DDR,#5
2383                     ; 68 	PC_CR1 &= ~BIT5;//float
2385  007e 721b500d      	bres	_PC_CR1,#5
2386                     ; 69 	PC_CR2 &= ~BIT5;//无中断
2388  0082 721b500e      	bres	_PC_CR2,#5
2389                     ; 71 	PC_DDR &= ~BIT6;//输入
2391  0086 721d500c      	bres	_PC_DDR,#6
2392                     ; 72 	PC_CR1 &= ~BIT6;//float
2394  008a 721d500d      	bres	_PC_CR1,#6
2395                     ; 73 	PC_CR2 &= ~BIT6;//无中断
2397  008e 721d500e      	bres	_PC_CR2,#6
2398                     ; 75 	PC_DDR &= ~BIT7;//输入
2400  0092 721f500c      	bres	_PC_DDR,#7
2401                     ; 76 	PC_CR1 &= ~BIT7;//float
2403  0096 721f500d      	bres	_PC_CR1,#7
2404                     ; 77 	PC_CR2 &= ~BIT7;//无中断
2406  009a 721f500e      	bres	_PC_CR2,#7
2407                     ; 79 	PD_DDR &= ~BIT3;//输入
2409  009e 72175011      	bres	_PD_DDR,#3
2410                     ; 80 	PD_CR1 &= ~BIT3;//float
2412  00a2 72175012      	bres	_PD_CR1,#3
2413                     ; 81 	PD_CR2 &= ~BIT3;//无中断
2415  00a6 72175013      	bres	_PD_CR2,#3
2416                     ; 92 	PD_DDR |= BIT2;//输出
2418  00aa 72145011      	bset	_PD_DDR,#2
2419                     ; 93 	PD_CR1 |= BIT2;//推挽
2421  00ae 72145012      	bset	_PD_CR1,#2
2422                     ; 94 	PD_CR2 &= ~BIT2;//低速
2424  00b2 72155013      	bres	_PD_CR2,#2
2425                     ; 95 	PD_ODR &= ~BIT2;
2427  00b6 7215500f      	bres	_PD_ODR,#2
2428                     ; 97 	PD_DDR |= BIT4;//输出
2430  00ba 72185011      	bset	_PD_DDR,#4
2431                     ; 98 	PD_CR1 |= BIT4;//推挽
2433  00be 72185012      	bset	_PD_CR1,#4
2434                     ; 99 	PD_CR2 &= ~BIT4;//低速
2436  00c2 72195013      	bres	_PD_CR2,#4
2437                     ; 100   PD_ODR &= ~BIT4;
2439  00c6 7219500f      	bres	_PD_ODR,#4
2440                     ; 103 	delay(1000) ;
2442  00ca ae03e8        	ldw	x,#1000
2443  00cd 89            	pushw	x
2444  00ce ae0000        	ldw	x,#0
2445  00d1 89            	pushw	x
2446  00d2 cd0000        	call	_delay
2448  00d5 5b04          	addw	sp,#4
2449                     ; 104 	PD_ODR |= BIT4;    //watch dog
2451  00d7 7218500f      	bset	_PD_ODR,#4
2452                     ; 105 	delay(100) ;
2454  00db ae0064        	ldw	x,#100
2455  00de 89            	pushw	x
2456  00df ae0000        	ldw	x,#0
2457  00e2 89            	pushw	x
2458  00e3 cd0000        	call	_delay
2460  00e6 5b04          	addw	sp,#4
2461                     ; 106 	PD_ODR &= ~BIT4;
2463  00e8 7219500f      	bres	_PD_ODR,#4
2464                     ; 108 }
2467  00ec 81            	ret
2569                     ; 111 main()
2569                     ; 112 {
2570                     	switch	.text
2571  00ed               _main:
2573  00ed 5214          	subw	sp,#20
2574       00000014      OFST:	set	20
2577                     ; 115 	s8 uart_rx_queue_data = 0 ;
2579  00ef 0f14          	clr	(OFST+0,sp)
2580                     ; 116 	u8 proc_cnt = 0 ;
2582  00f1 0f01          	clr	(OFST-19,sp)
2583                     ; 117 	u32 power_line_tx_data_h = 0 ;
2585  00f3 96            	ldw	x,sp
2586  00f4 1c000c        	addw	x,#OFST-8
2587  00f7 cd0000        	call	c_ltor
2589                     ; 118 	u32 power_line_tx_data_l = 0 ;	
2591  00fa 96            	ldw	x,sp
2592  00fb 1c0010        	addw	x,#OFST-4
2593  00fe cd0000        	call	c_ltor
2595                     ; 119 	u32 power_line_rx_data_h = 0 ;
2597  0101 ae0000        	ldw	x,#0
2598  0104 1f06          	ldw	(OFST-14,sp),x
2599  0106 ae0000        	ldw	x,#0
2600  0109 1f04          	ldw	(OFST-16,sp),x
2601                     ; 120 	u32 power_line_rx_data_l = 0 ;
2603  010b ae0000        	ldw	x,#0
2604  010e 1f0a          	ldw	(OFST-10,sp),x
2605  0110 ae0000        	ldw	x,#0
2606  0113 1f08          	ldw	(OFST-12,sp),x
2607                     ; 121 	u8 uart_rx_queue_cnt = 0 ;
2609  0115 7b03          	ld	a,(OFST-17,sp)
2610  0117 97            	ld	xl,a
2611                     ; 122 	u8 rand_delay_cnt = 0 ;
2613  0118 0f02          	clr	(OFST-18,sp)
2614                     ; 123 	delay(10000);
2616  011a ae2710        	ldw	x,#10000
2617  011d 89            	pushw	x
2618  011e ae0000        	ldw	x,#0
2619  0121 89            	pushw	x
2620  0122 cd0000        	call	_delay
2622  0125 5b04          	addw	sp,#4
2623                     ; 124 	GPIO_init();
2625  0127 cd0032        	call	_GPIO_init
2627                     ; 125 	power_line_init();	
2629  012a cd0000        	call	_power_line_init
2631                     ; 126 	CLK_ini();
2633  012d cd0000        	call	_CLK_ini
2635                     ; 128 	uart_ini(4800);
2637  0130 ae12c0        	ldw	x,#4800
2638  0133 cd0000        	call	_uart_ini
2640                     ; 131 	srand(0);
2642  0136 5f            	clrw	x
2643  0137 cd0000        	call	_srand
2645                     ; 133 	EnableInt;
2648  013a 9a            RIM
2650                     ; 135 	sys_time=0;
2652  013b 5f            	clrw	x
2653  013c cf0000        	ldw	_sys_time,x
2654  013f               L3151:
2655                     ; 139     uart_rx_queue_cnt = Queue_cnt(&uart_rx_queue) ;
2657  013f ae0000        	ldw	x,#_uart_rx_queue
2658  0142 cd0000        	call	_Queue_cnt
2660  0145 6b03          	ld	(OFST-17,sp),a
2661                     ; 140     if((uart_rx_queue_cnt >= 8) && (rand_delay_cnt == 0))
2663  0147 7b03          	ld	a,(OFST-17,sp)
2664  0149 a108          	cp	a,#8
2665  014b 2403          	jruge	L41
2666  014d cc0279        	jp	L7151
2667  0150               L41:
2669  0150 0d02          	tnz	(OFST-18,sp)
2670  0152 2703          	jreq	L61
2671  0154 cc0279        	jp	L7151
2672  0157               L61:
2673                     ; 142 			    Queue_get(&uart_rx_queue_data,&uart_rx_queue);
2675  0157 ae0000        	ldw	x,#_uart_rx_queue
2676  015a 89            	pushw	x
2677  015b 96            	ldw	x,sp
2678  015c 1c0016        	addw	x,#OFST+2
2679  015f cd0000        	call	_Queue_get
2681  0162 85            	popw	x
2682                     ; 143 			    if(((u8)uart_rx_queue_data) == 0x9b)
2684  0163 7b14          	ld	a,(OFST+0,sp)
2685  0165 a19b          	cp	a,#155
2686  0167 2703          	jreq	L02
2687  0169 cc02de        	jp	L5251
2688  016c               L02:
2689                     ; 145 					  power_line_tx_data_h = (((u32)uart_rx_queue_data) & 0xff) << 24 ;
2691  016c 7b14          	ld	a,(OFST+0,sp)
2692  016e a4ff          	and	a,#255
2693  0170 b703          	ld	c_lreg+3,a
2694  0172 3f02          	clr	c_lreg+2
2695  0174 3f01          	clr	c_lreg+1
2696  0176 3f00          	clr	c_lreg
2697  0178 a618          	ld	a,#24
2698  017a cd0000        	call	c_llsh
2700  017d 96            	ldw	x,sp
2701  017e 1c000c        	addw	x,#OFST-8
2702  0181 cd0000        	call	c_rtol
2704                     ; 146 					  Queue_get(&uart_rx_queue_data,&uart_rx_queue);	
2706  0184 ae0000        	ldw	x,#_uart_rx_queue
2707  0187 89            	pushw	x
2708  0188 96            	ldw	x,sp
2709  0189 1c0016        	addw	x,#OFST+2
2710  018c cd0000        	call	_Queue_get
2712  018f 85            	popw	x
2713                     ; 147 					  power_line_tx_data_h = power_line_tx_data_h + ((((u32)uart_rx_queue_data) & 0xff) << 16) ;
2715  0190 7b14          	ld	a,(OFST+0,sp)
2716  0192 a4ff          	and	a,#255
2717  0194 b703          	ld	c_lreg+3,a
2718  0196 3f02          	clr	c_lreg+2
2719  0198 3f01          	clr	c_lreg+1
2720  019a 3f00          	clr	c_lreg
2721  019c a610          	ld	a,#16
2722  019e cd0000        	call	c_llsh
2724  01a1 96            	ldw	x,sp
2725  01a2 1c000c        	addw	x,#OFST-8
2726  01a5 cd0000        	call	c_lgadd
2728                     ; 148 					  Queue_get(&uart_rx_queue_data,&uart_rx_queue);	
2730  01a8 ae0000        	ldw	x,#_uart_rx_queue
2731  01ab 89            	pushw	x
2732  01ac 96            	ldw	x,sp
2733  01ad 1c0016        	addw	x,#OFST+2
2734  01b0 cd0000        	call	_Queue_get
2736  01b3 85            	popw	x
2737                     ; 149 					  power_line_tx_data_h = power_line_tx_data_h + ((((u32)uart_rx_queue_data) & 0xff) << 8) ;
2739  01b4 7b14          	ld	a,(OFST+0,sp)
2740  01b6 a4ff          	and	a,#255
2741  01b8 5f            	clrw	x
2742  01b9 97            	ld	xl,a
2743  01ba 90ae0100      	ldw	y,#256
2744  01be cd0000        	call	c_umul
2746  01c1 96            	ldw	x,sp
2747  01c2 1c000c        	addw	x,#OFST-8
2748  01c5 cd0000        	call	c_lgadd
2750                     ; 150 					  Queue_get(&uart_rx_queue_data,&uart_rx_queue);	
2752  01c8 ae0000        	ldw	x,#_uart_rx_queue
2753  01cb 89            	pushw	x
2754  01cc 96            	ldw	x,sp
2755  01cd 1c0016        	addw	x,#OFST+2
2756  01d0 cd0000        	call	_Queue_get
2758  01d3 85            	popw	x
2759                     ; 151 					  power_line_tx_data_h = power_line_tx_data_h + (((u32)uart_rx_queue_data) & 0xff) ;
2761  01d4 7b14          	ld	a,(OFST+0,sp)
2762  01d6 a4ff          	and	a,#255
2763  01d8 96            	ldw	x,sp
2764  01d9 1c000c        	addw	x,#OFST-8
2765  01dc 88            	push	a
2766  01dd cd0000        	call	c_lgadc
2768  01e0 84            	pop	a
2769                     ; 153 						Queue_get(&uart_rx_queue_data,&uart_rx_queue);	
2771  01e1 ae0000        	ldw	x,#_uart_rx_queue
2772  01e4 89            	pushw	x
2773  01e5 96            	ldw	x,sp
2774  01e6 1c0016        	addw	x,#OFST+2
2775  01e9 cd0000        	call	_Queue_get
2777  01ec 85            	popw	x
2778                     ; 154 					  power_line_tx_data_l = ((((u32)uart_rx_queue_data) & 0xff) << 24) ;
2780  01ed 7b14          	ld	a,(OFST+0,sp)
2781  01ef a4ff          	and	a,#255
2782  01f1 b703          	ld	c_lreg+3,a
2783  01f3 3f02          	clr	c_lreg+2
2784  01f5 3f01          	clr	c_lreg+1
2785  01f7 3f00          	clr	c_lreg
2786  01f9 a618          	ld	a,#24
2787  01fb cd0000        	call	c_llsh
2789  01fe 96            	ldw	x,sp
2790  01ff 1c0010        	addw	x,#OFST-4
2791  0202 cd0000        	call	c_rtol
2793                     ; 155 						Queue_get(&uart_rx_queue_data,&uart_rx_queue);	
2795  0205 ae0000        	ldw	x,#_uart_rx_queue
2796  0208 89            	pushw	x
2797  0209 96            	ldw	x,sp
2798  020a 1c0016        	addw	x,#OFST+2
2799  020d cd0000        	call	_Queue_get
2801  0210 85            	popw	x
2802                     ; 156 					  power_line_tx_data_l = power_line_tx_data_l + ((((u32)uart_rx_queue_data) & 0xff) << 16) ;
2804  0211 7b14          	ld	a,(OFST+0,sp)
2805  0213 a4ff          	and	a,#255
2806  0215 b703          	ld	c_lreg+3,a
2807  0217 3f02          	clr	c_lreg+2
2808  0219 3f01          	clr	c_lreg+1
2809  021b 3f00          	clr	c_lreg
2810  021d a610          	ld	a,#16
2811  021f cd0000        	call	c_llsh
2813  0222 96            	ldw	x,sp
2814  0223 1c0010        	addw	x,#OFST-4
2815  0226 cd0000        	call	c_lgadd
2817                     ; 157 					  Queue_get(&uart_rx_queue_data,&uart_rx_queue);	
2819  0229 ae0000        	ldw	x,#_uart_rx_queue
2820  022c 89            	pushw	x
2821  022d 96            	ldw	x,sp
2822  022e 1c0016        	addw	x,#OFST+2
2823  0231 cd0000        	call	_Queue_get
2825  0234 85            	popw	x
2826                     ; 158 					  power_line_tx_data_l = power_line_tx_data_l + ((((u32)uart_rx_queue_data) & 0xff) << 8) ;
2828  0235 7b14          	ld	a,(OFST+0,sp)
2829  0237 a4ff          	and	a,#255
2830  0239 5f            	clrw	x
2831  023a 97            	ld	xl,a
2832  023b 90ae0100      	ldw	y,#256
2833  023f cd0000        	call	c_umul
2835  0242 96            	ldw	x,sp
2836  0243 1c0010        	addw	x,#OFST-4
2837  0246 cd0000        	call	c_lgadd
2839                     ; 159 					  Queue_get(&uart_rx_queue_data,&uart_rx_queue);	
2841  0249 ae0000        	ldw	x,#_uart_rx_queue
2842  024c 89            	pushw	x
2843  024d 96            	ldw	x,sp
2844  024e 1c0016        	addw	x,#OFST+2
2845  0251 cd0000        	call	_Queue_get
2847  0254 85            	popw	x
2848                     ; 160 					  power_line_tx_data_l = power_line_tx_data_l + (((u32)uart_rx_queue_data) & 0xff) ;
2850  0255 7b14          	ld	a,(OFST+0,sp)
2851  0257 a4ff          	and	a,#255
2852  0259 96            	ldw	x,sp
2853  025a 1c0010        	addw	x,#OFST-4
2854  025d 88            	push	a
2855  025e cd0000        	call	c_lgadc
2857  0261 84            	pop	a
2858                     ; 162 					  data_trans(power_line_tx_data_h,power_line_tx_data_l);
2860  0262 1e12          	ldw	x,(OFST-2,sp)
2861  0264 89            	pushw	x
2862  0265 1e12          	ldw	x,(OFST-2,sp)
2863  0267 89            	pushw	x
2864  0268 1e12          	ldw	x,(OFST-2,sp)
2865  026a 89            	pushw	x
2866  026b 1e12          	ldw	x,(OFST-2,sp)
2867  026d 89            	pushw	x
2868  026e cd0000        	call	_data_trans
2870  0271 5b08          	addw	sp,#8
2871                     ; 163 						curr_state = 0;						
2873  0273 725f0000      	clr	_curr_state
2875  0277 2065          	jra	L5251
2876  0279               L7151:
2877                     ; 169 			if(data_recv(&power_line_rx_data_h,&power_line_rx_data_l,1) != 0)
2879  0279 ae0001        	ldw	x,#1
2880  027c 89            	pushw	x
2881  027d 96            	ldw	x,sp
2882  027e 1c000a        	addw	x,#OFST-10
2883  0281 89            	pushw	x
2884  0282 96            	ldw	x,sp
2885  0283 1c0008        	addw	x,#OFST-12
2886  0286 cd0000        	call	_data_recv
2888  0289 5b04          	addw	sp,#4
2889  028b 4d            	tnz	a
2890  028c 2750          	jreq	L5251
2891                     ; 171 				Queue_set(((s8)(power_line_rx_data_h >> 24)),&uart_tx_queue);
2893  028e ae0000        	ldw	x,#_uart_tx_queue
2894  0291 89            	pushw	x
2895  0292 7b06          	ld	a,(OFST-14,sp)
2896  0294 cd0000        	call	_Queue_set
2898  0297 85            	popw	x
2899                     ; 172 				Queue_set(((s8)(power_line_rx_data_h >> 16)),&uart_tx_queue);
2901  0298 ae0000        	ldw	x,#_uart_tx_queue
2902  029b 89            	pushw	x
2903  029c 7b07          	ld	a,(OFST-13,sp)
2904  029e cd0000        	call	_Queue_set
2906  02a1 85            	popw	x
2907                     ; 173 				Queue_set(((s8)(power_line_rx_data_h >>  8)),&uart_tx_queue);
2909  02a2 ae0000        	ldw	x,#_uart_tx_queue
2910  02a5 89            	pushw	x
2911  02a6 7b08          	ld	a,(OFST-12,sp)
2912  02a8 cd0000        	call	_Queue_set
2914  02ab 85            	popw	x
2915                     ; 174 				Queue_set(((s8)(power_line_rx_data_h >>  0)),&uart_tx_queue);
2917  02ac ae0000        	ldw	x,#_uart_tx_queue
2918  02af 89            	pushw	x
2919  02b0 7b09          	ld	a,(OFST-11,sp)
2920  02b2 cd0000        	call	_Queue_set
2922  02b5 85            	popw	x
2923                     ; 176 				Queue_set(((s8)(power_line_rx_data_l >> 24)),&uart_tx_queue);
2925  02b6 ae0000        	ldw	x,#_uart_tx_queue
2926  02b9 89            	pushw	x
2927  02ba 7b0a          	ld	a,(OFST-10,sp)
2928  02bc cd0000        	call	_Queue_set
2930  02bf 85            	popw	x
2931                     ; 177 				Queue_set(((s8)(power_line_rx_data_l >> 16)),&uart_tx_queue);
2933  02c0 ae0000        	ldw	x,#_uart_tx_queue
2934  02c3 89            	pushw	x
2935  02c4 7b0b          	ld	a,(OFST-9,sp)
2936  02c6 cd0000        	call	_Queue_set
2938  02c9 85            	popw	x
2939                     ; 178 				Queue_set(((s8)(power_line_rx_data_l >>  8)),&uart_tx_queue);
2941  02ca ae0000        	ldw	x,#_uart_tx_queue
2942  02cd 89            	pushw	x
2943  02ce 7b0c          	ld	a,(OFST-8,sp)
2944  02d0 cd0000        	call	_Queue_set
2946  02d3 85            	popw	x
2947                     ; 179 				Queue_set(((s8)(power_line_rx_data_l >>  0)),&uart_tx_queue);
2949  02d4 ae0000        	ldw	x,#_uart_tx_queue
2950  02d7 89            	pushw	x
2951  02d8 7b0d          	ld	a,(OFST-7,sp)
2952  02da cd0000        	call	_Queue_set
2954  02dd 85            	popw	x
2956  02de               L5251:
2957                     ; 191 		data_to_uart();
2959  02de cd0000        	call	_data_to_uart
2962  02e1 ac3f013f      	jpf	L3151
2996                     ; 195 void wait(vu32 i)
2996                     ; 196 {
2997                     	switch	.text
2998  02e5               _wait:
3000       00000000      OFST:	set	0
3003  02e5               L3551:
3004                     ; 197 	while(i--);
3006  02e5 96            	ldw	x,sp
3007  02e6 1c0003        	addw	x,#OFST+3
3008  02e9 cd0000        	call	c_ltor
3010  02ec 96            	ldw	x,sp
3011  02ed 1c0003        	addw	x,#OFST+3
3012  02f0 a601          	ld	a,#1
3013  02f2 cd0000        	call	c_lgsbc
3015  02f5 cd0000        	call	c_lrzmp
3017  02f8 26eb          	jrne	L3551
3018                     ; 198 }
3021  02fa 81            	ret
3045                     ; 200 @far @interrupt void Tim4_int(void)//61Hz
3045                     ; 201 {
3047                     	switch	.text
3048  02fb               f_Tim4_int:
3053                     ; 202 	TIM4_SR =0;
3055  02fb 725f5344      	clr	_TIM4_SR
3056                     ; 204 }
3059  02ff 80            	iret
3081                     ; 205 @far @interrupt void trap_int(void)//软中断
3081                     ; 206 {
3082                     	switch	.text
3083  0300               f_trap_int:
3088                     ; 208 }
3091  0300 80            	iret
3119                     	xdef	f_trap_int
3120                     	xdef	f_Tim4_int
3121                     	xdef	_main
3122                     	xdef	_GPIO_init
3123                     	xdef	_TIM4_ini
3124                     	xdef	_CLK_ini
3125                     	xdef	_sys_time
3126                     	xref	_curr_state
3127                     	xref	_data_recv
3128                     	xref	_data_trans
3129                     	xref	_delay
3130                     	xref	_power_line_init
3131                     	xref	_uart_tx_queue
3132                     	xref	_uart_rx_queue
3133                     	xref	_data_to_uart
3134                     	xref	_uart_ini
3135                     	xref	_Queue_cnt
3136                     	xref	_Queue_get
3137                     	xref	_Queue_set
3138                     	xdef	_wait
3139                     	xdef	_SYS_task
3140                     	xref	_srand
3141                     	xref.b	c_lreg
3160                     	xref	c_lrzmp
3161                     	xref	c_lgsbc
3162                     	xref	c_lgadc
3163                     	xref	c_umul
3164                     	xref	c_lgadd
3165                     	xref	c_rtol
3166                     	xref	c_llsh
3167                     	xref	c_ltor
3168                     	end
