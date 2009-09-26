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
#include "autumn.h"
#include "actor.h"
#include "world.h"
#include "texture.h"
#include "texturemanager.h"
#include "env.h"
#include "canvas.h"

#include <QtCore/QDebug>

using namespace Autumn;
using namespace GL;
using namespace Sys;

static Env *gEnv = &Env::getInstance();
static TextureManager *gTex = &TextureManager::getInstance();

Game::Game() : mWorld(0), mGravity(true)
{
	qDebug() << "Autumn::Game created ...";
}

Game::~Game()
{
	qDebug() << "Autumn::Game destroyed ...";
}

bool Game::configure(void)
{
	// NO OPENGL SPECIFIC INITS HERE
	// BECAUSE ONLY THE ENVIRONMENT IS AVAILABLE
	// WHEN THIS IS CALLED !!!

	// Configure All Game Specific Settings Here
	gEnv->mWTitle = "-== Prototype2D - Autumn ==-";
	gEnv->mHideCursor = true;
	// turn on debug drawing by default
	// gEnv->mDebugDraw = true;

	gEnv->mBgColor[0] =  0.8f;
	gEnv->mBgColor[1] =  0.5f;
	gEnv->mBgColor[2] =  0.2f;

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
		mBkgNight = mWorld->createActor<Actor>("BackgroundNight");
		mBkgNight->setTexture("textures/autumn/field_dark.png");
		mBkgNight->setRect(0,0,gEnv->mSWidth,gEnv->mSHeight);
		mBkgNight->setBlending(Actor::B_SRC_ALPHA);
		mBkgNight->setTransparency(255.0f);
	}

	{
		mBkgDay = mWorld->createActor<Actor>("BackgroundDay");
		mBkgDay->setTexture("textures/autumn/field.jpg");
		mBkgDay->setRect(0,0,gEnv->mSWidth,gEnv->mSHeight);
		mBkgDay->setBlending(Actor::B_SRC_ALPHA);
	}

	{
		Actor *lActor = mWorld->createActor<Actor>("Foreground");
		lActor->setTexture("textures/autumn/fg_mask.png");
		lActor->setRect(0,0,gEnv->mSWidth,gEnv->mSHeight);
		lActor->setBlending(Actor::B_SRC_ALPHA);
		lActor->setZOrder(1000);
	}

	{
		mHB = mWorld->createActor<Actor>("HappyBirthday");
		mHB->setTexture("textures/autumn/hb.png");
		mHB->setBlending(Actor::B_SRC_ALPHA);
		mHB->setTransparency(255.0f);
		mHB->center(gEnv->mSWidth,gEnv->mSHeight);
		mHB->setZOrder(900);
	}

	{
		mPython = mWorld->createActor<Actor>("Python");
		mPython->setTexture("textures/autumn/python.png");
		mPython->setBlending(Actor::B_SRC_ALPHA);
		//mPython->setTransparency(255.0f);
		mPython->setPos(gEnv->mSWidth-100,gEnv->mSHeight+10);
		mPython->setZOrder(1001);
	}

	// SORT ONCE, JUST TO BE SURE
	mWorld->sortByZorder();

	// tick-tack!
	mTimer.start();
	mLastTime = mTimer.elapsed();

	// random seed
	srand(time(0));

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
	// RENDER WORLD
	mWorld->render();

	if( mTimer.elapsed() > 20*1000 ) // show after 20 sex
	{
		// RENDER CUSTOM TEXT ON THE TOP OF THE SCENE
		glPushMatrix();
			glPushAttrib(GL_ALL_ATTRIB_BITS);
				glColor3f(0.0f,1.0f,0.0f);
				pCanvas->renderText(160,500,"Sabinutza-Monicutza >:)",QFont("Arial",28,false));
			glPopAttrib();
		glPopMatrix();
	}
}

void Game::think(Canvas *pCanvas)
{
	Q_UNUSED(pCanvas);

	// UPDATE PHYSICS FIRST
	mWorld->updatePhysics();

	// UPDATE WORLD
	mWorld->update();

	int mNow = mTimer.elapsed();
	int mElapsed = mNow - mLastTime;
	if( mElapsed >= 300 )
	{
		{
			float lPosX = ( rand() / (RAND_MAX + 1.0) * (640 + 1 - 130) + 130 );
			float lPosY = 0;

			Actor *lActor = mWorld->createActor<Actor>("Leaf",true);
			//lActor->setRect(0,0,64,64);
			//lActor->setFlags(Actor::S_CIRCLE);
			lActor->setTexture("textures/autumn/leaf.png");
			lActor->setBlending(Actor::B_SRC_ALPHA);
			lActor->setPos(lPosX,lPosY);
			lActor->setRotation(rand()%180);
			lActor->setOffsets(20.0f,5.0f);
			lActor->setDensity(0.1f);
			lActor->applyPhysX();
		}

		mLastTime = mNow;

		mWorld->sortByZorder();
	}

	mBkgDay->setTransparency(mNow*0.01f);
	mBkgNight->setTransparency(255.0f-(mNow*0.01f));
	//mPython->setTransparency(255.0f-(mNow*0.03f));
	mHB->setTransparency(255.0f-(mNow*0.01f));

	int lPythonPos = mPython->getPosY();
	if( lPythonPos > (gEnv->mSHeight - 150))
	{
		lPythonPos -= 0.2;
		mPython->setPos(gEnv->mSWidth-100,lPythonPos);
	}
	else
	{
		mPython->setTransparency(mNow*0.01f);
	}
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
