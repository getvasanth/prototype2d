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
#include "pyp.h"
#include "actor.h"
#include "world.h"
#include "texture.h"
#include "texturemanager.h"
#include "env.h"

#include "background.h"
#include "block.h"
#include "pea.h"
#include "tri.h"
#include "toolpallette.h"
#include "canvas.h"

#include <QtCore/QDebug>

using namespace Pyp;
using namespace GL;
using namespace Sys;

static Env *gEnv = &Env::getInstance();
static TextureManager *gTex = &TextureManager::getInstance();

Game::Game() : mWorld(0),mPointer(0)
{
	qDebug() << "Pyp::Game created ...";
}

Game::~Game()
{
	qDebug() << "Pyp::Game destroyed ...";
}

bool Game::configure(void)
{
	// NO OPENGL SPECIFIC INITS HERE
	// BECAUSE ONLY THE ENVIRONMENT IS AVAILABLE
	// WHEN THIS IS CALLED !!!

	// Configure All Game Specific Settings Here
	gEnv->mWTitle = "Play With Your Pies";
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

	Actor *lGround = mWorld->getGroundActor();

	{
		// BUILD BLOCK BRIDGE -- JOINT TEST
		Actor *lPrevActor = lGround;
		const int lNumPlanks = 8;

		for(int i=0;i<lNumPlanks;i++)
		{
			Block *lActor = mWorld->createActor<Block>("Block",true);
			lActor->setTexture("textures/pyp/Block-Normal2.png");
			lActor->setRect(100+71*i,150,71,61);
			lActor->setBlending(Actor::B_SRC_ALPHA);
			lActor->setDensity(20.0f);
			lActor->setRestituition(0.0f);
			lActor->applyPhysX();

			mWorld->createJoint("Bridge",lPrevActor,lActor,100+71*i,150.0f,true);
			lPrevActor = lActor;
		}

		mWorld->createJoint("Bridge",lPrevActor,lGround,100+71*lNumPlanks,150.0f,true);
	}

	{
		// BUILD PEA BRIDGE -- JOINT TEST
		Actor *lPrevActor = lGround;
		const int lNumPlanks = 12;

		for(int i=0;i<lNumPlanks;i++)
		{
			Pea *lActor = mWorld->createActor<Pea>("Pea",true);
			lActor->setTexture("textures/pyp/Pea-Happy.png");
			lActor->setRect(100+37*i,350.0f,37,37);
			lActor->setBlending(Actor::B_SRC_ALPHA);
			lActor->setDensity(0.5f);
			lActor->setFriction(1.0f);
			//lActor->setRestituition(0.8f);
			lActor->applyPhysX();
			lActor->setZOrder(100.0f);

			mWorld->createJoint("BridgePea",lPrevActor,lActor,100+37*i,350.0f,true);
			lPrevActor = lActor;
		}

		mWorld->createJoint("BridgePea",lPrevActor,lGround,100+37*lNumPlanks,350.0f,true);
	}

	{
		Pea *lActor = mWorld->createActor<Pea>("Pea");
		lActor->setTexture("textures/pyp/Pea-Happy.png");
		lActor->setRect(230,20,37,37);
		lActor->setBlending(Actor::B_SRC_ALPHA);
		lActor->setDensity(0.5f);
		lActor->setFriction(1.0f);
		lActor->setRestituition(0.8f);
		lActor->applyPhysX();
		lActor->setZOrder(100.0f);
	}

	// SORT ONCE, JUST TO BE SURE
	mWorld->sortByZorder();

	// POINTER
	mPointer = new Actor("Pointer");
	mPointer->setRect(0.0f,0.0f,32,32);
	mPointer->setTexture("textures/pyp/Pointer-Standard2.png");
	mPointer->setBlending(Actor::B_SRC_ALPHA); // turn on blending

	// BACKGROUND
	mBackg = new Background("Background",0,gEnv->mSWidth,gEnv->mSHeight);
	// start animation ...
	mBackg->setAnimate(true,true);

	// TOOL PALLETTE
	mToolPallette = new ToolPallette("ToolPallette");
	mToolPallette->setRect(740,100,60,311);
	mToolPallette->setBlending(Actor::B_SRC_ALPHA);
	mToolPallette->init();

	return true;
}

