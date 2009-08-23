#include "jellyactor.h"
#include "actor.h"
#include "world.h"

using namespace GL;

namespace Jelly {

JellyActor::JellyActor(const QString &pName,World *pWorld) : Actor(pName,pWorld)
{
}

JellyActor::~JellyActor()
{
}

void JellyActor::applyPhysX(void)
{
	Actor::applyPhysX();

	if( mBody )
	{
		mBody->SetBullet(true);
		mBody->AllowSleeping(true);
	}
}

void JellyActor::addSpringForceUp(JellyActor *pActor)
{
	addSpringForce(pActor,true);
}

void JellyActor::addSpringForceDown(JellyActor *pActor)
{
	addSpringForce(pActor,false);
}

void JellyActor::addSpringForce(JellyActor *pActor, bool pUpDown)
{
	b2Vec2 lUp(0.0f, 1.0f);
	b2Vec2 lDown(0.0f, -1.0f);

	if( !pUpDown )
		qSwap(lUp,lDown);

	float32 lDesiredDist = (pUpDown)?0.0f:4.0f;

	b2Body *lBody2 = pActor->getBody();

	b2Vec2 lPA = mBody->GetWorldPoint(lUp);
	b2Vec2 lPB = lBody2->GetWorldPoint(lDown);

	b2Vec2 lDiff = lPB - lPA;
	b2Vec2 lVA = mBody->GetLinearVelocity() - b2Cross(mBody->GetWorldVector(lUp), mBody->GetAngularVelocity());
	b2Vec2 lVB = lBody2->GetLinearVelocity() - b2Cross(lBody2->GetWorldVector(lDown), lBody2->GetAngularVelocity());
	b2Vec2 lVdiff = lVB-lVA;
	float32 lDx = lDiff.Normalize();
	float32 lVrel = lVdiff.x*lDiff.x + lVdiff.y*lDiff.y;
	float32 lForceMag = -500*(lDx-lDesiredDist)-10.0f*lVrel;
	lDiff *= lForceMag;
	lBody2->ApplyForce(lDiff, mBody->GetWorldPoint(lUp));
	lDiff *= -1.0f;
	mBody->ApplyForce(lDiff,lBody2->GetWorldPoint(lDown));
	mBody->WakeUp();
	lBody2->WakeUp();
}

}
