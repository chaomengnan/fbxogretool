#include "MainWindow.h"
#include <QListWidget>
#include <QSplitter>

#include <XkDebugStr.h>
#include <XkTextCodec.h>
#include <vector>
#include "XkOgreGraphic.h"
#include "FbxOgre.h"

#pragma execution_character_set("utf-8")

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    m_ui.setupUi(this);

    m_pNode = 0;
    m_pEntity = 0;

    m_ogreWidget = new OgreWidget(this);
    m_ogreWidget->setFocusPolicy(Qt::StrongFocus);

    m_ui.tabWidget->addTab(m_ogreWidget, "Ogre");

    connect(m_ui.listMesh, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
        this, SLOT(onMeshDoubleClicked(QListWidgetItem*)));

    connect(m_ui.listAnimation, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
        this, SLOT(onAnimationDoubleClicked(QListWidgetItem*)));

    updateMeshList();
}

void MainWindow::updateMeshList()
{
    DBG_BEGIN_END0();
    Ogre::FileInfoListPtr fileInfo = Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo("DPCQ",
        "*.mesh");

    for(Ogre::FileInfoList::iterator it = fileInfo->begin();
        it != fileInfo->end();
        it ++)
    {
        XkDebugString("FileName: %s", it->filename.c_str());

        std::string strName = it->filename;
        Xk::TextCodec::asciiToUft8(strName);
        QString strItemName(strName.c_str());

        m_ui.listMesh->addItem(strItemName);
    }
}

void MainWindow::createBones(Ogre::Entity* pEntity, Ogre::SceneNode* pParentNode, BONE_NODE_MAP& nodeMap)
{
    Ogre::SkeletonPtr pSkeleton = pEntity->getMesh()->getSkeleton();

    for(int i=0; i<pSkeleton->getNumBones(); i++) {
        Ogre::Bone* pBone = pSkeleton->getBone(i);
        Ogre::Entity* pBoneEntity = gGraphic().getSceneManager()->createEntity("1x1x1box.mesh");
        BONE_NODE node;
        node.pEntity = pBoneEntity;
        Ogre::SceneNode* pNode = pParentNode->createChildSceneNode();
        pNode->attachObject(pBoneEntity);
        pNode->setPosition(pBone->_getDerivedPosition());
        pNode->setOrientation(pBone->_getDerivedOrientation());
        node.pNode = pNode;
        pNode->scale(3.0f, 3.0f, 3.0f);

        nodeMap.insert(BONE_NODE_MAP::value_type(pBone->getName(), node));
    }
}

void MainWindow::onMeshDoubleClicked(QListWidgetItem* item)
{
    QString strName = item->text();
    Ogre::String strMeshName = strName.toStdString();
    Xk::TextCodec::utf8ToAscii(strMeshName);
    m_ui.listAnimation->clear();
    m_ui.listSkeleton->clear();

    Ogre::MeshPtr mesh = Ogre::MeshManager::getSingleton().load(strMeshName, "Popular");
    Ogre::SkeletonPtr skeleton = mesh->getSkeleton();
    if(skeleton.isNull()) {
        //无骨骼动画，那么显示模型
        Ogre::String strEntityMesh = strMeshName;
        Xk::TextCodec::utf8ToAscii(strEntityMesh);
        if(m_pEntity != 0) {
            m_pNode->detachAllObjects();
            gGraphic().getSceneManager()->destroyEntity(m_pEntity);
            gGraphic().getSceneManager()->destroySceneNode(m_pNode);
        }

        m_pEntity = gGraphic().getSceneManager()->createEntity(strEntityMesh);
        m_pNode = gGraphic().getSceneManager()->getRootSceneNode()->createChildSceneNode();
        m_pNode->attachObject(m_pEntity);
        m_ogreWidget->setNode(m_pNode);
        return;
    }

    Ogre::Skeleton::LinkedSkeletonAnimSourceIterator it = skeleton->getLinkedSkeletonAnimationSourceIterator();

    while(it.hasMoreElements()) {
        Ogre::LinkedSkeletonAnimationSource source = it.getNext();
        std::string strLinkSkeleton = source.skeletonName;
        Xk::TextCodec::asciiToUft8(strLinkSkeleton);
        m_ui.listSkeleton->addItem(strLinkSkeleton.c_str());
    }

    QString strSkeletonFile;
    std::string strTmpSkeleton = skeleton->getName();
    Xk::TextCodec::asciiToUft8(strTmpSkeleton);
    strSkeletonFile = strTmpSkeleton.c_str();
    
    m_ui.listAnimation->setSkeletonFile(strSkeletonFile);
    m_ui.listAnimation->setMeshFile(strName);

    int nCount = skeleton->getNumAnimations();
    for(int i=0; i<nCount; i++) {
        Ogre::Animation* pAnim = skeleton->getAnimation(i);
        Ogre::String strAnimName = pAnim->getName();
        Xk::TextCodec::asciiToUft8(strAnimName);
        m_ui.listAnimation->addItem(strAnimName.c_str());
    }
}

void MainWindow::onAnimationDoubleClicked(QListWidgetItem* item)
{
    QString strName = item->text();
    Ogre::String strAnimName = strName.toStdString();
    Xk::TextCodec::utf8ToAscii(strAnimName);

    QListWidgetItem* pCurrItem = m_ui.listMesh->currentItem();
    QString strMeshName = pCurrItem->text();
    
    if(strMeshName.isEmpty()) return;

    Ogre::String strEntityMesh = strMeshName.toStdString();
    Xk::TextCodec::utf8ToAscii(strEntityMesh);
    if(m_pEntity != 0) {
        m_pNode->detachAllObjects();
        gGraphic().getSceneManager()->destroyEntity(m_pEntity);
        gGraphic().getSceneManager()->destroySceneNode(m_pNode);
    }

    m_pEntity = gGraphic().getSceneManager()->createEntity(strEntityMesh);
    m_pNode = gGraphic().getSceneManager()->getRootSceneNode()->createChildSceneNode();
    m_pNode->attachObject(m_pEntity);

    Ogre::AnimationState* pState = m_pEntity->getAnimationState(strAnimName);
    
    pState->setEnabled(true);
    pState->setLoop(true);
    m_ogreWidget->setAnimationState(pState);
    m_ogreWidget->setNode(m_pNode);

    BONE_NODE_MAP nodeMap;
    createBones(m_pEntity, m_pNode, nodeMap);
    m_ogreWidget->setEntity(m_pEntity);
    m_ogreWidget->setBoneNodeMap(nodeMap);

    DBGSTRING("Animation length: %f", pState->getLength());
}