#include "FbxOgre.h"
#include <XkDebugStr.h>

FbxManager* FbxOgre::FbxSdk::m_pSdkManager = 0;
FbxScene* FbxOgre::FbxSdk::m_pScene = 0;

namespace FbxOgre
{
	bool FbxSdk::initialize(const char* szSceneName)
	{
		m_pSdkManager = FbxManager::Create();
		if (!m_pSdkManager) {
			printf("failed to create fbx sdk manager\n");
			return false;
		}

		FbxIOSettings* ios = FbxIOSettings::Create(m_pSdkManager, IOSROOT);
		m_pSdkManager->SetIOSettings(ios);

		m_pScene = FbxScene::Create(m_pSdkManager, szSceneName);
		if (!m_pScene) {
			printf("Error to create scene %s", szSceneName);
			return false;
		}

		return true;
	}

	void FbxSdk::destroy()
	{
		if (m_pSdkManager)
			m_pSdkManager->Destroy();
	}

    void FbxSdk::clearScene()
    {
        m_pScene->Clear();
    }

	bool FbxSdk::loadScene(const char* szFileName)
	{
		int lFileMajor, lFileMinor, lFileRevision;
		int lSDKMajor, lSDKMinor, lSDKRevision;
		//int lFileFormat = -1;
		int i, lAnimStackCount;
		bool lStatus;
		char lPassword[1024];

		// Get the file version number generate by the FBX SDK.
		FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

		// Create an importer.
		FbxImporter* lImporter = FbxImporter::Create(m_pSdkManager, "");

		// Initialize the importer by providing a filename.
		const bool lImportStatus = lImporter->Initialize(szFileName, -1, m_pSdkManager->GetIOSettings());
		lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

		if (!lImportStatus)
		{
			FbxString error = lImporter->GetStatus().GetErrorString();
			FBXSDK_printf("Call to FbxImporter::Initialize() failed.\n");
			FBXSDK_printf("Error returned: %s\n\n", error.Buffer());

			if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
			{
				FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
				FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", szFileName, lFileMajor, lFileMinor, lFileRevision);
			}

			return false;
		}

		FBXSDK_printf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

		if (lImporter->IsFBX())
		{
			FBXSDK_printf("FBX file format version for file '%s' is %d.%d.%d\n\n", szFileName, lFileMajor, lFileMinor, lFileRevision);

			// From this point, it is possible to access animation stack information without
			// the expense of loading the entire file.

			FBXSDK_printf("Animation Stack Information\n");

			lAnimStackCount = lImporter->GetAnimStackCount();

			FBXSDK_printf("    Number of Animation Stacks: %d\n", lAnimStackCount);
			FBXSDK_printf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
			FBXSDK_printf("\n");

			for (i = 0; i < lAnimStackCount; i++)
			{
				FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

				FBXSDK_printf("    Animation Stack %d\n", i);
				FBXSDK_printf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
				FBXSDK_printf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

				// Change the value of the import name if the animation stack should be imported 
				// under a different name.
				FBXSDK_printf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

				// Set the value of the import state to false if the animation stack should be not
				// be imported. 
				FBXSDK_printf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
				FBXSDK_printf("\n");
			}

			// Set the import states. By default, the import states are always set to 
			// true. The code below shows how to change these states.
			(*(m_pSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_MATERIAL, true);
			(*(m_pSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_TEXTURE, true);
			(*(m_pSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_LINK, true);
			(*(m_pSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_SHAPE, true);
			(*(m_pSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_GOBO, true);
			(*(m_pSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_ANIMATION, true);
			(*(m_pSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
		}

		// Import the scene.
		lStatus = lImporter->Import(m_pScene);

		if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
		{
			FBXSDK_printf("Please enter password: ");

			lPassword[0] = '\0';

			FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
				scanf("%s", lPassword);
			FBXSDK_CRT_SECURE_NO_WARNING_END

				FbxString lString(lPassword);

			(*(m_pSdkManager->GetIOSettings())).SetStringProp(IMP_FBX_PASSWORD, lString);
			(*(m_pSdkManager->GetIOSettings())).SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

			lStatus = lImporter->Import(m_pScene);

			if (lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
			{
				FBXSDK_printf("\nPassword is wrong, import aborted.\n");
			}
		}

		// Destroy the importer.
		lImporter->Destroy();

		return lStatus;
	}

	bool FbxSdk::saveScene(const char* szFileName, bool bEmbedMedia)
	{
		int lMajor, lMinor, lRevision;
		bool lStatus = true;

		// Create an exporter.
		FbxExporter* lExporter = FbxExporter::Create(m_pSdkManager, "");

		// Set the export states. By default, the export states are always set to 
		// true except for the option eEXPORT_TEXTURE_AS_EMBEDDED. The code below 
		// shows how to change these states.
		(*(m_pSdkManager->GetIOSettings())).SetBoolProp(EXP_FBX_MATERIAL, true);
		(*(m_pSdkManager->GetIOSettings())).SetBoolProp(EXP_FBX_TEXTURE, true);
		(*(m_pSdkManager->GetIOSettings())).SetBoolProp(EXP_FBX_EMBEDDED, bEmbedMedia);
		(*(m_pSdkManager->GetIOSettings())).SetBoolProp(EXP_FBX_SHAPE, true);
		(*(m_pSdkManager->GetIOSettings())).SetBoolProp(EXP_FBX_GOBO, true);
		(*(m_pSdkManager->GetIOSettings())).SetBoolProp(EXP_FBX_ANIMATION, true);
		(*(m_pSdkManager->GetIOSettings())).SetBoolProp(EXP_FBX_GLOBAL_SETTINGS, true);

		// Initialize the exporter by providing a filename.
		if (lExporter->Initialize(szFileName, -1, m_pSdkManager->GetIOSettings()) == false)
		{
			FBXSDK_printf("Call to FbxExporter::Initialize() failed.\n");
			FBXSDK_printf("Error returned: %s\n\n", lExporter->GetStatus().GetErrorString());
			return false;
		}

		FbxManager::GetFileFormatVersion(lMajor, lMinor, lRevision);
		FBXSDK_printf("FBX file format version %d.%d.%d\n\n", lMajor, lMinor, lRevision);

		// Export the scene.
		lStatus = lExporter->Export(m_pScene);

		// Destroy the exporter.
		lExporter->Destroy();
		return lStatus;
	}

    bool Util::enumMeshVertex(const Ogre::String& strMesh, const Ogre::String& strGroupName, VERTEX_LIST& vecPos, INDEX_LIST& vecIndex)
    {
        Ogre::MeshPtr pMesh = Ogre::MeshManager::getSingleton().load(strMesh, strGroupName);

        int nVertexCount = 0;
        int nIndexCount = 0;
        DBGSTRING("NumSubMesh: %d", pMesh->getNumSubMeshes());

        for(int i=0; i<pMesh->getNumSubMeshes(); i++) {
            Ogre::SubMesh* pSubMesh = pMesh->getSubMesh(i);
            if(pSubMesh->useSharedVertices) {
                DBGSTRING("useSharedVertices");
                nVertexCount += pMesh->sharedVertexData->vertexCount;
            }else{
                nVertexCount += pSubMesh->vertexData->vertexCount;
            }
            nIndexCount += pSubMesh->indexData->indexCount;
            DBGSTRING("nIndexCount: %d", pSubMesh->indexData->indexCount);
        }

        DBGSTRING("nVertexCount: %d, nIndexCount: %d", nVertexCount, nIndexCount);

        for(int i=0; i<pMesh->getNumSubMeshes(); ++i) {
            Ogre::SubMesh* pSubMesh = pMesh->getSubMesh(i);

            Ogre::VertexData* pVertexData = pSubMesh->useSharedVertices ? pMesh->sharedVertexData : pSubMesh->vertexData;
            const Ogre::VertexElement* posElem = pVertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);
            const Ogre::VertexElement* uvElem = pVertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_TEXTURE_COORDINATES);
            //const Ogre::VertexElement* normalElem = pVertexData->vertexDeclaration->findElementBySemantic(Ogre::VES_NORMAL);

            Ogre::HardwareVertexBufferSharedPtr vbuf = pVertexData->vertexBufferBinding->getBuffer(posElem->getSource());
            Ogre::HardwareVertexBufferSharedPtr uvbuf = pVertexData->vertexBufferBinding->getBuffer(uvElem->getSource());
        
            unsigned char* pVertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
        
            float* pReal;
            float* pUVReal;
            //读取顶点
            for(size_t j=0; j<pVertexData->vertexCount; ++j, pVertex += vbuf->getVertexSize()) {
                VERTEX_DATA vd;

                posElem->baseVertexPointerToElement(pVertex, &pReal);
                Ogre::Vector3 pos(pReal[0], pReal[1], pReal[2]);
                vd.position = pos;

                vecPos.push_back(vd);
            }
            vbuf->unlock();

            //读取UV
            unsigned char* pUV = static_cast<unsigned char*>(uvbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            for(size_t j=0; j<pVertexData->vertexCount; ++j, pUV += uvbuf->getVertexSize()) {
                uvElem->baseVertexPointerToElement(pUV, &pUVReal);
                vecPos[j].uv = Ogre::Vector2(pUVReal[0], pUVReal[1]);
            }
            uvbuf->unlock();

            Ogre::IndexData* pIndexData = pSubMesh->indexData;
            Ogre::HardwareIndexBufferSharedPtr ibuf = pIndexData->indexBuffer;
            bool b32Bit = ibuf->getType() == Ogre::HardwareIndexBuffer::IT_32BIT;

            unsigned char* pIndex = static_cast<unsigned char*>(ibuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
            int nIndexSize = b32Bit ? 4 : 2;

            for(size_t k=0; k<pIndexData->indexCount; k++) {
                unsigned long ulIndex = 0;
                if(b32Bit) {
                    memcpy(&ulIndex, pIndex, 4);
                }else{
                    unsigned short usIndex = 0;
                    memcpy(&usIndex, pIndex, 2);
                    ulIndex = static_cast<unsigned long>(usIndex);
                }

                vecIndex.push_back(ulIndex);
                pIndex += nIndexSize;
            }
            ibuf->unlock();
        }

        return vecPos.size() > 0;
    }

    bool Util::createBoneMap(const Ogre::SkeletonPtr& pSkeleton, BONE_MAP& boneMap)
    {
        if(pSkeleton.isNull()) {
            return false;
        }

        DBGSTRING("Name: %s, BoneCount: %d", pSkeleton->getName().c_str(), pSkeleton->getNumBones());
        FbxScene* pScene = FbxSdk::getScene();

        std::vector<std::string> vecRootName;
        Ogre::Skeleton::BoneIterator rootIter = pSkeleton->getRootBoneIterator();
        while(rootIter.hasMoreElements()) {
            Ogre::Bone* pBone = rootIter.getNext();
            DBGSTRING("RootBone: %s", pBone->getName().c_str());

            FbxSkeleton* pSkeletonAttr = FbxSkeleton::Create(pScene, pBone->getName().c_str());
            pSkeletonAttr->SetSkeletonType(FbxSkeleton::eRoot);
            FbxNode* pNode = FbxNode::Create(pScene, pBone->getName().c_str());
            pNode->SetNodeAttribute(pSkeletonAttr);
            vecRootName.push_back(pBone->getName());

            //测试使用Derived
            Ogre::Vector3 bonePos = pBone->getPosition();
            Ogre::Vector3 boneScale = pBone->getScale();
            Ogre::Quaternion quatRot = pBone->getOrientation();

            Ogre::Matrix3 mat3;
            quatRot.ToRotationMatrix(mat3);
            Ogre::Radian angleP, angleY, angleR;
            mat3.ToEulerAnglesXYZ(angleY, angleP, angleR);

            Ogre::Vector3 boneRotation(angleY.valueDegrees(),
                angleP.valueDegrees(),
                angleR.valueDegrees());

            FbxDouble3 pos(bonePos.x, bonePos.y, bonePos.z);
            FbxDouble3 scale(boneScale.x, boneScale.y, boneScale.z);
            FbxDouble3 rotation(boneRotation.x, boneRotation.y, boneRotation.z);

            pNode->LclTranslation.Set(pos);
            pNode->LclScaling.Set(scale);
            pNode->LclRotation.Set(rotation);

            pScene->GetRootNode()->AddChild(pNode);
            BONE_DATA bd;
            bd.pSkeletonAttr = pSkeletonAttr;
            bd.pNode = pNode;
            bd.pBone = pBone;
            boneMap.insert(BONE_MAP::value_type(pBone->getName(), bd));
        }

        //创建所有Bone，除了Root
        for(int i=0; i<pSkeleton->getNumBones(); i++) {
            Ogre::Bone* pBone = pSkeleton->getBone(i);
            
            std::vector<std::string>::iterator it = std::find(vecRootName.begin(), vecRootName.end(), pBone->getName());
            if(it == vecRootName.end()) {
                //不是Root，创建
                DBGSTRING("Not root %s", pBone->getName().c_str());
                FbxSkeleton* pSkeletonAttr = FbxSkeleton::Create(pScene, pBone->getName().c_str());
                pSkeletonAttr->SetSkeletonType(FbxSkeleton::eLimbNode);
                FbxNode* pNode = FbxNode::Create(pScene, pBone->getName().c_str());
                pNode->SetNodeAttribute(pSkeletonAttr);

                Ogre::Vector3 bonePos = pBone->getPosition();
                Ogre::Vector3 boneScale = pBone->getScale();
                Ogre::Quaternion quatRot = pBone->getOrientation();

                Ogre::Matrix3 mat3;
                quatRot.ToRotationMatrix(mat3);
                Ogre::Radian angleP, angleY, angleR;
                mat3.ToEulerAnglesXYZ(angleY, angleP, angleR);

                Ogre::Vector3 boneRotation(angleY.valueDegrees(),
                    angleP.valueDegrees(),
                    angleR.valueDegrees());

                FbxDouble3 pos(bonePos.x, bonePos.y, bonePos.z);
                FbxDouble3 scale(boneScale.x, boneScale.y, boneScale.z);
                FbxDouble3 rotation(boneRotation.x, boneRotation.y, boneRotation.z);

                pNode->LclTranslation.Set(pos);
                pNode->LclScaling.Set(scale);
                pNode->LclRotation.Set(rotation);

                BONE_DATA bd;
                bd.pSkeletonAttr = pSkeletonAttr;
                bd.pNode = pNode;
                bd.pBone = pBone;
                boneMap.insert(BONE_MAP::value_type(pBone->getName(), bd));
            }
        }
        
        DBGSTRING("boneMap size: %d", boneMap.size());

        for(int i=0; i<pSkeleton->getNumBones(); i++) {
            Ogre::Bone* pBone = pSkeleton->getBone(i);
            Ogre::Node* pParentNode = pBone->getParent();
            if(pParentNode != NULL) {
                DBGSTRING("Bone: %s Parent: %s", pBone->getName().c_str(),
                    pParentNode->getName().c_str());
                BONE_MAP::const_iterator itParent = boneMap.find(pParentNode->getName());
                BONE_MAP::const_iterator itCurr = boneMap.find(pBone->getName());

                if(itParent == boneMap.end() || itCurr == boneMap.end()) continue;

                FbxNode* pFbxParentNode = itParent->second.pNode;
                FbxNode* pFbxChildNode = itCurr->second.pNode;

                pFbxParentNode->AddChild(pFbxChildNode);
            }
        }

        return boneMap.size() > 0;
    }

    bool Util::enumSkeleton(const char* szSkeleton, const char* szGroupName)
    {
        Ogre::SkeletonPtr pSkeleton = Ogre::SkeletonManager::getSingleton().load(szSkeleton, szGroupName).staticCast<Ogre::Skeleton>();

        if(pSkeleton.isNull()) {
            DBGSTRING("enumSkeleton load %s Failed", szSkeleton);
            return false;
        }

        DBGSTRING("Name: %s, BoneCount: %d", pSkeleton->getName().c_str(), pSkeleton->getNumBones());

        BONE_MAP boneMap;
        createBoneMap(pSkeleton, boneMap);

        int nAnimCount = pSkeleton->getNumAnimations();
        DBGSTRING("nAnimCount: %d", nAnimCount);
        for(int i=0; i<nAnimCount; i++) {
            
            Ogre::Animation* pAnim = pSkeleton->getAnimation(i);
            DBGSTRING("Animation: %s", pAnim->getName().c_str());
            DBGSTRING("Length: %f", pAnim->getLength());
            DBGSTRING("NumericTracks: %d", pAnim->getNumNumericTracks());
            DBGSTRING("VertexTracks: %d", pAnim->getNumVertexTracks());
            DBGSTRING("NodeTracks: %d", pAnim->getNumNodeTracks());

            FbxScene* pScene = FbxSdk::getScene();
            FbxAnimStack* pAnimStack = FbxAnimStack::Create(pScene, ("AnimationStack" + Ogre::StringConverter::toString(i)).c_str());
            FbxAnimLayer* pAnimLayer = FbxAnimLayer::Create(pScene, ("Base Layer" + Ogre::StringConverter::toString(i)).c_str() );
            pAnimStack->AddMember(pAnimLayer);

            Ogre::Animation::NodeTrackIterator iter = pAnim->getNodeTrackIterator();
            while(iter.hasMoreElements()) {

                Ogre::NodeAnimationTrack* track = iter.getNext();
                if(track == NULL) continue;
                DBGSTRING("track: %d, Node: %s", track->getHandle(), track->getAssociatedNode()->getName().c_str());

                Ogre::String strAssName = track->getAssociatedNode()->getName();
                /*
                if( strAssName != "Bip01_R_Clavicle" &&
                    strAssName != "Bip01_L_Clavicle" &&
                    strAssName != "Bip01_R_UpperArm" &&
                    strAssName != "Bip01_L_UpperArm" &&
                    strAssName != "Bip01_R_Forearm" &&
                    strAssName != "Bip01_L_Forearm" &&
                    strAssName != "Bip01_L_Hand" &&
                    strAssName != "Bip01_R_Hand" &&
                    strAssName != "Bip01_Head" &&
                    strAssName != "Bip01_Neck" &&
                    strAssName != "Bip01_L_Calf" &&
                    strAssName != "Bip01_R_Calf" &&
                    strAssName != "Bip01_R_Foot" &&
                    strAssName != "Bip01_L_Foot" &&
                    strAssName != "Bip01_R_Toe0" &&
                    strAssName != "Bip01_L_Toe0" &&
                    strAssName != "Bip01_Spine" &&
                    strAssName != "Bip01_Spine1" &&
                    strAssName != "Bone08" &&
                    strAssName.find("Finger") == std::string::npos
                    ) {
                    continue;
                }
                */
                //查找到此Track关联的节点
                BONE_MAP::iterator it = boneMap.find(track->getAssociatedNode()->getName());
                if(it == boneMap.end()) continue;

                BONE_DATA boneData = it->second;
                FbxAnimCurve* pRotateCurveX = boneData.pNode->LclRotation.GetCurve(pAnimLayer,
                    FBXSDK_CURVENODE_COMPONENT_X, true);
                FbxAnimCurve* pRotateCurveY = boneData.pNode->LclRotation.GetCurve(pAnimLayer,
                    FBXSDK_CURVENODE_COMPONENT_Y, true);
                FbxAnimCurve* pRotateCurveZ = boneData.pNode->LclRotation.GetCurve(pAnimLayer,
                    FBXSDK_CURVENODE_COMPONENT_Z, true);

                FbxAnimCurve* pTranslateX = boneData.pNode->LclTranslation.GetCurve(pAnimLayer,
                    FBXSDK_CURVENODE_COMPONENT_X, true);
                FbxAnimCurve* pTranslateY = boneData.pNode->LclTranslation.GetCurve(pAnimLayer,
                    FBXSDK_CURVENODE_COMPONENT_Y, true);
                FbxAnimCurve* pTranslateZ = boneData.pNode->LclTranslation.GetCurve(pAnimLayer,
                    FBXSDK_CURVENODE_COMPONENT_Z, true);

                int nKeyframe = track->getNumKeyFrames();
                
                DBGSTRING("Keyframe count: %d", nKeyframe);

                for(int k=0; k<nKeyframe; k++) {
                    FbxAnimCurveDef::EInterpolationType nInterMode = k == nKeyframe ? FbxAnimCurveDef::eInterpolationConstant : FbxAnimCurveDef::eInterpolationConstant;

                    FbxTime lTime;
                    Ogre::TransformKeyFrame* transKeyframe = track->getNodeKeyFrame(k);
                    /*
                    DBGSTRING("Time: %f, KeyFrame Rotation(%s), Translate(%s), Scale(%s)",
                        transKeyframe->getTime(),
                        Ogre::StringConverter::toString(transKeyframe->getRotation()).c_str(),
                        Ogre::StringConverter::toString(transKeyframe->getTranslate()).c_str(),
                        Ogre::StringConverter::toString(transKeyframe->getScale()).c_str());
                        */
                    Ogre::Quaternion quatKeyRot = transKeyframe->getRotation();
                    Ogre::Vector3 vecKeyTrans = transKeyframe->getTranslate();
                    Ogre::Quaternion quatBoneRot = boneData.pBone->getOrientation();
                    Ogre::Vector3 vecBonePos = boneData.pBone->getPosition();

                    Ogre::Quaternion quatRetRot = quatKeyRot * quatBoneRot;
                    Ogre::Vector3 vecRetPos = vecBonePos + vecKeyTrans;

                    Ogre::Matrix3 mat3;
                    quatRetRot.ToRotationMatrix(mat3);
                    Ogre::Radian angleP, angleY, angleR;
                    mat3.ToEulerAnglesXYZ(angleY, angleP, angleR);
                    DBGSTRING("angleY: %f, angleP: %f, angleR: %f", angleY.valueDegrees(), angleP.valueDegrees(), angleR.valueDegrees());
                    //boneData.pNode->EvaluateGlobalTransform(0);

                    float fTime = transKeyframe->getTime();
                    pRotateCurveX->KeyModifyBegin();
                    pRotateCurveY->KeyModifyBegin();
                    pRotateCurveZ->KeyModifyBegin();
                    pTranslateX->KeyModifyBegin();
                    pTranslateY->KeyModifyBegin();
                    pTranslateZ->KeyModifyBegin();

                    lTime.SetSecondDouble(fTime);
                    int nKeyIndex = pRotateCurveX->KeyAdd(lTime);
                    pRotateCurveX->KeySetValue(nKeyIndex, angleY.valueDegrees());
                    pRotateCurveX->KeySetInterpolation(nKeyIndex, nInterMode);

                    nKeyIndex = pRotateCurveY->KeyAdd(lTime);
                    pRotateCurveY->KeySetValue(nKeyIndex, angleP.valueDegrees());
                    pRotateCurveY->KeySetInterpolation(nKeyIndex, nInterMode);

                    nKeyIndex = pRotateCurveZ->KeyAdd(lTime);
                    pRotateCurveZ->KeySetValue(nKeyIndex, angleR.valueDegrees());
                    pRotateCurveZ->KeySetInterpolation(nKeyIndex, nInterMode);
                    
                    int nKeyIndexX = pTranslateX->KeyAdd(lTime);
                    pTranslateX->KeySetValue(nKeyIndexX, vecRetPos.x);
                    pTranslateX->KeySetInterpolation(nKeyIndexX, nInterMode);

                    int nKeyIndexY = pTranslateY->KeyAdd(lTime);
                    pTranslateY->KeySetValue(nKeyIndexY, vecRetPos.y);
                    pTranslateY->KeySetInterpolation(nKeyIndexY, nInterMode);

                    int nKeyIndexZ = pTranslateZ->KeyAdd(lTime);
                    pTranslateZ->KeySetValue(nKeyIndexZ, vecRetPos.z);
                    pTranslateZ->KeySetInterpolation(nKeyIndexZ, nInterMode);
                    
                    pRotateCurveX->KeyModifyEnd();
                    pRotateCurveY->KeyModifyEnd();
                    pRotateCurveZ->KeyModifyEnd();
                    pTranslateX->KeyModifyEnd();
                    pTranslateY->KeyModifyEnd();
                    pTranslateZ->KeyModifyEnd();
                }
                //break;
            }
        }

        //FbxSdk::saveScene("skeleton.fbx");

        return true;
    }

    bool Util::exportMesh(const char* szMesh, const char* szGroupName, const char* szFileName)
    {
        VERTEX_LIST vecPos;
        INDEX_LIST vecIndex;

        enumMeshVertex(szMesh, szGroupName, vecPos, vecIndex);

        if(vecPos.size() <= 0){
            DBGSTRING("获取Mesh顶点数据失败");
            return false;
        }

        FbxScene* pScene = FbxOgre::FbxSdk::getScene();
        pScene->Clear();

        FbxMesh* pMesh = FbxMesh::Create(pScene, szMesh);

        pMesh->InitControlPoints(vecPos.size());
        FbxVector4* pVertex = pMesh->GetControlPoints();
    
        for(int i=0; i<vecPos.size(); i++) {
            Ogre::Vector3 pos = vecPos[i].position;
            pVertex[i] = FbxVector4(pos.x, pos.y, pos.z);
        }

        //创建三角形
        int nTriangles = vecIndex.size() / 3;
    
        for(int i=0; i<nTriangles; i++) {
            int nIndex = i*3;
            pMesh->BeginPolygon();
            pMesh->AddPolygon(vecIndex[nIndex]);
            pMesh->AddPolygon(vecIndex[nIndex+1]);
            pMesh->AddPolygon(vecIndex[nIndex+2]);

            pMesh->EndPolygon();
        }

        FbxNode* pNode = FbxNode::Create(pScene, szMesh);

        pNode->SetNodeAttribute(pMesh);

        FbxGeometryElementUV* pUVElement = pMesh->CreateElementUV("uvset");
        pUVElement->SetMappingMode(FbxGeometryElement::eByControlPoint);
        pUVElement->SetReferenceMode(FbxGeometryElement::eDirect);
        pUVElement->GetDirectArray().Resize(vecPos.size());
        for(int i=0; i<vecPos.size(); i++) {
            FbxVector2 fbxuv(vecPos[i].uv.x, 1.0f - vecPos[i].uv.y);
            pUVElement->GetDirectArray().SetAt(i, fbxuv);
        }

        FbxNode* pRootNode = pScene->GetRootNode();
        pRootNode->AddChild(pNode);

        return FbxOgre::FbxSdk::saveScene(szFileName);
    }

}//namespace FbxOgre