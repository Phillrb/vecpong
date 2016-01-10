//
//  sounds.h
//
//  Created by Phillip Riscombe-Burton on 04/12/2015.
//  Based upon Christopher L. Tumber work on http://vectrexmuseum.com/
//  Copyright (c) 2015 Phillip Riscombe-Burton. All rights reserved.
//
//	Permission is hereby granted, free of charge, to any person obtaining a
//	copy of this software and associated documentation files (the
//	"Software"), to deal in the Software without restriction, including
//	without limitation the rights to use, copy, modify, merge, publish,
//	distribute, sublicense, and/or sell copies of the Software, and to
//	permit persons to whom the Software is furnished to do so, subject to
//	the following conditions:
//
//	The above copyright notice and this permission notice shall be included
//	in all copies or substantial portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//	OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
//	MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
//	IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
//	CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
//	TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//	SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef music_h
#define music_h

#pragma once

//for use with:
// - doSound()
// - initMusicCHK()

//Play new tune = 1, stop tune = 0, continue = 0x80
#define musicFlag (*((volatile unsigned int *) 0xc856))

//
// These are the tunes that are already built into the vectrex and the data they need.
//

//Music pre-existing in the Vectrex ROM
#define music_1  (const int *) 0xfd0d  //Power on, Crazy Coaster & Narrow Escape
#define music_2  (const int *) 0xfd1d  //Berzerk
#define music_3  (const int *) 0xfd81  //Armour attack
#define music_4  (const int *) 0xfdd3  //Scramble
#define music_5  (const int *) 0xfe38  //Solar quest
#define music_6  (const int *) 0xfe76
#define music_7  (const int *) 0xfec6
#define music_8  (const int *) 0xfef8  //Melody master
#define music_9  (const int *) 0xff26
#define music_10 (const int *) 0xff44
#define music_11 (const int *) 0xff62
#define music_12 (const int *) 0xff7A
#define music_13 (const int *) 0xff8f

//Set adsr and twang for in-built music
#define adsr (*((volatile unsigned int *) 0xc84f))
#define twang (*((volatile unsigned int *) 0xc851))

//ADSR
#define adsr_2_12       0xfd, 0x69   //ADSR for music 2 & 12
#define adsr_3_13       0xfd, 0xc3   //ADSR for music 3 & 13
#define adsr_4_9_10_11  0xfe, 0x28   //ADSR for music 4, 9, 10 &11
#define adsr_5          0xfe, 0x66   //ADSR for music 5
#define adsr_6          0xfe, 0xb2   //ADSR for music 6
#define adsr_7          0xfe, 0xe8   //ADSR for music 7
#define adsr_8          0xff, 0x16   //ADSR for music 8

//Twang
#define twang_2_4       0xfd, 0x79   //Twang for music 2 & 4
#define twang_flat      0xfe, 0xb6   //Flat twang

#endif /* music_h */
