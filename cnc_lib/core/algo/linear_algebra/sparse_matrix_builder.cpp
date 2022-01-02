#include "sparse_matrix_builder.h"

bool cnc::smatbuilder::indexComp::operator()(const cnc::smatbuilder::index &a, const cnc::smatbuilder::index &b) const
{
    if (a.second == b.second)
        return a.first < b.first;
    return a.second < b.second;
}

cnc::SparseMatrixBuilder::SparseMatrixBuilder(uint width, uint height, bool symetric) : w(width),h(height),S(symetric)
{

}

cnc::scalar cnc::SparseMatrixBuilder::get(uint i, uint j) const
{
    auto it = values.find({i,j});
    if (it == values.end())
        return 0;
    return (*it).second;
}

cnc::smatbuilder::SMBaccessor cnc::SparseMatrixBuilder::operator()(uint i, uint j)
{
    return smatbuilder::SMBaccessor(*this,{i,j});
}

void cnc::SparseMatrixBuilder::set(uint i, uint j, cnc::scalar x)
{
    values[{i,j}] = x;
    if (S && i != j)
        values[{j,i}] = x;
}

bool cnc::SparseMatrixBuilder::isNull(uint i,uint j) const
{
    return (values.find({i,j}) == values.end());
}

void cnc::SparseMatrixBuilder::deleteValue(uint i, uint j)
{
    values.erase({i,j});
}

void cnc::SparseMatrixBuilder::print() const
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

cnc::SparseMatrixBuilder cnc::SparseMatrixBuilder::transpose() const
{
    if (S)
        return *this;
    SMB t(h,w,S);
    for (const auto& ixv : values)
        t(ixv.first.second,ixv.first.first) = ixv.second;
    return t;
}

cnc::mat cnc::SparseMatrixBuilder::DenseMatrix() const
{
    mat D(h,w);
    for (uint j = 0;j<h;j++)
        for (uint i = 0;i<w;i++)
            D(i,j) = get(i,j);
    return D;
}

cnc::SparseMatrix::SparseMatrix(const cnc::SparseMatrixBuilder & SMB) : SparseMatrix(SMB.w,SMB.h,SMB.S)
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

void cnc::smatbuilder::SMBaccessor::operator=(cnc::scalar x)
{
    R.values[ix] = x;
    if (R.S)
        R.values[{ix.second,ix.first}] = x;
}

cnc::smatbuilder::SMBaccessor::operator scalar() const
{
    return R.get(ix.first,ix.second);
}


cnc::SMB cnc::algo::Cholesky(const cnc::SMB &A)
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

cnc::vec cnc::algo::UpperSolve(const cnc::SMB &A, const cnc::vec &b)
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

cnc::vec cnc::algo::LowerSolve(const cnc::SMB &A, const cnc::vec &b)
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

cnc::vec cnc::algo::ChainSolve(const cnc::MatrixMultiplicationChain &MMC,cnc::vec x)
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
