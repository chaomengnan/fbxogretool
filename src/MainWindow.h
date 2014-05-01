#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "OgreWidget.h"
#include <Ogre.h>
#include "ui_mainwindow.h"

class OgreWidget;
class QListWidget;
class QSplitter;
class QListWidgetItem;

struct VERTEX_DATA{
    Ogre::Vector3 position;
    Ogre::Vector3 normal;
    Ogre::Vector2 uv;
};

typedef std::vector<VERTEX_DATA> VERTEX_LIST;
typedef std::vector<unsigned long> INDEX_LIST;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow() {}
    void updateMeshList();
    void createBones(Ogre::Entity* pEntity, Ogre::SceneNode* pParentNode, BONE_NODE_MAP& nodeMap);

private slots:
    void onMeshDoubleClicked(QListWidgetItem* item);
    void onAnimationDoubleClicked(QListWidgetItem* item);

private:
    Ui::MainWindow m_ui;

    OgreWidget* m_ogreWidget;
    Ogre::SceneNode* m_pNode;
    Ogre::Entity* m_pEntity;
};

#endif