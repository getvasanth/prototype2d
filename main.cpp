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
#include <QtGui/QApplication>
#include "mainwindow.h"
#include "startupdlg.h"
#include "env.h"
#include "gamemanager.h"
#include "defines.h"

#include <QtOpenGL>
#include <QtCore/QDebug>

using namespace Sys;
using namespace Game;

#include "games/pyp/pyp.h"
#include "games/template/template.h"
#include "games/force/force.h"
#include "games/tail/tail.h"

int main(int argc, char *argv[])
{
	QApplication lApp(argc, argv);

	Env *lEnv = &Env::getInstance();
	GameManager *lGames = &GameManager::getInstance();

	// add all built-in games
	lGames->addGame("Pyp",new Pyp::Game());
	lGames->addGame("Template",new Template::Game());
	lGames->addGame("Apply Force",new Force::Game());
	lGames->addGame("GL Tail Clone",new Tail::Game());

#ifdef SHOW_STARTUP_DIALOG
	Startupdlg lDlg;
	lDlg.show();

	if( lDlg.exec() == QDialog::Rejected )
	{
		return 13;
	}
#endif

#ifdef WORLD_MULTISAMPLING
	QGLFormat lGLFrm = QGLFormat::defaultFormat();
	lGLFrm.setSampleBuffers(true);
	lGLFrm.setSamples(lEnv->mNumSamples);
	QGLFormat::setDefaultFormat(lGLFrm);
#endif

	// PASS IN THE GAME INSTANCE
	MainWindow *lWindow = new MainWindow(lGames->getGame());

	// WINDOWED MODE?
	if( lEnv->mFullscreen )
		lWindow->showFullScreen();
	else
		lWindow->show();

	// RUN
	int lRet = lApp.exec();

	// DELETE WINDOW
	delete lWindow;

	// remove all games
	lGames->removeAll();

	// exit
	return lRet;
}
