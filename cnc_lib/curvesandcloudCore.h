/**
  * @file curvesandcloudCore.h includes all the algorithms/math structure, doesn't need Qt
  */
#ifndef CURVESANDCLOUDCORE_H
#define CURVESANDCLOUDCORE_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "cnc_types.h"
#include "cnc_error.h"
#include "core/algo/lin_alg.h"
#include "core/algo/matrix_functions.h"
#include "core/algo/algo.h"
#include "core/algo/inout.h"
#include "core/quaternion.h"
#include "core/algo/geometry/rotations.h"
#include "core/algo/geometry/geometry.h"
#include "core/algo/geometry/combinatorial_complexes.h"
#include "core/algo/geometry/convex_hull.h"
#include "core/algo/geometry/diagrams/diagrams.h"
#include "core/algo/bit_operations.h"
#include "core/algo/calculus/calculus.h"
#include "core/algo/calculus/finite_differences.h"
#include "core/algo/calculus/integration_scheme.h"
#include "core/algo/calculus/lagrange_interpolator.h"
#include "core/algo/calculus/vector_calculus.h"
#include "core/algo/calculus/differential_equations.h"
#include "core/algo/calculus/complex_analysis.h"
#include "core/algo/calculus/finite_elements/finite_elements.h"
#include "core/algo/statistics/random_var.h"
#include "core/algo/statistics/statistics.h"
#include "core/algo/calculus/optimization.h"
#include "core/algo/graph/graph.h"
#include "core/algo/preconditionners.h"
#include "core/algo/calculus/fourier.h"
#include "core/sparse_matrix.h"
#include "core/time_handling.h"
#include "core/algo/geometry/polynomial_curve.h"
#include "core/dual_numbers.h"

#endif // CURVESANDCLOUDCORE_H
