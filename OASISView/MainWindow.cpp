#include "MainWindow.h"

#include <QAction>
#include <QDockWidget>
#include <QFileDialog>
#include <QMenuBar>
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
    connect(mCellNameComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(cellNameComboBoxChanged(QString)));

    mDock = new QDockWidget("", this);
    mDock->setAllowedAreas(Qt::LeftDockWidgetArea);

    mLayerListWidget = new QListWidget(mDock);

    mDock->setWidget(mLayerListWidget);
    addDockWidget(Qt::LeftDockWidgetArea, mDock);

    resizeDocks({mDock}, {120}, Qt::Horizontal);

    mOASISView = new OASISView();
    setCentralWidget(mOASISView);
}

MainWindow::~MainWindow()
{
    delete mOpenAction;
    delete mFileMenu;
    delete mCellNameComboBox;
    delete mDock;
    delete mLayerListWidget;
    delete mOASISView;
}

void MainWindow::openFileClicked() {
    QString filename = QFileDialog::getOpenFileName();
    qDebug() << "Open File:" << filename;
    mOASISData.parse(filename.toStdString());
    qDebug() << "Finished";
    vector<unsigned> layers = mOASISData.getLayers();
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

void MainWindow::cellNameComboBoxChanged(QString cellName) {
    drawCell(cellName.toStdString());
}

void MainWindow::drawCell(string cellName) {
    OASISParser::OASISCell* cell = mOASISData.getCell(cellName);
    mOASISView->updateCell(cell);
}