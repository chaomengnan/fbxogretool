#include "XkOgreGraphic.h"
#include <XkDebugStr.h>
#include "XkSkeletonManagerEx.h"

namespace Xk
{

OgreGraphic* OgreGraphic::m_pInstance = 0;

OgreGraphic::OgreGraphic() :
	m_pRoot(NULL), m_pRenderSystem(NULL), m_pSceneMgr(NULL)
{
	Ogre::String strPluginPath;

	strPluginPath = "plugins.cfg";

	m_pRoot = OGRE_NEW Ogre::Root(strPluginPath,
		"ogre.cfg", "Ogre.log");
    
    Ogre::SkeletonManager* pSkeletonManager = Ogre::SkeletonManager::getSingletonPtr();
    OGRE_DELETE pSkeletonManager;
    OGRE_NEW Xk::SkeletonManagerEx();
    
    setupResources();
    setupRenderSystem();
    setupSceneManager();
}

OgreGraphic::~OgreGraphic()
{
    OGRE_DELETE m_pRoot;
}

OgreGraphic& OgreGraphic::instance()
{
    if(m_pInstance==0)
        m_pInstance = new OgreGraphic();

    return *m_pInstance;
}

void OgreGraphic::destroy()
{
    if(m_pInstance)
        delete m_pInstance;
}

bool OgreGraphic::setupResources()
{
    Ogre::ConfigFile cf;
	cf.load("resources.cfg");

	Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

	Ogre::String strSecName, strTypeName, strArchName;
	while(seci.hasMoreElements()) {
		strSecName = seci.peekNextKey();
		Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
		Ogre::ConfigFile::SettingsMultiMap::iterator i;
		for(i = settings->begin(); i != settings->end(); ++i) {
			strTypeName = i->first;
			strArchName = i->second;

			Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
				strArchName, strTypeName, strSecName);
		}
	}

    return true;
}

bool OgreGraphic::setupRenderSystem()
{
	m_pRoot->showConfigDialog();

	//不创建窗口
	m_pRoot->initialise(false);

	return true;
}

bool OgreGraphic::setupSceneManager()
{
    m_pSceneMgr = m_pRoot->createSceneManager(Ogre::ST_EXTERIOR_CLOSE);
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	return true;
}

}//namespace Xk