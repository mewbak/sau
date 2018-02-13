cseg	segment	para public 'code'
	assume	cs:cseg,ds:cseg,ss:cseg
	.286
	include parse.asm
	org	100h
start:	mov	sp,offset stken
	mov	ax,sp
	shr	ax,4
	inc	ax
	mov	bx,cs
	add	bx,ax
	mov	wsg,bx
	add	bx,1000h
	mov	ssg,bx
	mov	dx,offset nme
	mov	ah,9
	int	21h
	parse	fnm,pnm
	mov	ax,3D00h
	mov	dx,pnm
	int	21h
	jc	mq
	xchg	ax,bx
	mov	ah,3Fh
	mov	cx,768
	mov	dx,offset pal
	int	21h
	jc	mq
	mov	ah,3Eh
	int	21h
	jc	mq
	mov	ax,3D00h
	mov	dx,fnm
	int	21h
	jnc	mwr
mq:	mov	dx,offset perr
	mov	ah,9
	int	21h
	int	20h
mq1:	mov	ax,3
	int	10h
	int	20h
mwr:	xchg	ax,bx
	mov	ah,3Fh
	mov	dx,offset pgc
	mov	cx,2
	int	21h
	jc	mq
	mov	ax,4202h
	cwd
	mov	cx,dx
	int	21h
	push	ax
	mov	ax,4200h
	cwd
	mov	cx,dx
	int	21h
	pop	cx
	dec	cx
	dec	cx
	cmp	cx,pgc
	jne	mld
	inc	cx
	inc	cx
	mov	ah,3Fh
	xor	dx,dx
	mov	ds,wsg
	int	21h
	mov	si,0Ah
	xor	di,di
	mov	es,cs:ssg
	call	cpsx
	jmp	short mfr
mld:	mov	ds,ssg
	xor	dx,dx
	mov	cx,8000h
mldl:	mov	ah,3Fh
	int	21h
	cmp	ax,cx
	jb	mfr
	mov	ax,ds
	add	ax,800h
	mov	ds,ax
	jmp	short mldl
mqq1:	jmp	mq1
mfr:	mov	ah,3Eh
	int	21h
	push	cs
	pop	ds
	mov	ax,13h
	int	10h
	mov	ax,1012h
	xor	bx,bx
	mov	cx,256
	mov	dx,offset pal
	push	cs
	pop	es
	int	10h
mfl:	mov	bx,idx
	inc	idx
	mov	ds,ssg
	cmp	bx,ds:[0000h]
	jae	mqq1
	shl	bx,2
	les	bx,[bx]+2
	mov	ax,es
	add	bx,2
	adc	ax,0
	mov	ah,bl
	shr	bx,4
	shl	al,4
	or	bh,al
	mov	al,ah
	and	ax,0Fh
	mov	si,ax
	mov	ax,ds
	add	ax,bx
	mov	ds,ax
	lodsw
	mov	bx,ax
	mov	cs:typ,ax
	xor	ax,ax
	lodsb
	mov	cs:hei,ax
	lodsw
	mov	cs:len,ax
	add	si,5		;buffer is 64K, so buffer size ignored
	test	bl,1
	jz	mfm1
	mov	ax,10h
	test	bl,4
	jz	mfm2
	lodsb
mfm2:	mov	cs:tbsg,ds
	mov	cs:tbof,si
	add	si,ax
mfm1:	test	bl,2
	jnz	mfm3
	mov	es,cs:wsg
	xor	di,di
	call	cpsx
	push	es
	pop	ds
	xor	si,si
mfm3:	mov	ax,0A000h
	mov	es,ax
	xor	ax,ax
	mov	di,ax
	mov	dx,cs:typ
	mov	bx,cs:tbof
mfl2:	push	di
	mov	cx,cs:len
	add	cx,di
mfl1:	lodsb
	or	al,al
	jnz	mfm5
	lodsb
	add	di,ax
	jmp	short mfm6
mfm5:	test	dl,1
	jz	mfm4
	push	ds
	mov	ds,cs:tbsg
	xlat
	pop	ds
mfm4:	stosb
mfm6:	cmp	cx,di
	ja	mfl1
	pop	di
	add	di,320
	dec	cs:hei
	jnz	mfl2
	mov	ah,0
	int	16h
	cmp	al,27
	jne	mfn
	jmp	mq1	
mfn:	xor	ax,ax
	mov	di,ax
	mov	cx,32000
	rep	stosw
	push	cs
	pop	ds
	jmp	mfl	
cpsx:	xor	ax,ax
mlp:	lodsb
	cmp	al,80h
	je	mwq
	cmp	al,0FEh
	jbe	mo1
	lodsw
	mov	cx,ax
mcp:	lodsw
	push	si
	mov	si,ax
mcp1:	cli
	seges
	rep	movsb
	sti
	pop	si
	jmp	short mlp
mo1:	jb	mo2
	lodsw
	mov	cx,ax
	lodsb
	rep	stosb
	jmp	short mlp
mo2:	mov	ch,0
	test	al,0C0h
	jns	mo4
	jnp	mo3
	and	al,3Fh
	add	al,3
	mov	cl,al
	jmp	short mcp
mo3:	and	al,3Fh
	mov	cl,al
	rep	movsb
	jmp	short mlp
mo4:	mov	cl,al
	shr	cl,4
	add	cl,3
	and	al,0Fh
	mov	ah,al
	lodsb
	push	si
	mov	si,di
	sub	si,ax
	jmp	short mcp1
mwq:	retn
nme	db	'.SHP files viewer',13,10
	db	'(C) 1996 by White Jaguar',13,10,'$'
perr	db	'Usage: SHPVIEW filename.SHP filename.PAL',13,10
	db	'Internal keys: ESC - immediate exit.',13,10,'$'
dlt	dw	0
idx	dw	0
fnm	dw	?
pnm	dw	?
wsg	dw	?
ssg	dw	?
pgc	dw	?
typ	dw	?
hei	dw	?
len	dw	?
tbof	dw	?
tbsg	dw	?
pal	db	768 dup(?)
stk	db	1024 dup(?)
stken	equ	this byte
cseg	ends
end	start