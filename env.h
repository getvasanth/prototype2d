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
#ifndef ENV_H
#define ENV_H

#include "types.h"

namespace Interface
{
  class IGame;
}

namespace Sys {

// TODO: add setters for Vectors
struct Env
{
	//! Constructor
	Env()
	{
		// fill in default values

		mFrameDelay		= 33;				//! 30 fps
		mAnimFrameDelay = 3;				//! 30 fps
		mTimeStep		= 1.0f / 60.0f;		//! 30 fps

		mFrameDelay		= 16;				//! 60 fps
		mAnimFrameDelay = 6;				//! 60 fps
		mTimeStep		= 1.0f / 60.0f;		//! 60 fps

		mIterations = 10;

		mNumContactPoints = 30;

		// default gravity
		mGravity[0] =  0.0f;
		mGravity[1] = 10.0f;

		mWWidth = 800;
		mWHeight= 600;
		mWTitle = "Prototype2D";
		mWResize= false;
		mFullscreen = false;

		mSWidth = 800;
		mSHeight= 600;

		mBasePath = "./data/";

		mDebugDraw = false;

		// Debug Lines == RED
		mDebugColor[0] = 1.0f;
		mDebugColor[1] = 0.0f;
		mDebugColor[2] = 0.0f;

		// BACKGROUND COLOR (light blueeeeeeeeee)
		mBgColor[0] = 0.67f;
		mBgColor[1] = 0.84f;
		mBgColor[2] = 0.90f;

		mHideCursor = true;
		mCaptCursor = true;

		//4x by default
		mNumSamples = 4;
	}
	//! Destructor
	virtual ~Env()
	{
	}
	/*!
		Conversion formula:

		1000 / mFramePerSec

		e.g:

		30 fps

		1000/30 = ~33.33 ms

		This is used by the global timer
		as an internal.
	*/
	int mFrameDelay;
	/*!
		Conversion formula:

		mFramesPerSec / 10

		e.g:

		30/10 = 3

		This is used by the "Actors" when
		animating, to control the actual
		animation speed as desired.
	*/
	int mAnimFrameDelay; // ~ 20 fps
	/*!
		Box2D Physics Time Step

		Conversion formula:

		1.0 / mFramePerSec

		e.g:

		60 fps

		1.0 / 60 = ~0.016
	*/
	float mTimeStep;
	/*!
		Box2D Physics Iteration
	*/
	int mIterations;
	/*!
		Box2D Initial Gravity
	*/
	t_vec2 mGravity;
	/*!
		Maximum Number of Contact Points
	*/
	unsigned short mNumContactPoints;
	//! Window Width
	int mWWidth;
	//! Window Height
	int mWHeight;

	//! Window Title
	const char *mWTitle;

	//! Resizable Flag
	bool mWResize;

	//! Fullscreen mode Flag
	bool mFullscreen;

	//! Screen Width
	int mSWidth;
	//! Screen Height
	int mSHeight;

	//! Base Data Path
	const char *mBasePath;

	//! Debug Draw flag when compiled with debug draw
	bool mDebugDraw;
	//! Vertex Outline Colors For Debug Draw
	t_vec3 mDebugColor;

	//! Background Color
	t_vec3 mBgColor;

	//! Show Mouse Pointer or Not
	bool mHideCursor;
	//! Enable Mouse Tracking
	bool mCaptCursor;

	//! Multisampling ( available only when compiled appropiately )
	unsigned short mNumSamples;

	//! Get the Instance of the environment
	static Env &getInstance( void )
	{
		static Env staticEnv;
		return staticEnv;
	}
};

/* Sys */ }

#endif // ENV_H
