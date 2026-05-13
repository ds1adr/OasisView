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

    void makeDummyData(fftw_complex *mask, SimulationConfig& config);
    void writeFFTW(SimulationConfig& config, fftw_complex* fft);
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

    void simulationSelected(int lowLeftX, int lowLeftY, int upperRightX, int upperRightY, float waveLength, float na, float sigma);
};
#endif // MAINWINDOW_H
