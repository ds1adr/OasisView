#include "MainWindow.h"

#include <QAction>
#include <QDockWidget>
#include <QFileDialog>
#include <QMenuBar>
#include <QToolBar>

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

    mDock = new QDockWidget("", this);
    mDock->setAllowedAreas(Qt::LeftDockWidgetArea);

    mLayerListWidget = new QListWidget(mDock);
    QListWidgetItem* item = new QListWidgetItem("Layer 1", mLayerListWidget);
    item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    item->setCheckState(Qt::Checked);

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
    delete mOASISView;
}

void MainWindow::openFileClicked() {
    QString filename = QFileDialog::getOpenFileName();
    qDebug() << "Open File:" << filename;
    mOASISData.parse(filename.toStdString());
    qDebug() << "Finished";
}
