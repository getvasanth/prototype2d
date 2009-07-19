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
#include "force.h"
#include "actor.h"
#include "world.h"
#include "texture.h"
#include "texturemanager.h"
#include "env.h"

#include <QtCore/QDebug>

using namespace Force;
using namespace GL;
using namespace Sys;

static Env *gEnv = &Env::getInstance();
static TextureManager *gTex = &TextureManager::getInstance();

Game::Game() : mGravity(false), mWorld(0), mPea(0), mPointer(0), mBoom(0)
{
	qDebug() << "Force::Game created ...";
}

Game::~Game()
{
	qDebug() << "Force::Game destroyed ...";
}

bool Game::configure(void)
{
	// NO OPENGL SPECIFIC INITS HERE
	// BECAUSE ONLY THE ENVIRONMENT IS AVAILABLE
	// WHEN THIS IS CALLED !!!

	// Configure All Game Specific Settings Here
	gEnv->mWTitle = "-== Prototype2D - Apply Force ==-";
	//gEnv->mGravity = 0.0f;

	//reset to white
	gEnv->mDebugColor[0] = 1.0f;
	gEnv->mDebugColor[1] = 1.0f;
	gEnv->mDebugColor[2] = 1.0f;

	// set bg color
	gEnv->mBgColor[0] = 0.4f;
	gEnv->mBgColor[1] = 0.2f;
	gEnv->mBgColor[2] = 0.6f;

	return true;
}

