#include "OgreWidget.h"
#include "XkOgreGraphic.h"
#include <QKeyEvent>
#include <QMessageBox>
#include <QBoxLayout>
#include <XkDebugStr.h>

#pragma execution_character_set("utf-8")

OgreWidget::OgreWidget(QWidget* parent) : m_nLastMouseX(-1), m_nLastMouseY(-1), m_bFirstPressed(false), m_bCtrlPressed(false),
	m_bResizing(false), m_fTime(0.0f), m_pAnimationState(0), m_fSpeed(1.0f), m_pEntity(0), m_pNode(0)
{
    //´´½¨´°¿Ú
    Ogre::Root* pRoot = gGraphic().getRoot();
    Ogre::SceneManager* pSceneMgr = gGraphic().getSceneManager();

    Ogre::NameValuePairList miscParams;
    miscParams["externalWindowHandle"] = Ogre::StringConverter::toString((size_t) this->winId());
	miscParams["vsync"] = "false";
	miscParams["FSAA"] = "8";
	miscParams["colourDepth"] = "32";

    m_pWindow = pRoot->createRenderWindow(
		Ogre::StringConverter::toString((unsigned long)this->winId()), this->width(), this->height(), false, &miscParams);

    m_pCamera = pSceneMgr->createCamera(Ogre::StringConverter::toString((size_t) this->winId()));
    m_pCamera->setPosition(Ogre::Vector3(0, 0, 100));
	m_pCamera->lookAt(Ogre::Vector3(0, 0, 0));
	m_pCamera->setNearClipDistance(1);
	m_pVp = m_pWindow->addViewport(m_pCamera);
	m_pVp->setBackgroundColour(Ogre::ColourValue(0.5,0.5,0.5, 1));
	m_pCamera->setAspectRatio(Ogre::Real(m_pVp->getActualWidth()) /
		Ogre::Real(m_pVp->getActualHeight()));

    pSceneMgr->setAmbientLight(Ogre::ColourValue());
    Ogre::Light* pLight = pSceneMgr->createLight();
    pLight->setPosition(300, 300, 300);

    setUpdatesEnabled(false);

	startTimer(2);
    m_timer.Restart();
}

OgreWidget::~OgreWidget()
{
    
}

void OgreWidget::updateAnimation()
{
    if(m_pAnimationState != 0) {
        //DBGSTRING("updateAnimation: %f", m_timer.elapsedSecond());
        //m_pAnimationState->addTime(m_fSpeed * m_timer.elapsedSecond());
        m_pAnimationState->setTimePosition(m_timer.elapsedSecond());
        //m_timer.Restart();
        
        updateBones();
        //DBGSTRING("TimePositon: %f", m_pAnimationState->getTimePosition());
    }
}

void OgreWidget::updateBones()
{
    if(!m_pEntity) return;

    Ogre::SkeletonInstance* pSkeleton = m_pEntity->getSkeleton();

    for(int i=0; i<pSkeleton->getNumBones(); i++) {
        Ogre::Bone* pBone = pSkeleton->getBone(i);

        std::string strName = pBone->getName();
        BONE_NODE_MAP::const_iterator it = m_nodeMap.find(strName);
        if(it != m_nodeMap.end()) {
            BONE_NODE node = it->second;
            node.pNode->setPosition(pBone->_getDerivedPosition());
            node.pNode->setOrientation(pBone->_getDerivedOrientation());
        }
    }
}

void OgreWidget::resizeEvent(QResizeEvent* evt)
{
	Q_UNUSED(evt);
    if(m_pWindow == NULL) return;

    m_pWindow->windowMovedOrResized();
    m_pCamera->setAspectRatio(
        Ogre::Real(m_pVp->getActualWidth()) /
        Ogre::Real(m_pVp->getActualHeight())
        );
}

void OgreWidget::timerEvent(QTimerEvent* evt)
{
	Q_UNUSED(evt);

    updateAnimation();

	gGraphic().updateBegin();
	gGraphic().updateEnd();
	m_pWindow->update();	
    
}

