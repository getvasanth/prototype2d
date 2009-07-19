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
#include "background.h"
#include "world.h"
#include "env.h"

#include <QtCore/QDebug>

using namespace GL;
using namespace Pyp;
using namespace Sys;

static Env *gEnv = &Env::getInstance();

Qloud::Qloud(const QString &pName,World *pWorld) : Actor(pName,pWorld), mSpeed(0.0f)
{
}

void Qloud::spawn(int pW, int pH, bool pHidden)
{
	// set random ... overlapping qlouds are OK :D
	float lX = rand()%(pW);
	float lY = rand()%(pH);

	// move out of the screen if hidden
	if( pHidden )
	{
		lX = -getWidth();
	}

	// set the random position
	_setPos(lX,lY);

	// randomize speed
	mSpeed = (rand() % 3) + 0.5f;
}

void Qloud::update(void)
{
}

void Qloud::update(int pW, int pH)
{
	//if( !mAnimate )
	//   return;

	// out of view ...
	if( getPosX() - getWidth() > pW )
		spawn(pW,pH,true);

	// animate on X ( from left to right )
	moveXY(mSpeed,0.0f);
}

Background::Background(const QString &pName,World *pWorld,int pW,int pH) : Actor(pName,pWorld)
{
	// set texture to self
	setTexture("textures/pyp/Background2.png");
	// set size
	setRect(0,0,pW,pH);

	// set seed
	srand(time(0));

	// set qlouds
	for(int i=0;i<NUM_CLOUDS;i++)
	{
		QString lName = "Cloud"+QString::number(i+1);

		mQlouds[i] = new Qloud(lName,pWorld);
		mQlouds[i]->setTexture("textures/pyp/"+lName+".png");
		mQlouds[i]->setSizeFromTexture();
		mQlouds[i]->setBlending(Actor::B_SRC_ALPHA);
		mQlouds[i]->spawn(pW,pH,false);
	}
}

Background::~Background()
{
	// delete qlouds
	for(int i=0;i<NUM_CLOUDS;i++)
		delete mQlouds[i];
}

void Background::render()
{
	// render the background
	Actor::render();

	// render the qlouds
	for(int i=0;i<NUM_CLOUDS;i++)
		mQlouds[i]->render();
}

void Background::update()
{
	if( !mAnimate )
		return;

	// update qlouds ... move them
	for(int i=0;i<NUM_CLOUDS;i++)
		mQlouds[i]->update((int)getWidth(),(int)getHeight()-120);
}
