#include <QtGui>
#include "mainwindow.h"
#include "finddialog.h"
#include "gotocelldialog.h"
#include "sortdialog.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QMessageBox>
#include <QFileDialog>
#include <QScrollArea>
#include "spreadsheet.h"
MainWindow::MainWindow()
{
    vectorCanvas=new VectorCanvas();
    qscrollarea=new QScrollArea();
    spreadsheet=new Spreadsheet();
    setCentralWidget(spreadsheet);

    createActions();
    createMenus();
    createContextMenu();
    createToolBars();
    createStatusBar();

    readSettings();

    findDialog=0;

    setWindowIcon(QIcon(":/images/images/go.png"));
    setCurrentFile("");

}

MainWindow::~MainWindow()
{

}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(okToContinue()){
        writeSettings();
        event->accept();
    }
    else{
        event->ignore();
    }
    qDebug()<<"close";
}

void MainWindow::newFile()
{
    if(okToContinue()){
        spreadsheet->clear();
        setCurrentFile("");
    }else{
       qDebug()<<"you should save ...";
    }

}

void MainWindow::open()
{
    if(okToContinue()){
        QString fileName=QFileDialog::getOpenFileName(this,tr("Open Spreadsheet"),".",tr("Spreadsheet files (* .sp)"));
        if(!fileName.isEmpty()){
            loadFile(fileName);
        }
    }
}

bool MainWindow::save()
{
    if(curFile.isEmpty()){
        return saveAs();
    }
    else{
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName=QFileDialog::getSaveFileName(this,tr("Save Spreadsheet"),".",tr("Spreadsheet files (*.sp)"));
    if(fileName.isEmpty())
        return false;
    return saveFile(fileName);
}

void MainWindow::goToCell()
{

}

void MainWindow::find()
{
    if(!findDialog){
        findDialog=new FindDialog();
        connect(findDialog,SIGNAL(findNext(QString,Qt::CaseSensitivity)),spreadsheet,SLOT(findNext(QString,Qt::CaseSensitivity)));
        connect(findDialog,SIGNAL(findPrevious(QString,Qt::CaseSensitivity)),spreadsheet,SLOT(findPrevious(QString,Qt::CaseSensitivity)));
        qDebug()<<"construct findDialog";
    }
    findDialog->show();
    findDialog->raise();
    findDialog->activateWindow();
}

void MainWindow::sort()
{

}

void MainWindow::about()
{

}



//void MainWindow::selectAll()
//{
//    qDebug()<<"all";
//}

void MainWindow::close()
{

}

void MainWindow::onDraw()
{
//    qscrollarea->setWidget(vectorCanvas);
//    qscrollarea->viewport()->setBackgroundRole(QPalette::Dark);
//    qscrollarea->viewport()->setAutoFillBackground(true);
//    qscrollarea->setWindowTitle(tr("canvas"));
//    qscrollarea->show();
    vectorCanvas->show();
}

void MainWindow::openRecentFile()
{

}

void MainWindow::upadteStatusBar()
{

}

void MainWindow::spreadsheetModified()
{

}

void MainWindow::createActions()
{
    //file actions
    newAction=new QAction(tr("&New"),this);
    newAction->setIcon(QIcon(":/images/go"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new spreadsheet file"));
    connect(newAction,SIGNAL(triggered(bool)),this,SLOT(newFile()));

    openAction=new QAction(tr("&Open"),this);
    openAction->setIcon(QIcon(""));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open a spreadsheet file"));
    connect(openAction,SIGNAL(triggered(bool)),this,SLOT(open()));

    saveAction=new QAction(tr("&Save"),this);
    saveAction->setIcon(QIcon(""));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save a spreadsheet file"));
    connect(saveAction,SIGNAL(triggered(bool)),this,SLOT(save()));

    saveAsAction=new QAction(tr("&SaveAs"),this);
    saveAsAction->setIcon(QIcon(""));
    saveAsAction->setShortcut(QKeySequence::Open);
    saveAsAction->setStatusTip(tr("Open a spreadsheet file"));
    connect(saveAsAction,SIGNAL(triggered(bool)),this,SLOT(saveAs()));

    for(int i=0;i<MaxRecentFiles;++i){
        recentFileActions[i]=new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i],SIGNAL(triggered(bool)),this,SLOT(openRecentFile()));
    }



    exitAction=new QAction(tr("E&xit"),this);
    exitAction->setShortcut(tr("Ctrl+Q"));
    exitAction->setStatusTip(tr("Exit the application"));
    connect(exitAction,SIGNAL(triggered(bool)),this,SLOT(closeEvent()));

    drawAction=new QAction(tr("&Draw"),this);
    drawAction->setShortcut(tr("Ctrl+D"));
    drawAction->setStatusTip(tr("open the draw canvas"));
    connect(drawAction,SIGNAL(triggered(bool)),this,SLOT(onDraw()));

    showGridAction=new QAction(tr("&Show Grid"),this);
    showGridAction->setCheckable(true);
    showGridAction->setStatusTip(tr("Show or hide the spreadsheet's grid"));
//    connect(showGridAction,SIGNAL(toggled(bool)),spreadsheet,SLOT(setShowGrid(bool)));

    aboutQtAction=new QAction(tr("About &Qt"),this);
    aboutQtAction->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction,SIGNAL(triggered(bool)),qApp,SLOT(aboutQt()));

    aboutAction=new QAction(tr("About &The Application"),this);
    aboutAction->setStatusTip(tr("Show  What is the application"));
//    connect(aboutQtAction,SIGNAL(triggered(bool)),qApp,SLOT(aboutQt()));

    //********edit action areas***********//
    cutAction=new QAction(tr("Cu&t"),this);
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("cut..."));
    connect(cutAction,SIGNAL(triggered(bool)),spreadsheet,SLOT(cut()));

    copyAction=new QAction(tr("&Copy"),this);
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("copy..."));
    connect(copyAction,SIGNAL(triggered(bool)),spreadsheet,SLOT(copy()));

    pasteAction=new QAction(tr("&Paste"),this);
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("Paste..."));
    connect(pasteAction,SIGNAL(triggered(bool)),spreadsheet,SLOT(paste()));

    deleteAction=new QAction(tr("&Delete"),this);
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("Delete..."));
    connect(deleteAction,SIGNAL(triggered(bool)),spreadsheet,SLOT(del()));
