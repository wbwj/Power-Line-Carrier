   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Generator V4.2.4 - 19 Dec 2007
2207                     ; 10 void EEPROM_WRITE_BYTE(int a,u8 d)
2207                     ; 11 {
2209                     	switch	.text
2210  0000               _EEPROM_WRITE_BYTE:
2212  0000 89            	pushw	x
2213       00000000      OFST:	set	0
2216                     ; 13 	FLASH_CR1 &= (unsigned char)(~0x01);
2218  0001 7211505a      	bres	_FLASH_CR1,#0
2219                     ; 14 	FLASH_CR1 |= 0x01;
2221  0005 7210505a      	bset	_FLASH_CR1,#0
2222                     ; 16 	FLASH_DUKR = 0xAE;
2224  0009 35ae5064      	mov	_FLASH_DUKR,#174
2225                     ; 17 	FLASH_DUKR = 0x56;
2227  000d 35565064      	mov	_FLASH_DUKR,#86
2228                     ; 18 	*(char *)a= d;
2230  0011 7b05          	ld	a,(OFST+5,sp)
2231  0013 1e01          	ldw	x,(OFST+1,sp)
2232  0015 f7            	ld	(x),a
2234  0016               L3541:
2235                     ; 20 	while((FLASH_IAPSR & 0x04) != 0x00);
2237  0016 c6505f        	ld	a,_FLASH_IAPSR
2238  0019 a504          	bcp	a,#4
2239  001b 26f9          	jrne	L3541
2240                     ; 21 }
2243  001d 85            	popw	x
2244  001e 81            	ret
2288                     ; 22 void EEPROM_WRITE_WORD(int a,u32 d)
2288                     ; 23 {
2289                     	switch	.text
2290  001f               _EEPROM_WRITE_WORD:
2292  001f 89            	pushw	x
2293       00000000      OFST:	set	0
2296                     ; 25 	FLASH_CR1 &= (unsigned char)(~0x01);
2298  0020 7211505a      	bres	_FLASH_CR1,#0
2299                     ; 26 	FLASH_CR1 |= 0x01;
2301  0024 7210505a      	bset	_FLASH_CR1,#0
2302                     ; 28 	FLASH_DUKR = 0xAE;
2304  0028 35ae5064      	mov	_FLASH_DUKR,#174
2305                     ; 29 	FLASH_DUKR = 0x56;
2307  002c 35565064      	mov	_FLASH_DUKR,#86
2308                     ; 31 	FLASH_CR2 |= 0x40;
2310  0030 721c505b      	bset	_FLASH_CR2,#6
2311                     ; 32 	FLASH_NCR2 &= (unsigned char)(~0x40);
2313  0034 721d505c      	bres	_FLASH_NCR2,#6
2314                     ; 34 	*((unsigned long *)a) = d;
2316  0038 7b08          	ld	a,(OFST+8,sp)
2317  003a e703          	ld	(3,x),a
2318  003c 7b07          	ld	a,(OFST+7,sp)
2319  003e e702          	ld	(2,x),a
2320  0040 7b06          	ld	a,(OFST+6,sp)
2321  0042 e701          	ld	(1,x),a
2322  0044 7b05          	ld	a,(OFST+5,sp)
2323  0046 f7            	ld	(x),a
2325  0047               L1051:
2326                     ; 35 	while((FLASH_IAPSR & 0x04) != 0x00); //EOP=1£¬EEPROM±à³Ì½áÊø
2328  0047 c6505f        	ld	a,_FLASH_IAPSR
2329  004a a504          	bcp	a,#4
2330  004c 26f9          	jrne	L1051
2331                     ; 36 }
2334  004e 85            	popw	x
2335  004f 81            	ret
2348                     	xdef	_EEPROM_WRITE_WORD
2349                     	xdef	_EEPROM_WRITE_BYTE
2368                     	end
