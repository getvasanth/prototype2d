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
#include "texturemanager.h"
#include "texture.h"
#include "env.h"

#include <QtCore/QFile>
#include <QtCore/QDebug>

using namespace GL;
using namespace Sys;

static Env *gEnv = &Env::getInstance();

TextureManager::TextureManager()
{
	mEmptyTexture = new Texture;
}

TextureManager::~TextureManager()
{
	if( !mEmptyTexture->drop() )
	{
		qDebug() << "Warning: [Texture] EmptyTexture was not released properly";
	}
	else
	{
		qDebug() << "Success: [Texture] EmptyTexture dropped";
	}
	mEmptyTexture = 0;
}

Texture *TextureManager::find( const QString &pFileName )
{
	const QString lFileName = QString(gEnv->mBasePath) + pFileName;

	if( !QFile::exists( lFileName ) )
		return mEmptyTexture;

	// try to find it inside our hash map
	TextureArray::const_iterator lIt = mTextures.find(lFileName);
	if( lIt != mTextures.end() )
	{
		return lIt.value();
	}

	// try to load and insert
	Texture *lTexture = new Texture();
	if( !lTexture->load( lFileName ) )
	{
		// failed to load delete
		lTexture->drop();
		lTexture = 0;
	}
	else
	{
		// insert it
		mTextures.insert(lFileName,lTexture);
		return lTexture;
	}

	// return empty texture
	return mEmptyTexture;
}

void TextureManager::removeAll( void )
{
	if( mTextures.empty() )
		return;

	TextureArray::iterator lIt	= mTextures.begin();
	TextureArray::iterator lEnd	= mTextures.end();

	for( ; lIt!=lEnd; ++lIt )
	{
		// TODO: just spit warning, but we should
		// really clean it up ...
		if( !lIt.value()->drop() )
		{
			qDebug() << "Warning: [Texture] " << lIt.key() << " was not released properly";
		}
		else
		{
			qDebug() << "Success: [Texture] " << lIt.key() << " dropped";
		}
	}

	mTextures.clear();
}
