#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>

#include "OASISParser/OASISData.h"
#include "OASISView.h"
#include "Simulator/Simulator.h"
#include "ILTThread.h"

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
    QMenu* mILTMenu;
    QAction* mILTAction;

    ILTThread* thread = nullptr;

    void drawCell(std::string cellName);
    void setDepthCombo(int depth);

    void writeVectorArray(SimulationConfig& config, std::vector<double>& intensity);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void makeMaskData(SimulationConfig& c, std::vector<double>& mask);

private slots:
    void openFileClicked();
    void simulationClicked();
    void cellNameComboBoxChanged(QString cellName);
    void cellDepthComboBoxChanged(QString depth);
    void statusChanged(QString& message);

    void simulation1DButtonClicked();
    void simulation2DButtonClicked();
    void simulationCancelClicked();

    void ILTClicked();

    void simulationSelected(int lowLeftX, int lowLeftY, int upperRightX, int upperRightY, float waveLength, float na, float sigma, float innerSigma);
    void simulation1DSelected(int pitch, int spaceWidth, int simulationWindow, float waveLength, float na, float sigma);
    void ILTSelected(int lowLeftX, int lowLeftY, int upperRightX, int upperRightY, float waveLength, float na, float sigma, float innerSigma, float threshould, int flipGrid, int maxCount);
};
#endif // MAINWINDOW_H
