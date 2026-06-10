#include "MainWindow.h"
#include "ILTDialog.h"
#include "simulationdialog.h"
#include "Simulation1DDialog.h"
#include "Simulation1DResultdialog.h"
#include "Simulator/Simulator.h"
#include "SimulationSelectionDialog.h"
#include "Simulator/cuSimulator.h"

#include <fstream>
#include <iostream>
#include <thread>
#include <random>

#include <QAction>
#include <QDockWidget>
#include <QFileDialog>
#include <QMenuBar>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mOpenAction = new QAction(QIcon(""), tr("&Open"), this);
    mOpenAction->setShortcut(QKeySequence::Open);
    mOpenAction->setStatusTip(tr("Open an OASIS file"));
    connect(mOpenAction, SIGNAL(triggered()), this, SLOT(openFileClicked()));

    mFileMenu = menuBar()->addMenu(tr("&File"));
    mFileMenu->addAction(mOpenAction);

    mSimulationAction = new QAction(QIcon(""), tr("Simulation"), this);
    mSimulationAction->setStatusTip("Run Simulation");
    connect(mSimulationAction, SIGNAL(triggered()), this, SLOT(simulationClicked()));
    mSimulationMenu = menuBar()->addMenu("Simulation");
    mSimulationMenu->addAction(mSimulationAction);

    mILTAction = new QAction(QIcon(""), tr("ILT"), this);
    mILTAction->setStatusTip("Run ILT Simulation");
    connect(mILTAction, SIGNAL(triggered()), this, SLOT(ILTClicked()));
    mILTMenu = menuBar()->addMenu("ILT");
    mILTMenu->addAction(mILTAction);

    mToolBar = addToolBar("File");
    mToolBar->addAction(mOpenAction);

    mToolBar = addToolBar("Simulation");
    mToolBar->addAction(mSimulationAction);

    mCellNameComboBox = new QComboBox(this);
    mToolBar->addWidget(mCellNameComboBox);
    connect(mCellNameComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(cellNameComboBoxChanged(QString)));

    mCellDepthLabel = new QLabel(this);
    mCellDepthLabel->setText("Cell Depth:");
    mToolBar->addWidget(mCellDepthLabel);

    mCellDepthComboBox = new QComboBox(this);
    mToolBar->addWidget(mCellDepthComboBox);
    connect(mCellDepthComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(cellDepthComboBoxChanged(QString)));

    mDock = new QDockWidget("", this);
    mDock->setAllowedAreas(Qt::LeftDockWidgetArea);

    mLayerListWidget = new QListWidget(mDock);

    mDock->setWidget(mLayerListWidget);
    addDockWidget(Qt::LeftDockWidgetArea, mDock);

    resizeDocks({mDock}, {120}, Qt::Horizontal);

    mOASISView = new OASISView();
    setCentralWidget(mOASISView);

    connect(mOASISView, SIGNAL(statusChanged(QString&)), this, SLOT(statusChanged(QString&)));

    centralWidget()->setMouseTracking(true);
}

MainWindow::~MainWindow()
{
    delete mOpenAction;
    delete mFileMenu;
    delete mSimulationAction;
    delete mSimulationMenu;
    delete mILTAction;
    delete mILTMenu;
    delete mCellNameComboBox;
    delete mCellDepthLabel;
    delete mCellDepthComboBox;
    delete mLayerListWidget;
    delete mDock;
    delete mOASISView;
}

void MainWindow::statusChanged(QString& message) {
    statusBar()->showMessage(message);
}

void MainWindow::openFileClicked() {
    QString filename = QFileDialog::getOpenFileName();
    qDebug() << "Open File:" << filename;
    mOASISData = OASISParser::OASISData();

    std::thread th(&OASISParser::OASISData::parse, &mOASISData, filename.toStdString());
    th.join();

    qDebug() << "Finished";
    vector<unsigned> layers = mOASISData.getLayers();
    int depth = mOASISData.getDepth();
    setDepthCombo(depth);
    mLayerListWidget->clear();
    for (int i : layers) {
        QListWidgetItem* item = new QListWidgetItem(tr("Layer ") + QString::number(i), mLayerListWidget);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Checked);
    }
    vector<string> cellNames = mOASISData.getAllCellNames();
    mCellNameComboBox->clear();
    for (string& cellName : cellNames) {
        mCellNameComboBox->addItem(QString::fromStdString(cellName));
    }
}

