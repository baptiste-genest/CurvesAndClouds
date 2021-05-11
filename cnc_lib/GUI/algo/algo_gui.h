/**
  * @file algo_gui.h
  *
  * All algorithms and types related to GUI computation (mainly send points from their mathematical spaces to the screen)
  *
*/
#ifndef ALGO_GUI_H
#define ALGO_GUI_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "core/algo/algo.h"
#include "cnc_types.h"
#include "core/algo/statistics/statistics.h"
#include "GUI/display_info.h"

#include <algorithm>

#include <QString>
#include <QPoint>
#include <QGuiApplication>
#include <QScreen>
#include <string>
#include <sstream>
#include <iomanip>

namespace cnc{

/** @brief Namespace for algorithms based on Qt UI functions and types
 *
 */
namespace algo_GUI
{

/**
 * @brief Qcloud cloud of QPointF
 */
typedef std::vector<QPointF> Qcloud;

QPointF vec_to_QPointF(const cnc::vec& x);

/**
 * @brief convert_cloud_to_qcloud convert 2D chaskal vector cloud to qpoint cloud
 * @return
 */
Qcloud convert_cloud_to_qcloud(const cloud&);

std::pair<range,range> compute_cloud_bounds(const Qcloud&);

/**
 * @brief mapping take a QPoint in value range to map it to the screen
 */
typedef std::function<QPoint(const QPointF&)> mapping;

/**
 * @brief gen_mapping_value_to_frame generates a mapper (a function) which takes a 2D mathematical point and project it into the screen
 * @param x_val_range range of possible x values for a point
 * @param y_val_range range of possible Y values for a point
 * @param fi information about the frame to project properly
 * @return said mapper
 */
mapping gen_mapping_value_to_frame(const range& x_val_range,const range& y_val_range, const frame_info& fi);

/**
 * @brief A function to convert a float to a qstring value
 * @param number the float we want to convert in QString
 * @param precision the number of precision we want in our number
 * @return
 */
QString to_qstring(float number, int precision);

/**
 * @brief A function to convert points to pixels
 * @param points the number of points we want to convert in pixels
 * @return
 */
double point_to_pixel(double points);

QColor vec_to_color(const cnc::vec& v);

typedef std::function<QColor(uint,uint)> texture_function;

}

}

#endif // ALGO_GUI_H
