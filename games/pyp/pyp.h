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
#ifndef GAME_H
#define GAME_H

#include "igame.h"
#include "world.h"

namespace Pyp {

class Background;
class ToolPallette;

class Game : public Interface::IGame
{
public:
	Game();
	virtual ~Game();

	virtual bool configure( void );

	virtual bool init(void);
	virtual bool shutdown(void);

	virtual void render(void);
	virtual void think(void);

	virtual bool updateMouse(int pX, int pY, int pButton, t_MouseState pState);
	virtual bool updateKeys(int pKey, t_KeyState pState);

protected:
	template <typename T>
	void _spawn(const int pX, const int pY,
				const QString &pTexture, float pBounce = 0.5f)
	{
		T *lActor = mWorld->createActor<T>("Block",true);

		lActor->setTexture(pTexture);
		lActor->setRect(pX-16,pY-16,71,61);
		lActor->setBlending(GL::Actor::B_SRC_ALPHA);
		lActor->setDensity(1.0f);
		lActor->setRestituition(pBounce);
		lActor->applyPhysX();
		//lActor->setZOrder(1.0f);

		mWorld->sortByZorder();
	}

protected:
	// World
	GL::World *mWorld;
	// Not Managed By World
	GL::Actor *mPointer;
	// Background
	Background *mBackg;
	// Tool Pallette
	ToolPallette *mToolPallette;
};

}

#endif // GAME_H
