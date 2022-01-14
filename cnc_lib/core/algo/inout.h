/**
  * @file inout.h file reading/writing and parsing algorithms
  */
#ifndef INOUT_H
#define INOUT_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "iostream"
#include "iomanip"
#include "fstream"
#include "core/algo/linear_algebra/lin_alg.h"
#include "core/algo/statistics/statistics.h"
#include "core/algo/geometry/simpleglmesh.h"
#include "core/algo/geometry/mesh2.h"
#include <QImage>
#include <regex>

namespace cnc {

namespace algo {

/**
 * @brief parse_from_uchar sample a cloud of data with coords in a file written in uchar
 * @param filename file to parse
 * @param chunk_size size of each data chunk
 * @param max_size possibly to truncate number of data
 * @return parsed cloud
 */
cloud parse_from_uchar(const std::string& filename,uint chunk_size,int max_size = -1);

cloud parse_csv(const std::string& filename);
std::vector<std::string> split(const std::string& str, char delim,bool non_void = false);
bool is_integer(const std::string& s);

void export_cloud(const std::string& output_file,const cloud& C);

algo::geometry::SimpleGLMesh import_mesh_from_obj(const std::string& input_file,float scale = 1.f);
void export_mesh_as_obj(const algo::geometry::SimpleGLMesh* m,std::string out_file);

std::array<vec,3> load_img_as_vec(const std::string& filename,uint& w,uint& h);

scalar stos(std::string x);

void export_mesh2(const algo::geometry::Mesh2& m,std::string out_file);
algo::geometry::MeshRef import_mesh2(std::string in_file);

}

}


#endif // INOUT_H
