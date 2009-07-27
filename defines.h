/*=============================================================================
 Copyright (c) 2009, Mihail Szabolcs
 All rights reserved.

 Redistribution and use in source and binary forms, with or
 without modification, are permitted provided that the following
 conditions are met:

   * 	Redistributions of source code must retain the above copyright
		notice, this list of conditions and the following disclaimer.

   * 	Redistributions in binary form must reproduce the above copyright
		notice, this list of conditions and the following disclaimer in
		the documentation and/or other materials provided with the
		distribution.

   * 	Neither the name of the Prototype2D nor the names of its contributors
		may be used to endorse or promote products derived from this
		software without specific prior written permission.

	THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
	LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
	OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
	SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
	INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
	CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
	ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
	THE POSSIBILITY OF SUCH DAMAGE.

	This file is part of Prototype2D.

==============================================================================*/
#ifndef DEFINES_H
#define DEFINES_H

#define WORLD_DEBUG_DRAW
#define WORLD_MULTISAMPLING
#define WORLD_VERTEX_ARRAYS

#define WORLD_SCALE_VALUE 10

#define W2S(x,y)    ( x * WORLD_SCALE_VALUE ),( y * WORLD_SCALE_VALUE )
#define W2S_(x)     ( x * WORLD_SCALE_VALUE )

#define S2W(x,y)    ( x / WORLD_SCALE_VALUE ),( y / WORLD_SCALE_VALUE )
#define S2W_(x)     ( x / WORLD_SCALE_VALUE )

#define PI 3.14159265358979323846
#define D2R( a ) ( ( (a) * PI ) / 180.0f )
#define R2D( a ) ( ( (a) * 180.0f ) / PI )

#define BIT( a ) ( 1 << (a) )

#include <QtCore/QDebug> //	qDebug()

#define SHOW_STARTUP_DIALOG

#endif // DEFINES_H
