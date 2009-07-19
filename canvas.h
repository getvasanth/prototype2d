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
#ifndef CANVAS_H
#define CANVAS_H

#include <QGLWidget>

#include <QtGui/QMouseEvent>
#include <QtGui/QKeyEvent>

#include <QtCore/QTimer>
#include <QDebug>

#include "defines.h"
#include "world.h"

namespace Interface
{
	class IGame;
}

namespace GL {

class Canvas : public QGLWidget
{
   Q_OBJECT

public:
	Canvas(Interface::IGame *pGame,QWidget *pParent=0);
	virtual ~Canvas();

public:
	virtual void setGame( Interface::IGame *pGame );

protected:
	void initializeGL(void);
	void resizeGL(int pWidth, int pHeight);
	void paintGL(void);

public:
	void updateGL(void);

protected:
	void begin2D(int pWidth, int pHeight);
	void end2D( void );

	void mouseMoveEvent(QMouseEvent *pEvent);
	void mousePressEvent(QMouseEvent *pEvent);
	void mouseReleaseEvent(QMouseEvent *pEvent);

	void keyPressEvent(QKeyEvent *pEvent);
	void keyReleaseEvent(QKeyEvent *pEvent);

#ifdef WORLD_VERTEX_ARRAYS
protected:
	void enableClientStates(void);
	void disableClientStates(void);
#endif

protected:
	QTimer *mTimer;
	Interface::IGame *mGame;
};

/* GL */}

#endif // CANVAS_H
