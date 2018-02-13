parse	macro	av0,av1,av2,av3,av4,av5,av6,av7
	irp	avar,<av0,av1,av2,av3,av4,av5,av6,av7>
	ifnb	<avar>
	mov	avar,0
	endif
	endm
	mov	si,80h
	lodsb
	cbw
	or	ax,ax
	jz	@@x
	mov	cx,ax
	mov	di,si
	mov	al,' '
	irp	avar,<av0,av1,av2,av3,av4,av5,av6,av7>
	local	@@1
	ifnb	<avar>
	repe	scasb
	je	@@x
	dec	di
	mov	avar,di
	inc	di
	repne	scasb
	jne	@@1
	dec	di
@@1:	mov	byte ptr es:[di],0
	jcxz	@@x
	inc	di
	endif
	endm
@@x	equ	this near
	endm
