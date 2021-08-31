#ifndef MESH_BANK_H
#define MESH_BANK_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "cnc_types.h"
#include <vector>

namespace cnc {

struct mesh_bundle{
    std::string filename;
    scalar scale_factor;
};

const static std::string data_prefix = "../../data/";

enum bank_mesh_names {
    venus,
    stanford_bunny,
    woman_head,
    horse,
    face_mask
};

const static std::vector<mesh_bundle> MESH_BANK = {
    {"venus.obj",0.6},
    {"bunny.obj",0.6},
    {"f_head.obj",0.1},
    {"horse.obj",0.03},
    {"nefertiti.obj",0.6}
};

}



#endif // MESH_BANK_H
