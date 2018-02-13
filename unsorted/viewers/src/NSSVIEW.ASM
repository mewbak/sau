cseg	segment	para public 'code'
	assume cs:cseg,ds:cseg
	include parse.asm
	.286
	org	100h
start:	mov	dx,offset wjcp
	mov	ah,9
	int	21h
	parse	svn1
	mov	ax,3D00h
	mov	dx,svn1
	or	dx,dx
	jnz	mw1
	jmp	mx
mw1:	int	21h
	jnc	mw2
	jmp	errd
mw2:	xchg	ax,bx
	mov	dx,offset pbuf
	mov	cx,300h
	mov	ah,3Fh
	int	21h
	mov	dx,311h
	xor	cx,cx
	mov	ax,4200h
	int	21h
	mov	ax,ds
	add	ax,1000h
	mov	ds,ax
	mov	cx,0FFFFh
	xor	dx,dx
	mov	ah,3Fh
	int	21h
	push	ax
	mov	ah,3Eh
	int	21h
	mov	ax,13h
	int	10h
	jc	errv
	mov	ax,1012h
	xor	bx,bx
	mov	cx,256
	mov	dx,offset pbuf
	int	10h
	jc	errv0
	mov	ax,0A000h
	mov	es,ax
	xor	si,si
m3:	xor	di,di
	lodsw
	mov	bx,ax
	lodsw
	mov	dx,ax
	add	si,6
ml2:	mov	cx,bx
	push	di
ml1:	lodsb
	test	al,al
	jz	m1
	mov	es:[di],al
m1:	inc	di
	loop	ml1
	pop	di
	add	di,320
	dec	dx
	jnz	ml2
	mov	ah,0
	int	16h
	cmp	al,' '
	jne	m2
	xor	di,di
	mov	cx,32000
	xor	ax,ax
	rep	stosw
m2:	pop	cx
	cmp	si,cx
	push	cx
	jb	m3
	mov	ax,3
	int	10h
	int	20h
mx:	mov	dx,offset perr
mx1:	mov	ah,9
	push	cs
	pop	ds
	int	21h
	int	20h	
errd:	mov	dx,offset derr
	jmp	short mx1
errv0:	mov	ax,3
	int	10h
errv:	mov	dx,offset aerr
	jmp	short mx1
wjcp	db	'.NSS files viewer',13,10
	db	'Copyright (C) 1999 by WhiteJaguar',13,10,10,'$'
perr	db	'Usage:NSSVIEW filename.NSS',13,10,'$'
derr	db	'Disk error!',13,10,'$'
aerr	db	'Video error!',13,10,'$'
svn1	dw	?
pbuf	db	768 dup(?)
cseg	ends
end	start