void MainWindow::simulationClicked() {
//    SimulationSelectionDialog dialog(this);
//    connect(&dialog, SIGNAL(simulation1DButtonClicked()), this, SLOT(simulation1DButtonClicked()));
//    connect(&dialog, SIGNAL(simulation2DButtonClicked()), this, SLOT(simulation2DButtonClicked()));
//    connect(&dialog, SIGNAL(simulationCancelClicked()), this, SLOT(simulationCancelClicked()));

//    dialog.exec();
    simulation2DButtonClicked();
}

void MainWindow::setDepthCombo(int depth) {
    mCellDepthComboBox->clear();
    for (int i = 0; i <= depth; i++) {
        mCellDepthComboBox->addItem(QString::number(i));
    }
}

void MainWindow::cellNameComboBoxChanged(QString cellName) {
    if (!cellName.isEmpty()) {
        drawCell(cellName.toStdString());
    }
}

void MainWindow::cellDepthComboBoxChanged(QString depth) {
    int d = depth.toInt();
    mOASISView->setMaxDrawDepth(d);
    mOASISView->update();
}

void MainWindow::makeMaskData(SimulationConfig& c, double* mask) {
    string cellName = mCellNameComboBox->currentText().toStdString();
    OASISParser::OASISCell* cell = mOASISData.getCell(cellName);
    mOASISView->makeMaskData(c, mask, cell);
}

void MainWindow::drawCell(string cellName) {
    OASISParser::OASISCell* cell = mOASISData.getCell(cellName);
    mOASISView->updateCell(&mOASISData, cell);
}

void MainWindow::simulation2DButtonClicked() {
    // Need to check if data is loaded
    string cellName = mCellNameComboBox->currentText().toStdString();
    if (cellName.empty()) {
        QMessageBox::information(this, "Information", "OASIS Data is not loaded.", QMessageBox::Ok);
        return;
    }
    SimulationDialog dialog(this);
    connect(&dialog, SIGNAL(simulationSelected(int,int,int,int,float,float,float, float)), this, SLOT(simulationSelected(int,int,int,int,float,float,float,float)));
    dialog.exec();
}

void MainWindow::simulationSelected(int lowLeftX, int lowLeftY, int upperRightX, int upperRightY, float waveLength, float na, float sigma, float innerSigma) {
    int windowX = upperRightX - lowLeftX;
    int windowY = upperRightY - lowLeftY;

    // set simulation config (NA, Sigma and etc)
    SimulationConfig config = SimulationConfig();
    config.wavelength = waveLength;
    config.NA = na;
    config.sigma = sigma;
    config.innerSigma = innerSigma;
    config.Nx = windowX;
    config.Ny = windowY;
    config.dx = ((double)windowX / (double)config.Nx);
    config.dy = ((double)windowY / (double)config.Ny);
    config.originX = lowLeftX;
    config.originY = lowLeftY;

    // make mask data from QOASISData (1D array with 2D size)
    int size = config.Nx * config.Ny;
    double* mask = new double[size];
    std::memset(mask, 0, sizeof(double) * size);

    //
    makeMaskData(config, mask);

    writeMask(config, mask);

    // results
    vector<double> intensity(size, 0);

    // run fft
    auto start = std::chrono::steady_clock::now();
#ifdef _CUDA_
    cu_simulate_2d_abbe(config, mask, intensity);
#else
    simulate_2d_abbe(config, mask, intensity);
#endif
    auto end = std::chrono::steady_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    std::cout << "Duration: " << elapsed.count() << " microseconds.\n" << std::endl;

    delete [] mask;

    // Display Dialog or Widget
    writeIntensity(config, intensity);
}

void MainWindow::simulation1DButtonClicked() {
    Simulation1DDialog dialog(this);
    connect(&dialog, SIGNAL(simulation1DSelected(int,int,int, float,float,float)), this, SLOT(simulation1DSelected(int,int,int, float,float,float)));
    dialog.exec();
}

void MainWindow::simulationCancelClicked() {

}

