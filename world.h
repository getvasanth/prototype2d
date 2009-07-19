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
#ifndef WORLD_H
#define WORLD_H

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QList>
#include <QtCore/QHash>

#include "Box2D/Box2D.h"

#include "actor.h"
#include "actorjoint.h"

namespace GL {

class ActorJoint;

struct ContactPoint
{
	Actor *mActor1;
	Actor *mActor2;

	t_point mPos;
};

class World : public QObject, private b2ContactListener
{
	Q_OBJECT

public:
	World();
	virtual ~World();

	void Add(const b2ContactPoint *pPoint);

	virtual void setGravity( float pX, float pY );
	virtual void setGravity( float pY );

	template <typename T>
	T *createActor( const QString &pName, bool pUnique = false )
	{
		Q_ASSERT( !pName.isEmpty() );

		QString lName = pName;

		if( pUnique )
			lName = _getUnique(pName);

		T *lActor = new T(lName,this);
		lActor->setZOrder(mZOrder);

		// push it
		mActors.push_back(lActor);

		// small increment to allow a large number
		// of objects within a relatively small range
		mZOrder += 0.000001;
		return static_cast<T *>(lActor);
	}

	virtual void setPhysicsParams( float pTimeStep, int pIters );
	virtual void updatePhysics( void );

	template <typename T>
	T *findActor( const QString &pName )
	{
		ActorArray::iterator lActor = _findActor(pName);

		if( lActor != mActors.end() )
			return static_cast<T*>((*lActor));

		return 0;
	}

	virtual bool removeActor( const QString &pName );
	virtual bool removeActor( Actor *pActor );
	virtual bool removeActorBody( Actor *pActor );

	virtual void removeAll( void );

	virtual void removeAllActors( bool pRemoveJoints=false );
	virtual void removeAllJoints( void );

	virtual int getCount( void ) const;
	virtual void sortByZorder( void );

	virtual void render( void );
	virtual void update( void );

	// PhysX drag-drop
	virtual bool grabActor(int pX, int pY);
	virtual void moveActor(int pX, int pY);
	virtual void dropActor(void);

	// PhysX -- get the currently grabbed actor
	template <typename T>
	Actor *getActor(void) const
	{
		return static_cast<T *>(mActor);
	}

	// Return the ground actor (this always exists)
	Actor *getGroundActor(void) const
	{
		return mGround;
	}

	virtual bool isActorGrabbed(void) const { return (mActor); }

	virtual b2World *getPhysicsWorld( void ) const { return mWorld;	}

	ActorJoint *createJoint( const QString &pName, Actor *pA1,
								  Actor *pA2, const float pX, const float pY,
								  bool pUnique = false );

	ActorJoint *createJoint( const QString &pName, Actor *pA1,
								  Actor *pA2, bool pUnique = false );

	bool removeJoint( b2Joint *pJoint );

	bool removeJoint( const QString &pName );
	bool removeJoint( Actor *pActor );
	bool removeJoint( const ActorJoint *pJoint );

signals:
	void onCollision(const ContactPoint *pContactPoint);

protected:
	//! Why is this a list? and not a QHash?
	/*!
		A QList is used here to avoid name collisions
		and make possible sorting by Z-Order.
	*/
	typedef QList<Actor *> ActorArray;
	//! Where is the joints array?
	/*!
	  Because we don't draw or update joints "manually"
	  there is no reason to mantain another array of pointers,
	  so the internal b2World joint list is used.
	*/

	//! Holds queued up contact points after each step()
	typedef QList<ContactPoint *> ContactArray;

	ContactArray mContacts;
	unsigned short mNumContacts;

protected:
	virtual void _allocContacts(unsigned short pNumContacts);
	virtual void _resetContacts( void );
	virtual void _dispatchContacts( void );
	virtual void _deallocContacts( void );

protected:
	virtual QString _getUnique( const QString &pName );
	virtual bool _removeJointData( b2Joint *pJoint );

	virtual ActorArray::iterator _findActor(const QString &pName);

protected:
	ActorArray mActors;

	float mTimeStep;
	int mIters;

	//! Z-Order counter
	/*!
		Contains the top-level
		Z-Order available in the "World".
	*/
	float mZOrder;

	b2AABB mWorldAABB;
	b2World *mWorld;

	// PhysX drag-drop
	b2MouseJoint *mMouseJoint;
	Actor *mActor;

	// Hidden Ground Actor
	Actor *mGround;

	bool mDoSleep;
};

/* GL */ }

#endif // WORLD_H
