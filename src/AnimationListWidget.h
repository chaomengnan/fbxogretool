#ifndef AnimationListWidget_H
#define AnimationListWidget_H

#include <QListWidget>
#include <QAction>
#include <QMenu>

class AnimationListWidget : public QListWidget
{
    Q_OBJECT
    
public:
    AnimationListWidget(QWidget* parent = 0);
    ~AnimationListWidget();

    void createActions();
    void setSkeletonFile(QString strFile)
    {
        m_strSkeletonFile = strFile;
    }

    void setMeshFile(QString strFile)
    {
        m_strMeshFile = strFile;
    }

protected:
    virtual void contextMenuEvent(QContextMenuEvent* evt);

private slots:
    void onExportSkeleton();

private:
    QAction* m_actExportSkeleton;
    QMenu* m_menu;
    QString m_strSkeletonFile;
    QString m_strMeshFile;
};

#endif