//    connect(selectAllAction,SIGNAL(triggered(bool)),spreadsheet,SLOT(selectAll()));
    selectRowAction=new QAction(tr("Row"),this);
    selectRowAction->setShortcut(tr("Ctrl+R"));
    selectRowAction->setStatusTip(tr("Select Row"));
    connect(selectRowAction,SIGNAL(triggered(bool)),spreadsheet,SLOT(selectCurrentRow()));

    selectColumnAction=new QAction(tr("Col"),this);
//    selectColumnAction->setShortcut(tr("Ctrl+Q"));
    selectColumnAction->setStatusTip(tr("Select Col"));
    connect(selectColumnAction,SIGNAL(triggered(bool)),spreadsheet,SLOT(selectCurrentColumn()));

    selectAllAction=new QAction(tr("All"),this);
    selectAllAction->setShortcut(tr("Ctrl+A"));
    selectAllAction->setStatusTip(tr("Exit the application"));
    connect(selectAllAction,SIGNAL(triggered(bool)),spreadsheet,SLOT(selectAll()));

    findAction=new QAction(tr("find"),this);
//    findAction->setShortcut(tr("Ctrl+Q"));
    findAction->setStatusTip(tr("find ...."));
    connect(findAction,SIGNAL(triggered(bool)),this,SLOT(find()));

    goToCellAction=new QAction(tr("goToCell"),this);
//    goToCellAction->setShortcut(tr("Ctrl+Q"));
    goToCellAction->setStatusTip(tr("go to cell..."));
//    connect(exitAction,SIGNAL(triggered(bool)),this,SLOT(close()));
    //********edit action areas end***********//

    sortAction=new QAction(tr("sort"),this);
//    goToCellAction->setShortcut(tr("Ctrl+Q"));
    sortAction->setStatusTip(tr("sort..."));
//    connect(exitAction,SIGNAL(triggered(bool)),this,SLOT(close()));

    recalculateAction=new QAction(tr("recalculate"),this);
//    goToCellAction->setShortcut(tr("Ctrl+Q"));
    recalculateAction->setStatusTip(tr("recalculate..."));
