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
#ifndef TOOLPALLETTE_H
#define TOOLPALLETTE_H

#include "actor.h"

namespace Pyp {

class ToolPallette;

class Tool : public GL::Actor
{
public:
	Tool(const QString &pName,GL::World *pWorld,ToolPallette *pParent);
	virtual ~Tool();

	virtual bool update(int lX, int lY,int pButton);
	virtual void render(void);

	virtual bool isSelected( void ) const;
	virtual void setSelected( bool pSelected );

public:
	enum
	{
		STANDARD = BIT(8+0),
		GEL		 = BIT(8+1),
		LEFT	 = BIT(8+2),
		RIGHT	 = BIT(8+3),
		SPRING	 = BIT(8+4),
		DELETE	 = BIT(8+5)
	};

protected:
	Actor *mSelected;
	bool mIsSelected;
	bool mIsOver;
	ToolPallette *mParent;
};

class ToolPallette : public GL::Actor
{
public:
	ToolPallette(const QString &pName,GL::World *pWorld=0);
	virtual ~ToolPallette();

	virtual void init(void);

	virtual void render(void);
	virtual bool update(int lX, int lY,int pButton);

	virtual Tool *getSelected( void ) const;

protected:
	friend class Tool;

	virtual void _addTool(int pTool, const QString &pName);

	virtual void _setSelected( void );
	virtual void _unselectAll( void );

protected:
	enum
	{
		NUM_TOOLS = 6
	};

	Tool *mTools[NUM_TOOLS];

	float mLastX;
	float mLastY;

	// holds the index of the selected tool
	int mSelected;
};

}

#endif // TOOLPALLETTE_H
