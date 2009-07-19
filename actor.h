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
#ifndef ACTOR_H
#define ACTOR_H

#include <QtOpenGL>
#include <QtCore/QString>

#include "Box2D/Box2D.h"

#include "types.h"
#include "defines.h"

namespace GL {

class Texture;
class World;

class Actor
{
public:
	Actor(const QString &pName = "", World *pWorld = 0);
	virtual ~Actor();

	virtual void setTexture(Texture *pTex);
	virtual void setTexture(const QString &pName);

	virtual void setColor(float pR, float pG, float pB);

	virtual void setRect(float pX, float pY, float pW, float pH);
	virtual void setSizeFromTexture(void);

	virtual void setPos(float pX, float pY);
	virtual void setSize(float pW, float pH);
	virtual void setOffsets(float pX, float pY);

	virtual void setRotation(float pRot);

	virtual void moveXY( float pDX, float pDY );

	virtual bool inRect( float pX, float pY );

	virtual float getPosX( void ) const;
	virtual float getPosY( void ) const;
	virtual void getPos( t_point *pPos );

	virtual float getWidth( void ) const;
	virtual float getHeight( void ) const;
	virtual void getSize( t_point *pSize );

	virtual float getHWidth( void ) const;
	virtual float getHHeight( void ) const;
	virtual void getHalfSize( t_point *pHSize );

	virtual float getRadius( void ) const;

	virtual void setFlag( unsigned long pFlag );
	virtual bool isFlag( unsigned long pFlag ) const;
	virtual void setFlags( unsigned long pFlags );
	virtual void removeFlag( unsigned long pFlag );
	virtual unsigned long getFlags( void ) const;

	virtual void setZOrder(float pZ);
	virtual float getZOrder( void ) const;

	virtual void setNumFrames( short pNumFrames );
	virtual	short getNumFrames( void ) const;

	virtual void setFrame( short pFrame );
	virtual short getFrame( void ) const;

	virtual void setFrameSize(float pX,float pY,float pW,float pH);

	virtual void setAnimate( bool pAnimate, bool pLoop = false );

	virtual void render(void);
	virtual void update(void);

	virtual void setName( const QString &pName );
	virtual QString &getName( void );
	virtual unsigned int getId( void ) const; //id == name hash

	// physics
	virtual void setDensity( float pDens );
	virtual void setFriction( float pFric );
	virtual void setRestituition( float pRest );

	virtual void applyPhysX(void);
	virtual void removePhysX(void);
	virtual void removeJoint(void);

	virtual bool isFrozen(void) const;

	virtual void setHasJoints( bool pState );
	virtual bool hasJoints( void ) const;

	virtual void applyForce(const float pX, const float pY, const bool pMass=true);
	virtual void applyImpulse(const float pX, const float pY);
	virtual void applyTorque(const float pT);

	virtual void setBody(b2Body *pBody) { mBody = pBody; }
	virtual b2Body *getBody( void ) const { return mBody; }

	virtual void setShape(const t_point *pVerts, unsigned short pNumVerts);
	virtual bool isShapeSet(void) const { return (mShapeDef.vertexCount); }

public:
	typedef enum e_blend
	{
		B_NONE		= -1,
		B_SRC_ALPHA	= GL_ONE_MINUS_SRC_ALPHA,
		B_ONE		= GL_ONE

	} t_blend;
	// enable / disable blending
	virtual void setBlending( t_blend pBlending );
	/*
		255	= fully transparent
		0	= opaque
	*/
	virtual void setTransparency( float pLevel );

	// Actor Shape Types
	enum
	{
		S_BOX		= BIT(0),
		S_CIRCLE	= BIT(1),
		S_CUSTOM	= BIT(2)
	};

	// visibility
	enum
	{
		V_HIDDEN = BIT(3)
	};

	// various/misc.
	enum
	{
		U_NOUPDATE = BIT(4),
		J_HASJOINTS= BIT(5) // reserved!
	};

protected:
	typedef enum e_pos
	{
		P_BL	= 0, /* Bottom Left		*/
		P_BR	= 1, /* Bottom Right	*/
		P_TR	= 2, /* Top Right		*/
		P_TL	= 3	 /* Top Left		*/
	} t_pos;

	virtual void _setTexCoord( short pFrame, t_point *pCoord, t_pos pPos );
	virtual void _setTexCoord( short pFrame, float pU, float pV, t_pos pPos );

	virtual void _setVertex( t_point *pVertex, t_pos pPos );
	virtual void _setVertex( float pX, float pY, t_pos pPos );

	virtual void _setPos( float pX, float pY );
	virtual void _setSize( float pW, float pH );
	virtual void _setRotation( float pRot );

	virtual void _draw(void);
	virtual void _drawDebug(void);
	virtual void _drawTextured(void);
	virtual void _drawBlended(void);

protected:
	//! Texture Assigned to the Actor
	Texture *mTexture;

	t_point mPos;	   // screen space position
	t_point mSize;	   // screen space size

	t_point mDPos;	// screen space drawing position
	t_point mHSize; // screen space half width / half height

	/*!
		Used to offset Graphics in Physics.
		See the provided game sample, and it will imediatelly make
		sense. :)
	*/
	t_point mOffsets; // screen space offsets ( mPos + mOffsets )

	float mRot;	// store the actual rotation
	float mZ; // z-order

	unsigned long mFlags; // store various flags (bit based)

	short mNumFrames;	// number of frames
	short mFrame;		// the current frame

	t_rect  mFrameRect;	// initial position + size of a frame
	bool mFrameLoop;	// whatever loop frames or not
	bool mAnimate;		// whatever animate or not

	// animation frame delay counter
	float mFrameCounter;

	typedef t_point t_texcoords[4];
	//! Texture Coordinate Array
	t_texcoords *mTexCoords;

	typedef t_point t_vertices;
	//! Vertex Array
	t_vertices *mVertices;

	//! Colors
	t_vec4 mColor;

	QString mName;
	unsigned int mId;

	t_blend mBlending;

	// TODO: move this into FLAGS
	bool mHasJoints;

	// PhysX Body
	b2Body *mBody;
	b2PolygonDef mShapeDef;

	//! Pointer to the World
	World *mWorld;
};

/* GL */ }

#endif // ACTOR_H