//    connect(exitAction,SIGNAL(triggered(bool)),this,SLOT(close()));

    autoRecalcAction=new QAction(tr("autoRecalc"),this);
//    goToCellAction->setShortcut(tr("Ctrl+Q"));
    autoRecalcAction->setStatusTip(tr("autoRecalc..."));
//    connect(exitAction,SIGNAL(triggered(bool)),this,SLOT(close()));

    penColorAction=new QAction(tr("penColor"),this);
    connect(penColorAction,SIGNAL(triggered(bool)),vectorCanvas,SLOT(onClickColor()));
}
void MainWindow::createMenus()
{
    fileMenu=menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAsAction);
    fileMenu->addAction(exitAction);
    separatorAction=fileMenu->addSeparator();
    for(int i=0;i<MaxRecentFiles;i++){
        fileMenu->addAction(recentFileActions[i]);
    }
    fileMenu->addSeparator();
    fileMenu->addAction(drawAction);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAction);

    editMenu=menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(deleteAction);

    selectSubMenu=editMenu->addMenu(tr("&Select"));
    selectSubMenu->addAction(selectRowAction);
    selectSubMenu->addAction(selectColumnAction);
    selectSubMenu->addAction(selectAllAction);

    editMenu->addSeparator();
    editMenu->addAction(findAction);
    editMenu->addAction(goToCellAction);

    toolsMenu=menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(recalculateAction);
    toolsMenu->addAction(sortAction);

    optionsMenu=menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(showGridAction);
    optionsMenu->addAction(autoRecalcAction);

    menuBar()->addSeparator();

    helpMenu=menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);



}

void MainWindow::createContextMenu()
{
    spreadsheet->addAction(cutAction);
    spreadsheet->addAction(copyAction);
    spreadsheet->addAction(pasteAction);
    spreadsheet->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
    fileToolBar=addToolBar(tr("&File"));
    fileToolBar->addAction(cutAction);
    fileToolBar->addAction(copyAction);
    fileToolBar->addAction(pasteAction);

}

void MainWindow::createStatusBar()
{
    locationLabel=new QLabel("W999");
    locationLabel->setAlignment((Qt::AlignHCenter));
    locationLabel->setMinimumSize(locationLabel->sizeHint());

    formulaLabel=new QLabel;
    formulaLabel->setIndent(3);

    statusBar()->addWidget(locationLabel);
    statusBar()->addWidget(formulaLabel,1);

//    connect(spreadsheet,SIGNAL(currentCellChanged(int,int,int,int)),this,SLOT(updateStatusBar()));
//    connect(spreadsheet,SIGNAL(modified()),this,SLOT(spreadsheetModified()));

//    updateStatusBar();

}

void MainWindow::readSettings()
{

}

void MainWindow::writeSettings()
{

}

bool MainWindow::okToContinue()
{
    if(isWindowModified()){
        int r=QMessageBox::warning(this,tr("Spreadsheet"),tr("The document has Modified.\n Do you want to Save your changes?"),QMessageBox::Yes|QMessageBox::No|QMessageBox::Cancel);
        if(r==QMessageBox::Yes){
            return save();
        }
        else if(r==QMessageBox::Cancel){
            return false;
        }
    }
    return true;
}

bool MainWindow::loadFile(const QString &fileName)
{
    if(!spreadsheet->readFile(fileName)){
        statusBar()->showMessage(tr("Loading canceled"),2000);
        return false;
    }
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"),2000);
    return true;
}

bool MainWindow::saveFile(const QString &fileName)
{
    if(!spreadsheet->writeFile(fileName)){
        statusBar()->showMessage(tr("Saving Canceled"),2000);
        return false;
    }
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"),2000);
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile=fileName;
    setWindowModified(false);

    QString showName=tr("Untitled");
    if(!curFile.isEmpty()){
        showName=strippedName(curFile);
        recentFiles.removeAll(curFile);
        recentFiles.prepend(curFile);
        upadteRecentFileActions();
    }

    setWindowTitle(tr("%1[*]-%2").arg(showName).arg(tr("Spreadsheet")));
}

void MainWindow::updateRecentFileActions()
{

}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::upadteRecentFileActions()
{

}
