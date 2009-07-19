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
#include "world.h"
#include "actor.h"
#include "env.h"

#include <QtAlgorithms>
#include <QtCore/QDebug>

using namespace GL;
using namespace Sys;

static Env *gEnv = &Env::getInstance();

// Sort by Z Order functor to be used with qSort
struct sortByZOrder
{
	bool operator()(const Actor *a, const Actor *b)
	{
		return (a->getZOrder() < b->getZOrder());
	}
};

World::World() : mNumContacts(0),
				 mZOrder(0.0f),
				 mWorld(0),
				 mMouseJoint(0),
				 mActor(0),
				 mGround(0)
{
	// set default physics simulation params
	setPhysicsParams(gEnv->mTimeStep,gEnv->mIterations);

	// set default physics bounds
	mWorldAABB.lowerBound.Set(S2W(-100.0f, -100.0f));
	mWorldAABB.upperBound.Set(S2W(gEnv->mSWidth+100,gEnv->mSHeight+100));

	// set default gravity
	b2Vec2 lGravity(gEnv->mGravity[0],gEnv->mGravity[1]);

	// create WORLD
	mDoSleep = true;
	mWorld = new b2World(mWorldAABB,lGravity,mDoSleep);

	// Get the Unique Ground Body
	b2Body *mGroundBody = mWorld->GetGroundBody();

	// make a wrapper actor for the ground body
	mGround = new Actor("Ground",this);
	mGround->setBody(mGroundBody);

	// Make sure to store it as User Data
	mGroundBody->SetUserData(mGround);

	// Alloc initial number of contact points
	_allocContacts(gEnv->mNumContactPoints);

	// Add Itself as Contact Listener
	mWorld->SetContactListener(this);
}

World::~World()
{
	// DeAlloc the number of contact points
	_deallocContacts();

	removeAll();

	if( mGround ) /// do we have a ground? FOR SURE!
	{
		// don't delete the actual
		// ground body ... LOL
		mGround->setBody(0);
		mGround->setHasJoints(false);

		delete mGround;
	}

	if( mWorld )
		delete mWorld;
}

void World::Add(const b2ContactPoint *pPoint)
{
	//! queue only a predefined number of contact points
	if( mNumContacts == gEnv->mNumContactPoints )
		return;

	// Current
	ContactPoint *lContact = mContacts[mNumContacts];

	// Get Bodies
	b2Body *lBody1 = pPoint->shape1->GetBody();
	b2Body *lBody2 = pPoint->shape2->GetBody();

	// Copy Over
	lContact->mActor1 = static_cast<Actor *>(lBody1->GetUserData());
	lContact->mActor2 = static_cast<Actor *>(lBody2->GetUserData());

	// Position (convert to Screen Space)
	lContact->mPos[0] = W2S_(pPoint->position.x);
	lContact->mPos[1] = W2S_(pPoint->position.y);

	// Inc
	mNumContacts++;
}

void World::setGravity( float pX, float pY )
{
	mWorld->SetGravity(b2Vec2(pX,pY));
}

void World::setGravity( float pY )
{
	setGravity(0.0f,pY);
}

void World::setPhysicsParams( float pTimeStep, int pIters )
{
	mTimeStep = pTimeStep;
	mIters = pIters;
}

void World::updatePhysics( void )
{
	//! reset all contact points prior stepping
	_resetContacts();

	//! Step Box2D
	mWorld->Step(mTimeStep,mIters);

	//! Dispatch all queued up contact points
	_dispatchContacts();
}

bool World::removeActor( const QString &pName )
{
	if( pName.isEmpty() || mActors.empty() || pName == "Ground" )
		return false;

	ActorArray::iterator lIt = _findActor(pName);
	if( lIt != mActors.end() )
	{
		Actor *lActor = (*lIt);

		// remove it from the world
		mActors.erase(lIt);

		// delete the actual actor
		delete lActor;
		lActor = 0;

		return true;
	}

	return false;
}

bool World::removeActor(Actor *pActor)
{
	Q_ASSERT( pActor != 0 );
	return removeActor( pActor->getName() );
}

bool World::removeActorBody(Actor *pActor)
{
	Q_ASSERT( pActor != 0 );

	b2Body *lBody = pActor->getBody();

	if( lBody )
	{
		qDebug() << "Success: [World] Removing body for " << pActor->getName();
		mWorld->DestroyBody(lBody);
		return true;
	}

	return false;
}

void World::removeAll( void )
{
	removeAllActors(true);
}

