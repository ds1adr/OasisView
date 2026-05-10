#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QLabel>
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
    QLabel* mCellDepthLabel;
    QComboBox* mCellDepthComboBox;
    QDockWidget* mDock;
    QListWidget* mLayerListWidget;
    OASISParser::OASISData mOASISData;
    OASISView* mOASISView;
    QMenu* mFileMenu;
    QAction* mOpenAction;
    QMenu* mSimulationMenu;
    QAction* mSimulationAction;

    void drawCell(std::string cellName);
    void setDepthCombo(int depth);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openFileClicked();
    void simulationClicked();
    void cellNameComboBoxChanged(QString cellName);
    void cellDepthComboBoxChanged(QString depth);
    void statusChanged(QString& message);
};
#endif // MAINWINDOW_H
