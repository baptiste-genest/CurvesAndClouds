#ifndef COMPLEX_ANALYSIS_H
#define COMPLEX_ANALYSIS_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif


#include "calculus.h"

namespace cnc {

namespace algo {

namespace calculus {

namespace complex {

std::vector<complex_scalar> roots_of_unity(uint n,scalar offset = scalar(0));

}

}

}

}

#endif // COMPLEX_ANALYSIS_H
