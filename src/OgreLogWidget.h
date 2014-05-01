#ifndef OGRE_LOG_WIDGET_H
#define OGRE_LOG_WIDGET_H

#include <QPlainTextEdit>
#include <Ogre.h>

class OgreLogWidget : public QPlainTextEdit, public Ogre::LogListener
{
    Q_OBJECT
public:
    OgreLogWidget(QWidget* parent = 0);
    ~OgreLogWidget();

    virtual void messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool& skipThisMessage );

private slots:
    void onTextChanged();

};


#endif