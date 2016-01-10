;;
;;  crt.s
;;
;;  Created by Martin White on 09/05/2014.
;;  Copyright (c) 2014 Martin White. All rights reserved.
;;
;;	Permission is hereby granted, free of charge, to any person obtaining a
;;	copy of this software and associated documentation files (the
;;	"Software"), to deal in the Software without restriction, including
;;	without limitation the rights to use, copy, modify, merge, publish,
;;	distribute, sublicense, and/or sell copies of the Software, and to
;;	permit persons to whom the Software is furnished to do so, subject to
;;	the following conditions:
;;
;;	The above copyright notice and this permission notice shall be included
;;	in all copies or substantial portions of the Software.
;;
;;	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
;;	OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
;;  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
;;	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
;;	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
;;	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
;;	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

;; Uncomment each BIOS routines as it's implemented
;;
;; Enable each BIOS with 1 to use
;;
;; Doing this prevents bloating the final ROM - space is precious!

;; A
;d_Abs_a_b						; ($F584)
;d_Abs_b						; ($F58B)
d_ADD_SCORE_A		== 0		; ($F85E)
;d_ADD_SCORE_D					; ($F87C)

;; B
;d_Bitmask_a					; ($F57E)

;; C
;d_Check0Ref					; ($F34F)
;d_Clear_C8_RAM					; ($F542)
d_CLEAR_SCORE		== 0		; ($F84F)
d_CLEAR_SOUND       == 1        ; ($F272)
;d_Clear_x_256					; ($F545)
;d_Clear_x_b					; ($F53F)
;d_Clear_x_b_80					; ($F550)
;d_Clear_x_b_a					; ($F552)
;d_Clear_x_d					; ($F548)
;d_Cold_Start					; ($F000)
;d_Compare_Score				; ($F8C7)

;; D
;d_Dec_3_Counters				; ($F55A)
;d_Dec_6_Counters				; ($F55E)
;d_Dec_Counters					; ($F563)
d_DELAY_0			== 0		; ($F579)
d_DELAY_1			== 0		; ($F575)
d_DELAY_2			== 0		; ($F571)
d_DELAY_3			== 0		; ($F56D)
d_DELAY_B			== 0		; ($F57A)
d_DELAY_RTS			== 0		; ($F57D)
;d_Display_Option				; ($F835)
d_DO_SOUND			== 1		; ($F289)
;d_Do_Sound_x					; ($F28C)
d_DOT_D             == 0		; ($F2C3)
d_DOT_HERE			== 0		; ($F2C5)
;d_Dot_ix_b						; ($F2BE)
;d_Dot_ix						; ($F2C1)
;d_Dot_List						; ($F2D5)
;d_Dot_List_Reset				; ($F2DE)
;d_DP_to_C8						; ($F1AF)
;d_DP_to_D0						; ($F1AA)
;d_Draw_Grid_VL					; ($FF9F)
d_DRAW_LINE_D		== 1		; ($F3DF)
;d_Draw_Pat_VL					; ($F437)
;d_Draw_Pat_VL_a				; ($F434)
;d_Draw_Pat_VL_d				; ($F439)
;d_Draw_VL						; ($F3DD)
;d_Draw_VL_a					; ($F3DA)
;d_Draw_VL_ab					; ($F3D8)
;d_Draw_VL_b					; ($F3D2)
;d_Draw_VL_mode					; ($F46E)
;d_Draw_VLc						; ($F3CE)
;d_Draw_VLcs					; ($F3D6)
d_DRAW_VLP			== 1		; ($F410)
d_DRAW_VLP_7F		== 0		; ($F408)
;d_Draw_VLp_b					; ($F40E)
;d_Draw_VLp_FF					; ($F404)
d_DRAW_VLP_SCALE	== 0		; ($F40C)

;; E
;d_EXPLOSION_SND                ; ($F92E)

;; G
;d_Get_Rise_Idx					; ($F5D9)
;d_Get_Run_Idx					; ($F5DB)

