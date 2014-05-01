#ifndef XK_SKELETONEX_H
#define XK_SKELETONEX_H

#include <OgreSkeleton.h>

namespace Xk
{

class SkeletonEx : public Ogre::Skeleton
{
public:
 	SkeletonEx(Ogre::ResourceManager* creator, const Ogre::String& name, Ogre::ResourceHandle handle,
 		const Ogre::String& group, bool isManual = false, Ogre::ManualResourceLoader* loader = 0);

protected:
	/** @copydoc Resource::loadImpl
	*/
	void loadImpl(void);
};

}//namespace Xk

#endif