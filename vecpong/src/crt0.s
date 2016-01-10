;;;
;;; Copyright 2006, 2007, 2008, 2009 by Brian Dominy <brian@oddchange.com>
;;; ported to Vectrex, 2013 by Frank Buss <fb@frank-buss.de>
;;; extended by Martin White 2014
;;; further extended by Phillip Riscombe-Burton 2015
;;;
;;; This file is part of GCC.
;;;
;;; GCC is free software; you can redistribute it and/or modify
;;; it under the terms of the GNU General Public License as published by
;;; the Free Software Foundation; either version 3, or (at your option)
;;; any later version.
;;;
;;; GCC is distributed in the hope that it will be useful,
;;; but WITHOUT ANY WARRANTY; without even the implied warranty of
;;; MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
;;; GNU General Public License for more details.

;;; You should have received a copy of the GNU General Public License
;;; along with GCC; see the file COPYING3.  If not see
;;; <http://www.gnu.org/licenses/>.

;; Overview of registers for reference:
;; A-reg - 8bit reg - general purpose
;; B-reg - 8bit reg - general purpose
;; D-reg - 16bit reg - a & b combined - general purpose
;;
;; X-reg - 16bit reg - indexed mode instructions
;; Y-reg - 16bit reg - indexed mode instructions
;;
;; U-reg - stack pointer
;; S-reg - stack pointer
;;
;; PC-reg - program counter
;;
;; DP-reg - direct page reg (0- 255)  - fast & short access.
;;               d0 is base address of VIA 6522
;;
;; CC-reg - conduction code reg - results of some instructions (zero, neg, carry, borrow)
;;
;; Significant values
;; HEX 0x80 = 128 dec = 10000000

	; Include file defines which BIOS functions to switch on
	.include /crt.s/

	; Declare external for main()
	.globl _main

;#define __STACK_TOP 0xcbea

	; Declare all linker sections, and combine them into a single bank
	.bank prog
	.area .text  (BANK=prog)
;;;	.area .ctors (BANK=prog)

	; uncomment if we start using RAM
	.bank ram(BASE=0xc880,SIZE=0x36a,FSFX=_ram)
;;;	.area .data  (BANK=ram)
	.area .bss   (BANK=ram)

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;
	;;; cartridge init block
	;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.area  .text
    .ascii "g GCE 2016"				; cart id and year
	.byte  0x80						; string end
	.word  no_startup_music         ; can use 0xfd0d or other inbuilt music

    ;; Can place multiple pieces of text on this screen
    .byte  0xf8, 0x50, 0x20, -0x36	; char height, char width, rel y, rel x
    .ascii "VECPONG"                ; game title
	.byte  0x80						; string end

    .byte  0xfc, 0x30, -0x13, -0x3C	; char height, char width, rel y, rel x
    .ascii "MADE WITH VEC-C"        ;
	.byte  0x80						; string end
    .byte  0xfc, 0x30, -0x20, -0x46	; char height, char width, rel y, rel x
    .ascii "GITHUB.COM/PHILLRB"     ;
	.byte  0x80						; string end

	.byte  0						; header end

	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	;;;
	;;; __start : Entry point to the program
	;;;
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	.area	.text
	.globl __start
__start:

;;	;; copy .data and .bss areas to RAM
;;	ldx	#s_.data
;;	ldy	#0xc880
;;	ldb	#0xd
;;copyData:
;;	lda	,x+
;;	sta	,y+
;;	decb
;;	bne	copyData

	; start C program
	jmp	_main

no_startup_music:
	.byte   0x80    ;just return end marker

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; General utility functions...
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Prerequisite to many bios functions
;; Requires no parameters from C
;; Set the DP register to 'D0'
    .globl _DP_to_D0
_DP_to_D0:
    jsr     0xf1aa
    rts

;; Prerequisite to many bios functions
;; Set the DP register to 'C8'
    .globl _DP_to_C8
_DP_to_C8:
    jsr     0xf1af
    rts

.if d_RESET_0_REF
	.globl _reset0Ref
_reset0Ref:
    jsr     _DP_to_D0
	jsr		0xf354
	rts
.endif

.if	d_WAIT_RECAL
	.globl _waitRecal
_waitRecal:
    jsr     _DP_to_D0
	jsr		0xf192
	rts
.endif

;; input from C compiler:
;; - 8-bit argument in register b
.if d_INTENS_A
	.globl _intensA
_intensA:
	pshs	d           ; save registers
    jsr     _DP_to_D0
	tfr		b, a		; a = intensity
	jsr		0xf2ab
	puls	d, pc       ; restore registers and PC (=return)
.endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Delays...
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Numbered delays require no parameters from C
.if d_DELAY_0
	.globl _delay0
_delay0:
	jsr		0xf579
	rts
.endif

.if d_DELAY_1
	.globl _delay1
_delay1:
	jsr		0xf575
	rts
.endif

.if d_DELAY_2
	.globl _delay2
_delay2:
	jsr		0xf571
	rts
.endif

.if d_DELAY_3
	.globl _delay3
_delay3:
	jsr		0xf56d
	rts
.endif

