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
#include "template.h"
#include "actor.h"
#include "world.h"
#include "texture.h"
#include "texturemanager.h"
#include "env.h"
#include "canvas.h"

#include <QtCore/QDebug>

using namespace Template;
using namespace GL;
using namespace Sys;

static Env *gEnv = &Env::getInstance();
static TextureManager *gTex = &TextureManager::getInstance();

Game::Game() : mWorld(0)
{
	qDebug() << "Template::Game created ...";
}

Game::~Game()
{
	qDebug() << "Template::Game destroyed ...";
}

bool Game::configure(void)
{
	// NO OPENGL SPECIFIC INITS HERE
	// BECAUSE ONLY THE ENVIRONMENT IS AVAILABLE
	// WHEN THIS IS CALLED !!!

	// Configure All Game Specific Settings Here
	gEnv->mWTitle = "-== Prototype2D - Template ==-";
	gEnv->mHideCursor = false;
	// turn on debug drawing by default
	// gEnv->mDebugDraw = true;

	gEnv->mBgColor[0] =  0.4f;
	gEnv->mBgColor[1] =  0.2f;
	gEnv->mBgColor[2] =  0.3f;

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
		Actor *lActor = mWorld->createActor<Actor>("Plate");
		lActor->setTexture("textures/pyp/Plate2.png");
		lActor->setRect(0,520,799,80);
		lActor->setBlending(Actor::B_SRC_ALPHA);
		lActor->setOffsets(0.0f,30.0f);
		lActor->applyPhysX(); // solid
	}

	{
		Actor *lActor = mWorld->createActor<Actor>("Pea");
		lActor->setFlags(Actor::S_CIRCLE);
		lActor->setTexture("textures/pyp/Pea-Happy.png");
		lActor->setRect(100,200,37,37);
		lActor->setBlending(Actor::B_SRC_ALPHA);
		lActor->setDensity(0.5f);
		lActor->setFriction(0.3f);
		lActor->setRestituition(0.8f);
		lActor->applyPhysX();
		lActor->setZOrder(100.0f);
	}

	// SORT ONCE, JUST TO BE SURE
	mWorld->sortByZorder();

	return true;
}

bool Game::shutdown(void)
{
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
}

void Game::think(Canvas *pCanvas)
{
	Q_UNUSED(pCanvas);

	// UPDATE PHYSICS FIRST
	mWorld->updatePhysics();

	// UPDATE WORLD
	mWorld->update();
}

bool Game::updateMouse(int pX, int pY, int pButton, t_MouseState pState)
{
	Q_UNUSED(pX);
	Q_UNUSED(pY);

	Q_UNUSED(pButton);
	Q_UNUSED(pState);

	return true;
}

bool Game::updateKeys(int pKey, t_KeyState pState)
{
	if( pState == K_DOWN )
	{
		switch( pKey )
		{
			case Qt::Key_D:
			{
				// toggle debug draw
				gEnv->mDebugDraw = !gEnv->mDebugDraw;
			}
			break;
		}
	}

	return false;
}
