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
#include "gamemanager.h"
#include "igame.h"

#include <QtCore/QDebug>

using namespace Game;
using namespace Interface;

GameManager::GameManager() : mGame(0)
{
}

GameManager::~GameManager()
{
}

void GameManager::setGame(const QString &pName)
{
	if( pName.isEmpty() )
		return;

	qDebug() << "Info: [GameManager] Selecting " << pName;
	mGame = findGame(pName);
}

IGame *GameManager::getGame( void ) const
{
	// return the game if it was set
	if( mGame )
		return mGame;

	// no game set, so return the first one
	if( !mGames.empty() )
		return mGames.begin().value();

	// no games whatsoever
	qDebug() << "Warning: [GameManager] No Games Found";
	return 0;
}

IGame *GameManager::findGame(const QString &pName)
{
	if( pName.isEmpty() || mGames.empty() )
		return 0;

	GameArray::const_iterator lIt = mGames.find(pName);

	if( lIt!= mGames.end() )
		return lIt.value();

	return 0;
}

void GameManager::addGame(const QString &pName, IGame *pGame)
{
	if( pName.isEmpty() || !pGame )
		return;

	if( !findGame(pName) )
		mGames.insert(pName,pGame);
}

void GameManager::removeAll( void )
{
	if( mGames.empty() )
		return;

	GameArray::iterator lIt		= mGames.begin();
	GameArray::iterator lEnd	= mGames.end();

	// release all games
	for( ; lIt!=lEnd; ++lIt )
		delete lIt.value();

	mGames.clear();
}
