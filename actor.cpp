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
#include "actor.h"
#include "texture.h"
#include "texturemanager.h"
#include "env.h"
#include "world.h"

#include <QtCore/QDebug>
#include <QtCore/QHash>
#include <math.h>

using namespace GL;
using namespace Sys;

// Global
static Env *gEnv = &Env::getInstance();
static TextureManager *gTex = &TextureManager::getInstance();

Actor::Actor( const QString &pName, World *pWorld ) :	mTexture(0),
														mFlags(0),
														mNumFrames(-1),
														mFrame(-1),
														mFrameLoop(false),
														mAnimate(false),
														mFrameCounter(0.0f),
														mTexCoords(0),
														mVertices(0),
														mName(""),
														mId(0),
														mBlending(B_NONE),
														mBody(0),
														mWorld(0)
{
	// no joints by defult
	setHasJoints(false);
	// set default offsets
	setOffsets(0.0f,0.0f);
	// set default position
	setPos(0.0f,0.0f);
	// set default size
	setSize(0.0f,0.0f);
	// default layer (Z)
	setZOrder(0.0f);
	// no set box flag by default
	setFlag(S_BOX);
	// default rotation
	setRotation(0.0f);
	// set frame rect to 0
	setFrameSize(0.0f,0.0f,0.0f,0.0f);
	// set density ( static by default = 0 density )
	setDensity(0.0f);
	// set friction
	setFriction(0.5f);
	// set restitution (bouncyness)
	setRestituition(0.3f);
	//! set default color to white
	setColor(1.0f,1.0f,1.0f);
	//! 100% opaque
	setTransparency(0);

	// set unique name and id
	if( !pName.isEmpty() )
		setName(pName);

	// set initial number of frames
	setNumFrames(1);

	// store pointer to world
	mWorld = pWorld;

	qDebug() << "Success: [Actor] "<< mName << " created";
}

Actor::~Actor()
{
	if( mBody && mWorld )
	{
		removeJoint(); // remove all associated joints
		removePhysX(); // remove body
	}

	if( mTexCoords )
		delete [] mTexCoords;

	if( mVertices )
		delete [] mVertices;

	if( mTexture )
		mTexture->drop();

	qDebug() << "Success: [Actor] "<< mName << " destroyed";
}

void Actor::setTexture(Texture *pTex)
{
	// drop the old texture
	if( mTexture )
		mTexture->drop();

	// NULL?
	if( !pTex )
	{
		mTexture = 0;
		return;
	}

	// inc reference count ...
	mTexture = pTex->grab();

	// set initial frame rect
	// the same size as the texture size
	setFrameSize(0.0f,0.0f,pTex->getWidth(),pTex->getHeight());
	// set the initial frames to 1 by default
	setNumFrames(1);
	// set initial size as the texture size
	setRect(0,0,pTex->getWidth(),pTex->getHeight());
}

void Actor::setTexture(const QString &pName)
{
	setTexture(gTex->find(pName));
}

void Actor::setColor(float pR, float pG, float pB)
{
	mColor[0] = pR;
	mColor[1] = pG;
	mColor[2] = pB;
}

void Actor::setRect(float pX, float pY, float pW, float pH)
{
	setPos(pX,pY);
	setSize(pW,pH);
}

void Actor::setSizeFromTexture(void)
{
	if( !mTexture )
		return;

	setSize(mTexture->getWidth(),mTexture->getHeight());
}

void Actor::setPos(float pX, float pY)
{
	_setPos(pX,pY);

	// set physics properties
	if( mBody )
	{
		// screen space to world space
		b2Vec2 lPos(S2W(pX,pY));
		mBody->SetXForm(lPos,D2R(mRot));
	}
}

// set the size of the actor
void Actor::setSize(float pW, float pH)
{
	_setSize(pW,pH);
}

void Actor::setOffsets(float pX, float pY)
{
	mOffsets[0] = pX;
	mOffsets[1] = pY;
}

void Actor::setRotation(float pRot)
{
	// FIXME: set body if physics!
	_setRotation(pRot);
}

//! Center the actor around a certain point (globally)
void Actor::center(float pW, float pH)
{
	setPos((pW-mSize[0])/2,(pH-mSize[1])/2);
}

