#ifndef JELLYACTOR_H
#define JELLYACTOR_H

#include "actor.h"
#include <QtCore/QString>

namespace GL
{
	class World;
}

namespace Jelly {

class JellyActor : public GL::Actor
{
public:
	JellyActor(const QString &pName,GL::World *pWorld);
	virtual ~JellyActor();

	virtual void applyPhysX(void);

	virtual void addSpringForceUp(JellyActor *pActor);
	virtual void addSpringForceDown(JellyActor *pActor);

protected:
	virtual void addSpringForce(JellyActor *pActor, bool pUpDown);
};

}

#endif // JELLYACTOR_H
