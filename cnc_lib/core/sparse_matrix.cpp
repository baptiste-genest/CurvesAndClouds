#include "sparse_matrix.h"

cnc::SparseMatrix::SparseMatrix(int dimension, bool is_symetric) :
    n(dimension),
    symetric(is_symetric)
{
    rowptr.push_back(0);
    nnz = 0;
}

void cnc::SparseMatrix::new_row()
{
    rowptr.push_back(nnz);
}

void cnc::SparseMatrix::add_in_row(int i, scalar x)
{
    colind.push_back(i);
    nnz++;
    v.push_back(x);
}

void cnc::SparseMatrix::end_construct()
{
    new_row();
}

cnc::vec cnc::SparseMatrix::conjugate_gradient(const cnc::vec &x0, const cnc::vec &b, float eps) const
{
    vec p = b,r = b,x = x0,Ap,pr;
    scalar alpha,beta;
    scalar err = 2*eps;
    while(err > eps){
        Ap = mult(p);
        alpha = r.norm2() / (p.scalar_product(Ap));
        x = x + p*alpha;
        pr = r;
        r = r - Ap*alpha;
        err = r.norm();
        beta = r.norm2()/(pr.norm2());
        p = r + p*beta;
        //err = (mult(x) - b).norm();
        std::cout << err << std::endl;
    }
    return x;
}

cnc::vec cnc::SparseMatrix::conjugate_gradient(const vec &b, float eps) const{
    return conjugate_gradient(vec(b.rowNum()),b,eps);
}

cnc::vec cnc::SparseMatrix::mult(const cnc::vec &x) const
{
    vec y(x.rowNum());
    for (int i = 0;i<n;i++){
        for (int k = rowptr[i];k<rowptr[i+1];k++)
            y(i) += v[k]*x(colind[k]);
    }
    return y;

}

cnc::scalar cnc::SparseMatrix::row_sum(uint j) const
{
    scalar s = 0;
    for (int k = rowptr[j];k<rowptr[j+1];k++)
        s += v[k];
    return s;
}