void World::removeAllActors( bool pRemoveJoints )
{
	if( pRemoveJoints )
		removeAllJoints();

	if( mActors.empty() )
		return;

	ActorArray::iterator lIt	= mActors.begin();
	ActorArray::iterator lEnd	= mActors.end();

	if( pRemoveJoints )
	{
		for( ; lIt!= lEnd; ++lIt )
		{
			Actor *lActor = (*lIt);

			lActor->setHasJoints(false);
			lActor->setBody(0);

			delete lActor;
		}
	}
	else
	{
		for( ; lIt!= lEnd; ++lIt )
		{
			Actor *lActor = (*lIt);
			lActor->setBody(0);

			delete lActor;
		}
	}

	mActors.clear();
}

void World::removeAllJoints( void )
{
	// Clean-Up Joint Data
	if( mWorld->GetJointCount() )
	{
		b2Joint *lJoint = mWorld->GetJointList();
		while( lJoint )
		{
			// remove just userData (autorelease==false)
			_removeJointData( lJoint );
			lJoint = lJoint->GetNext();
		}
	}
}

int World::getCount( void ) const
{
	return mActors.size();
}

void World::sortByZorder( void )
{
	if( mActors.empty() )
		return;

	qDebug() << "Success: [World] Sorting " << mActors.size() << " actors by Z order";
	qSort(mActors.begin(),mActors.end(),sortByZOrder());
}

void World::render( void )
{
	if( mActors.empty() )
		return;

	ActorArray::iterator lIt	= mActors.begin();
	ActorArray::iterator lEnd	= mActors.end();

	for( ; lIt!= lEnd; ++lIt )
		(*lIt)->render();
}

void World::update( void )
{
	if( mActors.empty() )
		return;

	// update physics world
	updatePhysics();

	ActorArray::iterator lIt	= mActors.begin();
	ActorArray::iterator lEnd	= mActors.end();

	for( ; lIt!= lEnd; ++lIt )
	{
		//updatePhysics();

		Actor *lActor = (*lIt);

		//! Remove frozen actors automatically :)
		if( lActor->isFrozen() )
		{
			removeActor(lActor);
		}
		else // update as necessary
		{
			lActor->update();
		}
	}
}

bool World::grabActor(int pX, int pY)
{
	if( mMouseJoint )
		return false;

	b2AABB lAABB;

	b2Vec2 lPos(S2W(pX,pY));
	b2Vec2 lDis(0.001f,0.001f);

	lAABB.lowerBound = lPos - lDis;
	lAABB.upperBound = lPos + lDis;

	// Query the world for overlapping shapes.
	const int lMaxCount = 10;
	b2Shape *lShapes[lMaxCount];
	b2Body *lBody = 0;

	int lCount = mWorld->Query(lAABB, lShapes, lMaxCount);
	for (int i = 0; i < lCount; ++i)
	{
		b2Body* lShapeBody = lShapes[i]->GetBody();
		if (lShapeBody->IsStatic() == false && lShapeBody->GetMass() > 0.0f)
		{
			bool lIsInside = lShapes[i]->TestPoint(lShapeBody->GetXForm(), lPos);
			if (lIsInside)
			{
				lBody = lShapes[i]->GetBody();
				break;
			}
		}
	}

	if (lBody)
	{
		b2MouseJointDef lMouseJDef;
		lMouseJDef.body1 = mWorld->GetGroundBody();
		lMouseJDef.body2 = lBody;
		lMouseJDef.target = lPos;
		lMouseJDef.maxForce = 1000.0f * lBody->GetMass();

		mMouseJoint = static_cast<b2MouseJoint*>(mWorld->CreateJoint(&lMouseJDef));
		lBody->WakeUp();

		mActor = static_cast<Actor *>(lBody->GetUserData());

		return true;
	}

	return false;
}

void World::moveActor(int pX, int pY)
{
	if( mMouseJoint )
	{
		mMouseJoint->SetTarget(b2Vec2(S2W(pX,pY)));
	}
}

void World::dropActor(void)
{
	if( mMouseJoint )
	{
		mWorld->DestroyJoint(mMouseJoint);
		mMouseJoint = 0;
		mActor = 0;
	}
}

ActorJoint *World::createJoint(const QString &pName, Actor *pA1,
							   Actor *pA2, const float pX, const float pY, bool pUnique)
{
	Q_ASSERT( !pName.isEmpty() );

	QString lName = pName;

	if( pUnique )
		lName = _getUnique(pName);

	return (new ActorJoint(lName,this))->init(pA1,pA2,pX,pY);
}

ActorJoint *World::createJoint( const QString &pName, Actor *pA1,
								Actor *pA2, bool pUnique )
{
	Q_ASSERT( !pName.isEmpty() );

	QString lName = pName;

	if( pUnique )
		lName = _getUnique(pName);

	return (new ActorJoint(lName,this))->init(pA1,pA2);
}

