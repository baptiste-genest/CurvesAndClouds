#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "cnc_types.h"
#include "algo/lin_alg.h"
#include <vector>

namespace cnc {

class SparseMatrix
{
public:
    SparseMatrix(int dimension,bool is_symetric = false);
    SparseMatrix();

    void new_row();
    void add_in_row(int i,scalar x);
    void end_construct();

    vec conjugate_gradient(const vec& x0,const vec& b,float eps) const;
    vec conjugate_gradient(const vec& b,float eps) const;
    vec mult(const vec& x) const;

    inline uint get_current_index(){
        return v.size()-1;
    }
    inline void set_at_index(int i,scalar x) {
        v[i] = x;
    }

    inline vec operator*(const vec& x) const {
        return mult(x);
    }

    scalar row_sum(uint j) const;
private:
    int n;
    int nnz;
    std::vector<int> rowptr;
    std::vector<int> colind;
    std::vector<scalar> v;
    bool symetric;

};

using smat = SparseMatrix;

}

#endif // SPARSE_MATRIX_H
