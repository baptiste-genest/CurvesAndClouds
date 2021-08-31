#ifndef SPARSE_MATRIX_H
#define SPARSE_MATRIX_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "cnc_types.h"
#include "algo/lin_alg.h"
#include "algo/algo.h"
#include "cnc_error.h"
#include <vector>

namespace cnc {

class SparseMatrix;

using smat = SparseMatrix;
namespace algo{
    std::pair<SparseMatrix,SparseMatrix> set_known_variables(const SparseMatrix& M,const std::vector<uint>& id);
    vec solve_for_kernel_with_known_variables(const SparseMatrix& M,const std::vector<uint>& id,const std::vector<scalar>& v,scalar eps = 1e-8);
    vec solve_for_kernel_with_known_variables(const std::pair<SparseMatrix,SparseMatrix>& M,uint N,const std::vector<uint>& id,const std::vector<scalar>& v,scalar eps = 1e-8);
}

class SparseMatrix
{
public:
    SparseMatrix(int dimension,bool is_symetric = false);
    SparseMatrix(int w,int h,bool is_symetric = false);
    SparseMatrix();

    void new_row();
    void add_in_row(int i,scalar x);
    void end_construct();

    vec conjugate_gradient(const vec& x0,const vec& b,scalar eps,bool log_error = false) const;
    vec conjugate_gradient(const vec& b,scalar eps,bool log_error = false) const;
    vec mult(const vec& x) const;

    inline uint get_current_index(){
        return v.size()-1;
    }
    inline void set_at_index(uint i,scalar x) {
        v[i] = x;
    }

    inline vec operator*(const vec& x) const {
        return mult(x);
    }

    inline uint get_height() const {
        return h;
    }

    inline uint get_width() const {
        return w;
    }

    scalar row_sum(uint j) const;

    void print() const;

    friend std::pair<SparseMatrix,SparseMatrix> algo::set_known_variables(const SparseMatrix& M,const std::vector<uint>& id);
    friend std::ostream& operator<<(std::ostream& o,const smat& M);

    inline void multiply_by_scalar(scalar lambda){
        for (scalar& val : v)
            val *= lambda;
    }

private:
    uint nb_threads;

    void parralel_sparse_matrix_vector_mult(const vec& x,vec& b,uint frow,uint lrow) const;
    uint w;
    uint h;
    int nnz;
    std::vector<uint> rowptr;
    std::vector<uint> colind;
    std::vector<scalar> v;
    bool symetric;

};



//std::pair<smat,smat> set_known_variables(const smat& M,const std::vector<uint>& id);

}

#endif // SPARSE_MATRIX_H
