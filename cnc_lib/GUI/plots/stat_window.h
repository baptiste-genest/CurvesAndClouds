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
class Stat_list{
public:
    std::vector<std::vector<bool>> P;

    bool operator()(plot_type a,int b) const {
        return P[(uint)a][b];
    }
};

/**
 * @brief The Stat_displayer class is a sub widget of a stat window to duplicate a plot frame and possibly add new plots over it
 */
class Stat_displayer : public QWidget {
    Q_OBJECT
public:
    Stat_displayer(Plot_frame*,QWidget*,const Stat_list& SL);

private:
    Plot_frame* ref;
    QWidget* QSL_parent;
    QStackedWidget* QSL;

    friend class Stat_window;
};


/**
 * @brief The Stat_window class is the window displaying plot frames duplicata and numerical informations
 */
class Stat_window : public QWidget
{
Q_OBJECT
public:
    Stat_window(Plot_frame* r,const Stat_list& p);

protected:
    void paintEvent(QPaintEvent*);

private:
    void init_window();

    bool init;
    QHBoxLayout* QHL;
    Plot_frame* FD;
    Plot_frame* ref;
    Stat_list SL;
    Stat_displayer* SD;
};

}

#endif // STAT_WINDOW_H
