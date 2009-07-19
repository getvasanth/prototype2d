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
#include "startupdlg.h"
#include "ui_startupdlg.h"
#include "env.h"
#include "gamemanager.h"

#include <QtCore/QDebug>

using namespace Sys;
using namespace Game;

static Env *gEnv = &Env::getInstance();
static GameManager *gGames = &GameManager::getInstance();

Startupdlg::Startupdlg(QWidget *parent) :
	QDialog(parent),
	m_ui(new Ui::Startupdlg)
{
	m_ui->setupUi(this);

	// no resize
	setMinimumSize(400,191);
	setMaximumSize(400,191);

	// set some pre-defined title
	setWindowTitle( gEnv->mWTitle );

	// add games
	GameManager::GameArray lGames = gGames->getGames();
	if( !lGames.empty() )
	{
		GameManager::GameArray::const_iterator lIt	= lGames.begin();
		GameManager::GameArray::const_iterator lEnd	= lGames.end();

		// add all games
		for( ; lIt!=lEnd; ++lIt )
			m_ui->comboBox_2->addItem(lIt.key(),lIt.key());
	}
	else
	{
		m_ui->comboBox_2->addItem("None",0);
	}
}

Startupdlg::~Startupdlg()
{
	delete m_ui;
}

void Startupdlg::changeEvent(QEvent *e)
{
	QDialog::changeEvent(e);
	switch (e->type()) {
	case QEvent::LanguageChange:
		m_ui->retranslateUi(this);
		break;
	default:
		break;
	}
}

void Startupdlg::accept( void )
{
	// setup the environment
	// based on the user's
	// selections

	gEnv->mFullscreen = m_ui->checkBox->isChecked();

	// get the currently selected resolution
	QString lRes = m_ui->comboBox->currentText();
	// split it
	QStringList lResList = lRes.split("x");

	if( lResList.size() == 2 )
	{
		// set the width and height
		gEnv->mWWidth = lResList[0].toInt();
		gEnv->mWHeight = lResList[1].toInt();
	}

	// get selected game
	gGames->setGame(m_ui->comboBox_2->currentText());

	// return
	QDialog::accept();
}