void Actor::moveXY( float pDX, float pDY )
{
	_setPos(mPos[0]+pDX,mPos[1]+pDY);
}

bool Actor::inRect( float pX, float pY )
{
	return( (mPos[0] <= pX) && (pX <= (mPos[0]+mSize[0])) &&
			(mPos[1] <= pY) && (pY <= (mPos[1]+mSize[1])) );
}

float Actor::getPosX( void ) const
{
	return mPos[0];
}

float Actor::getPosY( void ) const
{
	return mPos[1];
}

void Actor::getPos( t_point *pPos )
{
	Q_ASSERT( pPos != 0 );
	(*pPos)[0] = mPos[0];
	(*pPos)[1] = mPos[1];
}

float Actor::getWidth( void ) const
{
	return mSize[0];
}

float Actor::getHeight( void ) const
{
	return mSize[1];
}

void Actor::getSize( t_point *pSize )
{
	Q_ASSERT( pSize != 0 );
	(*pSize)[0] = mSize[0];
	(*pSize)[1] = mSize[1];
}

float Actor::getHWidth( void ) const
{
	return mHSize[0];
}

float Actor::getHHeight( void ) const
{
	return mHSize[1];
}

void Actor::getHalfSize( t_point *pHSize )
{
	Q_ASSERT( pHSize != 0 );
	(*pHSize)[0] = mHSize[0];
	(*pHSize)[1] = mHSize[1];
}

float Actor::getRadius( void ) const
{
	if( !isFlag(S_CIRCLE) )
		return 0.0f;

	//
	if( mHSize[1] > mHSize[0] )
		return mHSize[1];

	return mHSize[0];
}

void Actor::setFlag( unsigned long pFlag )
{
	mFlags |= pFlag;
}

bool Actor::isFlag(unsigned long pFlag) const
{
	return (mFlags & pFlag);
}

void Actor::setFlags( unsigned long pFlags )
{
	mFlags = pFlags;
}

void Actor::removeFlag( unsigned long pFlag )
{
	mFlags &= ~pFlag;
}

unsigned long Actor::getFlags( void ) const
{
	return mFlags;
}

void Actor::setZOrder(float pZ)
{
	mZ = pZ;
}

float Actor::getZOrder( void ) const
{
	return mZ;
}

void Actor::setNumFrames( short pNumFrames )
{
	if( !mTexture && mNumFrames > 1 )
		return;

	// delete the existing pre-calc
	if( mTexCoords )
		delete [] mTexCoords;

	mNumFrames = pNumFrames;
	setFrame(0);

	// pre-calc all texture coordinates
	mTexCoords = new t_texcoords[ mNumFrames ];

	if( mNumFrames == 1 )
	{
		// set default texture coordinates ( full texture, stretched )
		_setTexCoord(0,0.0f,1.0f,P_BL);
		_setTexCoord(0,1.0f,1.0f,P_BR);
		_setTexCoord(0,1.0f,0.0f,P_TR);
		_setTexCoord(0,0.0f,0.0f,P_TL);

		return;
	}

	// try to guess in the correct frame width
	//if( mNumFrames > 1 && (mFrameRect[2] == mTexture->getWidth() || mFrameRect[2] == 0) )
	{
		mFrameRect[2] = (float)(mTexture->getWidth() / mNumFrames);
	}

	// the height will never change
	float lY = (float)(mFrameRect[3] == mTexture->getHeight())?(mFrameRect[1]):((-mFrameRect[1])+mFrameRect[3]);
	float lH = (float)(lY + mFrameRect[3]);

	//qDebug() << lY << " , " << mFrameRect[3];

	lY /= mTexture->getHeight();
	lH /= mTexture->getHeight();

	// clamp
	if( lY > 1.0f )
		lY = 1.0f;

	if( lH > 1.0f )
		lH = 1.0f;

	// invert V (upside down)
	lY = 1.0f - lY;
	lH = 1.0f - lH;

	//
	for(unsigned short lFrame=0; lFrame<mNumFrames; lFrame++)
	{
		float lX = (float)(lFrame * mFrameRect[2]);
		float lW = (float)(lX + mFrameRect[2]);

		lX /= mTexture->getWidth();
		lW /= mTexture->getWidth();

		// clamp
		if( lX > 1.0f )
			lX = 1.0f;

		if( lW > 1.0f )
			lW = 1.0f;

		// animate using texture coordinates
		_setTexCoord(lFrame,lX,lY,P_BL);
		_setTexCoord(lFrame,lW,lY,P_BR);
		_setTexCoord(lFrame,lW,lH,P_TR);
		_setTexCoord(lFrame,lX,lH,P_TL);
	}
}

