#include "MainWindow.h"
#include "simulationdialog.h"

#include <thread>

#include <QAction>
#include <QDockWidget>
#include <QFileDialog>
#include <QMenuBar>
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
    delete mCellNameComboBox;
    delete mCellDepthLabel;
    delete mCellDepthComboBox;
    delete mDock;
    delete mLayerListWidget;
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
    // Need to check if data is loaded
    SimulationDialog dialog(this);
    connect(&dialog, SIGNAL(simulationSelected(int,int,int,int,float,float,float)), this, SLOT(simulationSelected(int,int,int,int,float,float,float)));
    dialog.exec();
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

void MainWindow::drawCell(string cellName) {
    OASISParser::OASISCell* cell = mOASISData.getCell(cellName);
    mOASISView->updateCell(&mOASISData, cell);
}

void MainWindow::simulationSelected(int lowLeftX, int lowLeftY, int upperRightX, int upperRightY, float waveLength, float na, float sigma) {
    // make mask data from QOASISData (1D array with 2D size)

    // set simulation config (NA, Sigma and etc)

    // run fft

    // Display Dialog or Widget
}