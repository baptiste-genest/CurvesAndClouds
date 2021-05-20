#ifndef MESH_H
#define MESH_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "triangle.h"

#include "core/algo/algo.h"
#include "core/algo/lin_alg.h"
#include "cnc_error.h"
#include "cnc_types.h"

namespace cnc {

namespace algo {

namespace calculus {

namespace finite_elements{

class Mesh
{
public:
    Mesh();

private:
    constexpr static uint dim = 2;
};

}}}}


#endif // MESH_H
