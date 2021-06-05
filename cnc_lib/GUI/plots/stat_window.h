/**
  *
  * @file stat_window.h
  *
  * defines Stat window class which allows to get various numerical data from plot
  */
#ifndef STAT_WINDOW_H
#define STAT_WINDOW_H

#include "cnc_types.h"
#include "cnc_error.h"
#include "plot_frame.h"

#include "QTimer"
#include "QMainWindow"
#include "QHBoxLayout"
#include "QStackedWidget"
#include "QStackedLayout"
#include "QLabel"

namespace cnc {

/**
 * @brief The Stat_list class exists to transmit selected data type to the stat displayer
 */
class StatList{
public:
    std::vector<std::vector<bool>> P;

    bool operator()(plot_type a,int b) const {
        return P[(uint)a][b];
    }
};

/**
 * @brief The Stat_displayer class is a sub widget of a stat window to duplicate a plot frame and possibly add new plots over it
 */
class StatDisplayer : public QWidget {
    Q_OBJECT
public:
    StatDisplayer(PlotFrame*,QWidget*,const StatList& SL);

private:
    PlotFrame* ref;
    QWidget* QSL_parent;
    QStackedWidget* QSL;

    friend class StatWindow;
};


/**
 * @brief The Stat_window class is the window displaying plot frames duplicata and numerical informations
 */
class StatWindow : public QWidget
{
Q_OBJECT
public:
    StatWindow(PlotFrame* r,const StatList& p);

protected:
    void paintEvent(QPaintEvent*);

private:
    void init_window();

    bool init;
    QHBoxLayout* QHL;
    PlotFrame* FD;
    PlotFrame* ref;
    StatList SL;
    StatDisplayer* SD;
};

}

#endif // STAT_WINDOW_H
