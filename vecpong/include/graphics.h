//
//  graphics.h
//
//  Created by Phillip Riscombe-Burton on 05/04/2015.
//  Based upon original work by Martin White circa 2014
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

#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#pragma once

//
// Used to store 'sprites' as a list of vectors
// Please use https://github.com/Phillrb/VectorGen to generate these lists :)
//

// Pattern list
// 1st Byte:	0 = Blank Line
// 				255 = Solid Line
// 2nd Byte:	Y
// 3rd Byte:	X
//
// Terminate with a 1

const int ballSprite[] =
{
    0, -15, -23,
    255, 20, 0,
    255, 0, 20,
    255, -20, 0,
    255, 0, -20,
    1
};

//Credits
const int nesCartSprite[] =
{
    0, -50, -20,
    255, 0, 20,
    255, -10, -10,
    255, 10, -10,
    0, -20, -40,
    255, 20, 0,
    255, 0, -10,
    255, 120, 0,
    255, 0, 10,
    255, -10, 0,
    255, 0, 30,
    255, 10, 0,
    255, 0, -30,
    0, 0, 30,
    255, 0, 80,
    255, -120, 0,
    255, 0, -10,
    255, -20, 0,
    255, 0, -100,
    0, 120, 40, //break up move as Y is larger then 1 signed byte can hold
    0, 20, 0,   //2nd part of move
    255, -80, 0,
    255, 0, 60,
    255, 80, 0,
    1
};

//Player Paddles
const int drawPlayer1Sprite[] =
{
    255,-10,-30,
    255,0,60,
    255,10,-30,
    1
};

const int drawPlayer2Sprite[] =
{
    255,10,-30,
    255,0,60,
    255,-10,-30,
    1
};

#endif