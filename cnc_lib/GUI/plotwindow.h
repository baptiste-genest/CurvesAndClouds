/**
  * @file plotwindow.h main class of the lib, starting point to plot!
  */
#ifndef PLOTWINDOW_H
#define PLOTWINDOW_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include <QMainWindow>
#include "plots/plot_tab.h"

#include <QTabWidget>
#include <vector>
#include <QString>
//#include <QDesktopWidget>
#include <QResizeEvent>

namespace cnc {

/*! \class Plot_window
 *  \brief Base class that instanciates a Qt window and allows displaying plots
 */
class Plot_window : public QMainWindow
{
    Q_OBJECT;

public:

    Plot_window();

    ~Plot_window();

    /**
     * @brief add_tab adds a new tab to the window
     * @return pointer to it
     */
    Plot_tab* add_tab(const QString&);

protected:
    QTabWidget* tabs = nullptr;
    std::vector<Plot_tab*> tabs_list;

private:
    //std::vector<Plot_tab*> tabs_list;
    void resizeEvent(QResizeEvent* event);

    bool init = false;

private slots:
    void call_resize();
};

}
#endif // PLOTWINDOW_H
