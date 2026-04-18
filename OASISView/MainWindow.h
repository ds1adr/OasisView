#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QListWidget>
#include <QMainWindow>

#include "OASISParser/OASISData.h"
#include "OASISView.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QToolBar* mToolBar;
    QComboBox* mCellNameComboBox;
    QDockWidget* mDock;
    QListWidget* mLayerListWidget;
    OASISParser::OASISData mOASISData;
    OASISView* mOASISView;
    QMenu* mFileMenu;
    QAction* mOpenAction;

    void drawCell(std::string cellName);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openFileClicked();
    void cellNameComboBoxChanged(QString cellName);
    void statusChanged(const QString& message);
};
#endif // MAINWINDOW_H
