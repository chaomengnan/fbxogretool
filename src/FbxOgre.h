#ifndef FBX_OGRE_H
#define FBX_OGRE_H

#include <fbxsdk.h>
#include <Ogre.h>
#include <map>

namespace FbxOgre
{

	class FbxSdk
	{
	private:
		FbxSdk() {}

	public:
		static bool initialize(const char* szSceneName = "fbxogre_scene");
		static void destroy();
        static void clearScene();
		static bool loadScene(const char* szFileName);
		static bool saveScene(const char* szFileName, bool bEmbedMedia = true);

		static FbxManager* getSdkManager() { return m_pSdkManager; }
		static FbxScene* getScene() { return m_pScene; }

	private:
		static FbxManager* m_pSdkManager;
		static FbxScene* m_pScene;
	};

    struct VERTEX_DATA{
        Ogre::Vector3 position;
        Ogre::Vector3 normal;
        Ogre::Vector2 uv;
    };

    struct BONE_DATA{
        FbxSkeleton* pSkeletonAttr;
        FbxNode* pNode;
        Ogre::Bone* pBone;
    };

    typedef std::vector<VERTEX_DATA> VERTEX_LIST;
    typedef std::vector<unsigned long> INDEX_LIST;
    typedef std::map<std::string, BONE_DATA> BONE_MAP;

    class Util
    {
    private:
        Util() {}

    public:
        //static FbxDouble3 OgreVector3ToFbxDouble3(const Ogre::Vector3& vecInput);

        static bool enumMeshVertex(const Ogre::String& strMesh, const Ogre::String& strGroupName, VERTEX_LIST& vecPos, INDEX_LIST& vecIndex);
        static bool createBoneMap(const Ogre::SkeletonPtr& pSkeleton, BONE_MAP& boneMap);
        static bool enumSkeleton(const char* szSkeleton, const char* szGroupName);
        static bool exportMesh(const char* szMesh, const char* szGroupName, const char* szFileName);
    };

}//namespace FbxOgre

#endif