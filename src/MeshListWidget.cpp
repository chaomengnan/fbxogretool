#include "MeshListWidget.h"
#include <XkDebugStr.h>
#include <XkTextCodec.h>
#include "FbxOgre.h"
#include <QFileDialog>

#pragma execution_character_set("utf-8")

MeshListWidget::MeshListWidget(QWidget* parent)
{
    createActions();
}

MeshListWidget::~MeshListWidget()
{

}

void MeshListWidget::createActions()
{
    m_actExportMesh = new QAction("µ¼³öMesh", this);
    m_menu = new QMenu(this);
    m_menu->addAction(m_actExportMesh);

    connect(m_actExportMesh, SIGNAL(triggered()), this, SLOT(onExportMesh()));
}

void MeshListWidget::contextMenuEvent(QContextMenuEvent* evt)
{
    m_menu->exec(QCursor::pos());
}

void MeshListWidget::onExportMesh()
{
    DBG_BEGIN_END0();
    
    QListWidgetItem* item = this->currentItem();
    if(item == NULL) return;

    QString strMesh = item->text();

    std::string strMeshName = strMesh.toStdString();
    Xk::TextCodec::utf8ToAscii(strMeshName);

    QString strFile = QFileDialog::getSaveFileName(this);
    if(strFile.isEmpty()) return;

    std::string strFilePath = strFile.toStdString();
    Xk::TextCodec::utf8ToAscii(strFilePath);
    DBGSTRING("Mesh: %s, File: %s", strMeshName.c_str(), strFilePath.c_str());
    FbxOgre::Util::exportMesh(strMeshName.c_str(), "DPCQ", strFilePath.c_str());
}