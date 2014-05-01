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

	void importSkeleton(Ogre::DataStreamPtr& stream, Ogre::Skeleton* pDest);// û��
protected:
	void readAnimation(Ogre::DataStreamPtr& stream, Ogre::Skeleton* pSkel);	// û��
	void readAnimationTrack(Ogre::DataStreamPtr& stream, Ogre::Animation* anim, Ogre::Skeleton* pSkel);	// ����
};

}

#endif