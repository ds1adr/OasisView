#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "OASISView.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
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