void MainWindow::simulation1DSelected(int pitch, int spaceWidth, int simulationWindow, float waveLength, float na, float sigma) {
    SimulationConfig1D config = SimulationConfig1D();
    config.wavelength = waveLength;
    config.NA = na;
    config.sigma = sigma;
    config.N = simulationWindow; // TODO: Temporal value
    config.dx = 1.0;  // TODO: temporal value
    config.pitch = pitch;
    config.spaceWidth = spaceWidth;

    vector<double> mask(config.N, 0);
    vector<double> intensity(config.N, 0);
    vector<double> spectrum(config.N, 0);

    simulate_1d(config, mask, spectrum, intensity);

    Simulation1DResultDialog maskDialog(this);
    maskDialog.setValues(config, mask);
    maskDialog.exec();

    Simulation1DResultDialog spectrumDialog(this);
    spectrumDialog.setValues(config, spectrum);
    spectrumDialog.exec();

    Simulation1DResultDialog intensityDialog(this);
    intensityDialog.setValues(config, intensity);
    intensityDialog.exec();
}

void MainWindow::writeMask(SimulationConfig& config, double* mask) {
    double pos_x = 0;
    double pos_y = 0;
    QString fileName = QFileDialog::getSaveFileName();

    QString gnuPlotFileName = fileName + ".plt";
    QString outputFileName = fileName + ".txt";

    ofstream gnuos = ofstream(gnuPlotFileName.toStdString());

    gnuos << "set pm3d map" << endl;
    gnuos << "set palette rgbformulae 33,13,10" << endl;
    gnuos << "set contour base" << endl;
    gnuos << "set cntrparam levels 10" << endl;
    gnuos << "set cntrparam cubicspline" << endl;

    gnuos << "splot " << "\"" << outputFileName.toStdString() << "\"" << " with pm3d" << endl;

    ofstream os = ofstream(outputFileName.toStdString());

    if (!os.is_open()) {
        // throw error
        return;
    }

    os << "# X Y Z" << endl;

    for (int jy = 0; jy < config.Ny; jy++) {
        pos_x = 0;
        for (int ix = 0; ix < config.Nx; ix++) {
            os << pos_x << " " << pos_y << " " << mask[jy * config.Nx + ix] << endl;
            pos_x += config.dx;
        }
        os << endl;
        pos_y += config.dy;
    }

    os.close();
}

void MainWindow::writeIntensity(SimulationConfig& config, std::vector<double>& intensity) {
    double pos_x = 0;
    double pos_y = 0;
    QString fileName = QFileDialog::getSaveFileName();

    QString gnuPlotFileName = fileName + ".plt";
    QString outputFileName = fileName + ".txt";

    ofstream gnuos = ofstream(gnuPlotFileName.toStdString());

    gnuos << "set pm3d map" << endl;
    gnuos << "set palette rgbformulae 33,13,10" << endl;
    gnuos << "set contour base" << endl;
    gnuos << "set cntrparam levels 10" << endl;
    gnuos << "set cntrparam cubicspline" << endl;

    gnuos << "splot " << "\"" << outputFileName.toStdString() << "\"" << " with pm3d" << endl;

    ofstream os = ofstream(outputFileName.toStdString());

    if (!os.is_open()) {
        // throw error
        return;
    }

    os << "# X Y Z" << endl;

    for (int jy = 0; jy < config.Ny; jy++) {
        pos_x = 0;
        for (int ix = 0; ix < config.Nx; ix++) {
            os << pos_x << " " << pos_y << " " << intensity[jy * config.Nx + ix] << endl;
            pos_x += config.dx;
        }
        os << endl;
        pos_y += config.dy;
    }

    os.close();
}

void MainWindow::ILTClicked() {
    string cellName = mCellNameComboBox->currentText().toStdString();
    if (cellName.empty()) {
        QMessageBox::information(this, "Information", "OASIS Data is not loaded.", QMessageBox::Ok);
        return;
    }
    ILTDialog dialog = ILTDialog(this);
    connect(&dialog, SIGNAL(ILTSelected(int, int, int, int, float, float, float, float, int, int)), this, SLOT(ILTSelected(int, int, int, int, float, float, float, float, int, int)));
    dialog.exec();
}


