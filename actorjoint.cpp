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
#include "actorjoint.h"
#include "world.h"

#include <QtCore/QHash>

#include "Box2D/Box2D.h"

using namespace GL;

ActorJoint::ActorJoint(const QString &pName, World *pWorld) :	mName(pName),
																mId(qHash(pName)),
																mJoint(0),
																mWorld(pWorld)
{
	Q_ASSERT( mWorld != 0 );

	qDebug() << "Success: [ActorJoint] " << mName << " created";
}

ActorJoint::~ActorJoint()
{
	shutdown();

	qDebug() << "Success: [ActorJoint] " << mName << "destroyed";
}

QString &ActorJoint::getName(void)
{
	return mName;
}

unsigned int ActorJoint::getId(void) const
{
	return mId;
}

b2RevoluteJoint *ActorJoint::getJoint(void) const
{
	return mJoint;
}

ActorJoint *ActorJoint::init(Actor *pA1,Actor *pA2,const float pX,const float pY)
{
	Q_ASSERT( mWorld != 0 );
	Q_ASSERT( pA1 != 0 );
	Q_ASSERT( pA2 != 0 );

	if( mJoint )
		return this;

	b2RevoluteJointDef lJointDef;
	lJointDef.collideConnected = true;
	lJointDef.userData = this;

	// mark them
	if( pA1 )
		pA1->setHasJoints(true);

	if( pA2 )
		pA2->setHasJoints(true);

	// grab bodies & initialize
	lJointDef.Initialize(pA1->getBody(),pA2->getBody(),b2Vec2(S2W(pX,pY)));

	b2World *lWorld = mWorld->getPhysicsWorld();
	mJoint = static_cast<b2RevoluteJoint *>(lWorld->CreateJoint(&lJointDef));
	return this;
}

ActorJoint *ActorJoint::init(Actor *pA1,Actor *pA2)
{
	Q_ASSERT( mWorld != 0 );
	Q_ASSERT( pA1 != 0 );
	Q_ASSERT( pA2 != 0 );

	if( mJoint )
		return this;

	b2RevoluteJointDef lJointDef;
	lJointDef.collideConnected = true;
	lJointDef.userData = this;

	// grab bodies
	lJointDef.body1 = pA1->getBody();
	lJointDef.body2 = pA2->getBody();

	// mark them
	if( pA1 )
		pA1->setHasJoints(true);

	if( pA2 )
		pA2->setHasJoints(true);

	// initialize
	b2World *lWorld = mWorld->getPhysicsWorld();
	mJoint = static_cast<b2RevoluteJoint *>(lWorld->CreateJoint(&lJointDef));
	return this;
}

void ActorJoint::shutdown(void)
{
	if( !mJoint )
		return;

	b2Body *lBody1 = mJoint->GetBody1();
	b2Body *lBody2 = mJoint->GetBody2();

	if( lBody1->GetUserData() )
	{
		Actor *lActor = static_cast<Actor *>(lBody1->GetUserData());
		lActor->setHasJoints(false);
	}

	if( lBody2->GetUserData() )
	{
		Actor *lActor = static_cast<Actor *>(lBody2->GetUserData());
		lActor->setHasJoints(false);
	}

	mJoint->SetUserData(0);

	b2World *lWorld = mWorld->getPhysicsWorld();
	lWorld->DestroyJoint(mJoint);
}
