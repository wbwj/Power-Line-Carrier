   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
2271                     ; 8 void Queue_ini(u8 *d,u8 num,Queue *q)
2271                     ; 9 {
2273                     	switch	.text
2274  0000               _Queue_ini:
2276  0000 89            	pushw	x
2277       00000000      OFST:	set	0
2280                     ; 10 	q->start=0;
2282  0001 1e06          	ldw	x,(OFST+6,sp)
2283  0003 7f            	clr	(x)
2284                     ; 11 	q->end=0;
2286  0004 1e06          	ldw	x,(OFST+6,sp)
2287  0006 6f01          	clr	(1,x)
2288                     ; 12 	q->len=num;
2290  0008 7b05          	ld	a,(OFST+5,sp)
2291  000a 1e06          	ldw	x,(OFST+6,sp)
2292  000c e702          	ld	(2,x),a
2293                     ; 13 	q->isFull=0;	//空
2295  000e 1e06          	ldw	x,(OFST+6,sp)
2296  0010 6f05          	clr	(5,x)
2297                     ; 14 	q->q_data=d;
2299  0012 1e06          	ldw	x,(OFST+6,sp)
2300  0014 1601          	ldw	y,(OFST+1,sp)
2301  0016 ef03          	ldw	(3,x),y
2302                     ; 15 	q->data_cnt=0;
2304  0018 1e06          	ldw	x,(OFST+6,sp)
2305  001a 6f06          	clr	(6,x)
2306                     ; 16 }
2309  001c 85            	popw	x
2310  001d 81            	ret
2357                     ; 18 s8 Queue_get(s8 *d,Queue *q)
2357                     ; 19 {
2358                     	switch	.text
2359  001e               _Queue_get:
2361  001e 89            	pushw	x
2362       00000000      OFST:	set	0
2365                     ; 20 	if(q->end==q->start)	//要么队列满，要么空
2367  001f 1e05          	ldw	x,(OFST+5,sp)
2368  0021 e601          	ld	a,(1,x)
2369  0023 1e05          	ldw	x,(OFST+5,sp)
2370  0025 f1            	cp	a,(x)
2371  0026 260e          	jrne	L5251
2372                     ; 22 		if(q->isFull==0)	//若队列空
2374  0028 1e05          	ldw	x,(OFST+5,sp)
2375  002a 6d05          	tnz	(5,x)
2376  002c 2604          	jrne	L7251
2377                     ; 24 			return 1;
2379  002e a601          	ld	a,#1
2381  0030 202f          	jra	L01
2382  0032               L7251:
2383                     ; 28 			q->isFull=0;
2385  0032 1e05          	ldw	x,(OFST+5,sp)
2386  0034 6f05          	clr	(5,x)
2387  0036               L5251:
2388                     ; 31 	*d=q->q_data[q->start++];
2390  0036 1e05          	ldw	x,(OFST+5,sp)
2391  0038 f6            	ld	a,(x)
2392  0039 9097          	ld	yl,a
2393  003b 7c            	inc	(x)
2394  003c 909f          	ld	a,yl
2395  003e 5f            	clrw	x
2396  003f 97            	ld	xl,a
2397  0040 1605          	ldw	y,(OFST+5,sp)
2398  0042 90ee03        	ldw	y,(3,y)
2399  0045 90bf00        	ldw	c_x,y
2400  0048 72bb0000      	addw	x,c_x
2401  004c f6            	ld	a,(x)
2402  004d 1e01          	ldw	x,(OFST+1,sp)
2403  004f f7            	ld	(x),a
2404                     ; 32 	(q->data_cnt)--;
2406  0050 1e05          	ldw	x,(OFST+5,sp)
2407  0052 6a06          	dec	(6,x)
2408                     ; 33 	if(q->start>=q->len)
2410  0054 1e05          	ldw	x,(OFST+5,sp)
2411  0056 f6            	ld	a,(x)
2412  0057 1e05          	ldw	x,(OFST+5,sp)
2413  0059 e102          	cp	a,(2,x)
2414  005b 2503          	jrult	L3351
2415                     ; 35 		q->start=0;
2417  005d 1e05          	ldw	x,(OFST+5,sp)
2418  005f 7f            	clr	(x)
2419  0060               L3351:
2420                     ; 37 	return 0;
2422  0060 4f            	clr	a
2424  0061               L01:
2426  0061 85            	popw	x
2427  0062 81            	ret
2471                     ; 40 s8 Queue_set(s8 d,Queue *q)
2471                     ; 41 {
2472                     	switch	.text
2473  0063               _Queue_set:
2475  0063 88            	push	a
2476       00000000      OFST:	set	0
2479                     ; 42 	if(q->isFull==1)	//若队列满了
2481  0064 1e04          	ldw	x,(OFST+4,sp)
2482  0066 e605          	ld	a,(5,x)
2483  0068 a101          	cp	a,#1
2484  006a 2605          	jrne	L7551
2485                     ; 44 		return 1;
2487  006c a601          	ld	a,#1
2490  006e 5b01          	addw	sp,#1
2491  0070 81            	ret
2492  0071               L7551:
2493                     ; 46 	q->q_data[q->end++]=d;
2495  0071 1e04          	ldw	x,(OFST+4,sp)
2496  0073 e601          	ld	a,(1,x)
2497  0075 9097          	ld	yl,a
2498  0077 6c01          	inc	(1,x)
2499  0079 909f          	ld	a,yl
2500  007b 5f            	clrw	x
2501  007c 97            	ld	xl,a
2502  007d 1604          	ldw	y,(OFST+4,sp)
2503  007f 90ee03        	ldw	y,(3,y)
2504  0082 90bf00        	ldw	c_x,y
2505  0085 72bb0000      	addw	x,c_x
2506  0089 7b01          	ld	a,(OFST+1,sp)
2507  008b f7            	ld	(x),a
2508                     ; 47 	(q->data_cnt)++;
2510  008c 1e04          	ldw	x,(OFST+4,sp)
2511  008e 6c06          	inc	(6,x)
2512                     ; 48 	if(q->end>=q->len)
2514  0090 1e04          	ldw	x,(OFST+4,sp)
2515  0092 e601          	ld	a,(1,x)
2516  0094 1e04          	ldw	x,(OFST+4,sp)
2517  0096 e102          	cp	a,(2,x)
2518  0098 2504          	jrult	L1651
2519                     ; 50 		q->end=0;
2521  009a 1e04          	ldw	x,(OFST+4,sp)
2522  009c 6f01          	clr	(1,x)
2523  009e               L1651:
2524                     ; 52 	if(q->start==q->end)	//说明队列恰好满了
2526  009e 1e04          	ldw	x,(OFST+4,sp)
2527  00a0 f6            	ld	a,(x)
2528  00a1 1e04          	ldw	x,(OFST+4,sp)
2529  00a3 e101          	cp	a,(1,x)
2530  00a5 2606          	jrne	L3651
2531                     ; 54 		q->isFull=1;
2533  00a7 1e04          	ldw	x,(OFST+4,sp)
2534  00a9 a601          	ld	a,#1
2535  00ab e705          	ld	(5,x),a
2536  00ad               L3651:
2537                     ; 56 	return 0;
2539  00ad 4f            	clr	a
2542  00ae 5b01          	addw	sp,#1
2543  00b0 81            	ret
2580                     ; 59 u8 Queue_cnt(Queue *q)
2580                     ; 60 {
2581                     	switch	.text
2582  00b1               _Queue_cnt:
2586                     ; 61 	return (q->data_cnt);
2588  00b1 e606          	ld	a,(6,x)
2591  00b3 81            	ret
2638                     ; 65 s8 Queue_pre_read(s8 *d,Queue *q)
2638                     ; 66 {
2639                     	switch	.text
2640  00b4               _Queue_pre_read:
2642  00b4 89            	pushw	x
2643       00000000      OFST:	set	0
2646                     ; 67 	if(q->end==q->start)	//要么队列满，要么空
2648  00b5 1e05          	ldw	x,(OFST+5,sp)
2649  00b7 e601          	ld	a,(1,x)
2650  00b9 1e05          	ldw	x,(OFST+5,sp)
2651  00bb f1            	cp	a,(x)
2652  00bc 260a          	jrne	L1361
2653                     ; 69 		if(q->isFull==0)	//若队列空
2655  00be 1e05          	ldw	x,(OFST+5,sp)
2656  00c0 6d05          	tnz	(5,x)
2657  00c2 2604          	jrne	L1361
2658                     ; 71 			return 1;
2660  00c4 a601          	ld	a,#1
2662  00c6 2016          	jra	L02
2663  00c8               L1361:
2664                     ; 75 	*d=q->q_data[q->start];
2666  00c8 1e05          	ldw	x,(OFST+5,sp)
2667  00ca f6            	ld	a,(x)
2668  00cb 5f            	clrw	x
2669  00cc 97            	ld	xl,a
2670  00cd 1605          	ldw	y,(OFST+5,sp)
2671  00cf 90ee03        	ldw	y,(3,y)
2672  00d2 90bf00        	ldw	c_x,y
2673  00d5 72bb0000      	addw	x,c_x
2674  00d9 f6            	ld	a,(x)
2675  00da 1e01          	ldw	x,(OFST+1,sp)
2676  00dc f7            	ld	(x),a
2677                     ; 77 	return 0;
2679  00dd 4f            	clr	a
2681  00de               L02:
2683  00de 85            	popw	x
2684  00df 81            	ret
2697                     	xdef	_Queue_cnt
2698                     	xdef	_Queue_pre_read
2699                     	xdef	_Queue_get
2700                     	xdef	_Queue_set
2701                     	xdef	_Queue_ini
2702                     	xref.b	c_x
2721                     	end
