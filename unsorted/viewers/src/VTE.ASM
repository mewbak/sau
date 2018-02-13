cseg	segment	para public 'code'
	assume cs:cseg,ds:cseg
	org	100h
	.286
	include	parse.asm
start:	mov	dx,offset wjcp
	mov	ah,9
	int	21h
	parse	pnm1a,pnm2a,tnma
	mov	ax,3D00h
	mov	dx,pnm1a
	int	21h
	jnc	nx
nnx:	mov	dx,offset perr
	mov	ah,9
	int	21h
	int	20h
nx:	xchg	ax,bx
	mov	dx,offset pal1
	mov	ah,3Fh
	mov	cx,300h
	int	21h
	mov	ah,3Eh
	int	21h
	mov	ax,3D00h
	mov	dx,pnm2a
	int	21h
	jc	nnx
	xchg	ax,bx
	mov	dx,offset pal2
	mov	ah,3Fh
	mov	cx,300h
	int	21h
	mov	ah,3Eh
	int	21h
	mov	di,offset ttb
	mov	al,0
	mov	cx,256
mlt:	stosb
	inc	ax
	loop	mlt
	mov	ax,3D00h
	mov	dx,tnma
	int	21h
	jnc	mtnx
	mov	tnma,offset tnm
	jmp	short mts
mtnx:	xchg	ax,bx
	mov	dx,offset ttb
	mov	ah,3Fh
	mov	cx,100h
	int	21h
	mov	ah,3Eh
	int	21h
mts:	mov	di,offset pal1
	call	bfd
	mov	bl1,cl
	mov	di,offset pal2
	call	bfd
	mov	bl2,cl
	mov	ax,13h
	int	10h
	mov	ax,0A000h
	mov	es,ax
	xor	di,di
	call	strip
	mov	di,0E100h
	call	strip
	mov	di,1900h
	mov	al,bl1
	mov	ah,al
	mov	cx,4800
	rep	stosw
	mov	di,0BB80h
	mov	al,bl2
	mov	ah,al
	mov	cx,4800
	rep	stosw
	mov	di,4B4Eh
	mov	ax,202h
	call	sqr
	mov	di,4B80h
	mov	ax,303h
	call	sqr
	mov	al,0FFh
	out	21h,al
	jmp	mkst
mlp:	mov	dx,3DAh
ma1:	in	al,dx
	test	al,8
	jz	ma1
ma2:	in	al,dx
	test	al,8
	jnz	ma2
	mov	si,offset pal1
	mov	dl,0C8h
	mov	al,0
	out	dx,al
	inc	dx
	mov	cx,300h
	cli
	rep	outsb
	sti
	mov	cx,40
	call	swait
	mov	si,offset mypal
	mov	dl,0C8h
	mov	al,0
	out	dx,al
	inc	dx
	mov	cx,12
	cli
	rep	outsb
	sti
	mov	cx,20
	call	swait
	mov	si,offset pal2+12
	mov	dl,0C8h
	mov	al,4
	out	dx,al
	inc	dx
	mov	cx,2F4h
	cli
	rep	outsb
	sti
	mov	cx,270
	call	swait
	mov	si,offset pal2
	mov	dl,0C8h
	mov	al,0
	out	dx,al
	inc	dx
	mov	cx,12
	cli
	rep	outsb
	sti
;успевает записать около 9 цветов за время вывода строки изображения
	in	al,64h
	test	al,1
	jz	mlp
	in	al,60h
	cmp	al,1
	jne	mknx
	jmp	quit
mk9:	mov	ah,3Ch
	mov	cx,20h
	mov	dx,tnma
	int	21h
	jc	mnw
	xchg	ax,bx
	mov	dx,offset ttb
	mov	ah,40h
	mov	cx,100h
	int	21h
	mov	ah,3Eh
	int	21h
mnw:	mov	al,0FFh
	out	21h,al
	jmp	mlp
mknx:	cmp	al,28
	je	mk8	
	cmp	al,60
	je	mk9
	cmp	al,71
	jb	mlj
	je	mk5
	cmp	al,82
	ja	mlj
	je	mk7
	cmp	al,73
	je	mk6
	cmp	al,75
	je	mk1
	cmp	al,77
	je	mk2
	cmp	al,79
	je	mk3
	cmp	al,81
	je	mk4
mlj:	jmp	mlp
mk8:	mov	bl,el1
	mov	bh,0
	mov	al,el2
	mov	ttb[bx],al
	jmp	mlp
mk7:	xor	stp,11h
	jmp	mlp
mk1:	call	hcd
	mov	al,stp
	sub	el1,al
	jmp	short mku
