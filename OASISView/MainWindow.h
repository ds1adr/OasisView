#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QListWidget>
#include <QMainWindow>

#include "OASISParser/OASISData.h"
#include "OASISView.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QToolBar* mToolBar;
    QDockWidget* mDock;
    QListWidget* mLayerListWidget;
    OASISParser::OASISData mOASISData;
    OASISView* mOASISView;
    QMenu* mFileMenu;
    QAction* mOpenAction;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openFileClicked();
};
#endif // MAINWINDOW_H