;; I
;d_Init_Music					; ($F68D)
d_INIT_MUSIC_BUF	== 0        ; ($F533)
d_INIT_MUSIC_CHK	== 1		; ($F687)
;d_Init_Music_dft				; ($F692)
;d_Init_OS						; ($F18B)
;d_Init_OS_RAM					; ($F164)
;d_Init_VIA						; ($F14C)
;d_Intensity_1F					; ($F29D)
;d_Intensity_3F					; ($F2A1)
;d_Intensity_5F					; ($F2A5)
;d_Intensity_7F					; ($F2A9)
d_INTENS_A			== 1		; ($F2AB)

;; J
d_JOY_ANALOG		== 1		; ($F1F5)
d_JOY_DIGITAL		== 0		; ($F1F8)

;; M
;d_Mov_Draw_VL					; ($F3BC)
;d_Mov_Draw_VL_ab				; ($F3B7)
;d_Mov_Draw_VL_a				; ($F3B9)
;d_Mov_Draw_VL_d				; ($F3BE)
;d_Mov_Draw_VLc_a				; ($F3AD)
;d_Mov_Draw_VLc_b				; ($F3B1)
;d_Mov_Draw_VLcs				; ($F3B5)
;d_Move_Mem_a					; ($F683)
;d_Move_Mem_a_1					; ($F67F)
d_MOVETO_D			== 1		; ($F312)
d_MOVETO_D_7F		== 0		; ($F2FC)
;d_Moveto_ix					; ($F310)
;d_Moveto_ix_7F					; ($F30C)
;d_Moveto_ix_b					; ($F30E)
;d_Moveto_ix_FF					; ($F308)
;d_Moveto_x_7F					; ($F2F2)

;; N
;d_New_High_Score				; ($F8D8)

;; O
;d_Obj_Hit						; ($F8FF)
;d_Obj_Will_Hit					; ($F8F3)
;d_Obj_Will_Hit_u				; ($F8E5)

;; P
d_PRINT_STR_D		== 1		; ($F37A)
;d_Print_List					; ($F38A)
;d_Print_List_chk				; ($F38C)
;d_Print_List_hw				; ($F385)
d_PRINT_SHIPS		== 0		; ($F391)
d_PRINT_SHIPS_X		== 1        ; ($F393)
d_PRINT_STR			== 1		; ($F495)
;d_Print_Str_hwyx				; ($F373)
;d_Print_Str_yx					; ($F378)

;; R
;d_Random						; ($F517)
;d_Random_3						; ($F511)
;d_READ_BTN_MASKED	== 0		; ($F1B4)
d_READ_BTN			== 1		; ($F1BA)
;d_Recalibrate					; ($F2E6)
;d_Reset_Pen					; ($F35B)
;d_Reset0Int					; ($F36B)
d_RESET_0_REF		== 1		; ($F354)
;d_Reset0Ref_D0					; ($F34A)
;d_Rise_Run_Angle				; ($F593)
;d_Rise_Run_Idx					; ($F5EF)
;d_Rise_Run_Len					; ($F603)
;d_Rise_Run_X					; ($F5FF)
;d_Rise_Run_Y					; ($F601)
;d_Rot_VL						; ($F616)
;d_Rot_VL_ab					; ($F610)
;d_ROT_VL_M_DFT		== 0		; ($F62B)
d_ROT_VL_MODE		== 0		; ($F61F)

;; S
;d_Select_Game					; ($F7A9)
;d_Set_Refresh					; ($F1A2)
d_SOUND_BYTE		== 1    	; ($F256)
;d_Sound_Byte_x					; ($F259)
;d_Sound_Byte_raw				; ($F25B)
;d_Sound_Bytes					; ($F27D)
;d_Sound_Bytes_x				; ($F284)
;d_Strip_Zeros					; ($F8B7)

;; W
d_WAIT_RECAL		== 1		; ($F192)
;d_Warm_Start					; ($F06C)

;; X
;d_Xform_Rise					; ($F663)
;d_Xform_Rise_a					; ($F661)
;d_Xform_Run					; ($F65D)
;d_Xform_Run_a					; ($F65B)