void OgreWidget::paintEvent(QPaintEvent* evt)
{
	Q_UNUSED(evt);

	gGraphic().updateBegin();
	gGraphic().updateEnd();
	m_pWindow->update();	
}

void OgreWidget::keyPressEvent(QKeyEvent* evt)
{
    if(evt->key() == Qt::Key_F){
        Ogre::PolygonMode mode = m_pCamera->getPolygonMode();
        if(mode == Ogre::PM_WIREFRAME) {
            m_pCamera->setPolygonMode(Ogre::PM_SOLID);
        }else{
            m_pCamera->setPolygonMode(Ogre::PM_WIREFRAME);
        }
    }else if(evt->key() == Qt::Key_Control) {
        m_bCtrlPressed = true;
    }else if(evt->key() == Qt::Key_Up) {
        m_fSpeed = m_fSpeed*2.0f;
    }else if(evt->key() == Qt::Key_Down) {
        m_fSpeed = m_fSpeed * 0.5f;
    }else if(evt->key() == Qt::Key_Space) {
        if(m_pEntity) {
            if(m_pEntity->isVisible()) {
                m_pEntity->setVisible(false);
            }else{
                m_pEntity->setVisible(true);
            }
        }
    }
}

void OgreWidget::keyReleaseEvent(QKeyEvent* evt)
{
    if(evt->key() == Qt::Key_Control){
        m_bCtrlPressed = false;
    }
}

void OgreWidget::mousePressEvent(QMouseEvent* evt)
{
    if(evt->buttons() & Qt::LeftButton ) {
        m_bFirstPressed = true;
    }
	if(evt->buttons() & Qt::RightButton) {
		m_bFirstPressed = true;
	}
}

void OgreWidget::mouseReleaseEvent(QMouseEvent* evt)
{
    if(evt->buttons() & Qt::LeftButton) {
        m_bFirstPressed = false;
    }
	if(evt->buttons() & Qt::RightButton) {
		m_bFirstPressed = false;
	}
}

void OgreWidget::mouseMoveEvent(QMouseEvent* evt)
{
    if(evt->buttons() & Qt::LeftButton) {
        int nRelX = evt->x() - m_nLastMouseX;
        int nRelY = evt->y() - m_nLastMouseY;
        if(!m_bFirstPressed) {
            Ogre::Radian rotX = Ogre::Degree(nRelX * 0.13);
            Ogre::Radian rotY = Ogre::Degree(nRelY * 0.13);
            if(m_bCtrlPressed) {
                m_pCamera->yaw(rotX);
                m_pCamera->pitch(rotY);
            }else{
                if(m_pNode) {
                    m_pNode->yaw(rotX);
                    m_pNode->pitch(rotY);
                }
            }
        }else{
            m_bFirstPressed = false;
        }
        m_nLastMouseX = evt->x();
        m_nLastMouseY = evt->y();   
    }

	if(evt->buttons() & Qt::RightButton) {
		int nRelX = evt->x() - m_nLastMouseX;
		int nRelY = evt->y() - m_nLastMouseY;

		if(!m_bFirstPressed) {
			Ogre::Vector3 transVec = Ogre::Vector3::ZERO;
			transVec.x -= nRelX * 0.05;
			transVec.y += nRelY * 0.05;
			m_pCamera->moveRelative(transVec);
		}else{
            m_bFirstPressed = false;
        }

		m_nLastMouseX = evt->x();
        m_nLastMouseY = evt->y();
	}
}
	
void OgreWidget::wheelEvent(QWheelEvent* evt)
{
    Ogre::Vector3 dir = m_pCamera->getDirection();
    dir.normalise();
    Ogre::Vector3 pos = m_pCamera->getPosition();
    Ogre::Vector3 newpos = pos + (evt->delta()/10.0f)*dir;
    m_pCamera->setPosition(newpos);
}