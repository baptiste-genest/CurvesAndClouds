#include "cncmainwindow.h"

CNCMainWindow::CNCMainWindow(){
    cnc::Plot_window();
    createActions();
    createMenus();
    this->tabs->setGeometry(0, 20, 500, 520);
}

void CNCMainWindow::createActions(){
    // Basic Menu
    // ## newPointsCloud
    newPointsCloudAct = new QAction(tr("Points Cloud"), this);
    newPointsCloudAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_P));
    newPointsCloudAct->setStatusTip(tr("Creates a new tab with a points cloud"));
    connect(newPointsCloudAct, &QAction::triggered, this, &CNCMainWindow::newPointsCloud);
    // ## newFunctionPlot
    newFunctionPlotAct = new QAction(tr("Function Plot"), this);
    newFunctionPlotAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_F));
    newFunctionPlotAct->setStatusTip(tr("Creates a new tab with a function plot"));
    connect(newFunctionPlotAct, &QAction::triggered, this, &CNCMainWindow::newFunctionPlot);
    // ## newBlankTabAct
    newBlankTabAct = new QAction(tr("Blank"), this);
    newBlankTabAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_B));
    newBlankTabAct->setStatusTip(tr("Creates a blank tab"));
    connect(newBlankTabAct, &QAction::triggered, this, &CNCMainWindow::newBlankTab);
    // # exportPdf
    exportPdfAct = new QAction(tr("Export as PDF"), this);
    exportPdfAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_E));
    exportPdfAct->setStatusTip(tr("Export all the plots as a pdf file"));
    connect(exportPdfAct, &QAction::triggered, this, &CNCMainWindow::exportPdf);
    // # quit
    quitAct = new QAction(tr("Quit"), this);
    quitAct->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q));
    quitAct->setStatusTip(tr("Quit the application, don't forget to be king with Rodrigo"));
    connect(quitAct, &QAction::triggered, this, &CNCMainWindow::quit);

    // Points cloud Menu
    // # importData
    importDataAct = new QAction(tr("Import Data"), this);
    importDataAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_I));
    importDataAct->setStatusTip(tr("Import your data as CSV"));
    connect(importDataAct, &QAction::triggered, this, &CNCMainWindow::importData);
    // # addPointsCloudInTab
    addPointsCloudInTabAct = new QAction(tr("Add a Points Cloud"), this);
    addPointsCloudInTabAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_P));
    addPointsCloudInTabAct->setStatusTip(tr("Add a points cloud to your tab"));
    connect(addPointsCloudInTabAct, &QAction::triggered, this, &CNCMainWindow::addPointsCloudInTab);

    // Function plot Menu
    // # exportData
    exportDataAct = new QAction(tr("Export Data"), this);
    exportDataAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_E));
    exportDataAct->setStatusTip(tr("Export your datas as an CSV file"));
    connect(exportDataAct, &QAction::triggered, this, &CNCMainWindow::exportData);
    // # addFunctionPlotInTab
    addFunctionPlotInTabAct = new QAction(tr("Add a Function Plot"), this);
    addFunctionPlotInTabAct->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_F));
    addFunctionPlotInTabAct->setStatusTip(tr("Add a function plot to your tab"));
    connect(addFunctionPlotInTabAct, &QAction::triggered, this, &CNCMainWindow::addFunctionPlotInTab);
}

void CNCMainWindow::createMenus(){
    // Basic Menu
    basicMenu = menuBar()->addMenu(tr("Basic"));
    // # newTab Menu
    newTab = basicMenu->addMenu(tr("New Tab..."));
    newTab->addAction(newPointsCloudAct);
    newTab->addAction(newFunctionPlotAct);
    newTab->addAction(newBlankTabAct);
    //
    basicMenu->addSeparator();
    basicMenu->addAction(exportPdfAct);
    basicMenu->addSeparator();
    basicMenu->addAction(quitAct);

    // PointsCloud Menu
    pointsCloudMenu = menuBar()->addMenu(tr("Points Cloud"));
    pointsCloudMenu->addAction(addPointsCloudInTabAct);
    pointsCloudMenu->addSeparator();
    pointsCloudMenu->addAction(importDataAct);

    // FunctionPlot Menu
    functionPlotMenu = menuBar()->addMenu(tr("Function Plot"));
    functionPlotMenu->addAction(addFunctionPlotInTabAct);
    functionPlotMenu->addSeparator();
    functionPlotMenu->addAction(exportDataAct);
}

