   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
2161                     	switch	.data
2162  0000               _curr_state:
2163  0000 00            	dc.b	0
2164                     .const:	section	.text
2165  0000               _next_state_table:
2166  0000 00            	dc.b	0
2167  0001 01            	dc.b	1
2168  0002 01            	dc.b	1
2169  0003 00            	dc.b	0
2170  0004 02            	dc.b	2
2171  0005 01            	dc.b	1
2172  0006 03            	dc.b	3
2173  0007 02            	dc.b	2
2213                     ; 12 void delay(volatile unsigned long delay_time)   //delay(1000) is about 2.28ms at 16MHz
2213                     ; 13 {
2215                     	switch	.text
2216  0000               _delay:
2218       00000000      OFST:	set	0
2221  0000               L1541:
2222                     ; 14 	while(delay_time--) ;
2224  0000 96            	ldw	x,sp
2225  0001 1c0003        	addw	x,#OFST+3
2226  0004 cd0000        	call	c_ltor
2228  0007 96            	ldw	x,sp
2229  0008 1c0003        	addw	x,#OFST+3
2230  000b a601          	ld	a,#1
2231  000d cd0000        	call	c_lgsbc
2233  0010 cd0000        	call	c_lrzmp
2235  0013 26eb          	jrne	L1541
2236                     ; 15 }
2239  0015 81            	ret
2276                     ; 17 void power_line_init(void)
2276                     ; 18 {
2277                     	switch	.text
2278  0016               _power_line_init:
2280  0016 5204          	subw	sp,#4
2281       00000004      OFST:	set	4
2284                     ; 22   reg_value = 0x1362d7;
2286  0018 ae62d7        	ldw	x,#25303
2287  001b 1f03          	ldw	(OFST-1,sp),x
2288  001d ae0013        	ldw	x,#19
2289  0020 1f01          	ldw	(OFST-3,sp),x
2290                     ; 24   reg_value &= 0xffffffbf ;        //clr watch dog
2292  0022 7b04          	ld	a,(OFST+0,sp)
2293  0024 a4bf          	and	a,#191
2294  0026 6b04          	ld	(OFST+0,sp),a
2295                     ; 25   reg_value &= 0xffffbfff;         //sel sync mode
2297  0028 7b03          	ld	a,(OFST-1,sp)
2298  002a a4bf          	and	a,#191
2299  002c 6b03          	ld	(OFST-1,sp),a
2300                     ; 26  	reg_value |= 0x18 ;              //set baud rate to 4800
2302  002e 7b04          	ld	a,(OFST+0,sp)
2303  0030 aa18          	or	a,#24
2304  0032 6b04          	ld	(OFST+0,sp),a
2305                     ; 27 	reg_value &= 0xfffe7fff;         //set MCLK to 16MHz
2307  0034 7b03          	ld	a,(OFST-1,sp)
2308  0036 a47f          	and	a,#127
2309  0038 6b03          	ld	(OFST-1,sp),a
2310  003a 7b02          	ld	a,(OFST-2,sp)
2311  003c a4fe          	and	a,#254
2312  003e 6b02          	ld	(OFST-2,sp),a
2313                     ; 28 	reg_value &= 0xfffffffe;         //set MCLK to 110KHz
2315  0040 7b04          	ld	a,(OFST+0,sp)
2316  0042 a4fe          	and	a,#254
2317  0044 6b04          	ld	(OFST+0,sp),a
2318                     ; 29 	reg_value |= 0x00800000;         //enable input filter
2320  0046 7b02          	ld	a,(OFST-2,sp)
2321  0048 aa80          	or	a,#128
2322  004a 6b02          	ld	(OFST-2,sp),a
2323                     ; 31 	write_ctrl_reg(reg_value);
2325  004c ae229e        	ldw	x,#8862
2326  004f 89            	pushw	x
2327  0050 ae0092        	ldw	x,#146
2328  0053 89            	pushw	x
2329  0054 cd0110        	call	_write_ctrl_reg
2331  0057 5b04          	addw	sp,#4
2332                     ; 32   delay(500);
2334  0059 ae01f4        	ldw	x,#500
2335  005c 89            	pushw	x
2336  005d ae0000        	ldw	x,#0
2337  0060 89            	pushw	x
2338  0061 ad9d          	call	_delay
2340  0063 5b04          	addw	sp,#4
2342  0065 2017          	jra	L3741
2343  0067               L1741:
2344                     ; 35 		 write_ctrl_reg(reg_value);
2346  0067 1e03          	ldw	x,(OFST-1,sp)
2347  0069 89            	pushw	x
2348  006a 1e03          	ldw	x,(OFST-1,sp)
2349  006c 89            	pushw	x
2350  006d cd0110        	call	_write_ctrl_reg
2352  0070 5b04          	addw	sp,#4
2353                     ; 36 		 delay(500);
2355  0072 ae01f4        	ldw	x,#500
2356  0075 89            	pushw	x
2357  0076 ae0000        	ldw	x,#0
2358  0079 89            	pushw	x
2359  007a ad84          	call	_delay
2361  007c 5b04          	addw	sp,#4
2362  007e               L3741:
2363                     ; 33 	while(reg_value != read_ctrl_reg())
2365  007e ad18          	call	_read_ctrl_reg
2367  0080 96            	ldw	x,sp
2368  0081 1c0001        	addw	x,#OFST-3
2369  0084 cd0000        	call	c_lcmp
2371  0087 26de          	jrne	L1741
2372                     ; 39 	PC_CR2 |= BIT6;//ÓÐÖÐ¶Ï
2374  0089 721c500e      	bset	_PC_CR2,#6
2375                     ; 40 	EXTI_CR1 &= ~BIT5 ;
2377  008d 721b50a0      	bres	_EXTI_CR1,#5
2378                     ; 41 	EXTI_CR1 |=  BIT4 ;
2380  0091 721850a0      	bset	_EXTI_CR1,#4
2381                     ; 44 }
2384  0095 5b04          	addw	sp,#4
2385  0097 81            	ret
2436                     ; 46 unsigned long read_ctrl_reg(void)
2436                     ; 47 {
2437                     	switch	.text
2438  0098               _read_ctrl_reg:
2440  0098 520a          	subw	sp,#10
2441       0000000a      OFST:	set	10
2444                     ; 48 	unsigned long return_value = 0;
2446  009a ae0000        	ldw	x,#0
2447  009d 1f03          	ldw	(OFST-7,sp),x
2448  009f ae0000        	ldw	x,#0
2449  00a2 1f01          	ldw	(OFST-9,sp),x
2450                     ; 49 	unsigned long curr_bit = 0 ;
2452  00a4 96            	ldw	x,sp
2453  00a5 1c0005        	addw	x,#OFST-5
2454  00a8 cd0000        	call	c_ltor
2456                     ; 51     set_pin_rxtx(0x1);     //set pin rxtx
2458  00ab a601          	ld	a,#1
2459  00ad cd03d3        	call	_set_pin_rxtx
2461                     ; 52     wait_posedge(); //find posedge for Tcr
2463  00b0 cd037f        	call	_wait_posedge
2465                     ; 53     _asm("NOP");
2468  00b3 9d            NOP
2470                     ; 54 		_asm("NOP");
2473  00b4 9d            NOP
2475                     ; 56     set_pin_reg_data(0x1);  //set pin reg_data
2477  00b5 a601          	ld	a,#1
2478  00b7 cd03c5        	call	_set_pin_reg_data
2480                     ; 57     set_pin_rxtx(0x1);     //set pin rxtx
2482  00ba a601          	ld	a,#1
2483  00bc cd03d3        	call	_set_pin_rxtx
2485                     ; 59 	for(i=0;i<=23;i++)
2487  00bf 5f            	clrw	x
2488  00c0 1f09          	ldw	(OFST-1,sp),x
2489  00c2               L7151:
2490                     ; 61 		wait_posedge();
2492  00c2 cd037f        	call	_wait_posedge
2494                     ; 63 		curr_bit = get_pin_rxd() ;
2496  00c5 cd03ab        	call	_get_pin_rxd
2498  00c8 6b08          	ld	(OFST-2,sp),a
2499  00ca 4f            	clr	a
2500  00cb 6b07          	ld	(OFST-3,sp),a
2501  00cd 6b06          	ld	(OFST-4,sp),a
2502  00cf 6b05          	ld	(OFST-5,sp),a
2503                     ; 65 		return_value |= ((curr_bit) << (23-i)) ;
2505  00d1 96            	ldw	x,sp
2506  00d2 1c0005        	addw	x,#OFST-5
2507  00d5 cd0000        	call	c_ltor
2509  00d8 a617          	ld	a,#23
2510  00da 100a          	sub	a,(OFST+0,sp)
2511  00dc cd0000        	call	c_llsh
2513  00df 96            	ldw	x,sp
2514  00e0 1c0001        	addw	x,#OFST-9
2515  00e3 cd0000        	call	c_lgor
2517                     ; 59 	for(i=0;i<=23;i++)
2519  00e6 1e09          	ldw	x,(OFST-1,sp)
2520  00e8 1c0001        	addw	x,#1
2521  00eb 1f09          	ldw	(OFST-1,sp),x
2524  00ed 9c            	rvf
2525  00ee 1e09          	ldw	x,(OFST-1,sp)
2526  00f0 a30018        	cpw	x,#24
2527  00f3 2fcd          	jrslt	L7151
2528                     ; 70 	set_pin_reg_data(0x0);   //clr pin reg_data
2530  00f5 4f            	clr	a
2531  00f6 cd03c5        	call	_set_pin_reg_data
2533                     ; 72 	delay(1000) ;
2535  00f9 ae03e8        	ldw	x,#1000
2536  00fc 89            	pushw	x
2537  00fd ae0000        	ldw	x,#0
2538  0100 89            	pushw	x
2539  0101 cd0000        	call	_delay
2541  0104 5b04          	addw	sp,#4
2542                     ; 73 	return return_value ;
2544  0106 96            	ldw	x,sp
2545  0107 1c0001        	addw	x,#OFST-9
2546  010a cd0000        	call	c_ltor
2550  010d 5b0a          	addw	sp,#10
2551  010f 81            	ret
2595                     ; 76 void write_ctrl_reg(unsigned long write_data)
2595                     ; 77 {
2596                     	switch	.text
2597  0110               _write_ctrl_reg:
2599  0110 89            	pushw	x
2600       00000002      OFST:	set	2
2603                     ; 80     wait_posedge();   //find posedge for Tcr
2605  0111 cd037f        	call	_wait_posedge
2607                     ; 81     _asm("NOP");
2610  0114 9d            NOP
2612                     ; 82 		_asm("NOP");
2615  0115 9d            NOP
2617                     ; 84     set_pin_reg_data(0x1);   //set pin reg_data
2619  0116 a601          	ld	a,#1
2620  0118 cd03c5        	call	_set_pin_reg_data
2622                     ; 85     set_pin_rxtx(0x0);   //clr pin rxtx
2624  011b 4f            	clr	a
2625  011c cd03d3        	call	_set_pin_rxtx
2627                     ; 89 	for(i=0;i<=23;i++) //low code effciency may result in wrong writing
2629  011f 5f            	clrw	x
2630  0120 1f01          	ldw	(OFST-1,sp),x
2631  0122               L3451:
2632                     ; 91 		if(((write_data >> (23-i)) & 0x1) == 0)
2634  0122 96            	ldw	x,sp
2635  0123 1c0005        	addw	x,#OFST+3
2636  0126 cd0000        	call	c_ltor
2638  0129 a617          	ld	a,#23
2639  012b 1002          	sub	a,(OFST+0,sp)
2640  012d cd0000        	call	c_lursh
2642  0130 b603          	ld	a,c_lreg+3
2643  0132 a501          	bcp	a,#1
2644  0134 2606          	jrne	L1551
2645                     ; 93 			set_pin_txd(0x0);
2647  0136 4f            	clr	a
2648  0137 cd03b7        	call	_set_pin_txd
2651  013a 2005          	jra	L3551
2652  013c               L1551:
2653                     ; 97 			set_pin_txd(0x1);
2655  013c a601          	ld	a,#1
2656  013e cd03b7        	call	_set_pin_txd
2658  0141               L3551:
2659                     ; 100 		wait_posedge();
2661  0141 cd037f        	call	_wait_posedge
2663                     ; 89 	for(i=0;i<=23;i++) //low code effciency may result in wrong writing
2665  0144 1e01          	ldw	x,(OFST-1,sp)
2666  0146 1c0001        	addw	x,#1
2667  0149 1f01          	ldw	(OFST-1,sp),x
2670  014b 9c            	rvf
2671  014c 1e01          	ldw	x,(OFST-1,sp)
2672  014e a30018        	cpw	x,#24
2673  0151 2fcf          	jrslt	L3451
2674                     ; 105 	set_pin_rxtx(0x1);    //set pin rxtx
2676  0153 a601          	ld	a,#1
2677  0155 cd03d3        	call	_set_pin_rxtx
2679                     ; 106 	set_pin_reg_data(0x0);   //clr pin reg_data
2681  0158 4f            	clr	a
2682  0159 cd03c5        	call	_set_pin_reg_data
2684                     ; 107 	set_pin_txd(0x0);
2686  015c 4f            	clr	a
2687  015d cd03b7        	call	_set_pin_txd
2689                     ; 108 	delay(1000) ;
2691  0160 ae03e8        	ldw	x,#1000
2692  0163 89            	pushw	x
2693  0164 ae0000        	ldw	x,#0
2694  0167 89            	pushw	x
2695  0168 cd0000        	call	_delay
2697  016b 5b04          	addw	sp,#4
2698                     ; 109 }
2701  016d 85            	popw	x
2702  016e 81            	ret
2761                     ; 111 void data_trans(unsigned long tx_data_h,unsigned long tx_data_l)
2761                     ; 112 {
2762                     	switch	.text
2763  016f               _data_trans:
2765  016f 5206          	subw	sp,#6
2766       00000006      OFST:	set	6
2769                     ; 115 	tx_data_real = tx_data_h ;
2771  0171 1e0b          	ldw	x,(OFST+5,sp)
2772  0173 1f03          	ldw	(OFST-3,sp),x
2773  0175 1e09          	ldw	x,(OFST+3,sp)
2774  0177 1f01          	ldw	(OFST-5,sp),x
2775                     ; 117 	wait_posedge();    //find posedge for Tcr
2777  0179 cd037f        	call	_wait_posedge
2779                     ; 119   delay(100) ;
2781  017c ae0064        	ldw	x,#100
2782  017f 89            	pushw	x
2783  0180 ae0000        	ldw	x,#0
2784  0183 89            	pushw	x
2785  0184 cd0000        	call	_delay
2787  0187 5b04          	addw	sp,#4
2788                     ; 120   set_pin_reg_data(0x0);   //clr pin reg_data
2790  0189 4f            	clr	a
2791  018a cd03c5        	call	_set_pin_reg_data
2793                     ; 121   set_pin_rxtx(0x0);  //clr pin rxtx
2795  018d 4f            	clr	a
2796  018e cd03d3        	call	_set_pin_rxtx
2798                     ; 123 	delay(50) ;       //delay for Tcc
2800  0191 ae0032        	ldw	x,#50
2801  0194 89            	pushw	x
2802  0195 ae0000        	ldw	x,#0
2803  0198 89            	pushw	x
2804  0199 cd0000        	call	_delay
2806  019c 5b04          	addw	sp,#4
2807                     ; 125 	for(i=0;i<=31;i++) //low code effciency may result in wrong writing
2809  019e 5f            	clrw	x
2810  019f 1f05          	ldw	(OFST-1,sp),x
2811  01a1               L7751:
2812                     ; 127 		if(((tx_data_real >> (31-i)) & 0x1) == 0)
2814  01a1 96            	ldw	x,sp
2815  01a2 1c0001        	addw	x,#OFST-5
2816  01a5 cd0000        	call	c_ltor
2818  01a8 a61f          	ld	a,#31
2819  01aa 1006          	sub	a,(OFST+0,sp)
2820  01ac cd0000        	call	c_lursh
2822  01af b603          	ld	a,c_lreg+3
2823  01b1 a501          	bcp	a,#1
2824  01b3 2606          	jrne	L5061
2825                     ; 129 			set_pin_txd(0x0);
2827  01b5 4f            	clr	a
2828  01b6 cd03b7        	call	_set_pin_txd
2831  01b9 2005          	jra	L7061
2832  01bb               L5061:
2833                     ; 133 			set_pin_txd(0x1);
2835  01bb a601          	ld	a,#1
2836  01bd cd03b7        	call	_set_pin_txd
2838  01c0               L7061:
2839                     ; 135 		wait_posedge();
2841  01c0 cd037f        	call	_wait_posedge
2843                     ; 136 		wait_negedge();
2845  01c3 cd0395        	call	_wait_negedge
2847                     ; 125 	for(i=0;i<=31;i++) //low code effciency may result in wrong writing
2849  01c6 1e05          	ldw	x,(OFST-1,sp)
2850  01c8 1c0001        	addw	x,#1
2851  01cb 1f05          	ldw	(OFST-1,sp),x
2854  01cd 9c            	rvf
2855  01ce 1e05          	ldw	x,(OFST-1,sp)
2856  01d0 a30020        	cpw	x,#32
2857  01d3 2fcc          	jrslt	L7751
2858                     ; 140   tx_data_real = tx_data_l ;
2860  01d5 1e0f          	ldw	x,(OFST+9,sp)
2861  01d7 1f03          	ldw	(OFST-3,sp),x
2862  01d9 1e0d          	ldw	x,(OFST+7,sp)
2863  01db 1f01          	ldw	(OFST-5,sp),x
2864                     ; 141 	for(i=0;i<=31;i++) //low code effciency may result in wrong writing
2866  01dd 5f            	clrw	x
2867  01de 1f05          	ldw	(OFST-1,sp),x
2868  01e0               L1161:
2869                     ; 143 		if(((tx_data_real >> (31-i)) & 0x1) == 0)
2871  01e0 96            	ldw	x,sp
2872  01e1 1c0001        	addw	x,#OFST-5
2873  01e4 cd0000        	call	c_ltor
2875  01e7 a61f          	ld	a,#31
2876  01e9 1006          	sub	a,(OFST+0,sp)
2877  01eb cd0000        	call	c_lursh
2879  01ee b603          	ld	a,c_lreg+3
2880  01f0 a501          	bcp	a,#1
2881  01f2 2606          	jrne	L7161
2882                     ; 145 			set_pin_txd(0x0);
2884  01f4 4f            	clr	a
2885  01f5 cd03b7        	call	_set_pin_txd
2888  01f8 2005          	jra	L1261
2889  01fa               L7161:
2890                     ; 149 			set_pin_txd(0x1);
2892  01fa a601          	ld	a,#1
2893  01fc cd03b7        	call	_set_pin_txd
2895  01ff               L1261:
2896                     ; 151 		wait_posedge();
2898  01ff cd037f        	call	_wait_posedge
2900                     ; 152 		wait_negedge();
2902  0202 cd0395        	call	_wait_negedge
2904                     ; 141 	for(i=0;i<=31;i++) //low code effciency may result in wrong writing
2906  0205 1e05          	ldw	x,(OFST-1,sp)
2907  0207 1c0001        	addw	x,#1
2908  020a 1f05          	ldw	(OFST-1,sp),x
2911  020c 9c            	rvf
2912  020d 1e05          	ldw	x,(OFST-1,sp)
2913  020f a30020        	cpw	x,#32
2914  0212 2fcc          	jrslt	L1161
2915                     ; 156 	set_pin_rxtx(0x1);   //set pin rxtx
2917  0214 a601          	ld	a,#1
2918  0216 cd03d3        	call	_set_pin_rxtx
2920                     ; 157 	set_pin_reg_data(0x0);   //clr pin reg_data
2922  0219 4f            	clr	a
2923  021a cd03c5        	call	_set_pin_reg_data
2925                     ; 158 	set_pin_txd(0x0);
2927  021d 4f            	clr	a
2928  021e cd03b7        	call	_set_pin_txd
2930                     ; 159 	delay(1000) ;
2932  0221 ae03e8        	ldw	x,#1000
2933  0224 89            	pushw	x
2934  0225 ae0000        	ldw	x,#0
2935  0228 89            	pushw	x
2936  0229 cd0000        	call	_delay
2938  022c 5b04          	addw	sp,#4
2939                     ; 161 }
2942  022e 5b06          	addw	sp,#6
2943  0230 81            	ret
3019                     ; 163 unsigned char data_recv(unsigned long *return_value_h,unsigned long *return_value_l,int try_times)
3019                     ; 164 {
3020                     	switch	.text
3021  0231               _data_recv:
3023  0231 89            	pushw	x
3024  0232 5207          	subw	sp,#7
3025       00000007      OFST:	set	7
3028                     ; 166 	unsigned long curr_bit = 0 ;
3030  0234 96            	ldw	x,sp
3031  0235 1c0002        	addw	x,#OFST-5
3032  0238 cd0000        	call	c_ltor
3034                     ; 167 	unsigned char  frame_head_find = 0 ;
3036  023b 7b01          	ld	a,(OFST-6,sp)
3037  023d 97            	ld	xl,a
3038                     ; 171   *return_value_h = 0x0;
3040  023e 1e08          	ldw	x,(OFST+1,sp)
3041  0240 a600          	ld	a,#0
3042  0242 e703          	ld	(3,x),a
3043  0244 a600          	ld	a,#0
3044  0246 e702          	ld	(2,x),a
3045  0248 a600          	ld	a,#0
3046  024a e701          	ld	(1,x),a
3047  024c a600          	ld	a,#0
3048  024e f7            	ld	(x),a
3049                     ; 172 	*return_value_l = 0x0;
3051  024f 1e0c          	ldw	x,(OFST+5,sp)
3052  0251 a600          	ld	a,#0
3053  0253 e703          	ld	(3,x),a
3054  0255 a600          	ld	a,#0
3055  0257 e702          	ld	(2,x),a
3056  0259 a600          	ld	a,#0
3057  025b e701          	ld	(1,x),a
3058  025d a600          	ld	a,#0
3059  025f f7            	ld	(x),a
3060                     ; 180 	frame_head_find = frame_head_srch(try_times);
3062  0260 1e0e          	ldw	x,(OFST+7,sp)
3063  0262 cd0300        	call	_frame_head_srch
3065  0265 6b01          	ld	(OFST-6,sp),a
3066                     ; 182 	if(frame_head_find == 0)
3068  0267 0d01          	tnz	(OFST-6,sp)
3069  0269 2605          	jrne	L5561
3070                     ; 184 		return 0 ;
3072  026b 4f            	clr	a
3074  026c acfd02fd      	jpf	L02
3075  0270               L5561:
3076                     ; 187 	*return_value_h = (((unsigned long)FRAME_HEAD) << 24) ;
3078  0270 1e08          	ldw	x,(OFST+1,sp)
3079  0272 a600          	ld	a,#0
3080  0274 e703          	ld	(3,x),a
3081  0276 a600          	ld	a,#0
3082  0278 e702          	ld	(2,x),a
3083  027a a600          	ld	a,#0
3084  027c e701          	ld	(1,x),a
3085  027e a69b          	ld	a,#155
3086  0280 f7            	ld	(x),a
3087                     ; 189 	for(i=0;i<=23;i++)
3089  0281 5f            	clrw	x
3090  0282 1f06          	ldw	(OFST-1,sp),x
3091  0284               L7561:
3092                     ; 191 		wait_posedge(); //find posedge
3094  0284 cd037f        	call	_wait_posedge
3096                     ; 192 		curr_bit = get_pin_rxd() ;
3098  0287 cd03ab        	call	_get_pin_rxd
3100  028a 6b05          	ld	(OFST-2,sp),a
3101  028c 4f            	clr	a
3102  028d 6b04          	ld	(OFST-3,sp),a
3103  028f 6b03          	ld	(OFST-4,sp),a
3104  0291 6b02          	ld	(OFST-5,sp),a
3105                     ; 193     *return_value_h |= ((curr_bit) << (23-i)) ;
3107  0293 1e08          	ldw	x,(OFST+1,sp)
3108  0295 7b05          	ld	a,(OFST-2,sp)
3109  0297 b703          	ld	c_lreg+3,a
3110  0299 7b04          	ld	a,(OFST-3,sp)
3111  029b b702          	ld	c_lreg+2,a
3112  029d 7b03          	ld	a,(OFST-4,sp)
3113  029f b701          	ld	c_lreg+1,a
3114  02a1 7b02          	ld	a,(OFST-5,sp)
3115  02a3 b700          	ld	c_lreg,a
3116  02a5 a617          	ld	a,#23
3117  02a7 1007          	sub	a,(OFST+0,sp)
3118  02a9 cd0000        	call	c_llsh
3120  02ac cd0000        	call	c_lgor
3122                     ; 189 	for(i=0;i<=23;i++)
3124  02af 1e06          	ldw	x,(OFST-1,sp)
3125  02b1 1c0001        	addw	x,#1
3126  02b4 1f06          	ldw	(OFST-1,sp),x
3129  02b6 9c            	rvf
3130  02b7 1e06          	ldw	x,(OFST-1,sp)
3131  02b9 a30018        	cpw	x,#24
3132  02bc 2fc6          	jrslt	L7561
3133                     ; 196 	for(i=0;i<=31;i++)
3135  02be 5f            	clrw	x
3136  02bf 1f06          	ldw	(OFST-1,sp),x
3137  02c1               L5661:
3138                     ; 198 		wait_posedge(); //find posedge
3140  02c1 cd037f        	call	_wait_posedge
3142                     ; 199 		curr_bit = get_pin_rxd() ;
3144  02c4 cd03ab        	call	_get_pin_rxd
3146  02c7 6b05          	ld	(OFST-2,sp),a
3147  02c9 4f            	clr	a
3148  02ca 6b04          	ld	(OFST-3,sp),a
3149  02cc 6b03          	ld	(OFST-4,sp),a
3150  02ce 6b02          	ld	(OFST-5,sp),a
3151                     ; 200     *return_value_l |= ((curr_bit) << (31-i)) ;
3153  02d0 1e0c          	ldw	x,(OFST+5,sp)
3154  02d2 7b05          	ld	a,(OFST-2,sp)
3155  02d4 b703          	ld	c_lreg+3,a
3156  02d6 7b04          	ld	a,(OFST-3,sp)
3157  02d8 b702          	ld	c_lreg+2,a
3158  02da 7b03          	ld	a,(OFST-4,sp)
3159  02dc b701          	ld	c_lreg+1,a
3160  02de 7b02          	ld	a,(OFST-5,sp)
3161  02e0 b700          	ld	c_lreg,a
3162  02e2 a61f          	ld	a,#31
3163  02e4 1007          	sub	a,(OFST+0,sp)
3164  02e6 cd0000        	call	c_llsh
3166  02e9 cd0000        	call	c_lgor
3168                     ; 196 	for(i=0;i<=31;i++)
3170  02ec 1e06          	ldw	x,(OFST-1,sp)
3171  02ee 1c0001        	addw	x,#1
3172  02f1 1f06          	ldw	(OFST-1,sp),x
3175  02f3 9c            	rvf
3176  02f4 1e06          	ldw	x,(OFST-1,sp)
3177  02f6 a30020        	cpw	x,#32
3178  02f9 2fc6          	jrslt	L5661
3179                     ; 208 	return 1 ;
3181  02fb a601          	ld	a,#1
3183  02fd               L02:
3185  02fd 5b09          	addw	sp,#9
3186  02ff 81            	ret
3229                     ; 211 unsigned char frame_head_srch(int try_times)
3229                     ; 212 {
3230                     	switch	.text
3231  0300               _frame_head_srch:
3233  0300 89            	pushw	x
3234  0301 5203          	subw	sp,#3
3235       00000003      OFST:	set	3
3238                     ; 213 	unsigned char next_state = 0;
3240  0303 0f03          	clr	(OFST+0,sp)
3242  0305 204f          	jra	L5171
3243  0307               L1171:
3244                     ; 217 		wait_posedge();
3246  0307 ad76          	call	_wait_posedge
3248                     ; 218 		if(get_pin_rxd() ==  ((FRAME_HEAD >> (7-curr_state)) & 0x1))
3250  0309 ae009b        	ldw	x,#155
3251  030c a607          	ld	a,#7
3252  030e c00000        	sub	a,_curr_state
3253  0311 4d            	tnz	a
3254  0312 2704          	jreq	L42
3255  0314               L62:
3256  0314 57            	sraw	x
3257  0315 4a            	dec	a
3258  0316 26fc          	jrne	L62
3259  0318               L42:
3260  0318 01            	rrwa	x,a
3261  0319 a401          	and	a,#1
3262  031b 5f            	clrw	x
3263  031c 02            	rlwa	x,a
3264  031d 1f01          	ldw	(OFST-2,sp),x
3265  031f 01            	rrwa	x,a
3266  0320 cd03ab        	call	_get_pin_rxd
3268  0323 5f            	clrw	x
3269  0324 97            	ld	xl,a
3270  0325 1301          	cpw	x,(OFST-2,sp)
3271  0327 2617          	jrne	L1271
3272                     ; 220 			if(curr_state == 7)
3274  0329 c60000        	ld	a,_curr_state
3275  032c a107          	cp	a,#7
3276  032e 2608          	jrne	L5271
3277                     ; 222 				curr_state = 0 ;
3279  0330 725f0000      	clr	_curr_state
3280                     ; 223 				return 1 ;
3282  0334 a601          	ld	a,#1
3284  0336 2024          	jra	L03
3285  0338               L5271:
3286                     ; 226 			next_state = curr_state + 1;
3288  0338 c60000        	ld	a,_curr_state
3289  033b 4c            	inc	a
3290  033c 6b03          	ld	(OFST+0,sp),a
3292  033e 2011          	jra	L7271
3293  0340               L1271:
3294                     ; 230 			next_state = next_state_table[curr_state];
3296  0340 c60000        	ld	a,_curr_state
3297  0343 5f            	clrw	x
3298  0344 97            	ld	xl,a
3299  0345 d60000        	ld	a,(_next_state_table,x)
3300  0348 6b03          	ld	(OFST+0,sp),a
3301                     ; 231 			try_times -- ;
3303  034a 1e04          	ldw	x,(OFST+1,sp)
3304  034c 1d0001        	subw	x,#1
3305  034f 1f04          	ldw	(OFST+1,sp),x
3306  0351               L7271:
3307                     ; 234 		curr_state = next_state ;
3309  0351 7b03          	ld	a,(OFST+0,sp)
3310  0353 c70000        	ld	_curr_state,a
3311  0356               L5171:
3312                     ; 215 	while(try_times > 0)
3314  0356 9c            	rvf
3315  0357 1e04          	ldw	x,(OFST+1,sp)
3316  0359 2cac          	jrsgt	L1171
3317                     ; 237 	return 0 ;
3319  035b 4f            	clr	a
3321  035c               L03:
3323  035c 5b05          	addw	sp,#5
3324  035e 81            	ret
3358                     ; 240 unsigned char conti_0_check(int conti_0_num)
3358                     ; 241 {
3359                     	switch	.text
3360  035f               _conti_0_check:
3362  035f 89            	pushw	x
3363       00000000      OFST:	set	0
3366  0360 200a          	jra	L7471
3367  0362               L5471:
3368                     ; 244 		wait_posedge();
3370  0362 ad1b          	call	_wait_posedge
3372                     ; 245 		if(get_pin_rxd() !=  0x0)
3374  0364 ad45          	call	_get_pin_rxd
3376  0366 4d            	tnz	a
3377  0367 2703          	jreq	L7471
3378                     ; 247 			return 0 ;
3380  0369 4f            	clr	a
3382  036a 2011          	jra	L43
3383  036c               L7471:
3384                     ; 242 	while(conti_0_num--)
3386  036c 1e01          	ldw	x,(OFST+1,sp)
3387  036e 1d0001        	subw	x,#1
3388  0371 1f01          	ldw	(OFST+1,sp),x
3389  0373 1c0001        	addw	x,#1
3390  0376 a30000        	cpw	x,#0
3391  0379 26e7          	jrne	L5471
3392                     ; 252 	return 1 ;
3394  037b a601          	ld	a,#1
3396  037d               L43:
3398  037d 85            	popw	x
3399  037e 81            	ret
3423                     ; 255 void wait_posedge(void)
3423                     ; 256 {
3424                     	switch	.text
3425  037f               _wait_posedge:
3429                     ; 257 	if((PC_IDR & BIT6) != 0)
3431  037f c6500b        	ld	a,_PC_IDR
3432  0382 a540          	bcp	a,#64
3433  0384 2707          	jreq	L3002
3435  0386               L3771:
3436                     ; 259 	  	while((PC_IDR & BIT6) != 0) ;
3438  0386 c6500b        	ld	a,_PC_IDR
3439  0389 a540          	bcp	a,#64
3440  038b 26f9          	jrne	L3771
3442  038d               L3002:
3443                     ; 262 	while((PC_IDR & BIT6) == 0) ;
3445  038d c6500b        	ld	a,_PC_IDR
3446  0390 a540          	bcp	a,#64
3447  0392 27f9          	jreq	L3002
3448                     ; 263 }
3451  0394 81            	ret
3475                     ; 265 void wait_negedge(void)
3475                     ; 266 {
3476                     	switch	.text
3477  0395               _wait_negedge:
3481                     ; 267 	if((PC_IDR & BIT6) == 0)
3483  0395 c6500b        	ld	a,_PC_IDR
3484  0398 a540          	bcp	a,#64
3485  039a 2607          	jrne	L3302
3487  039c               L3202:
3488                     ; 269 	  	while((PC_IDR & BIT6) == 0) ;
3490  039c c6500b        	ld	a,_PC_IDR
3491  039f a540          	bcp	a,#64
3492  03a1 27f9          	jreq	L3202
3494  03a3               L3302:
3495                     ; 272 	while((PC_IDR & BIT6) != 0) ;
3497  03a3 c6500b        	ld	a,_PC_IDR
3498  03a6 a540          	bcp	a,#64
3499  03a8 26f9          	jrne	L3302
3500                     ; 273 }
3503  03aa 81            	ret
3527                     ; 275 unsigned char get_pin_rxd(void)
3527                     ; 276 {
3528                     	switch	.text
3529  03ab               _get_pin_rxd:
3533                     ; 277 	if((PB_IDR & BIT4) == 0)  //get rxd
3535  03ab c65006        	ld	a,_PB_IDR
3536  03ae a510          	bcp	a,#16
3537  03b0 2602          	jrne	L7402
3538                     ; 279 		return 0x0;
3540  03b2 4f            	clr	a
3543  03b3 81            	ret
3544  03b4               L7402:
3545                     ; 283 		return 0x1;
3547  03b4 a601          	ld	a,#1
3550  03b6 81            	ret
3583                     ; 287 void set_pin_txd(unsigned char set_to_val)
3583                     ; 288 {
3584                     	switch	.text
3585  03b7               _set_pin_txd:
3589                     ; 289 	if(set_to_val == 0x0)
3591  03b7 4d            	tnz	a
3592  03b8 2606          	jrne	L7602
3593                     ; 291 		PC_ODR &= ~BIT4;
3595  03ba 7219500a      	bres	_PC_ODR,#4
3597  03be 2004          	jra	L1702
3598  03c0               L7602:
3599                     ; 295 		PC_ODR |= BIT4;
3601  03c0 7218500a      	bset	_PC_ODR,#4
3602  03c4               L1702:
3603                     ; 297 }
3606  03c4 81            	ret
3639                     ; 299 void set_pin_reg_data(unsigned char set_to_val)
3639                     ; 300 {
3640                     	switch	.text
3641  03c5               _set_pin_reg_data:
3645                     ; 301 	if(set_to_val == 0x0)
3647  03c5 4d            	tnz	a
3648  03c6 2606          	jrne	L7012
3649                     ; 303 		PB_ODR &= ~BIT5;    //clr pin reg_data
3651  03c8 721b5005      	bres	_PB_ODR,#5
3653  03cc 2004          	jra	L1112
3654  03ce               L7012:
3655                     ; 307 		PB_ODR |= BIT5;   //set pin reg_data
3657  03ce 721a5005      	bset	_PB_ODR,#5
3658  03d2               L1112:
3659                     ; 309 }
3662  03d2 81            	ret
3695                     ; 311 void set_pin_rxtx(unsigned char set_to_val)
3695                     ; 312 {
3696                     	switch	.text
3697  03d3               _set_pin_rxtx:
3701                     ; 313 	if(set_to_val == 0x0)
3703  03d3 4d            	tnz	a
3704  03d4 2606          	jrne	L7212
3705                     ; 315 		PC_ODR &= ~BIT3;   //clr pin rxtx
3707  03d6 7217500a      	bres	_PC_ODR,#3
3709  03da 2004          	jra	L1312
3710  03dc               L7212:
3711                     ; 319 		PC_ODR |= BIT3;   //set pin rxtx
3713  03dc 7216500a      	bset	_PC_ODR,#3
3714  03e0               L1312:
3715                     ; 321 }
3718  03e0 81            	ret
3742                     ; 323 unsigned char get_pin_cdpd(void)
3742                     ; 324 {
3743                     	switch	.text
3744  03e1               _get_pin_cdpd:
3748                     ; 325 	if((PA_IDR & BIT3) == 0)  //get cdpd
3750  03e1 c65001        	ld	a,_PA_IDR
3751  03e4 a508          	bcp	a,#8
3752  03e6 2602          	jrne	L3412
3753                     ; 327 		return 0x0;
3755  03e8 4f            	clr	a
3758  03e9 81            	ret
3759  03ea               L3412:
3760                     ; 331 		return 0x1;
3762  03ea a601          	ld	a,#1
3765  03ec 81            	ret
3797                     	xdef	_next_state_table
3798                     	xdef	_curr_state
3799                     	xdef	_conti_0_check
3800                     	xdef	_get_pin_cdpd
3801                     	xdef	_wait_negedge
3802                     	xdef	_wait_posedge
3803                     	xdef	_set_pin_rxtx
3804                     	xdef	_set_pin_reg_data
3805                     	xdef	_set_pin_txd
3806                     	xdef	_get_pin_rxd
3807                     	xdef	_frame_head_srch
3808                     	xdef	_data_recv
3809                     	xdef	_data_trans
3810                     	xdef	_write_ctrl_reg
3811                     	xdef	_read_ctrl_reg
3812                     	xdef	_delay
3813                     	xdef	_power_line_init
3814                     	xref.b	c_lreg
3833                     	xref	c_lursh
3834                     	xref	c_lgor
3835                     	xref	c_llsh
3836                     	xref	c_lcmp
3837                     	xref	c_lrzmp
3838                     	xref	c_lgsbc
3839                     	xref	c_ltor
3840                     	end