short Actor::getNumFrames( void ) const
{
	return mNumFrames;
}

void Actor::setFrame( short pFrame )
{
	// same frame? no valid frame number? don't re-calc bail :|
	if(pFrame > mNumFrames)
	{
		pFrame = 0;
		return;
	}

	// loop or set the actual next frame ...
	if( pFrame == mNumFrames )
	{
		if( mFrameLoop ) // go back to the 1st frame
		{
			mFrame = 0;
		}
	}
	else // set the actual frame
	{
		mFrame = pFrame;
	}
}

short Actor::getFrame( void ) const
{
	return mFrame;
}

void Actor::setFrameSize(float pX,float pY,float pW,float pH)
{
	mFrameRect[0] = pX;
	mFrameRect[1] = pY;
	mFrameRect[2] = pW;
	mFrameRect[3] = pH;
}

void Actor::setAnimate( bool pAnimate, bool pLoop )
{
	mAnimate = pAnimate;
	mFrameLoop = pLoop;
}

/*
	+-----+
	|     |
	|     |
	+-----+

	-x,-y // bottom left
	 x,-y // bottom right

	 x, y // top right
	-x, y // top left
*/
void Actor::_draw(void)
{
	glPushMatrix();

	glTranslatef(mDPos[0],mDPos[1],0.0f);
	glRotatef(mRot,0.0f,0.0f,1.0f);

#ifdef WORLD_VERTEX_ARRAYS
	glTexCoordPointer(2,GL_FLOAT,0,mTexCoords[mFrame]);
	glVertexPointer(2,GL_FLOAT,0,mVertices);
	glDrawArrays(GL_TRIANGLE_FAN,0,4);
#else
	glBegin(GL_TRIANGLE_FAN);
	glTexCoord2fv(mTexCoords[mFrame][P_BL]); glVertex2f( -mHSize[0],  -mHSize[1] );
	glTexCoord2fv(mTexCoords[mFrame][P_BR]); glVertex2f(  mHSize[0],  -mHSize[1] );
	glTexCoord2fv(mTexCoords[mFrame][P_TR]); glVertex2f(  mHSize[0],   mHSize[1] );
	glTexCoord2fv(mTexCoords[mFrame][P_TL]); glVertex2f( -mHSize[0],   mHSize[1] );
	glEnd();
#endif

	glPopMatrix();
}

// TODO: make this render via Vertex Arrays as well :)
void Actor::_drawDebug(void)
{
	if( !isShapeSet() || !gEnv->mDebugDraw )
		return;

	glPushMatrix();

	glTranslatef(mDPos[0],mDPos[1],0.0f);
	glRotatef(mRot,0.0f,0.0f,1.0f);

	glBegin(GL_LINE_LOOP);
	if( float lR = getRadius() ) // draw spheres
	{
		for(unsigned short i=0;i<=360;i+=5)
			glVertex2f(sin(D2R(i)) * lR, cos(D2R(i)) * lR);
	}
	else
	{
		for(unsigned short i=0;i<mShapeDef.vertexCount;i++)
			glVertex2f(W2S(mShapeDef.vertices[i].x,mShapeDef.vertices[i].y));
	}
	glEnd();

	glPopMatrix();
}

void Actor::_drawBlended(void)
{
	if( mBlending == B_NONE )
	{
		_drawTextured();
		return;
	}

	glEnable( GL_BLEND );
	glBlendFunc(GL_SRC_ALPHA,mBlending);
	_drawTextured();
	glDisable( GL_BLEND );
}

