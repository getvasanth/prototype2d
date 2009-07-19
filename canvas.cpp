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
#include "canvas.h"
#include "env.h"

#include "igame.h"
#include <QGLWidget>
#include <QtOpenGL>
#include <QtGui/QApplication>

using namespace GL;
using namespace Sys;
using namespace Interface;

// Global
static Env *gEnv = &Env::getInstance();

Canvas::Canvas(Interface::IGame *pGame, QWidget *pParent) :  QGLWidget(pParent),
															 mTimer(new QTimer(this)),
															 mGame(0)
{
	// MOUSE
	setMouseTracking(gEnv->mCaptCursor);

	// HIDE if requested
	if( gEnv->mHideCursor )
		setCursor(Qt::BlankCursor);

	// KEYBOARD
	setFocusPolicy(Qt::StrongFocus);

	// RESIZE
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

	// CONNECT THE GL UPDATE
	connect(mTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
	mTimer->start(gEnv->mFrameDelay); // start at the given FPS

	// SET THE GAME
	setGame(pGame);
}

Canvas::~Canvas()
{
#ifdef WORLD_VERTEX_ARRAYS
	// SHUTDOWN CLIENT STATES
	disableClientStates();
#endif

	// SHUTDOWN UPDATE TIMER
	if( mTimer )
		delete mTimer;

	// SHUTDOWN GAME INSTANCE
	setGame(0);
}

void Canvas::setGame(IGame *pGame)
{
	// shutdown the previous game
	if( mGame )
		mGame->shutdown();

	mGame = pGame;
}

void Canvas::initializeGL(void)
{
	Q_ASSERT( mGame != 0 );

	// set a visually pleasant background color
	glClearColor(gEnv->mBgColor[0],
				 gEnv->mBgColor[1],
				 gEnv->mBgColor[2], 1.0f);

	glEnable(GL_TEXTURE_2D);  // we always want textures
	glDisable(GL_DEPTH_TEST); // make sure it's off

	// ask for real nice lines and points ... :P
	glHint(GL_POINT_SMOOTH_HINT,GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT,GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POINT_SMOOTH);

#ifdef WORLD_MULTISAMPLING
	glEnable(GL_MULTISAMPLE);

	int lBufs;
	int lSamples;
	glGetIntegerv(GL_SAMPLE_BUFFERS, &lBufs);
	glGetIntegerv(GL_SAMPLES, &lSamples);
	qDebug("Success: [Canvas] %d buffers and %d samples", lBufs, lSamples);
#endif

#ifdef WORLD_VERTEX_ARRAYS
	// SETUP CLIENT STATES
	enableClientStates();
#endif

	// TIME TO INIT THE ACTUAL GAME
	mGame->init();
}

void Canvas::resizeGL(int pWidth, int pHeight)
{
	if (pHeight==0)
	{
		pHeight=1;
	}

	glViewport(0, 0, pWidth, pHeight);
}

void Canvas::paintGL(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	begin2D(gEnv->mSWidth,gEnv->mSHeight);

	// IF IT'S NULL, AN ASSERT ALREADY TRIGGERS
	// INSIDE INITIALIZEGL, SO IT SHOULD NEVER GET
	// HERE AS NULL ...
	mGame->render();

	end2D();
}

void Canvas::updateGL(void)
{
	QGLWidget::updateGL();
	mGame->think();
}

void Canvas::begin2D( int pWidth, int pHeight )
{
	// go into 2D mode (ortho)
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, pWidth, pHeight, 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void Canvas::end2D( void )
{
	// get out of 2D mode
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

void Canvas::mouseMoveEvent(QMouseEvent *pEvent)
{
	// IF IT'S NULL, AN ASSERT ALREADY TRIGGERS
	// INSIDE INITIALIZEGL, SO IT SHOULD NEVER GET
	// HERE AS NULL ...
	mGame->updateMouse(pEvent->x(),pEvent->y(),pEvent->buttons(),IGame::M_MOVE);
	QGLWidget::mouseMoveEvent( pEvent ); // HANDLE UN-HANDLED EVENTS
}

void Canvas::mousePressEvent(QMouseEvent *pEvent)
{
	// IF IT'S NULL, AN ASSERT ALREADY TRIGGERS
	// INSIDE INITIALIZEGL, SO IT SHOULD NEVER GET
	// HERE AS NULL ...
	mGame->updateMouse(pEvent->x(),pEvent->y(),pEvent->buttons(),IGame::M_DOWN);
	QGLWidget::mousePressEvent(pEvent); // HANDLE UN-HANDLED EVENTS
}

void Canvas::mouseReleaseEvent(QMouseEvent *pEvent)
{
	// IF IT'S NULL, AN ASSERT ALREADY TRIGGERS
	// INSIDE INITIALIZEGL, SO IT SHOULD NEVER GET
	// HERE AS NULL ...
	mGame->updateMouse(pEvent->x(),pEvent->y(),pEvent->buttons(),IGame::M_UP);
	QGLWidget::mouseReleaseEvent(pEvent); // HANDLE UN-HANDLED EVENTS
}

void Canvas::keyPressEvent(QKeyEvent *pEvent)
{
	// IF IT'S NULL, AN ASSERT ALREADY TRIGGERS
	// INSIDE INITIALIZEGL, SO IT SHOULD NEVER GET
	// HERE AS NULL ...

	// GRAB
	int lKey = pEvent->key();

	// HANDLE ESCAPE HERE
	if( lKey == Qt::Key_Escape )
		QApplication::quit();
	else if( !mGame->updateKeys(lKey,IGame::K_DOWN) ) // PASS IN ALL OTHER KEYS
		QGLWidget::keyPressEvent(pEvent); // HANDLE UN-HANDLED KEYS
}

void Canvas::keyReleaseEvent(QKeyEvent *pEvent)
{
	// IF IT'S NULL, AN ASSERT ALREADY TRIGGERS
	// INSIDE INITIALIZEGL, SO IT SHOULD NEVER GET
	// HERE AS NULL ...

	// GRAB
	if( !mGame->updateKeys(pEvent->key(),IGame::K_UP) ) // PASS IN ALL OTHER KEYS
		QGLWidget::keyReleaseEvent(pEvent); // HANDLE UN-HANDLED KEYS
}

#ifdef WORLD_VERTEX_ARRAYS
void Canvas::enableClientStates(void)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Canvas::disableClientStates(void)
{
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}
#endif
