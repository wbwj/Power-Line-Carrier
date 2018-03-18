   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
2161                     	switch	.data
2162  0000               L3241_p_front:
2163  0000 0000          	dc.w	0
2244                     ; 12 void STimer_start(STimer *t)//注册定时器
2244                     ; 13 {
2246                     	switch	.text
2247  0000               _STimer_start:
2251                     ; 14 	t->p=p_front;
2253  0000 90ce0000      	ldw	y,L3241_p_front
2254  0004 ef04          	ldw	(4,x),y
2255                     ; 15 	p_front=t;
2257  0006 cf0000        	ldw	L3241_p_front,x
2258                     ; 16 }
2261  0009 81            	ret
2299                     ; 18 void STimer_int(void)
2299                     ; 19 {//每次调用都对整个链表进行遍历
2300                     	switch	.text
2301  000a               _STimer_int:
2303  000a 89            	pushw	x
2304       00000002      OFST:	set	2
2307                     ; 20 	STimer *p=p_front;
2309  000b ce0000        	ldw	x,L3241_p_front
2310  000e 1f01          	ldw	(OFST-1,sp),x
2312  0010 2023          	jra	L7151
2313  0012               L3151:
2314                     ; 23 		if(p->time!=0)
2316  0012 1e01          	ldw	x,(OFST-1,sp)
2317  0014 e601          	ld	a,(1,x)
2318  0016 fa            	or	a,(x)
2319  0017 2716          	jreq	L3251
2320                     ; 25 			p->time--;
2322  0019 1e01          	ldw	x,(OFST-1,sp)
2323  001b 9093          	ldw	y,x
2324  001d fe            	ldw	x,(x)
2325  001e 1d0001        	subw	x,#1
2326  0021 90ff          	ldw	(y),x
2327                     ; 26 			if(p->time==0)
2329  0023 1e01          	ldw	x,(OFST-1,sp)
2330  0025 e601          	ld	a,(1,x)
2331  0027 fa            	or	a,(x)
2332  0028 2605          	jrne	L3251
2333                     ; 28 				p->cb();
2335  002a 1e01          	ldw	x,(OFST-1,sp)
2336  002c ee02          	ldw	x,(2,x)
2337  002e fd            	call	(x)
2339  002f               L3251:
2340                     ; 31 		p=p->p;
2342  002f 1e01          	ldw	x,(OFST-1,sp)
2343  0031 ee04          	ldw	x,(4,x)
2344  0033 1f01          	ldw	(OFST-1,sp),x
2345  0035               L7151:
2346                     ; 21 	while(p!=0)
2348  0035 1e01          	ldw	x,(OFST-1,sp)
2349  0037 26d9          	jrne	L3151
2350                     ; 33 }
2353  0039 85            	popw	x
2354  003a 81            	ret
2381                     	xdef	_STimer_int
2382                     	xdef	_STimer_start
2401                     	end
