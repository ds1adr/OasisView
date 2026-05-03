#include "MainWindow.h"

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

    mToolBar = addToolBar("File");
    mToolBar->addAction(mOpenAction);

    mCellNameComboBox = new QComboBox(this);
    mToolBar->addWidget(mCellNameComboBox);
    connect(mCellNameComboBox, SIGNAL(currentTextChanged(QString&)), this, SLOT(cellNameComboBoxChanged(QString&)));

    mCellDepthLabel = new QLabel(this);
    mCellDepthLabel->setText("Cell Depth:");
    mToolBar->addWidget(mCellDepthLabel);

    mCellDepthComboBox = new QComboBox(this);
    mToolBar->addWidget(mCellDepthComboBox);
    connect(mCellDepthComboBox, SIGNAL(currentTextChanged(QString&)), this, SLOT(cellDepthComboBoxChanged(QString&)));

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

void MainWindow::setDepthCombo(int depth) {
    mCellDepthComboBox->clear();
    for (int i = 0; i <= depth; i++) {
        mCellDepthComboBox->addItem(QString::number(i));
    }
}

void MainWindow::cellNameComboBoxChanged(QString& cellName) {
    if (!cellName.isEmpty()) {
        drawCell(cellName.toStdString());
    }
}

void MainWindow::cellDepthComboBoxChanged(QString& depth) {
    int d = depth.toInt();
    mOASISView->setMaxDrawDepth(d);
    mOASISView->update();
}

void MainWindow::drawCell(string cellName) {
    OASISParser::OASISCell* cell = mOASISData.getCell(cellName);
    mOASISView->updateCell(&mOASISData, cell);
}
