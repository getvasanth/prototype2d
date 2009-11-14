# -------------------------------------------------
# Project created by QtCreator 2009-07-11T23:01:10
# -------------------------------------------------
QT += opengl
TARGET = bin/Prototype2D
TEMPLATE = app
SOURCES += main.cpp \
	mainwindow.cpp \
	world.cpp \
	texture.cpp \
	canvas.cpp \
	actor.cpp \
	texturemanager.cpp \
	startupdlg.cpp \
	games/pyp/background.cpp \
	games/pyp/pyp.cpp \
	games/pyp/block.cpp \
	games/pyp/toolpallette.cpp \
	games/pyp/pea.cpp \
	gamemanager.cpp \
	games/pyp/tri.cpp \
	games/template/template.cpp \
	actorjoint.cpp \
	games/force/force.cpp \
	games/tail/tail.cpp \
	games/anim/anim.cpp \
	games/jelly/jelly.cpp \
	games/jelly/jellyactor.cpp \
	games/autumn/autumn.cpp
HEADERS += mainwindow.h \
	world.h \
	texture.h \
	canvas.h \
	actor.h \
	texturemanager.h \
	utils.h \
	env.h \
	startupdlg.h \
	types.h \
	defines.h \
	games/pyp/background.h \
	igame.h \
	games/pyp/pyp.h \
	games/pyp/block.h \
	games/pyp/toolpallette.h \
	games/pyp/pea.h \
	gamemanager.h \
	games/pyp/tri.h \
	games/template/template.h \
	actorjoint.h \
        games/force/force.h \
	games/tail/tail.h \
	games/anim/anim.h \
	games/jelly/jelly.h \
	games/jelly/jellyactor.h \
	games/autumn/autumn.h
FORMS += mainwindow.ui \
	startupdlg.ui
LIBS += -L"Box2D"
LIBS += -lBox2D
# Disable qDebug() and qWarning() output
# DEFINES += QT_NO_DEBUG_OUTPUT
