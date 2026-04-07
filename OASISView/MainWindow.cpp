#include "MainWindow.h"

#include <QAction>
#include <QFileDialog>
#include <QMenuBar>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    mOpenAction = new QAction(QIcon(""), tr("&Open"), this);
    mOpenAction->setShortcut(QKeySequence::Open);
    mOpenAction->setStatusTip(tr("Open an OASIS file"));
    connect(mOpenAction, SIGNAL(triggered()), this, SLOT(openFileClicked()));

    mFileMenu = menuBar()->addMenu(tr("&File"));
    mFileMenu->addAction(mOpenAction);

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
}
