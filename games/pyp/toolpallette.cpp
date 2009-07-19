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
#include "toolpallette.h"

#include <QtCore/QDebug>

using namespace GL;
using namespace Pyp;

Tool::Tool(const QString &pName,World *pWorld,ToolPallette *pParent) :
		Actor(pName,pWorld),
		mSelected(0),
		mIsSelected(false),
		mIsOver(false),
		mParent(0)
{
	// selected
	mSelected = new Actor("Selected");
	mSelected->setTexture("textures/pyp/Tool-Selected2.png");
	mSelected->setRect(0,0,47,47);
	mSelected->setBlending(Actor::B_SRC_ALPHA);

	mParent = pParent;

	assert( mParent );
}

Tool::~Tool()
{
	if( mSelected )
		delete mSelected;
}

bool Tool::update(int pX, int pY, int pButton)
{
	mIsOver = false;

	if( inRect(pX,pY) )
	{
		mIsOver = true;

		mSelected->setPos(getPosX(),getPosY());

		// toggle
		if( pButton == Qt::LeftButton )
		{
			// save the initial tool state
			bool lOldSelected = mIsSelected;

			// de-select all
			mParent->_unselectAll();

			// toggle effect
			mIsSelected = !lOldSelected;

			render(); // re-draw imediatelly ...

			// find the selected tool's index
			mParent->_setSelected();

			return true;
		}
	}

	return false;
}

void Tool::render(void)
{
	Actor::render();

	if( mIsSelected || (mIsOver && !mIsSelected) )
		mSelected->render();
}

bool Tool::isSelected( void ) const
{
	return mIsSelected;
}

void Tool::setSelected( bool pSelected )
{
	mIsSelected = pSelected;
}

ToolPallette::ToolPallette(const QString &pName,World *pWorld) : Actor(pName,pWorld),
																 mLastX(0.0f),
																 mLastY(0.0f),
																 mSelected(-1)
{
	// set texture to self
	setTexture("textures/pyp/Tool-Background2.png");

	// set them to NULL
	for(int i=0;i<NUM_TOOLS;i++)
		mTools[i] = 0;
}

ToolPallette::~ToolPallette()
{
	for(int i=0;i<NUM_TOOLS;i++)
		delete mTools[i];
}

void ToolPallette::init(void)
{
	// already initialized
	if( mTools[0] )
		return;

	// compute starting position
	mLastX = getPosX();
	mLastY = getPosY();

	// adjust
	mLastX += 8;
	mLastY += 8;

	// add all tools
	_addTool(0,"StandardBlock2");
	_addTool(1,"GelBlock2");
	_addTool(2,"LeftRamp2");
	_addTool(3,"RightRamp2");
	_addTool(4,"Spring2");
	_addTool(5,"Delete2");
}

void ToolPallette::render(void)
{
	// render the pallette background
	Actor::render();

	// render the tools
	for(int i=0;i<NUM_TOOLS;i++)
		mTools[i]->render();
}

bool ToolPallette::update(int lX, int lY,int pButton)
{
	bool lRet = false;

	// update the tools
	for(int i=0;i<NUM_TOOLS;i++)
		if( mTools[i]->update(lX,lY,pButton) )
			lRet = true;

	return lRet;
}

Tool *ToolPallette::getSelected( void ) const
{
	return (mSelected==-1)?0:mTools[mSelected];
}

void ToolPallette::_addTool(int pTool,const QString &pName)
{
	mTools[pTool] = new Tool(pName,mWorld,this);
	mTools[pTool]->setTexture("textures/pyp/Tool-"+pName+".png");
	mTools[pTool]->setRect(mLastX,mLastY,47,47);
	mTools[pTool]->setBlending(Actor::B_SRC_ALPHA);
	mTools[pTool]->setFlag(BIT(8+pTool)); // set the index as a flag

	mLastY += 47 + 3;
}

void ToolPallette::_unselectAll(void)
{
	// un-select the tools
	for(int i=0;i<NUM_TOOLS;i++)
		mTools[i]->setSelected(false);
}

void ToolPallette::_setSelected(void)
{
	// assume no tool was selected
	mSelected = -1;

	// find the selected
	for(int i=0;i<NUM_TOOLS;i++)
	{
		if( mTools[i]->isSelected() )
		{
			mSelected = i;
			break;
		}
	}
}
