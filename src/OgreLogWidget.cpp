#include "OgreLogWidget.h"
#include <XkTextCodec.h>

OgreLogWidget::OgreLogWidget(QWidget* parent)
{
    Ogre::LogManager::getSingleton().getDefaultLog()->addListener(this);

    connect(this, SIGNAL(textChanged()), this, SLOT(onTextChanged()));
}

OgreLogWidget::~OgreLogWidget()
{

}

void OgreLogWidget::messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool& skipThisMessage )
{
    std::string strMessage = message;
    Xk::TextCodec::asciiToUft8(strMessage);

    QString strLog(strMessage.c_str());

    insertPlainText(strLog + "\n");
}

void OgreLogWidget::onTextChanged()
{
    QTextCursor cursor = this->textCursor();
    cursor.movePosition(QTextCursor::End);
    this->setTextCursor(cursor);
}