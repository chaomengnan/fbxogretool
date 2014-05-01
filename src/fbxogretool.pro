TEMPLATE = app
QT += widgets

TARGET = fbxogretool

HEADERS += FbxOgre.h\
           MainWindow.h\
		   OgreWidget.h\
		   XkOgreGraphic.h\
		   XkSkeletonEx.h\
		   XkSkeletonManagerEx.h\
		   XkSkeletonSerializerEx.h\
		   MeshListWidget.h\
		   OgreLogWidget.h\
		   AnimationListWidget.h
SOURCES += main.cpp\
		   OgreWidget.cpp\
		   XkOgreGraphic.cpp\
		   XkSkeletonEx.cpp\
		   XkSkeletonManagerEx.cpp\
		   XkSkeletonSerializerEx.cpp\
		   MainWindow.cpp\
		   FbxOgre.cpp\
		   MeshListWidget.cpp\
		   OgreLogWidget.cpp\
		   AnimationListWidget.cpp

Debug:DESTDIR = ../../bin/debug
Release:DESTDIR = ../../bin/release

FORMS += mainwindow.ui
		   
win32{
INCLUDEPATH += $$(SNH_HOME)/cloud-ogresdk-v1-9-0/include/OGRE
INCLUDEPATH += $$(SNH_HOME)/poco/include
INCLUDEPATH += $$(FBX_SDK_HOME)/include
INCLUDEPATH += ../include

LIBS += -L$$(SNH_HOME)/poco/lib
Debug:LIBS += -L$$PWD/../lib/debug
Debug:LIBS += -L$$(FBX_SDK_HOME)/lib/vs2012/x86/debug
Release:LIBS += -L$$PWD/../lib/release
Debug:LIBS += -L$$(FBX_SDK_HOME)/lib/vs2012/x86/release

Debug:LIBS += -L$$(SNH_HOME)/cloud-ogresdk-v1-9-0/lib/debug\
              -L$$(SNH_HOME)/cloud-ogresdk-v1-9-0/lib/debug/opt
			  
Release:LIBS += -L$$(SNH_HOME)/cloud-ogresdk-v1-9-0/lib/release\
                -L$$(SNH_HOME)/cloud-ogresdk-v1-9-0/lib/release/opt
				


Debug:LIBS += -lOgreMain_d -lOgreOverlay_d -lOgrePaging_d -lOgreRTShaderSystem_d\
              -lOgreTerrain_d -lOgreVolume_d -lOIS_d -lPlugin_BSPSceneManager_d\
			  -lPlugin_CgProgramManager_d -lPlugin_OctreeSceneManager_d\
			  -lPlugin_OctreeZone_d -lPlugin_ParticleFX_d -lPlugin_PCZSceneManager_d\
			  -lRenderSystem_Direct3D9_d -lRenderSystem_Direct3D11_d
Release:LIBS += -lOgreMain -lOgreOverlay -lOgrePaging -lOgreRTShaderSystem\
              -lOgreTerrain -lOgreVolume -lOIS -lPlugin_BSPSceneManager\
			  -lPlugin_CgProgramManager -lPlugin_OctreeSceneManager\
			  -lPlugin_OctreeZone -lPlugin_ParticleFX -lPlugin_PCZSceneManager\
			  -lRenderSystem_Direct3D9 -lRenderSystem_Direct3D11

LIBS += -lxkcore -llibfbxsdk

DEFINES += POCO_STATIC
}

