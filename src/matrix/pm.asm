GLOBAL  _READ_MSW, _READ_CR0, _WRITE_CR0, _LGDT, _UPDATE_CS

SEGMENT _TEXT PUBLIC CLASS=CODE USE16

_READ_MSW:
        smsw    ax
        retn

_READ_CR0:
        mov     eax, cr0	; read CR0 to eax
        mov     edx, eax
        shr     edx, 16		; dx:ax = CR0 (return value)
        retn

_WRITE_CR0:
        push    bp
        mov     bp, sp
        mov     eax, [ss:bp+4]	; eax = 32-bit parameter
        mov     cr0, eax
        pop     bp
        retn

_LGDT:
	push	bp
	mov	bp, sp
	push	bx
	mov     bx, [ss:bp+4]	; ds:bx = pointer to GDTR structure
	lgdt	[ds:bx]		; load GDTR
	pop	bx
	pop	bp
	retn

_UPDATE_CS:
	push	bp
	mov	bp, sp
	mov     ax, [ss:bp+4]	; ax = new cs
        push    ax		; push segment
        push    word .1		; push offset
        retf			; we have a new cs now
.1:
	pop	bp
	retn

SEGMENT _DATA PUBLIC CLASS=DATA
