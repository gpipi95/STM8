   1                     ; C Compiler for STM8 (COSMIC Software)
   2                     ; Parser V4.10.2 - 02 Nov 2011
   3                     ; Generator (Limited) V4.3.7 - 29 Nov 2011
2577                     ; 12 int  main()
2577                     ; 13 {
2579                     	switch	.text
2580  0000               _main:
2584                     ; 14     PD_DDR = 0x08;
2586  0000 35085011      	mov	_PD_DDR,#8
2587                     ; 15     PD_CR1 = 0x08; // push-pull
2589  0004 35085012      	mov	_PD_CR1,#8
2590                     ; 16     PD_CR2 = 0x00;
2592  0008 725f5013      	clr	_PD_CR2
2593  000c               L1561:
2594                     ; 18         PD_ODR = PD_ODR | 0x08; // PD3 output to 1
2596  000c 7216500f      	bset	_PD_ODR,#3
2597                     ; 19         DelayMs(1000);          // delay 100MS
2599  0010 ae03e8        	ldw	x,#1000
2600  0013 ad0b          	call	_DelayMs
2602                     ; 20         PD_ODR = PD_ODR & 0xF7; // PD3 output 0
2604  0015 7217500f      	bres	_PD_ODR,#3
2605                     ; 21         DelayMs(1000);          // 100MS
2607  0019 ae03e8        	ldw	x,#1000
2608  001c ad02          	call	_DelayMs
2611  001e 20ec          	jra	L1561
2654                     ; 25 void DelayMs(unsigned int time)
2654                     ; 26 {
2655                     	switch	.text
2656  0020               _DelayMs:
2658  0020 89            	pushw	x
2659  0021 88            	push	a
2660       00000001      OFST:	set	1
2663  0022 201b          	jra	L1071
2664  0024               L7761:
2665                     ; 29         for (i = 0; i < 250; i++) {
2667  0024 0f01          	clr	(OFST+0,sp)
2668  0026               L5071:
2671  0026 0c01          	inc	(OFST+0,sp)
2674  0028 7b01          	ld	a,(OFST+0,sp)
2675  002a a1fa          	cp	a,#250
2676  002c 25f8          	jrult	L5071
2677                     ; 31         for (i = 0; i < 75; i++) {
2679  002e 0f01          	clr	(OFST+0,sp)
2680  0030               L3171:
2683  0030 0c01          	inc	(OFST+0,sp)
2686  0032 7b01          	ld	a,(OFST+0,sp)
2687  0034 a14b          	cp	a,#75
2688  0036 25f8          	jrult	L3171
2689                     ; 33         time--;
2691  0038 1e02          	ldw	x,(OFST+1,sp)
2692  003a 1d0001        	subw	x,#1
2693  003d 1f02          	ldw	(OFST+1,sp),x
2694  003f               L1071:
2695                     ; 28     while (time != 0) {
2697  003f 1e02          	ldw	x,(OFST+1,sp)
2698  0041 26e1          	jrne	L7761
2699                     ; 35 }
2702  0043 5b03          	addw	sp,#3
2703  0045 81            	ret
2716                     	xdef	_main
2717                     	xdef	_DelayMs
2736                     	end
