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
#include "fstream"
#include "core/algo/lin_alg.h"
#include "core/algo/statistics/statistics.h"
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
std::vector<std::string> split(const std::string& str, char delim);
bool is_integer(const std::string& s);

void export_cloud(const std::string& output_file,const cloud& C);
}

}


#endif // INOUT_H