bool Game::init(void)
{
	// ALL OPENGL SPECIFIC INITS HERE
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// 'CREATE' THE WORLD
	mWorld = new World;

	// hook up ...
	connect(mWorld,SIGNAL(onCollision(const ContactPoint*)),
			this,SLOT(handleCollision(const ContactPoint*)));

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

	// VALERIA
	{
		Actor *lActor = mWorld->createActor<Actor>("V");
		lActor->setTexture("textures/valeria/v.png");
		lActor->setRect(b2Random(100,300),b2Random(100,300),64,64);
		lActor->setDensity(1.0f);
		lActor->setRestituition(0.3f);
		lActor->applyPhysX();
		lActor->setZOrder(5.0f);

		lActor = mWorld->createActor<Actor>("Va");
		lActor->setTexture("textures/valeria/a.png");
		lActor->setRect(b2Random(100,300),b2Random(100,300),64,64);
		lActor->setDensity(1.0f);
		lActor->setRestituition(0.3f);
		lActor->applyPhysX();
		lActor->setZOrder(5.0f);

		lActor = mWorld->createActor<Actor>("Val");
		lActor->setTexture("textures/valeria/l.png");
		lActor->setRect(b2Random(100,300),b2Random(100,300),64,64);
		lActor->setDensity(1.0f);
		lActor->setRestituition(0.3f);
		lActor->applyPhysX();
		lActor->setZOrder(5.0f);

		lActor = mWorld->createActor<Actor>("Vale");
		lActor->setTexture("textures/valeria/e.png");
		lActor->setRect(b2Random(100,300),b2Random(100,300),64,64);
		lActor->setDensity(1.0f);
		lActor->setRestituition(0.3f);
		lActor->applyPhysX();
		lActor->setZOrder(5.0f);

		lActor = mWorld->createActor<Actor>("Valer");
		lActor->setTexture("textures/valeria/r.png");
		lActor->setRect(b2Random(100,300),b2Random(100,300),64,64);
		lActor->setDensity(1.0f);
		lActor->setRestituition(0.3f);
		lActor->applyPhysX();
		lActor->setZOrder(5.0f);

		lActor = mWorld->createActor<Actor>("Valeri");
		lActor->setTexture("textures/valeria/i.png");
		lActor->setRect(b2Random(100,300),b2Random(100,300),64,64);
		lActor->setDensity(1.0f);
		lActor->setRestituition(0.3f);
		lActor->applyPhysX();
		lActor->setZOrder(5.0f);

		lActor = mWorld->createActor<Actor>("Valeria");
		lActor->setTexture("textures/valeria/a.png");
		lActor->setRect(b2Random(100,300),b2Random(100,300),64,64);
		lActor->setDensity(1.0f);
		lActor->setRestituition(0.3f);
		lActor->applyPhysX();
		lActor->setZOrder(5.0f);
	}

	{
		Actor *lActor = mWorld->createActor<Actor>("Pea");
		lActor->setFlags(Actor::S_CIRCLE);
		lActor->setTexture("textures/valeria/valeria.png");
		lActor->setRect(250,250,37,37);
		lActor->setBlending(Actor::B_SRC_ALPHA);
		lActor->setDensity(1.5f);
		lActor->setFriction(0.3f);
		lActor->setRestituition(0.8f);
		lActor->applyPhysX();
		lActor->setZOrder(5.0f);

		mPea = lActor;
	}

	{
		Actor *lActor = mWorld->createActor<Actor>("Boom");
		lActor->setTexture("textures/valeria/boom.png");
		lActor->setRect(-100,-100,71,42);
		lActor->setBlending(Actor::B_SRC_ALPHA);
		lActor->setZOrder(6.0f);
		lActor->setTransparency(80.0f);

		mBoom = lActor;
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

void Game::render(void)
{
	// RENDER WORLD
	mWorld->render();

	// RENDER POINTER
	mPointer->render();
}

void Game::think(void)
{
	// UPDATE PHYSICS FIRST
	mWorld->updatePhysics();

	// UPDATE WORLD
	mWorld->update();

	// hide after some time ...
	static int mSomeDelay = 0;
	if( ++mSomeDelay == 150 )
	{
		mBoom->setPos(-100,-100);
		mSomeDelay = 0;
	}
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
		mBoom->setPos(-100,-100);
	}

	if( pState == IGame::M_MOVE )
		mWorld->moveActor(pX,pY);

	if( pState == IGame::M_UP )
	{
		mWorld->dropActor();
		mBoom->setPos(-100,-100);
	}

	return true;
}

bool Game::updateKeys(int pKey, t_KeyState pState)
{
	if( pState == K_DOWN )
	{
		switch( pKey )
		{
			case Qt::Key_W:
			{
				mPea->applyForce(0.0f,-200.0f);
			}
			break;
			case Qt::Key_S:
			{
				mPea->applyForce(0.0f,200.0f);
			}
			break;
			case Qt::Key_A:
			{
				//mPea->applyTorque(-200.0f);
				mPea->applyForce(-200.0f,0.0f);
			}
			break;
			case Qt::Key_D:
			{
				//mPea->applyTorque(200.0f);
				mPea->applyForce(200.0f,0.0f);
			}
			break;
			case Qt::Key_F:
			{
				// toggle debug draw
				gEnv->mDebugDraw = !gEnv->mDebugDraw;
			}
			break;
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
			// drop another block
			case Qt::Key_B:
			{
				const char lBlocks[6] =
				{
					'v','a','l','e','r','i'
				};

				const int lI = static_cast<int>(b2Random(0,6));

				Actor *lActor = mWorld->createActor<Actor>("Valeria",true);
				lActor->setTexture("textures/valeria/"+QString(lBlocks[lI])+".png");
				lActor->setRect(b2Random(60,500),b2Random(60,500),64,64);
				lActor->setDensity(2.0f);
				lActor->setRestituition(0.5f);
				lActor->applyPhysX();

				mWorld->sortByZorder();
			}
			break;
			// drop another (red) pea
			case Qt::Key_P:
			{
				Actor *lActor = mWorld->createActor<Actor>("Pea",true);
				lActor->setFlags(Actor::S_CIRCLE);
				lActor->setTexture("textures/valeria/valeria.png");
				lActor->setRect(b2Random(60,400),b2Random(60,400),37,37);
				lActor->setBlending(Actor::B_SRC_ALPHA);
				lActor->setDensity(1.5f);
				lActor->setFriction(0.3f);
				lActor->setRestituition(0.8f);
				lActor->applyPhysX();
			}
			break;
		}
	}

	return false;
}

void Game::handleCollision(const ContactPoint *pPoint)
{
	QString lName1 = pPoint->mActor1->getName();
	QString lName2 = pPoint->mActor2->getName();
	//const t_point *lP = &pPoint->mPos;

	// SHOW BIM-BA-RA-BOOOOOM!
	if( pPoint->mActor2->isFlag(Actor::S_CIRCLE) )
	{
		mBoom->setPos(pPoint->mActor2->getPosX()-45,
					  pPoint->mActor2->getPosY()-45);
	}

	if( gEnv->mDebugDraw )
		qDebug() << "Collision" << lName1 << " with " << lName2 ;//<< " at position "<<*lP[0]<<","<<*lP[1];
}
