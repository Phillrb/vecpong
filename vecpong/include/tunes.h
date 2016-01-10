//
//  graphics.h
//
//  Created by Phillip Riscombe-Burton on 05/04/2015.
//  Based upon original work by Martin White circa 2014
//  Copyright (c) 2016 Phillip Riscombe-Burton. All rights reserved.
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

#ifndef _TUNES_H
#define _TUNES_H

#include "notes.h"
#include "music.h"

//
// Used for custom tunes.
// These pieces of music include the adsr and twang headers and then the music data
//

//Music data bytes: ;
//  Bits 0-5 = frequency ;
//  Bit 6 clear = tone ;
//  Bit 6 set = noise ;
//  Bit 7 set = next music data byte is for next channel ;
//  Bit 7 clear, play note with duration in next music data byte: ;
//  bits 0-5 = duration ;
//  bit 6 = unused ;
//  bit 7 set = end of music ;

#define N(x) 64+x   //Use 'noise' instead of 'tone' (Bit 6)
#define _ +128      //Inform that the next byte is for next channel (Bit 7)

//The ADSR table is simply 32 nibbles (16 bytes) of amplitude values.
//The ADSR table sets up a 'fading' of the notes
//E.G. PONG bleeps are abrubt but an organ is long

//The twang table is 8 signed bytes to modify the base frequency of
//each note being played. Each channel has a different limit to its
//twang table index (6-8) to keep them out of phase to each other.
//usually just all 0 bytes
//Vibrato


//Very poor - Walk this way riff!
const unsigned int gameStart[] =
{
    // HEADER:
    adsr_7, //ADSR table address in Vectrex ROM
    twang_flat, //TWANG table address
    
    //DATA:
    /*Ch1*/  /*Ch2*/ /*Ch3*/ /*Length*/
    A4  ,                      12 ,
    AS4 ,                      2  ,
    B4  ,                      6  ,
    E5  ,                      10 ,
    0x40,                      10 , //Wanted a pause but got a cymbal
    A4  ,                      8 ,
    AS4 ,                      2  ,
    B4  ,                      6  ,
    E5  ,                      14 ,
    E4  ,                      18 ,
    0x80        //0x80 is end marker for music
};


#endif /* _TUNES_H */
