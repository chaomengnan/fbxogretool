#ifndef MESH_LIST_WIDGET_H
#define MESH_LIST_WIDGET_H

#include <QListWidget>
#include <QAction>
#include <QMenu>

class MeshListWidget : public QListWidget
{
    Q_OBJECT
    
public:
    MeshListWidget(QWidget* parent = 0);
    ~MeshListWidget();
    void createActions();

protected:
    virtual void contextMenuEvent(QContextMenuEvent* evt);

private slots:
    void onExportMesh();

private:
    QAction* m_actExportMesh;
    QMenu* m_menu;
};

#endif