bool World::removeJoint( b2Joint *pJoint )
{
	Q_ASSERT( pJoint !=  0);

	ActorJoint *lJoint = static_cast<ActorJoint *>(pJoint->GetUserData());
	if( lJoint )
		delete lJoint;

	return true;
}

bool World::removeJoint( const QString &pName )
{
	if( mWorld->GetJointCount() )
	{
		unsigned int lHash = qHash(pName);

		b2Joint *lJoint = mWorld->GetJointList();
		while( lJoint )
		{
			ActorJoint *lActor = static_cast<ActorJoint *>(lJoint->GetUserData());

			if( !lActor )
				continue;

			if( lHash == lActor->getId() )
				return removeJoint(lJoint);

			lJoint = lJoint->GetNext();
		}
	}

	return false;
}

bool World::removeJoint( Actor *pActor )
{
	Q_ASSERT( pActor != 0 );

	b2Body *lBody = pActor->getBody();

	if( !pActor->hasJoints() || !lBody )
	{
		qDebug() << "Success: [World] No associated joints with " << pActor->getName();
		return true;
	}

	if( mWorld->GetJointCount() )
	{
		b2Joint *lJoint = mWorld->GetJointList();
		while( lJoint )
		{
			b2Joint *lPrevJoint = lJoint;
			lJoint = lJoint->GetNext();

			b2Body *lBody1 = lPrevJoint->GetBody1();
			b2Body *lBody2 = lPrevJoint->GetBody2();

			ActorJoint *lActorJoint = static_cast<ActorJoint *>(lPrevJoint->GetUserData());

			if( lBody1 == lBody || lBody2 == lBody )
			{
				qDebug() << "Success: [World] Removing joint " << lActorJoint->getName() << " associated with " << pActor->getName();
				removeJoint(lPrevJoint);
			}
		}

		return true;
	}

	return false;
}

bool World::removeJoint( const ActorJoint *pJoint )
{
	Q_ASSERT( pJoint != 0 );

	return removeJoint(pJoint->getJoint());
}

void World::_allocContacts(unsigned short pNumContacts)
{
	Q_ASSERT( pNumContacts != 0 );

	if( !mContacts.isEmpty() )
		return;

	for(unsigned short i=0;i<pNumContacts;i++)
	{
		ContactPoint *lPoint = new ContactPoint;
		lPoint->mActor1 = 0;
		lPoint->mActor2 = 0;

		mContacts.push_back(lPoint);
	}

	qDebug() << "Allocated Contact Points Queue with a size of " << pNumContacts;
}

void World::_resetContacts( void )
{
	if( mContacts.isEmpty() )
		return;

	ContactArray::iterator lIt	= mContacts.begin();
	ContactArray::iterator lEnd	= mContacts.end();

	for( ; lIt!= lEnd; ++lIt )
	{
		ContactPoint *lPoint = (*lIt);
		lPoint->mActor1 = 0;
		lPoint->mActor2 = 0;
	}

	mNumContacts = 0;
}

void World::_dispatchContacts( void )
{
	if( mContacts.isEmpty() )
		return;

	//! Emit signals
	for(unsigned short i=0;i<mNumContacts;i++)
		emit onCollision(mContacts[i]);
}

void World::_deallocContacts( void )
{
	if( mContacts.isEmpty() )
		return;

	ContactArray::iterator lIt	= mContacts.begin();
	ContactArray::iterator lEnd	= mContacts.end();

	for( ; lIt!= lEnd; ++lIt )
		delete (*lIt);

	mContacts.clear();

	qDebug() << "DeAllocated Contact Points Queue ...";
}

QString World::_getUnique( const QString &pName )
{
	if( pName.isEmpty() )
		return pName;

	return pName+"_"+QString::number(clock()+getCount()+rand()%16000);
}

bool World::_removeJointData( b2Joint *pJoint )
{
	Q_ASSERT( pJoint != 0 );

	if( !pJoint->GetUserData() )
		return false;

	ActorJoint *lJoint = static_cast<ActorJoint *>(pJoint->GetUserData());

	if(lJoint)
	{
		//
		qDebug() << "Success: [World] Pre-Removing joint " << lJoint->getName();
		delete lJoint;

		// clean-user data
		pJoint->SetUserData(0);

		return true;
	}

	return false;
}

World::ActorArray::iterator World::_findActor(const QString &pName)
{
	if( pName.isEmpty() || mActors.empty() )
		return mActors.end();

	unsigned int lHash = qHash(pName);

	ActorArray::iterator lIt	= mActors.begin();
	ActorArray::iterator lEnd	= mActors.end();

	for( ; lIt!= lEnd; ++lIt )
		if( (*lIt)->getId() == lHash )
			return lIt;

	return lEnd;
}
