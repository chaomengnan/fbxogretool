#include "AnimationListWidget.h"
#include <XkDebugStr.h>
#include <XkTextCodec.h>
#include "FbxOgre.h"
#include <QFileDialog>

#pragma execution_character_set("utf-8")

AnimationListWidget::AnimationListWidget(QWidget* parent)
{
    createActions();
}

AnimationListWidget::~AnimationListWidget()
{

}

void AnimationListWidget::createActions()
{
    m_actExportSkeleton = new QAction("导出Skeleton", this);
    m_menu = new QMenu(this);
    m_menu->addAction(m_actExportSkeleton);

    connect(m_actExportSkeleton, SIGNAL(triggered()), this, SLOT(onExportSkeleton()));
}

void AnimationListWidget::contextMenuEvent(QContextMenuEvent* evt)
{
    m_menu->exec(QCursor::pos());
}

void AnimationListWidget::onExportSkeleton()
{
    DBG_BEGIN_END0();
    QListWidgetItem* item = this->currentItem();
    if(item == NULL) return;

    QString strTmp = item->text();

    std::string strAnimName = strTmp.toStdString();
    Xk::TextCodec::utf8ToAscii(strAnimName);

    //执行导出指定名称的骨骼动画
    std::string strSkeletonFile = m_strSkeletonFile.toStdString();
    Xk::TextCodec::utf8ToAscii(strSkeletonFile);

    std::string strMeshFile = m_strMeshFile.toStdString();
    Xk::TextCodec::utf8ToAscii(strMeshFile);
    DBGSTRING("MeshFile: %s SkeletonFile: %s AnimName: %s", strMeshFile.c_str(), strSkeletonFile.c_str(), strAnimName.c_str());

    FbxOgre::FbxSdk::clearScene();
    //FbxOgre::Util::enumSkeleton(strSkeletonFile.c_str(), "DPCQ");
    FbxOgre::BONE_MAP boneMap;
    Ogre::SkeletonPtr pSkeleton = Ogre::SkeletonManager::getSingleton().load(strSkeletonFile, "DPCQ").staticCast<Ogre::Skeleton>();
    if(pSkeleton.isNull()) return;

    FbxOgre::Util::createBoneMap(pSkeleton, boneMap);
    FbxOgre::FbxSdk::saveScene("fbxogretool.fbx");
}