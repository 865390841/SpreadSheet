#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <vectorcanvas.h>
#include <QAction>
#include <QToolBar>

class QAction;
class QLabel;
class FindDialog;
class Spreadsheet;
class QScrollArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();
protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void open();
    bool save();
    bool saveAs();
    void goToCell();
    void find();
    void sort();
    void about();
//    void selectAll();
    void close();
    void onDraw();
    void openRecentFile();
    void upadteStatusBar();
    void spreadsheetModified();

private:
    void createActions();
    void createMenus();
    void createContextMenu();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool okToContinue();
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);
    void upadteRecentFileActions();

    Spreadsheet *spreadsheet;
    VectorCanvas *vectorCanvas;
    QScrollArea *qscrollarea;
    FindDialog *findDialog;
    QLabel *locationLabel;
    QLabel *formulaLabel;
    QStringList recentFiles;
    QString curFile;

    enum {MaxRecentFiles=5};
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *selectSubMenu;
    QMenu *toolsMenu;
    QMenu *optionsMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;   

    //file action
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exitAction;

    QAction *showGridAction;
    QAction *drawAction;

    //edit action
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAction;
    QAction *selectRowAction;
    QAction *selectColumnAction;
    QAction *selectAllAction;
    QAction *findAction;
    QAction *goToCellAction;

    //tools action
    QAction *sortAction;
    QAction *recalculateAction;
    QAction *autoRecalcAction;

    //help action
    QAction *aboutAction;
    QAction *aboutQtAction;

    //othrt action
    QAction *penColorAction;

};

#endif // MAINWINDOW_H