.if d_DELAY_RTS
	.globl _delayRTS
_delayRTS:
	jsr		0xf57d
	rts
.endif

;; input from C compiler:
;; - 8-bit argument (unsigned int) count in register b
.if d_DELAY_B
	.globl _delayB
_delayB:
	jsr		0xf57a
	rts
.endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Move functions...
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; input from C compiler:
;; - first 8-bit argument x in register b
;; - second 8-bit argument y on stack
.if d_MOVETO_D
	.globl _moveToD
_moveToD:
	pshs	a, b, dp	; save registers
    jsr     _DP_to_D0
	lda		5, s		; a = y coordinate
	jsr		0xf312		; b = x coordinate
	puls	a, b, dp, pc	; restore registers and PC (=return)
.endif

;; input from C compiler:
;; - first 8-bit argument x in register b
;; - second 8-bit argument y on stack
.if d_MOVETO_D_7F
	.globl _moveToD7f
_moveToD7f:
	pshs	a, b, dp	; save registers
    jsr     _DP_to_D0
	lda		5, s		; a = y coordinate
	jsr		0xf2fc		; b = x coordinate
	puls	a, b, dp, pc	; restore registers and PC (=return)
.endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Rotate functions...
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
; input from C compiler:
; - 1st 16-bit argument in register x: pointer to packet style list
; - 2nd 16-bit argument in register u: pointer to new packet style list
.if d_ROT_VL_MODE
	.globl _rotVLMode
_rotVLMode:
    puls    u
    puls    u
	pshs	a, b, x, dp, pc
	jsr     _DP_to_C8
	lda     #05
	jsr		0xf61f
	puls	a, b, x, dp, pc
.endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Draw functions...
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; input from C compiler:
;; - 1st 16-bit argument in register x: pointer to packet style list
;; - 2nd 8-bit argument in register b: zskip: 0 = skip zeroing integrator, 1 = zero integrators
.if d_DRAW_VLP
	.globl _drawVLp
_drawVLp:
	pshs	a, b, x, dp	; save registers
    jsr     _DP_to_D0
	stb		0xc824		; ZSKIP
	jsr		0xf410
	puls	a, b, x, dp, pc	; restore registers and PC (=return)
.endif

.if d_DRAW_LINE_D
	.globl _drawLineD
_drawLineD:
	pshs	a, b, dp		; save registers
	clr		0xc823			; Set number of lines to 1 by clearing 0xC823
	jsr     _DP_to_D0
	lda		5, s			; get a back from the stack = Y pos (rel)
	jsr		0xf3df
	puls	a, b, dp, pc
.endif

; input from C compiler:
; - 1st 16-bit argument in register x: pointer to packet style list
; - 2nd 8-bit argument in register b: zskip: 0 = skip zeroing integrator, 1 = zero integrators
.if d_DRAW_VLP_7F
	.globl _drawVLp7F
_drawVLp7F:
	pshs	a, b, x, dp	; save registers
	jsr     _DP_to_D0
	stb		0xc824		; ZSKIP
	jsr		0xf408
	puls	a, b, x, dp, pc	; restore registers and PC (=return)
.endif

; input from C compiler:
; - 1st 16-bit argument in register x: pointer to packet style list
; - 2nd 8-bit argument in register b: zskip: 0 = skip zeroing integrator, 1 = zero integrators
.if d_DRAW_VLP_SCALE
	.globl _drawVLpScale
_drawVLpScale:
	pshs	a, b, x, dp	; save registers
	jsr     _DP_to_D0
	stb		0xc824		; ZSKIP
	jsr		0xf40C
	puls	a, b, x, dp, pc	; restore registers and PC (=return)
.endif

.if d_DOT_D
    .globl _dotD
_dotD:
    pshs	a, b, dp	; save registers
	jsr     _DP_to_D0
    lda     5, s        ; get a back from the stack = Y pos (rel)
    jsr     0xf2c3
    puls	a, b, dp, pc
.endif

.if d_DOT_HERE
    .globl _dotHere
_dotHere:
    pshs	a, b, dp	; save registers
    jsr     _DP_to_D0
    jsr     0xf2c5
    puls	a, b, dp, pc
.endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; String functions...
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; input from C compiler:
;; - 1st 8-bit value argument Y to register A
;; - 2nd 8-bit value argument X to register B
;; - 3rd pointer to string in register U
.if d_PRINT_STR_D
    .globl _printStrD
_printStrD:
	pshs	a, b, u, dp	; save registers
	jsr     _DP_to_D0
	lda		5, s		; a = y coordinate
    tfr     x, u        ; x holds string pointer - copy to u
	jsr		0xf37a		; b = x coordinate
	puls	a, b, u, dp, pc	; restore registers and PC (=return)
.endif

;; input from C compiler:
;; - 1st pointer to string in register X - copy to U
.if d_PRINT_STR
	.globl _printStr
_printStr:
	pshs	d, u, dp
	jsr     _DP_to_D0
	tfr		x, u     ;;x holds string pointer - copy to u
	jsr		0xf495
	puls	d, u, dp, pc
.endif

