#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QComboBox>
#include <QLabel>
#include <QListWidget>
#include <QMainWindow>

#include "OASISParser/OASISData.h"
#include "OASISView.h"
#include "Simulator/Simulator.h"

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

    static void makeDummyData(const SimulationConfig& config, double *mask);
    void writeMask(SimulationConfig& config, double* mask);
    void writeIntensity(SimulationConfig& config, std::vector<double>& intensity);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void openFileClicked();
    void simulationClicked();
    void cellNameComboBoxChanged(QString cellName);
    void cellDepthComboBoxChanged(QString depth);
    void statusChanged(QString& message);

    void simulation1DButtonClicked();
    void simulation2DButtonClicked();
    void simulationCancelClicked();

    void simulationSelected(int lowLeftX, int lowLeftY, int upperRightX, int upperRightY, float waveLength, float na, float sigma);
    void simulation1DSelected(int pitch, int spaceWidth, int simulationWindow, float waveLength, float na, float sigma);
};
#endif // MAINWINDOW_H
