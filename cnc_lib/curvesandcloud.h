/**
  * @file curvesandcloud.h main file to include all the modules, requires Qt
  */
#ifndef CURVESANDCLOUD_H
#define CURVESANDCLOUD_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "curvesandcloudCore.h"
#include <QApplication>
#include "GUI/plotwindow.h"
#include "GUI/plots/formula_tex.h"
#include "GUI/algo/algo_gui.h"
#include "GUI/algo/mutablevalue.h"
#include "GUI/plots/3D/scene_viewer.h"

#endif // CURVESANDCLOUD_H
