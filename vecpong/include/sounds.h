//
//  sounds.h
//
//  Created by Phillip Riscombe-Burton on 04/12/2015.
//  Based upon chrissalo work on http://www.playvectrex.com/
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

#ifndef sounds_h
#define sounds_h

#pragma once

//http://vectrexmuseum.com/share/coder/other/TEXT/SOUND/SOUND.TXT
//For use with:
// - soundByte()
// - clearSound()

typedef enum {
    SCVoice1Data = 0,
    SCVoice2Data = 2,
    SCVoice3Data = 4,
    SCVoiceGenerators = 7,
    SCVoice1Volume = 8,
    SCVoice2Volume = 9,
    SCVoice3Volume = 10
} SoundControl;

#define SCAllVoicesOff 0xff //1111111
#define SCVoice1Only   0xfe //1111110
#define SCVoice2Only   0xfd //1111101
#define SCVoice3Only   0xfb //1111011

//Register     Function
//0      -  Lower 8 Bits of Frequency for Tone generator 1
//1      -  Upper 4 Bits of Frequency for Tone generator 1
//2      -  Lower 8 Bits of Frequency for Tone generator 2
//3      -  Upper 4 Bits of Frequency for Tone generator 2
//4      -  Lower 8 Bits of Frequency for Tone generator 3
//5      -  Upper 4 Bits of Frequency for Tone generator 3
//6      -  5 Bit frequency for Noise generator
//7      -  Tone/Noise On/Off switches for Voices 1,2 and 3
//8      -  Volume for Voice 1 (0-15)
//9      -  Volume for Voice 2 (0-15)
//10     -  Volume for Voice 3 (0-15)
//11+ ADSR and wave form envelopes

//More detail:
//0, 2, 4     - Tone Frequency is a 12 byte value from #$000 to #$fff
//3           - Noise Frequency is a 5 bit value from #$00-#$1f
//8, 9, 10    - Volume must be set! 0 to 15
//7           - Select Sound generators - 0 is ON! #$fe turns on voice 1 only
//                  Bit        Effect
//                  0    -   Voice 1 use Tone Generator 1 On/Off
//                  1    -   Voice 2 use Tone Generator 2 On/Off
//                  2    -   Voice 3 use Tone Generator 3 On/Off
//                  3    -   Voice 1 use Noise Generator On/Off
//                  4    -   Voice 2 use Noise Generator On/Off
//                  5    -   Voice 3 use Noise Generator On/Off
//                  (Bits 6 and 7 are unused)


#endif /* sounds_h */
