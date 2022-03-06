#include "sparse_matrix_builder.h"
using namespace cnc::sparse;
using namespace cnc;

bool smatbuilder::indexComp::operator()(const smatbuilder::index &a, const smatbuilder::index &b) const
{
    if (a.second == b.second)
        return a.first < b.first;
    return a.second < b.second;
}

SparseMatrixBuilder::SparseMatrixBuilder(uint width, uint height, bool symetric) : w(width),h(height),S(symetric)
{

}

scalar SparseMatrixBuilder::get(uint i, uint j) const
{
    auto it = values.find({i,j});
    if (it == values.end())
        return 0;
    return (*it).second;
}

void SparseMatrixBuilder::operator*=(scalar x)
{
    for (auto& v : values)
        v.second*=x;
}

void SparseMatrixBuilder::operator+=(const SMB & other)
{
    for (const auto& v: other.values)
        values[v.first] = values[v.first]+v.second;
}

smatbuilder::SMBaccessor SparseMatrixBuilder::operator()(uint i, uint j)
{
    return smatbuilder::SMBaccessor(*this,{i,j});
}

void SparseMatrixBuilder::set(uint i, uint j, scalar x)
{
    values[{i,j}] = x;
    if (S && i != j)
        values[{j,i}] = x;
}

bool SparseMatrixBuilder::isNull(uint i,uint j) const
{
    return (values.find({i,j}) == values.end());
}

void SparseMatrixBuilder::deleteValue(uint i, uint j)
{
    values.erase({i,j});
}

void SparseMatrixBuilder::print() const
{
    for (uint j = 0;j<height();j++){
        for (uint i = 0;i<width();i++){
            if (isNull(i,j))
                std::cout << "0 ";
            else
                std::cout << values.at({i,j}) << ' ';
        }
        std::cout << std::endl;
    }
}

SparseMatrixBuilder SparseMatrixBuilder::transpose() const
{
    if (S)
        return *this;
    SMB t(h,w,S);
    for (const auto& ixv : values)
        t(ixv.first.second,ixv.first.first) = ixv.second;
    return t;
}

mat SparseMatrixBuilder::DenseMatrix() const
{
    mat D(h,w);
    for (uint j = 0;j<h;j++)
        for (uint i = 0;i<w;i++)
            D(i,j) = get(i,j);
    return D;
}

SparseMatrix::SparseMatrix(const SparseMatrixBuilder & SMB) : SparseMatrix(SMB.w,SMB.h,SMB.S)
{
    uint j = 0;
    for (const auto& iv : SMB.values){
        if (iv.first.second != j){
            for (uint i = j;i<iv.first.second;i++)
                new_row();
            j = iv.first.second;
        }
        add_in_row(iv.first.first,iv.second);
    }
    for (;j<=SMB.h;j++)
        new_row();
}

void smatbuilder::SMBaccessor::operator=(scalar x)
{
    R.values[ix] = x;
    if (R.S)
        R.values[{ix.second,ix.first}] = x;
}

smatbuilder::SMBaccessor::operator scalar() const
{
    return R.get(ix.first,ix.second);
}


SMB algo::Cholesky(const SMB &A)
{
    uint n = A.width();
    SMB L(n);
    for (uint i = 0;i<n;i++){
        for (uint j = 0;j<=i;j++) {
            scalar sum = 0;
            for (uint k = 0;k<j;k++)
                sum += L(k,i)*L(k,j);
            if (i == j)
                L(i,i) = std::sqrt(A(i,i)-sum);
            else
                L(j,i) = 1.0/L(j,j)*(A(j,i)-sum);
        }
    }
    return L;
}

vec algo::UpperSolve(const SMB &A, const vec &b)
{
    vec x(b.size());
    uint n = A.width();
    for (int i = n-1;i>=0;i--){
        scalar sum = 0;
        for (uint k = i+1;k<n;k++)
            sum+= A(k,i)*b(k);
        x(i) = (b(i)-sum)/A(i,i);
    }
    return x;
}

vec algo::LowerSolve(const SMB &A, const vec &b)
{
    vec x(b.size());
    int n = A.width();
    for (int j = 0;j<n;j++){
        scalar sum = 0;
        for (int i = j-1;i>=0;i--)
            sum+= A(i,j)*b(i);
        x(j) = (b(j)-sum)/A(j,j);
    }
    return x;
}

vec algo::ChainSolve(const MatrixMultiplicationChain &MMC,vec x)
{
    for (const auto& factor : MMC){
        switch (factor.second) {
            case MatrixType::upper:
                x = UpperSolve(factor.first,x);
                break;
            case MatrixType::lower:
                x = LowerSolve(factor.first,x);
                break;
        }
    }
    return x;
}
