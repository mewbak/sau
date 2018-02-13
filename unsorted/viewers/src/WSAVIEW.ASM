cseg	segment	para public 'code'
	assume	cs:cseg,ds:cseg,ss:cseg
	.286
	include parse.asm
	org	100h
start:	mov	dx,offset nme
	mov	ah,9
	int	21h
	parse	fnm,pnm
	mov	ax,3D00h
	mov	dx,pnm
	int	21h
	jc	mnx
	xchg	ax,bx
	mov	ah,3Fh
	mov	cx,768
	mov	dx,offset pal
	int	21h
	jc	mq
	mov	ah,3Eh
	int	21h
	jc	mq
mnx:	mov	ax,3D00h
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
	mov	cx,14
	int	21h
	jc	mq
	cmp	pfg,1
	jne	mfr
mpl1:	mov	dx,offset fix
	mov	cx,4
	mov	ah,3Fh
	int	21h
	les	dx,fix
	mov	cx,es
	mov	ax,cx
	or	ax,dx
	jz	mpl1
	mov	ax,4200h
	int	21h
	mov	dx,offset pal
	mov	cx,768
	mov	dlt,cx
	mov	ah,3Fh
	int	21h
mfr:	push	bx
	mov	ax,13h
	int	10h
	mov	ax,1012h
	xor	bx,bx
	mov	cx,256
	mov	dx,offset pal
	push	cs
	pop	es
	int	10h
	pop	bx
	mov	ax,cs
	add	ax,1000h
	mov	es,ax
	xor	di,di
	mov	ax,di
	mov	cx,32000
	rep	stosw
mfl:	mov	dx,idx
	inc	idx
	cmp	dx,pgc
	ja	mq1
	xor	cx,cx
	shl	dx,2
	add	dx,14
	adc	cx,0
	mov	ax,4200h
	int	21h
	mov	dx,offset fix
	mov	cx,6
	mov	ah,3Fh
	int	21h
	les	dx,fix
	mov	cx,es
	mov	ax,cx
	or	ax,dx
	jz	mfl
	mov	si,fix1
	sub	si,dx
	add	dx,dlt
	adc	cx,0
	mov	ax,4200h
	int	21h
	mov	cx,si
	mov	ax,cs
	add	ax,2000h
	mov	ds,ax
	add	ax,1000h
	mov	es,ax
	xor	dx,dx
	mov	ah,3Fh
	int	21h
	push	bx
	xor	di,di
	mov	cx,cs:bsz
	mov	ax,di
	rep	stosb
	mov	di,ax
	mov	si,ax
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
mwq:	push	es
	pop	ds
	mov	ax,cs
	add	ax,1000h
	mov	es,ax
	xor	si,si
	mov	di,si
mdw:	mov	ah,0
	lodsb
	or	al,al
	js	md2
	jnz	md1
	lodsb
md0:	mov	cx,ax
	lodsb
mdl1:	xor	es:[di],al
	inc	di
	loop	mdl1
	jmp	short mdw
md1:	mov	cx,ax
mdl2:	lodsb
	xor	es:[di],al
	inc	di
	loop	mdl2
	jmp	short mdw
md2:	and	al,7Fh
	jz	md3
md4:	add	di,ax
	jmp	short mdw
md3:	lodsw
	or	ax,ax
	jz	mwi
	jns	md4
	test	ah,40h
	pushf
	and	ah,3Fh
	popf
	jz	md1
	jmp	short md0
mwi:	push	es
	pop	ds
	mov	ax,0A000h
	mov	es,ax
	xor	si,si
	mov	di,si
	mov	bx,cs:len
	mov	dx,cs:hei
	mov	ax,di
mlo:	mov	cx,bx
	rep	movsb
	add	ax,320
	mov	di,ax
	dec	dx
	jnz	mlo
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
	pop	bx
	jmp	mfl	
nme	db	'.WSA files viewer',13,10
	db	'(C) 1996 by White Jaguar',13,10,'$'
perr	db	'Usage: WSAVIEW filename.WSA [filename.PAL]',13,10
	db	'Internal keys: ESC - immediate exit.',13,10,'$'
dlt	dw	0
idx	dw	0
pgc	dw	?
fill	dd	?
len	dw	?
hei	dw	?
bsz	dw	?
pfg	dw	?
fnm	dw	?
pnm	dw	?
hdl	dw	?
fix	dd	?
fix1	dw	?
pal	db	768 dup(?)
cseg	ends
end	start