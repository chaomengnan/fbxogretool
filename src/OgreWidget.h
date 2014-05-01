#ifndef OGRE_WIDGET_H
#define OGRE_WIDGET_H

#include <QWidget>
#include <QLabel>
#include <Ogre.h>
#include <XkTimer.h>
#include <map>

struct BONE_NODE{
    Ogre::Entity* pEntity;
    Ogre::SceneNode* pNode;
};

typedef std::map<std::string, BONE_NODE > BONE_NODE_MAP;

class OgreWidget : public QWidget
{
	Q_OBJECT

public:
	OgreWidget(QWidget* parent = 0);
	~OgreWidget();

    void updateAnimation();
    void updateBones();

	Ogre::RenderWindow* getRenderWindow()
	{
		return m_pWindow;
	}

	void setNode(Ogre::SceneNode* pNode)
	{
		m_pNode = pNode;
	}

    void setLightNode(Ogre::SceneNode* pNode)
    {
        m_pLightNode = pNode;
    }

    void setAnimationState(Ogre::AnimationState* pState)
    {
        m_timer.Restart();
        m_pAnimationState = pState;
    }

    void setEntity(Ogre::Entity* pEntity)
    {
        m_pEntity = pEntity;
    }

    void setBoneNodeMap(const BONE_NODE_MAP& nodeMap)
    {
        m_nodeMap = nodeMap;
    }

protected:
	void resizeEvent(QResizeEvent* evt);
	void timerEvent(QTimerEvent* evt);
	void paintEvent(QPaintEvent* evt);

	void keyPressEvent(QKeyEvent* evt);
	void keyReleaseEvent(QKeyEvent* evt);
	void mousePressEvent(QMouseEvent* evt);
	void mouseReleaseEvent(QMouseEvent* evt);
	void mouseMoveEvent(QMouseEvent* evt);
	void wheelEvent(QWheelEvent* evt);

private:
	Ogre::Camera* m_pCamera;
    Ogre::RenderWindow* m_pWindow;
    Ogre::Viewport* m_pVp;
	Ogre::SceneNode* m_pNode;
    Ogre::SceneNode* m_pLightNode;
    int m_nLastMouseX;
    int m_nLastMouseY;
    bool m_bFirstPressed;
	bool m_bResizing;
    bool m_bCtrlPressed;

    Xk::Timer m_timer;
    Ogre::Real m_fTime;
    Ogre::Real m_fSpeed;

    Ogre::Entity* m_pEntity;

    BONE_NODE_MAP m_nodeMap;

    Ogre::AnimationState* m_pAnimationState;
};

#endif