;; input from C compiler:
;; A reg - ship icon char
;; B reg - number of ships
;; U is trashed at end
.if d_PRINT_SHIPS
    .globl _printShips
_printShips:
	pshs	d, u
    jsr     _DP_to_D0
    lda     6, s    ; get ship icon
    jsr     0xf391
    puls	d, u, pc
.endif

;; input from C compiler:
;; A reg - ship icon char
;; B reg - number of ships
;; X reg - y,x co-ords
;; U is trashed at end
.if d_PRINT_SHIPS_X
    .globl _printShipsX
_printShipsX:
	pshs	d, x, u
    jsr     _DP_to_D0
    lda     10, s   ; get y
    ldb     9, s    ; get x
    tfr     d, x    ; move x,y to X reg
    lda     8, s    ; get ship icon
    ldb     1, s    ; get number of ships
    jsr     0xf393
    puls	d, x, u, pc
.endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; I/O functions...
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
.if d_JOY_ANALOG
	.globl _joyAnalog
_joyAnalog:
;    lda     0x4F
;    sta     0xc823
	jsr     _DP_to_D0
	jsr		0xf1f5
	rts
.endif

.if d_JOY_DIGITAL
	.globl _joyDigital
_joyDigital:
	pshs	a, b, x, dp
	jsr     _DP_to_D0
	jsr		0xf1f8
	puls	a, b, x, dp, pc
.endif

; set the joyEnableFlags
;BUG: P2 U/D ENABLED PRODUCES BUZZ IN PARAJ EMU!
.if d_JOY_ANALOG | d_JOY_DIGITAL
    .globl _joyEnableFlags
_joyEnableFlags:
	pshs	a, b, x, dp     ; save registers
    stb     0xc81f          ; p1 l/r = 1 to be enabled
    lda		5, s
    cmpa    #0
    beq     _joyP1UDFlag
    lda     #3              ; p1 u/d = 3 to be enabled
_joyP1UDFlag:
    sta     0xc820          ; either store 0 or 3
    lda		8, s
    cmpa    #0
    beq     _joyP2LRFlag
    lda     #5              ; p2 l/r = 5 to be enabled
_joyP2LRFlag:
    sta     0xc821
    lda		9, s
    cmpa    #0
    beq     _joyP2UDFlag
    lda     #7              ; p2 l/r = 5 to be enabled
_joyP2UDFlag:
    sta     0xc822
    puls	a, b, x, dp, pc        ; restore registers and PC (=return)
.endif

; $C81A must be set to a power of 2, to control conversion resolution; 0x80 is least accurate, and 0x00 is most accurate.
.if d_JOY_ANALOG
    .globl _joyAnalogResolution
_joyAnalogResolution:
    ldx     0xc81a      ; store it in appropriate RAM location
    lda     0x00
    sta     ,x+
    rts
.endif

.if d_READ_BTN
	.globl _readButton
_readButton:
	pshs	b, x, dp
	jsr		0xf1ba
	puls	b, x, dp, pc
.endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Music functions...
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;

.if d_DO_SOUND
	.globl _doSound
_doSound:
    pshs	d, x, u	; save registers
	jsr     _DP_to_D0
	jsr		0xf289
	puls	d, x, u, pc	; restore registers and PC (=return)
.endif

; input from C compiler:
; - 1st 16-bit argument in register x: pointer to music list
.if d_INIT_MUSIC_CHK
    .globl _initMusicCHK
_initMusicCHK:
	pshs	d, x, y, u, dp	; save registers
    jsr     _DP_to_C8
	tfr		x, u
	jsr		0xf687
	puls	d, x, y, u, dp, pc	; restore registers and PC (=return)
.endif

.if d_INIT_MUSIC_BUF
    .globl _initMusicBuf
_initMusicBuf:
    pshs	d, x
	jsr		0xf533
	puls	d, x, pc
.endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Sound functions...
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;

; A-reg = which of the 15 sound chip registers to modify
; B-reg = the byte of sound data
.if d_SOUND_BYTE
    .globl _soundByte
_soundByte:
    pshs	d, dp       ; save registers
    jsr     _DP_to_D0
    lda		5, s        ;get music reg off stack
    jsr     0xf256
    puls	d, dp, pc	; restore registers and PC (=return)
.endif

; Turns off all sounds
.if d_CLEAR_SOUND
    .globl _clearSound
_clearSound:
    jsr     _DP_to_D0
    jsr     0xf272
    rts
.endif

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; Score functions...
;;
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;
;; input from C compiler:
;; - first 16-bit argument score location in register X
;;
.if d_CLEAR_SCORE
	.globl _clearScore
_clearScore:
	pshs	d
	lda		#0x80
	sta		6, x
	jsr		0xf84f
	puls	d, pc
.endif

;; 1st 8-bit argument value in register b
;; 1st 16-bit argument score location in register X
;; 
;; Not sure what to do about U? BIOS mentions it contains BCD value of 8bit value
.if d_ADD_SCORE_A
	.globl _addScoreA
_addScoreA:
	pshs	d, u
	tfr		b, a
	jsr		0xf85e
	puls	d, u, pc
.endif
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

	.end __start
