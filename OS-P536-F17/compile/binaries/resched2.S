
resched2.o:     file format elf32-littlearm


Disassembly of section .text:

00000000 <resched2>:
   0:	e92d4070 	push	{r4, r5, r6, lr}
   4:	e0611181 	rsb	r1, r1, r1, lsl #3
   8:	e1a01181 	lsl	r1, r1, #3
   c:	e59f30c4 	ldr	r3, [pc, #196]	; d8 <resched2+0xd8>
  10:	e18300b1 	strh	r0, [r3, r1]
  14:	e59f30c0 	ldr	r3, [pc, #192]	; dc <resched2+0xdc>
  18:	e5933000 	ldr	r3, [r3]
  1c:	e3530000 	cmp	r3, #0
  20:	da000003 	ble	34 <resched2+0x34>
  24:	e3a02001 	mov	r2, #1
  28:	e59f30ac 	ldr	r3, [pc, #172]	; dc <resched2+0xdc>
  2c:	e5c32004 	strb	r2, [r3, #4]
  30:	e8bd8070 	pop	{r4, r5, r6, pc}
  34:	e59f30a4 	ldr	r3, [pc, #164]	; e0 <resched2+0xe0>
  38:	e5930000 	ldr	r0, [r3]
  3c:	e060c180 	rsb	ip, r0, r0, lsl #3
  40:	e1a0c18c 	lsl	ip, ip, #3
  44:	e59f308c 	ldr	r3, [pc, #140]	; d8 <resched2+0xd8>
  48:	e08c4003 	add	r4, ip, r3
  4c:	e19c20b3 	ldrh	r2, [ip, r3]
  50:	e3520001 	cmp	r2, #1
  54:	1a00000d 	bne	90 <resched2+0x90>
  58:	e1d420f2 	ldrsh	r2, [r4, #2]
  5c:	e59f1080 	ldr	r1, [pc, #128]	; e4 <resched2+0xe4>
  60:	e1d110b0 	ldrh	r1, [r1]
  64:	e59f507c 	ldr	r5, [pc, #124]	; e8 <resched2+0xe8>
  68:	e6bf6071 	sxth	r6, r1
  6c:	e0856186 	add	r6, r5, r6, lsl #3
  70:	e1d660f4 	ldrsh	r6, [r6, #4]
  74:	e7955186 	ldr	r5, [r5, r6, lsl #3]
  78:	e1520005 	cmp	r2, r5
  7c:	c8bd8070 	popgt	{r4, r5, r6, pc}
  80:	e3a0e002 	mov	lr, #2
  84:	e18ce0b3 	strh	lr, [ip, r3]
  88:	e6bf1071 	sxth	r1, r1
  8c:	ebfffffe 	bl	0 <insert>
  90:	e59f304c 	ldr	r3, [pc, #76]	; e4 <resched2+0xe4>
  94:	e1d300f0 	ldrsh	r0, [r3]
  98:	ebfffffe 	bl	0 <dequeue>
  9c:	e59f303c 	ldr	r3, [pc, #60]	; e0 <resched2+0xe0>
  a0:	e5830000 	str	r0, [r3]
  a4:	e0600180 	rsb	r0, r0, r0, lsl #3
  a8:	e1a00180 	lsl	r0, r0, #3
  ac:	e59f3024 	ldr	r3, [pc, #36]	; d8 <resched2+0xd8>
  b0:	e0801003 	add	r1, r0, r3
  b4:	e3a02001 	mov	r2, #1
  b8:	e18020b3 	strh	r2, [r0, r3]
  bc:	e3a02002 	mov	r2, #2
  c0:	e59f3024 	ldr	r3, [pc, #36]	; ec <resched2+0xec>
  c4:	e5832000 	str	r2, [r3]
  c8:	e2840004 	add	r0, r4, #4
  cc:	e2811004 	add	r1, r1, #4
  d0:	ebfffffe 	bl	0 <ctxsw>
  d4:	e8bd8070 	pop	{r4, r5, r6, pc}
	...

000000f0 <resched_cntl2>:
  f0:	e92d4008 	push	{r3, lr}
  f4:	e3500001 	cmp	r0, #1
  f8:	0a000002 	beq	108 <resched_cntl2+0x18>
  fc:	e3500002 	cmp	r0, #2
 100:	0a00000b 	beq	134 <resched_cntl2+0x44>
 104:	ea000019 	b	170 <resched_cntl2+0x80>
 108:	e59f2088 	ldr	r2, [pc, #136]	; 198 <resched_cntl2+0xa8>
 10c:	e5923000 	ldr	r3, [r2]
 110:	e2831001 	add	r1, r3, #1
 114:	e5821000 	str	r1, [r2]
 118:	e3530000 	cmp	r3, #0
 11c:	1a000015 	bne	178 <resched_cntl2+0x88>
 120:	e3a02000 	mov	r2, #0
 124:	e59f306c 	ldr	r3, [pc, #108]	; 198 <resched_cntl2+0xa8>
 128:	e5c32004 	strb	r2, [r3, #4]
 12c:	e3a00001 	mov	r0, #1
 130:	e8bd8008 	pop	{r3, pc}
 134:	e59f305c 	ldr	r3, [pc, #92]	; 198 <resched_cntl2+0xa8>
 138:	e5933000 	ldr	r3, [r3]
 13c:	e3530000 	cmp	r3, #0
 140:	da00000e 	ble	180 <resched_cntl2+0x90>
 144:	e2433001 	sub	r3, r3, #1
 148:	e59f2048 	ldr	r2, [pc, #72]	; 198 <resched_cntl2+0xa8>
 14c:	e5823000 	str	r3, [r2]
 150:	e3530000 	cmp	r3, #0
 154:	1a00000b 	bne	188 <resched_cntl2+0x98>
 158:	e5d23004 	ldrb	r3, [r2, #4]
 15c:	e3530000 	cmp	r3, #0
 160:	0a00000a 	beq	190 <resched_cntl2+0xa0>
 164:	ebfffffe 	bl	0 <resched>
 168:	e3a00001 	mov	r0, #1
 16c:	e8bd8008 	pop	{r3, pc}
 170:	e3e00000 	mvn	r0, #0
 174:	e8bd8008 	pop	{r3, pc}
 178:	e3a00001 	mov	r0, #1
 17c:	e8bd8008 	pop	{r3, pc}
 180:	e3e00000 	mvn	r0, #0
 184:	e8bd8008 	pop	{r3, pc}
 188:	e3a00001 	mov	r0, #1
 18c:	e8bd8008 	pop	{r3, pc}
 190:	e3a00001 	mov	r0, #1
 194:	e8bd8008 	pop	{r3, pc}
 198:	00000000 	.word	0x00000000