void MainWindow::ILTSelected(int lowLeftX, int lowLeftY, int upperRightX, int upperRightY, float waveLength, float na, float sigma, float innerSigma, float threshould, int flipGrid, int maxCount) {
    int windowX = upperRightX - lowLeftX;
    int windowY = upperRightY - lowLeftY;

    // set simulation config (NA, Sigma and etc)
    SimulationConfig config = SimulationConfig();
    config.wavelength = waveLength;
    config.NA = na;
    config.sigma = sigma;
    config.innerSigma = innerSigma;
    config.Nx = windowX;
    config.Ny = windowY;
    config.dx = ((double)windowX / (double)config.Nx);
    config.dy = ((double)windowY / (double)config.Ny);
    config.originX = lowLeftX;
    config.originY = lowLeftY;

    // make mask data from QOASISData (1D array with 2D size)
    int size = config.Nx * config.Ny;
    double* mask = new double[size];
    std::memset(mask, 0, sizeof(double) * size);

    vector<double> targetIntensity(size, 0);

    //
    makeMaskData(config, mask);
    makeTargetIntensityFromMask(config, targetIntensity, mask, threshould);

    // results
    vector<double> intensity(size, 0);

    // first run for reference
#ifdef _CUDA_
    cu_simulate_2d_abbe(config, mask, intensity);
#else
    simulate_2d_abbe(config, mask, intensity);
#endif
    // calculate initial cost function
    double minCost = getCost(config, targetIntensity, intensity);

    double *flipedMask = new double[size];
    for (int i = 0; i < size; i++) {
        flipedMask[i] = mask[i];
    }
    int count = 0;
    do {
        // flip mask randomly
        auto location = flipMask(config, flipGrid, flipedMask);

        // simulate_2d
#ifdef _CUDA_
        cu_simulate_2d_abbe(config, flipedMask, intensity);
#else
        simulate_2d_abbe(config, flipedMask, intensity);
#endif

        // calculate cost function
        double cost = getCost(config, targetIntensity, intensity);

        // if cost function is larger than before, roll back
        if (cost < minCost) {
            minCost = cost;
        } else {
            rollbackMask(config, flipGrid, mask, location);
        }
    } while(count < maxCount);


    delete [] mask;
    delete [] flipedMask;

    // Display Dialog or Widget
    writeIntensity(config, intensity);
}

std::tuple<int, int> MainWindow::flipMask(SimulationConfig& c, int flipGrid, double* mask) {
    int countX = (int)(c.Nx * c.dx)/flipGrid;
    int countY = (int)(c.Ny * c.dy)/flipGrid;
    int gridCountX = flipGrid / (int)(c.Nx * c.dx);
    int gridCountY = flipGrid / (int)(c.Ny * c.dy);

    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_int_distribution<int> distribX(0, countX);
    std::uniform_int_distribution<int> distribY(0, countY);

    int randX = distribX(gen);
    int randY = distribY(gen);

    int x = randX * gridCountX;
    int y = randY * gridCountY;

    for (int jy = y; jy < y + gridCountY; jy++) {
        for (int ix = x; ix < x + gridCountX; ix++) {
            mask[jy * c.Nx + ix] = (mask[jy * c.Nx + ix]) > 0 ? 0.0 : 1.0 / (c.Nx * c.Ny);
        }
    }

    return std::make_tuple(x, y);
}

void MainWindow::rollbackMask(SimulationConfig& c, int flipGrid, double* mask, std::tuple<int, int>& locTuple) {
    int gridCountX = flipGrid / (int)(c.Nx * c.dx);
    int gridCountY = flipGrid / (int)(c.Ny * c.dy);

    int x = get<0>(locTuple);
    int y = get<1>(locTuple);

    for (int jy = y; jy < y + gridCountY; jy++) {
        for (int ix = x; ix < x + gridCountX; ix++) {
            mask[jy * c.Nx + ix] = (mask[jy * c.Nx + ix]) > 0 ? 0.0 : 1.0 / (c.Nx * c.Ny);
        }
    }
}

void MainWindow::makeTargetIntensityFromMask(SimulationConfig&c, std::vector<double>& target, double* mask, double threshould) {
    int size = c.Nx * c.Ny;

    for (int i = 0; i < size; i++) {
        if (mask[i] > 0) {
            target[i] = threshould * 2.0;
        } else {
            target[i] = 0.0;
        }
    }
}