void Actor::_drawTextured(void)
{
	// no texture? draw as-is
	if( !mTexture )
	{
		_draw();
		return;
	}

	mTexture->enable();
	_draw();
	mTexture->disable();
}

void Actor::render(void)
{
	// don't render if it's not visible!
	if( mFlags & V_HIDDEN )
		return;

	// custom color with transparency
	glColor4fv(mColor);
	_drawBlended();

#ifdef WORLD_DEBUG_DRAW // draw an red outline in debug
	glColor3fv(gEnv->mDebugColor);
	_drawDebug();
#endif
}

void Actor::update(void)
{
	// don't update if it was requested
	if( mFlags & U_NOUPDATE )
		return;

	if( mBody ) // update PhysX body
	{
		b2Vec2 lPos = mBody->GetPosition();

		_setRotation(R2D(mBody->GetAngle()));
		_setPos(W2S_(lPos.x),W2S_(lPos.y));
	}

	if( !mAnimate ) // animation turned off?
		return;

	// animation delay
	if( ++mFrameCounter < gEnv->mAnimFrameDelay )
		return;

	// reset, then move on ...
	mFrameCounter = 0.0f;

	if( mNumFrames >= 1 )
	{
		// increment by one
		setFrame(mFrame+1);
	}
	else
	{
		// always show the 1st frame
		setFrame(0);
	}
}

void Actor::setName( const QString &pName )
{
	mName = pName;
	mId = qHash(pName);
}

QString &Actor::getName( void )
{
	return mName;
}

unsigned int Actor::getId( void ) const
{
	return mId;
}

void Actor::setShape(const t_point *pVerts, unsigned short pNumVerts)
{
	Q_ASSERT( pVerts != 0	);
	Q_ASSERT( pNumVerts > 0 );

	//! Set only when possible
	if( !(mFlags & S_CUSTOM) )
		return;

	// copy over the vertices
	mShapeDef.vertexCount = pNumVerts;
	for(unsigned short i = 0; i<pNumVerts; i++)
	{
		//! Convert to WORLD
		float lX = S2W_((pVerts[i][0]-mOffsets[0]));
		float lY = S2W_((pVerts[i][1]-mOffsets[1]));

		mShapeDef.vertices[i].Set(lX,lY);
	}
}

void Actor::setDensity( float pDens )
{
	mShapeDef.density = pDens;
}

void Actor::setFriction( float pFric )
{
	mShapeDef.friction = pFric;
}

void Actor::setRestituition( float pRest )
{
	mShapeDef.restitution = pRest;
}

void Actor::applyPhysX(void)
{
	Q_ASSERT( mWorld != 0 );

	//! Remove the existing body
	if( mBody )
		removePhysX();

	b2BodyDef lBodyDef;
	b2World *lWorld = mWorld->getPhysicsWorld();

	//! Set initial rotation
	lBodyDef.angle = D2R(mRot);
	//! Set initial position
	lBodyDef.position.Set(S2W((mPos[0]+mHSize[0]),(mPos[1]+mHSize[1])));

	mBody = lWorld->CreateBody(&lBodyDef);

	if( isFlag(S_CUSTOM) )
	{
		if( isShapeSet() )
		{
			//! use the predefined shape
			mBody->CreateShape(&mShapeDef);
		}
		else
		{
			qDebug() << "Warning: [Actor] Custom Shape but no vertices set";
			setFlag(S_BOX); // fallback to BOX
		}
	}

	if( isFlag(S_BOX) )
	{
		//! Set Shape
		mShapeDef.SetAsBox(S2W((mHSize[0]-mOffsets[0]),(mHSize[1]-mOffsets[1])));
		mBody->CreateShape(&mShapeDef);
	}
	else if( isFlag(S_CIRCLE) )
	{
		b2CircleDef lShapeDef;
		// copy them over
		lShapeDef.restitution = mShapeDef.restitution;
		lShapeDef.density = mShapeDef.density;
		lShapeDef.friction = mShapeDef.friction;
		//! Set Shape
		lShapeDef.radius = S2W_((getRadius()-mOffsets[0]));
		mBody->CreateShape(&lShapeDef);
		//! Fake it, not used anyway :)
		mShapeDef.vertexCount = 1;
	}

	// auto-calc mass if density set (i.e > 0)
	if( mShapeDef.density )
		mBody->SetMassFromShapes();

	//! Store itself as userData
	mBody->SetUserData(this);
}

