   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
2161                     	switch	.data
2162  0000               _uart_busy:
2163  0000 00            	dc.b	0
2164  0001               _baud:
2165  0001 12c0          	dc.w	4800
2217                     ; 19 void uart_ini(u16 b)
2217                     ; 20 {
2219                     	switch	.text
2220  0000               _uart_ini:
2222  0000 89            	pushw	x
2223  0001 5204          	subw	sp,#4
2224       00000004      OFST:	set	4
2227                     ; 21 	baud=b;
2229  0003 cf0001        	ldw	_baud,x
2230                     ; 23 	PD_DDR &= ~(1<<6);//输入（输出）
2232  0006 721d5011      	bres	_PD_DDR,#6
2233                     ; 24 	PD_CR1 |= (1<<6);//上拉（浮空）
2235  000a 721c5012      	bset	_PD_CR1,#6
2236                     ; 25 	PD_CR2 &= ~(1<<6);//无中断（中断）
2238  000e 721d5013      	bres	_PD_CR2,#6
2239                     ; 27 	PD_DDR |= (1<<5);//输出（输入）
2241  0012 721a5011      	bset	_PD_DDR,#5
2242                     ; 28 	PD_CR1 |= (1<<5);//推挽（开漏）
2244  0016 721a5012      	bset	_PD_CR1,#5
2245                     ; 29 	PD_CR2 &= ~(1<<5);//低速（高速）
2247  001a 721b5013      	bres	_PD_CR2,#5
2248                     ; 31 	b=16e6/b;  //16M外部时钟
2250  001e 1e05          	ldw	x,(OFST+1,sp)
2251  0020 cd0000        	call	c_uitof
2253  0023 96            	ldw	x,sp
2254  0024 1c0001        	addw	x,#OFST-3
2255  0027 cd0000        	call	c_rtol
2257  002a ae0000        	ldw	x,#L5541
2258  002d cd0000        	call	c_ltor
2260  0030 96            	ldw	x,sp
2261  0031 1c0001        	addw	x,#OFST-3
2262  0034 cd0000        	call	c_fdiv
2264  0037 cd0000        	call	c_ftoi
2266  003a 1f05          	ldw	(OFST+1,sp),x
2267                     ; 32 	UART1_BRR2=((b&0xf000)>>8)+(b&0x000f);
2269  003c 7b06          	ld	a,(OFST+2,sp)
2270  003e a40f          	and	a,#15
2271  0040 6b04          	ld	(OFST+0,sp),a
2272  0042 7b05          	ld	a,(OFST+1,sp)
2273  0044 97            	ld	xl,a
2274  0045 7b06          	ld	a,(OFST+2,sp)
2275  0047 9f            	ld	a,xl
2276  0048 a4f0          	and	a,#240
2277  004a 97            	ld	xl,a
2278  004b 4f            	clr	a
2279  004c 02            	rlwa	x,a
2280  004d 4f            	clr	a
2281  004e 01            	rrwa	x,a
2282  004f 01            	rrwa	x,a
2283  0050 1b04          	add	a,(OFST+0,sp)
2284  0052 2401          	jrnc	L6
2285  0054 5c            	incw	x
2286  0055               L6:
2287  0055 c75233        	ld	_UART1_BRR2,a
2288                     ; 33 	UART1_BRR1=((b&0x0ff0)>>4);
2290  0058 1e05          	ldw	x,(OFST+1,sp)
2291  005a 01            	rrwa	x,a
2292  005b a4f0          	and	a,#240
2293  005d 01            	rrwa	x,a
2294  005e a40f          	and	a,#15
2295  0060 01            	rrwa	x,a
2296  0061 54            	srlw	x
2297  0062 54            	srlw	x
2298  0063 54            	srlw	x
2299  0064 54            	srlw	x
2300  0065 01            	rrwa	x,a
2301  0066 c75232        	ld	_UART1_BRR1,a
2302  0069 02            	rlwa	x,a
2303                     ; 36 	UART1_CR2=0x2c;//00101100//禁止发送中断
2305  006a 352c5235      	mov	_UART1_CR2,#44
2306                     ; 39 	uart_busy=0;
2308  006e 725f0000      	clr	_uart_busy
2309                     ; 40 	UART1_SR=0xc0;
2311  0072 35c05230      	mov	_UART1_SR,#192
2312                     ; 42 	Queue_ini(uart_rx_queue_buff,sizeof(uart_rx_queue_buff),&uart_rx_queue);
2314  0076 ae0087        	ldw	x,#_uart_rx_queue
2315  0079 89            	pushw	x
2316  007a 4b40          	push	#64
2317  007c ae0040        	ldw	x,#L3241_uart_rx_queue_buff
2318  007f cd0000        	call	_Queue_ini
2320  0082 5b03          	addw	sp,#3
2321                     ; 43 	Queue_ini(uart_tx_queue_buff,sizeof(uart_tx_queue_buff),&uart_tx_queue);
2323  0084 ae0080        	ldw	x,#_uart_tx_queue
2324  0087 89            	pushw	x
2325  0088 4b40          	push	#64
2326  008a ae0000        	ldw	x,#L5241_uart_tx_queue_buff
2327  008d cd0000        	call	_Queue_ini
2329  0090 5b03          	addw	sp,#3
2330                     ; 44 }
2333  0092 5b06          	addw	sp,#6
2334  0094 81            	ret
2370                     ; 46 char data_to_uart(void)
2370                     ; 47 {
2371                     	switch	.text
2372  0095               _data_to_uart:
2374  0095 88            	push	a
2375       00000001      OFST:	set	1
2378                     ; 49 	if(UART1_SR & 0x40)//接收中断
2380  0096 c65230        	ld	a,_UART1_SR
2381  0099 a540          	bcp	a,#64
2382  009b 2721          	jreq	L5741
2383                     ; 51 			if(Queue_get(&c,&uart_tx_queue) == 0)
2385  009d ae0080        	ldw	x,#_uart_tx_queue
2386  00a0 89            	pushw	x
2387  00a1 96            	ldw	x,sp
2388  00a2 1c0003        	addw	x,#OFST+2
2389  00a5 cd0000        	call	_Queue_get
2391  00a8 85            	popw	x
2392  00a9 4d            	tnz	a
2393  00aa 260e          	jrne	L7741
2394                     ; 53 			    UART1_SR &= ~0x40;
2396  00ac 721d5230      	bres	_UART1_SR,#6
2397                     ; 54 				  UART1_DR=(char)c;
2399  00b0 7b01          	ld	a,(OFST+0,sp)
2400  00b2 c75231        	ld	_UART1_DR,a
2401                     ; 55 				  return 1;
2403  00b5 a601          	ld	a,#1
2406  00b7 5b01          	addw	sp,#1
2407  00b9 81            	ret
2408  00ba               L7741:
2409                     ; 59 				  return 0;
2411  00ba 4f            	clr	a
2414  00bb 5b01          	addw	sp,#1
2415  00bd 81            	ret
2416  00be               L5741:
2417                     ; 64 		  return 0;
2419  00be 4f            	clr	a
2422  00bf 5b01          	addw	sp,#1
2423  00c1 81            	ret
2450                     ; 69 @far @interrupt void uart1_int(void)
2450                     ; 70 {
2452                     	switch	.text
2453  00c2               f_uart1_int:
2456  00c2 3b0002        	push	c_x+2
2457  00c5 be00          	ldw	x,c_x
2458  00c7 89            	pushw	x
2459  00c8 3b0002        	push	c_y+2
2460  00cb be00          	ldw	x,c_y
2461  00cd 89            	pushw	x
2464                     ; 71 	if(UART1_SR & 0x20)//接收中断
2466  00ce c65230        	ld	a,_UART1_SR
2467  00d1 a520          	bcp	a,#32
2468  00d3 270b          	jreq	L5151
2469                     ; 73 		Queue_set(UART1_DR,&uart_rx_queue);
2471  00d5 ae0087        	ldw	x,#_uart_rx_queue
2472  00d8 89            	pushw	x
2473  00d9 c65231        	ld	a,_UART1_DR
2474  00dc cd0000        	call	_Queue_set
2476  00df 85            	popw	x
2477  00e0               L5151:
2478                     ; 77 	UART1_SR&=~0x20;
2480  00e0 721b5230      	bres	_UART1_SR,#5
2481                     ; 78 }
2484  00e4 85            	popw	x
2485  00e5 bf00          	ldw	c_y,x
2486  00e7 320002        	pop	c_y+2
2487  00ea 85            	popw	x
2488  00eb bf00          	ldw	c_x,x
2489  00ed 320002        	pop	c_x+2
2490  00f0 80            	iret
2610                     	xdef	f_uart1_int
2611                     	xdef	_baud
2612                     	xdef	_uart_busy
2613                     	switch	.bss
2614  0000               L5241_uart_tx_queue_buff:
2615  0000 000000000000  	ds.b	64
2616  0040               L3241_uart_rx_queue_buff:
2617  0040 000000000000  	ds.b	64
2618  0080               _uart_tx_queue:
2619  0080 000000000000  	ds.b	7
2620                     	xdef	_uart_tx_queue
2621  0087               _uart_rx_queue:
2622  0087 000000000000  	ds.b	7
2623                     	xdef	_uart_rx_queue
2624                     	xdef	_data_to_uart
2625                     	xdef	_uart_ini
2626                     	xref	_Queue_get
2627                     	xref	_Queue_set
2628                     	xref	_Queue_ini
2629                     .const:	section	.text
2630  0000               L5541:
2631  0000 4b742400      	dc.w	19316,9216
2632                     	xref.b	c_x
2633                     	xref.b	c_y
2653                     	xref	c_ftoi
2654                     	xref	c_fdiv
2655                     	xref	c_rtol
2656                     	xref	c_uitof
2657                     	xref	c_ltor
2658                     	end
