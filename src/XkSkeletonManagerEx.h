#ifndef XK_SKELETONMANAGEREX_H
#define XK_SKELETONMANAGEREX_H

#include <OgreSkeletonManager.h>

namespace Xk
{

class SkeletonManagerEx	: public Ogre::SkeletonManager
{
public:
	SkeletonManagerEx(void);
protected:

	/// @copydoc ResourceManager::createImpl
	Ogre::Resource* createImpl(const Ogre::String& name, Ogre::ResourceHandle handle, 
		const Ogre::String& group, bool isManual, Ogre::ManualResourceLoader* loader, 
		const Ogre::NameValuePairList* createParams);

};

}//namespace Xk

#endif