#ifndef QT_NO_CONTEXTMENU
void CNCMainWindow::contextMenuEvent(QContextMenuEvent *event){
    QMenu menu(this);
    menu.exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU

void CNCMainWindow::newPointsCloud(){
    bool ok;
    QString text = QInputDialog::getText(this, tr("Name for Points' Cloud tab:"), tr("Name of the tab who want to create:"), QLineEdit::Normal, QDir::home().dirName(), &ok);

    if (ok && !text.isEmpty()){
        cnc::Plot_tab* tab = this->add_tab(text);
        /*
        cnc::Plot_frame* frame = tab->add_frame();
        cnc::Plot_layer* layer = frame->add_layer();
        layer->new_figure_from_file("../../data/rodrigo.jpg");
        cnc::tex_stream ts;
        ts << text.toStdString() << cnc::tex::endl;
        ts << "Points' Cloud tab !!!" << cnc::tex::endl;
        layer->add_text_frame_from_tex_stream(ts);
        */
    }
}

void CNCMainWindow::newFunctionPlot(){
    bool ok;
    QString text = QInputDialog::getText(this, tr("Name for Function Plot tab:"), tr("Name of the tab who want to create:"), QLineEdit::Normal, QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty()){
        cnc::Plot_tab* tab = this->add_tab(text);
        cnc::Plot_frame* frame = tab->add_frame();
        cnc::Plot_layer* layer = frame->add_layer();
        layer->new_figure_from_file("../../data/rodrigo.jpg");
        cnc::tex_stream ts;
        ts << text.toStdString() << cnc::tex::endl;
        ts << "Function Plot tab !!!" << cnc::tex::endl;
        layer->add_text_frame_from_tex_stream(ts);
    }
}

void CNCMainWindow::newBlankTab(){
    bool ok;
    QString text = QInputDialog::getText(this, tr("Name for Blank tab:"), tr("Name of the tab who want to create:"), QLineEdit::Normal, QDir::home().dirName(), &ok);
    if (ok && !text.isEmpty()){
        cnc::Plot_tab* tab = this->add_tab(text);
        cnc::Plot_frame* frame = tab->add_frame();
        cnc::Plot_layer* layer = frame->add_layer();
        layer->new_figure_from_file("../../data/rodrigo.jpg");
        cnc::tex_stream ts;
        ts << text.toStdString() << " tab !!!" << cnc::tex::endl;
        layer->add_text_frame_from_tex_stream(ts);
    }
}

void CNCMainWindow::exportPdf(){

}

void CNCMainWindow::quit(){
    this->close();
}

void CNCMainWindow::importData(){
    cnc::Plot_tab* tab;
    cnc::cloud c;
    QString fileName;
    QMessageBox msgBox;

    try {
        fileName = QFileDialog::getOpenFileName(this,
            tr("Open .csv file"), "/home", tr("*.csv;; All files (*)"));
        c = cnc::algo::parse_csv(fileName.toStdString());
        auto pc = cnc::algo::stat::project_into_2D(c);
        if (this->tabs->currentIndex() != 0){
            tab = this->tabs_list[this->tabs->currentIndex()];
        } else {
            newPointsCloud();
            tab = this->tabs_list[this->tabs_list.size()-1];
        }
        cnc::Plot_frame* frame = tab->add_frame(2, 2);
        frame->add_layer()->new_point_cloud(c)->set_axis_policy(cnc::min_max_range);
    } catch (Cnc_error& e){
        msgBox.setText("The file cannot be imported.");
        QString msg("We could not import your file because: ");
        msgBox.setInformativeText(msg.append(e.what().c_str()));
        msgBox.exec();
    }
}

void CNCMainWindow::addPointsCloudInTab(){
    //if (this->tabs->)
    std::cout << this->tabs->currentIndex() << std::endl;
}

void CNCMainWindow::exportData(){

}

void CNCMainWindow::addFunctionPlotInTab(){

}
