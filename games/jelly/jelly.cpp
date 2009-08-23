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
#include "jelly.h"
#include "jellyactor.h"
#include "actor.h"
#include "world.h"
#include "texture.h"
#include "texturemanager.h"
#include "env.h"
#include "canvas.h"

#include <QtCore/QDebug>

using namespace Jelly;
using namespace GL;
using namespace Sys;

static Env *gEnv = &Env::getInstance();
static TextureManager *gTex = &TextureManager::getInstance();

Game::Game() : mWorld(0), mPointer(0), mGravity(true)
{
	qDebug() << "Jelly::Game created ...";
}

Game::~Game()
{
	mJelly.clear();
	qDebug() << "Jelly::Game destroyed ...";
}

bool Game::configure(void)
{
	// NO OPENGL SPECIFIC INITS HERE
	// BECAUSE ONLY THE ENVIRONMENT IS AVAILABLE
	// WHEN THIS IS CALLED !!!

	// Configure All Game Specific Settings Here
	gEnv->mWTitle = "-== Prototype2D - Jelly (Springs) ==-";
	gEnv->mHideCursor = true;
	// turn on debug drawing by default
	// gEnv->mDebugDraw = true;

	gEnv->mBgColor[0] =  0.5f;
	gEnv->mBgColor[1] =  0.3f;
	gEnv->mBgColor[2] =  0.6f;

	return true;
}

bool Game::init(void)
{
	// ALL OPENGL SPECIFIC INITS HERE
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// 'CREATE' THE WORLD
	mWorld = new World;

	// LOAD/CREATE ALL ASSETS/ACTORS HERE
	{
		Actor *lActor = mWorld->createActor<Actor>("Bottom");
		lActor->setTexture("textures/misc/wall.png");
		lActor->setRect(0,570,800,30);
		lActor->applyPhysX();
	}

	{
		Actor *lActor = mWorld->createActor<Actor>("Top");
		lActor->setTexture("textures/misc/wall.png");
		lActor->setRect(0,0,800,30);
		lActor->applyPhysX();
	}

	{
		Actor *lActor = mWorld->createActor<Actor>("Left");
		lActor->setTexture("textures/misc/wall.png");
		lActor->setRect(0,30,30,540);
		lActor->applyPhysX();
	}

	{
		Actor *lActor = mWorld->createActor<Actor>("Right");
		lActor->setTexture("textures/misc/wall.png");
		lActor->setRect(770,30,30,540);
		lActor->applyPhysX();
	}

	for (int i=0; i<5; i++)
	{
		JellyActor *lActor = mWorld->createActor<JellyActor>("Pea");
		lActor->setFlags(Actor::S_CIRCLE);
		lActor->setTexture("textures/pyp/Pea-Happy.png");
		lActor->setRect(100,(100*i)+37,37,37);
		lActor->setBlending(Actor::B_SRC_ALPHA);
		lActor->setDensity(1.0f);
		lActor->setFriction(0.3f);
		//lActor->setRestituition(0.3f);
		lActor->applyPhysX();

		//!
		mJelly.push_back(lActor);
	}

	// SORT ONCE, JUST TO BE SURE
	mWorld->sortByZorder();

	// POINTER
	mPointer = new Actor("Pointer");
	mPointer->setRect(0.0f,0.0f,32,32);
	mPointer->setTexture("textures/pyp/Pointer-Standard2.png");
	mPointer->setBlending(Actor::B_SRC_ALPHA); // turn on blending

	return true;
}

bool Game::shutdown(void)
{
	if( mPointer )
		delete mPointer;

	if( mWorld )
		delete mWorld;

	// flush all textures
	gTex->removeAll();

	return true;
}

void Game::render(Canvas *pCanvas)
{
	Q_UNUSED(pCanvas);

	// RENDER WORLD
	mWorld->render();

	// RENDER POINTER
	mPointer->render();
}

void Game::think(Canvas *pCanvas)
{
	Q_UNUSED(pCanvas);

	// UPDATE PHYSICS FIRST
	mWorld->updatePhysics();

	// UPDATE JELLY SPRINGS
	for(int i=0;i<(5-1);i++)
	{
		mJelly[i]->addSpringForceUp(mJelly[i+1]);
		mJelly[i]->addSpringForceDown(mJelly[i+1]);
	}

	// UPDATE WORLD
	mWorld->update();
}

bool Game::updateMouse(int pX, int pY, int pButton, t_MouseState pState)
{
	Q_UNUSED(pButton);

	// clamp
	if( pX > gEnv->mSWidth )
		pX = gEnv->mSWidth;

	if( pX < 0 )
		pX = 0;

	if( pY > gEnv->mSHeight )
		pY = gEnv->mSHeight;

	if( pY < 0 )
		pY = 0;

	// UPDATE POINTER
	mPointer->setPos(pX,pY);

	// HANDLE OBJECT PICKING ...
	if( pState == IGame::M_DOWN )
	{
		mWorld->grabActor(pX,pY);
	}

	if( pState == IGame::M_MOVE )
		mWorld->moveActor(pX,pY);

	if( pState == IGame::M_UP )
	{
		mWorld->dropActor();
	}

	return true;
}

bool Game::updateKeys(int pKey, t_KeyState pState)
{
	if( pState == K_DOWN )
	{
		switch( pKey )
		{
			// toggle gravity
			case Qt::Key_G:
			{
				mGravity = !mGravity;
				qDebug() << "Gravity "<< ((mGravity)?"on":"off");
				if( mGravity )
					mWorld->setGravity(10.0f);
				else
					mWorld->setGravity(0.0f);
			}
			break;
			case Qt::Key_F:
			{
				// toggle debug draw
				gEnv->mDebugDraw = !gEnv->mDebugDraw;
			}
			break;
		}
	}

	return false;
}
