#ifndef SPARSE_MATRIX_BUILDER_H
#define SPARSE_MATRIX_BUILDER_H

#include "../../sparse_matrix.h"
#include <map>

namespace cnc {
namespace algo {
class SparsePreconditionners;

}
using SMB = SparseMatrixBuilder;

namespace smatbuilder {
using index = std::pair<uint,uint>;
struct indexComp{
    bool operator()(const index& a,const index& b) const;
};
using container = std::map<index,scalar,indexComp>;
class SMBaccessor{
public:
    void operator=(scalar x);
    operator scalar() const;
private:
    friend class cnc::SparseMatrixBuilder;
    SMBaccessor(SMB& ref,index i) : R(ref),ix(i){}
    SMB& R;
    index ix;
};
}


class SparseMatrixBuilder
{
public:
    SparseMatrixBuilder(uint width,uint height,bool symetric = false);
    SparseMatrixBuilder(uint N,bool symetric = false) : SparseMatrixBuilder(N,N,symetric){}
    SparseMatrixBuilder() : SparseMatrixBuilder(0,false) {
    }
    smatbuilder::SMBaccessor operator()(uint i,uint j);

    scalar get(uint i,uint j) const;

    scalar operator()(uint i,uint j) const{
        return get(i,j);
    }

    void set(uint i,uint j,scalar x);
    const smatbuilder::container& getValues() const{
        return values;
    }
    uint width() const {return w;}
    uint height() const {return h;}
    bool isNull(uint i,uint j) const;
    void deleteValue(uint i,uint j);
    void print() const;
    SparseMatrixBuilder transpose() const;
    mat DenseMatrix() const;
private:
    friend class SparseMatrix;
    friend class smatbuilder::SMBaccessor;
    friend class cnc::algo::SparsePreconditionners;
    smatbuilder::container values;
    uint w,h;
    bool S;
};

enum class MatrixType {
    lower,
    upper
    /*
    diagonal,
    tridiagonal
    */
};

using MatrixFactor = std::pair<SMB,MatrixType>;
using MatrixMultiplicationChain = std::vector<MatrixFactor>;

namespace algo {

vec UpperSolve(const SMB& A,const vec& x);
vec LowerSolve(const SMB& A,const vec& x);
SMB Cholesky(const SMB& A);
vec ChainSolve(const MatrixMultiplicationChain& MMC,vec x);

}


}


#endif // SPARSE_MATRIX_BUILDER_H
