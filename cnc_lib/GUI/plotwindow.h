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
#include "plots/plot_group.h"

#include <QTabWidget>
#include <QLabel>
#include <vector>
#include <QString>
//#include <QDesktopWidget>
#include <QResizeEvent>
#include <QApplication>

namespace cnc {

class MutableValue;
class MutScalar;
/*! \class Plot_window
 *  \brief Base class that instanciates a Qt window and allows displaying plots
 */
class PlotWindow : public QMainWindow
{
    Q_OBJECT;

public:

    PlotWindow();

    ~PlotWindow();

    /**
     * @brief add_tab adds a new tab to the window
     * @return pointer to it
     */
    PlotTab* add_tab(const QString&);
    MutScalar add_mutable_scalar_by_cursor(range v,const QString&,uint = 100);

    MutScalar get_time_variable(uint tickrate);

    int run();

protected:
    QTabWidget* tabs = nullptr;
    std::vector<PlotTab*> tabs_list;

    QVBoxLayout* sliders_box = nullptr;
    QWidget* sliders_panel = nullptr;
    std::vector<QLabel*> sliders_text;

private:
    void resizeEvent(QResizeEvent* event) override;

    friend class MutableValueCursor;
    friend class MutableValueTicker;

    bool init = false;

    std::vector<MutableValue*> mv;


private slots:
    void call_resize();
};

}
#endif // PLOTWINDOW_H