void Actor::removePhysX(void)
{
	Q_ASSERT( mWorld != 0 );

	if( mWorld->removeActorBody(this) )
		mBody = 0;
}

void Actor::removeJoint(void)
{
	Q_ASSERT( mWorld != 0 );

	mWorld->removeJoint(this);
}

bool Actor::isFrozen(void) const
{
	if( !mBody )
		return false;

	return mBody->IsFrozen();
}

void Actor::setHasJoints( bool pState )
{
	mHasJoints = pState;
}

bool Actor::hasJoints( void ) const
{
	return mHasJoints;
}

void Actor::applyForce(const float pX, const float pY, const bool pMass)
{
	Q_ASSERT( mBody != 0 );

	const float lM = mBody->GetMass();
	const float lX = (pMass)?pX*lM:pX;
	const float lY = (pMass)?pY*lM:pY;

	mBody->ApplyForce(b2Vec2(lX,lY),mBody->GetWorldCenter());
}

void Actor::applyImpulse(const float pX, const float pY)
{
	Q_ASSERT( mBody != 0 );
	mBody->ApplyImpulse(b2Vec2(pX,pY),mBody->GetWorldCenter());
}

void Actor::applyTorque(const float pT)
{
	Q_ASSERT( mBody != 0 );
	mBody->ApplyTorque(pT);
}

void Actor::setBlending( t_blend pBlending )
{
	mBlending = pBlending;
}

void Actor::setTransparency( float pLevel )
{
	if( pLevel < 0.0f )
		pLevel = 0; // fully opaque

	if( pLevel > 255.0f )
		pLevel = 255.0f; // full transparent

	// invert ( 0 == opaque, 255 == fully transparent )
	pLevel = 255 - pLevel;

	// convert to 0..1 interval
	mColor[3] = (float)(pLevel / 255.0f);
}

void Actor::_setTexCoord( short pFrame, t_point *pCoord, t_pos pPos )
{
	_setTexCoord( pFrame, (*pCoord)[0], (*pCoord)[1], pPos );
}

void Actor::_setTexCoord( short pFrame, float pU, float pV, t_pos pPos )
{
	mTexCoords[ pFrame ][ pPos ][ 0 ] = pU;
	mTexCoords[ pFrame ][ pPos ][ 1 ] = pV;
}

void Actor::_setVertex( t_point *pVertex, t_pos pPos )
{
	_setVertex((*pVertex)[0],(*pVertex)[1],pPos);
}

void Actor::_setVertex( float pX, float pY, t_pos pPos )
{
	mVertices[ pPos ][ 0 ] = pX;
	mVertices[ pPos ][ 1 ] = pY;
}

void Actor::_setPos(float pX, float pY)
{
	mDPos[0] = mPos[0] = pX;
	mDPos[1] = mPos[1] = pY;

	_setSize(-1,-1);
}

void Actor::_setSize(float pW, float pH)
{
	if( pW != -1 && pH != -1 )
	{
		// store initial size
		mSize[0] = pW; // screen space
		mSize[1] = pH; // screen space

		// calculate half size
		mHSize[0] = mSize[0] / 2; // screen space
		mHSize[1] = mSize[1] / 2; // screen space

		//! Setup Vertex Array
		if( mVertices )
			delete mVertices;

		mVertices = new t_vertices[4];

		_setVertex(-mHSize[0],-mHSize[1],P_BL);
		_setVertex( mHSize[0],-mHSize[1],P_BR);
		_setVertex( mHSize[0], mHSize[1],P_TR);
		_setVertex(-mHSize[0], mHSize[1],P_TL);
	}

	if( !mBody ) // no PhysX, draw centered
	{
		mDPos[0] = mPos[0] + mHSize[0];	// screen space
		mDPos[1] = mPos[1] + mHSize[1];	// screen space
	}
}

void Actor::_setRotation(float pRot)
{
	mRot = pRot;
}