bool Game::shutdown(void)
{
	if( mPointer )
		delete mPointer;

	if( mBackg )
		delete mBackg;

	if( mToolPallette )
		delete mToolPallette;

	if( mWorld )
		delete mWorld;

	// flush all textures
	gTex->removeAll();

	return true;
}

void Game::render(Canvas *pCanvas)
{
	Q_UNUSED(pCanvas);

	// RENDER BACKGROUND BEFORE ANYTHING ELSE
	mBackg->render();

	// RENDER WORLD
	mWorld->render();

	// RENDER TOOL PALLETTE
	mToolPallette->render();

	// ALWAYS ON TOP
	mPointer->render();
}

void Game::think(Canvas *pCanvas)
{
	Q_UNUSED(pCanvas);

	// UPDATE PHYSICS FIRST
	mWorld->updatePhysics();

	// UPDATE BACKGROUND
	mBackg->update();

	// UPDATE WORLD
	mWorld->update();
}

bool Game::updateMouse(int pX, int pY, int pButton, t_MouseState pState)
{
	// clamp
	if( pX > gEnv->mSWidth )
		pX = gEnv->mSWidth;

	if( pX < 0 )
		pX = 0;

	if( pY > gEnv->mSHeight )
		pY = gEnv->mSHeight;

	if( pY < 0 )
		pY = 0;

	mPointer->setPos(pX,pY);

	// if a tool was selected / de-selected STOP!
	if( mToolPallette->update(pX,pY,pButton) )
		return true;

	Tool *lTool = mToolPallette->getSelected();
	unsigned long lFlags = ((lTool)?lTool->getFlags():0);

	if( pState == IGame::M_DOWN )
	{
		if( mWorld->grabActor(pX,pY) && pButton == Qt::LeftButton )
		{
			Actor *lActor = mWorld->getActor<Actor>();
			if( lActor && lFlags & Tool::DELETE )
			{
				// drop it, then remove it
				mWorld->dropActor();
				mWorld->removeActor(lActor);
				lActor = 0;
			}
		}
	}

	if( pState == IGame::M_MOVE )
		mWorld->moveActor(pX,pY);

	if( pState == IGame::M_UP )
		mWorld->dropActor();

	if( mWorld->isActorGrabbed() )
		return true;

	if( pButton == Qt::LeftButton && pState == IGame::M_DOWN )
	{
		if( !lFlags )
		{
			qDebug() << "no tool selected";
			return false;
		}

		if( lFlags & Tool::STANDARD )
			_spawn<Block>(pX,pY,"textures/pyp/Block-Normal2.png");
		else if( lFlags & Tool::GEL )
			_spawn<Block>(pX,pY,"textures/pyp/Block-Gel2.png");
		else if( lFlags & Tool::LEFT )
			_spawn<Tri>(pX,pY,"textures/pyp/Block-LeftRamp2.png");
		else if( lFlags & Tool::RIGHT )
			_spawn<Tri2>(pX,pY,"textures/pyp/Block-RightRamp2.png");
		else if( lFlags & Tool::SPRING )
			_spawn<Block>(pX,pY,"textures/pyp/Block-Spring2.png",1.0f);
	}

	return true;
}

bool Game::updateKeys(int pKey, t_KeyState pState)
{
	if( pState == K_DOWN )
	{
		switch( pKey )
		{
			case Qt::Key_Space:
			{
				Pea *actor = mWorld->createActor<Pea>("Pea",true);
				actor->setTexture("textures/pyp/Pea-Happy.png");
				actor->setRect(230,20,37,37);
				actor->setBlending(Actor::B_SRC_ALPHA);
				actor->setDensity(20.0f);
				actor->setFriction(1.0f);
				actor->setRestituition(0.8f);
				actor->applyPhysX();
				actor->setZOrder(100.0f);
			}
			break;

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