mk2:	call	hcd
	mov	al,stp
	add	el1,al
mku:	call	hcu
	jmp	short mlp0
mk3:	call	lcd
	mov	al,stp
	sub	el2,al
	jmp	short mkd
mk4:	call	lcd
	mov	al,stp
	add	el2,al
	jmp	short mkd
mk5:	call	hcd
	call	lcd
	mov	al,el1
	sub	al,stp
	jmp	short mkt
mk6:	call	hcd
	call	lcd
mkst:	mov	al,el1
	add	al,stp
mkt:	mov	el1,al
	mov	bx,offset ttb
	xlat
	mov	el2,al
	call	hcu
mkd:	call	lcu
mlp0:	call	pout
	call	ppick
	jmp	mlp
quit:	mov	al,0
	out	21h,al
	mov	ax,3
	int	10h
	int	20h
bfd:	xor	ax,ax
	mov	cx,100h
mpl:	scasw
	jne	mlop
	scasb
	je	mfd
	dec	di
mlop:	inc	di
	loop	mlop
	neg	cl
mfd:	neg	cl
	retn
strip:	mov	cx,20
m2:	push	cx
	mov	al,0
	mov	cx,100h
m1:	stosb
	inc	ax
	loop	m1
	pop	cx
	add	di,64
	loop	m2
	retn
sqr:	mov	cx,80
mlq:	push	cx
	mov	cl,25
	rep	stosw
	pop	cx
	add	di,270
	loop	mlq
	retn
pout:	mov	al,el1
	mov	dx,601h
	call	pout0
	mov	al,el2
	mov	dx,121Dh
pout0:	push	es
	push	cs
	pop	es
	mov	bx,offset ctb
	mov	bp,offset stg1
	aam	16
	xlat
	mov	ds:[bp]+8,al
	mov	al,ah
	xlat
	mov	ds:[bp]+6,al
	mov	ax,1302h
	mov	cx,6
	mov	bh,0
	int	10h
	pop	es
	retn
ppick:	mov	bl,el1
	mov	bh,0
	mov	ax,bx
	add	bx,bx
	add	bx,ax
	mov	ax,word ptr pal1[bx]
	mov	bl,pal1[bx]+2
	mov	word ptr col1,ax
	mov	col1+2,bl
	mov	bl,el2
	mov	bh,0
	mov	ax,bx
	add	bx,bx
	add	bx,ax
	mov	ax,word ptr pal2[bx]
	mov	bl,pal2[bx]+2
	mov	word ptr col2,ax
	mov	col2+2,bl
	retn
swait:	mov	dl,0DAh
ma3:	in	al,dx
	test	al,1
	jnz	ma3
ma4:	in	al,dx
	test	al,1
	jz	ma4
	loop	ma3
	retn
hcd:	mov	al,0
hc:	mov	bl,el1
	mov	bh,0
	lea	di,[bx]+1B80h
	mov	cl,5
mcs1:	stosb
	add	di,319
	loop	mcs1
	retn
hcu:	mov	al,1
	jmp	short hc
lcd:	mov	al,0
lc:	mov	bl,el2
	mov	bh,0
	lea	di,[bx]+0D840h
	mov	cl,5
mcs2:	stosb
	add	di,319
	loop	mcs2
	retn
lcu:	mov	al,1
	jmp	short lc
wjcp	db	'Visual TTB Editor',13,10
	db	'Copyright (C) 1995 by WhiteJaguar',13,10,10,'$'
perr	db	'Usage:VTE src_pal.PAL dest_pal.PAL [ttb_file.TTB]',13,10
	db	'Internal keys:ESC-immediate exit,',13,10
	db	'              Left/Right-choose source color,',13,10
	db	'              End/PgDn-choose destination color,',13,10
	db	'              Home/PgUp-choose source color with dest. color'
	db	13,10,'                        selection through TTB,',13,10
	db	'              Ins-switch selectors movement speed,',13,10
	db	'              Enter-select new dest.color in TTB,',13,10
	db	'              F2-save updated TTB to disk',13,10,'$'
ctb	db	'0123456789ABCDEF'
tnm	db	'default.ttb',0
stg1	db	219,0,219,0,8,8
stg	db	0,1,0,1
stp	db	1
el1	db	255
el2	db	0
mypal	db	0,0,0,63,63,63
col1	db	?,?,?
col2	db	?,?,?
pnm1a	dw	?
pnm2a	dw	?
tnma	dw	?
bl1	db	?
bl2	db	?
pal1	db	768 dup(?)
pal2	db	768 dup(?)
ttb	db	256 dup(?)
cseg	ends
end	start
