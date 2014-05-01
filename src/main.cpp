#include <QApplication>
#include "MainWindow.h"
#include "FbxOgre.h"
#include "XkOgreGraphic.h"
#include "XkDebugStr.h"

void createBoneAndAnimation();

int main(int argc, char** argv)
{
    QApplication app(argc, argv);
    gGraphic().instance();
    
    FbxOgre::FbxSdk::initialize();

    MainWindow win;
    win.show();

    //createBoneAndAnimation();
    app.exec();

    FbxOgre::FbxSdk::destroy();
    gGraphic().destroy();

    return 0;
}

void createBoneAndAnimation()
{
    FbxScene* pScene = FbxOgre::FbxSdk::getScene();
    FbxSkeleton* pRootSkeletonAttr = FbxSkeleton::Create(pScene, "RootBone");
    pRootSkeletonAttr->SetSkeletonType(FbxSkeleton::eRoot);
    FbxNode* pRootNode = FbxNode::Create(pScene, "RootBoneNode");

    pRootNode->SetNodeAttribute(pRootSkeletonAttr);

    pRootNode->LclRotation.Set(FbxDouble3(0, 0, 0));
    pRootNode->LclTranslation.Set(FbxDouble3(20, 0, 0));

    FbxSkeleton* pChildSkeletonAttr = FbxSkeleton::Create(pScene, "ChildBone");
    pChildSkeletonAttr->SetSkeletonType(FbxSkeleton::eLimbNode);
    FbxNode* pChildNode = FbxNode::Create(pScene, "ChildBoneNode");

    pChildNode->SetNodeAttribute(pChildSkeletonAttr);
    pChildNode->LclRotation.Set(FbxDouble3(0, 0, 0));
    pChildNode->LclTranslation.Set(FbxDouble3(50, 0, 0));

    FbxSkeleton* pEndSkeletonAttr = FbxSkeleton::Create(pScene, "EndBone");
    pEndSkeletonAttr->SetSkeletonType(FbxSkeleton::eLimbNode);
    FbxNode* pEndNode = FbxNode::Create(pScene, "EndBoneNode");

    pEndNode->SetNodeAttribute(pEndSkeletonAttr);
    pEndNode->LclTranslation.Set(FbxDouble3(10, 0, 0));

    pRootNode->AddChild(pChildNode);
    pChildNode->AddChild(pEndNode);

    pScene->GetRootNode()->AddChild(pRootNode);

    //创建动画数据
    FbxAnimStack* pAnimStack = FbxAnimStack::Create(pScene, "AnimationStack");
    FbxAnimLayer* pAnimLayer = FbxAnimLayer::Create(pScene, "Base Layer");
    pAnimStack->AddMember(pAnimLayer);

    FbxAnimCurve* pRotateY = pRootNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);

    pRotateY->KeyModifyBegin();

    FbxTime lTime;
    lTime.SetSecondDouble(0);
    int nKeyIndex = pRotateY->KeyAdd(lTime);
    pRotateY->KeySetValue(nKeyIndex, 0);
    pRotateY->KeySetInterpolation(nKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

    lTime.SetSecondDouble(5);
    nKeyIndex = pRotateY->KeyAdd(lTime);
    pRotateY->KeySetValue(nKeyIndex, 90);
    pRotateY->KeySetInterpolation(nKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

    pRotateY->KeyModifyEnd();

    //旋转子骨骼
    FbxAnimCurve* pChildRotateX = pChildNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
    FbxAnimCurve* pChildRotateY = pChildNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
    FbxAnimCurve* pChildRotateZ = pChildNode->LclRotation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);
    pChildRotateX->KeyModifyBegin();
    pChildRotateY->KeyModifyBegin();
    pChildRotateZ->KeyModifyBegin();

    lTime.SetSecondDouble(0);
    nKeyIndex = pChildRotateY->KeyAdd(lTime);
    pChildRotateX->KeyAdd(lTime);
    pChildRotateZ->KeyAdd(lTime);

    pChildRotateX->KeySetValue(nKeyIndex, 0);
    pChildRotateY->KeySetValue(nKeyIndex, 0);
    pChildRotateZ->KeySetValue(nKeyIndex, 0);
    pChildRotateX->KeySetInterpolation(nKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
    pChildRotateY->KeySetInterpolation(nKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
    pChildRotateZ->KeySetInterpolation(nKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

    lTime.SetSecondDouble(5);
    nKeyIndex = pChildRotateX->KeyAdd(lTime);
    pChildRotateY->KeyAdd(lTime);
    pChildRotateZ->KeyAdd(lTime);
    pChildRotateX->KeySetValue(nKeyIndex, 90);
    pChildRotateY->KeySetValue(nKeyIndex, 90);
    pChildRotateZ->KeySetValue(nKeyIndex, 90);
    pChildRotateX->KeySetInterpolation(nKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
    pChildRotateY->KeySetInterpolation(nKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
    pChildRotateZ->KeySetInterpolation(nKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
    pChildRotateX->KeyModifyEnd();
    pChildRotateY->KeyModifyEnd();
    pChildRotateZ->KeyModifyEnd();

    //Translate子骨骼
    /*
    FbxAnimCurve* pChildTranslateX = pChildNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_X, true);
    FbxAnimCurve* pChildTranslateY = pChildNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Y, true);
    FbxAnimCurve* pChildTranslateZ = pChildNode->LclTranslation.GetCurve(pAnimLayer, FBXSDK_CURVENODE_COMPONENT_Z, true);

    pChildTranslateX->KeyModifyBegin();
    pChildTranslateY->KeyModifyBegin();
    pChildTranslateZ->KeyModifyBegin();

    lTime.SetSecondDouble(0);
    nKeyIndex = pChildTranslateX->KeyAdd(lTime);
    pChildTranslateY->KeyAdd(lTime);
    pChildTranslateZ->KeyAdd(lTime);

    FbxVector4 trans(0, 0, 0, 1);
    FbxAMatrix globalMat = pChildNode->EvaluateGlobalTransform(lTime);
    FbxVector4 globalTrans = globalMat.MultT(trans);
    DBGSTRING("globalTrans:( %f, %f, %f, %f)", globalTrans[0], globalTrans[1], globalTrans[2], globalTrans[3]);
    pChildTranslateX->KeySetValue(nKeyIndex, globalTrans[0]);
    pChildTranslateY->KeySetValue(nKeyIndex, globalTrans[1]);
    pChildTranslateZ->KeySetValue(nKeyIndex, globalTrans[2]);
    pChildTranslateX->KeySetInterpolation(nKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
    pChildTranslateY->KeySetInterpolation(nKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
    pChildTranslateZ->KeySetInterpolation(nKeyIndex, FbxAnimCurveDef::eInterpolationLinear);

    lTime.SetSecondDouble(5);
    trans.Set(0, 30, 0);
    globalMat = pChildNode->EvaluateGlobalTransform(lTime);
    globalTrans = globalMat.MultT(trans);
    nKeyIndex = pChildTranslateX->KeyAdd(lTime);
    pChildTranslateY->KeyAdd(lTime);
    pChildTranslateZ->KeyAdd(lTime);
    DBGSTRING("globalTrans:( %f, %f, %f, %f)", globalTrans[0], globalTrans[1], globalTrans[2], globalTrans[3]);
    pChildTranslateX->KeySetValue(nKeyIndex, globalTrans[0]);
    pChildTranslateY->KeySetValue(nKeyIndex, globalTrans[1]);
    pChildTranslateZ->KeySetValue(nKeyIndex, globalTrans[2]);
    pChildTranslateX->KeySetInterpolation(nKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
    pChildTranslateY->KeySetInterpolation(nKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
    pChildTranslateZ->KeySetInterpolation(nKeyIndex, FbxAnimCurveDef::eInterpolationLinear);
    pChildTranslateX->KeyModifyEnd();
    pChildTranslateY->KeyModifyEnd();
    pChildTranslateZ->KeyModifyEnd();
    */
    FbxOgre::FbxSdk::saveScene("testanimation.fbx");
}