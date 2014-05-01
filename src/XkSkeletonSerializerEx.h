#ifndef XK_SKELETONSERIALIZEREX_H
#define XK_SKELETONSERIALIZEREX_H

#include <OgreSkeletonSerializer.h>

namespace Xk
{

class SkeletonSerializerEx : public Ogre::SkeletonSerializer
{
public:
	SkeletonSerializerEx(void);
	~SkeletonSerializerEx(void);

	void importSkeleton(Ogre::DataStreamPtr& stream, Ogre::Skeleton* pDest);// 没改
protected:
	void readAnimation(Ogre::DataStreamPtr& stream, Ogre::Skeleton* pSkel);	// 没改
	void readAnimationTrack(Ogre::DataStreamPtr& stream, Ogre::Animation* anim, Ogre::Skeleton* pSkel);	// 改了
};

}

#endif