#ifndef CNCMAINWINDOW_H
#define CNCMAINWINDOW_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "curvesandcloud.h"
#include <QActionGroup>
#include <QAction>
#include <QMenuBar>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QDir>

class CNCMainWindow : public cnc::Plot_window {
    Q_OBJECT;

public:
    CNCMainWindow();

protected:
#ifndef QT_NO_CONTEXTMENU
    void contextMenuEvent(QContextMenuEvent *event) override;
#endif // QT_NO_CONTEXTMENU

private:
    void createActions();
    void createMenus();
    QMenu *basicMenu;
    QMenu *pointsCloudMenu;
    QMenu *functionPlotMenu;
    QMenu *newTab;
    QAction *newPointsCloudAct;
    QAction *newFunctionPlotAct;
    QAction *newBlankTabAct;
    QAction *exportPdfAct;
    QAction *quitAct;
    QAction *importDataAct;
    QAction *addPointsCloudInTabAct;
    QAction *exportDataAct;
    QAction *addFunctionPlotInTabAct;

private slots:
    void newPointsCloud();
    void newFunctionPlot();
    void newBlankTab();
    void exportPdf();
    void quit();
    void importData();
    void addPointsCloudInTab();
    void exportData();
    void addFunctionPlotInTab();
};

#endif // CNCMAINWINDOW_H
