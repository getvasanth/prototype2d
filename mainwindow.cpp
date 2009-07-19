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
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "canvas.h"
#include "env.h"
#include "igame.h"

using namespace GL;
using namespace Interface;
using namespace Sys;

MainWindow::MainWindow(IGame *pGame, QWidget *pParent)
	: QMainWindow(pParent), mUi(new Ui::MainWindow)
{
	Q_ASSERT( pGame != 0 );

	//! CONFIGURE GAME PRE-MATURELY
	pGame->configure();

	mUi->setupUi(this);

	//! GRAB ENVIRONMENT
	Env *lEnv = &Env::getInstance();

	//! set the window title
	setWindowTitle( lEnv->mWTitle );

	//! set initial window size
	setMinimumSize(lEnv->mWWidth,lEnv->mWHeight);

	//! reisizeable or not?
	if( !lEnv->mWResize && !lEnv->mFullscreen )
		setMaximumSize(lEnv->mWWidth,lEnv->mWHeight);

	//! new GL Canvas
	mCanvas = new Canvas(pGame);

	//! Set the margins to 0 (fill the whole area)
	mUi->gridLayout->setMargin(0);
	mUi->gridLayout->addWidget( mCanvas );
}

MainWindow::~MainWindow()
{
	delete mCanvas;
	delete mUi;
}
