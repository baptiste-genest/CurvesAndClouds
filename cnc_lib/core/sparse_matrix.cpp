#include "sparse_matrix.h"

cnc::SparseMatrix::SparseMatrix(int dimension, bool is_symetric) :
    SparseMatrix(dimension,dimension,is_symetric)
{
}

cnc::SparseMatrix::SparseMatrix(int _w, int _h, bool is_symetric) :
    w(_w),
    h(_h),
    symetric(is_symetric)
{
    nb_threads = 4;
    rowptr.push_back(0);
    nnz = 0;
}

void cnc::SparseMatrix::new_row()
{
    rowptr.push_back(nnz);
}

void cnc::SparseMatrix::add_in_row(int i, scalar x)
{
    if (i < 0)
        throw Cnc_error("Can't append at negative index");
    colind.push_back(i);
    nnz++;
    v.push_back(x);
}

void cnc::SparseMatrix::end_construct()
{
    new_row();
}

cnc::vec cnc::SparseMatrix::conjugate_gradient(const cnc::vec &x0, const cnc::vec &b, scalar eps,bool log_error) const
{
    vec p = b,r = b,x = x0,Ap,pr;
    scalar alpha,beta;
    scalar err = 2*eps;
    scalar ibm = 1./b.norm_inf();
    while(err > eps){
        Ap = mult(p);
        alpha = r.norm2() / (p.scalar_product(Ap));
        x = x + p*alpha;
        pr = r;
        r = r - Ap*alpha;
        //err = r.norm();
        beta = r.norm2()/(pr.norm2());
        p = r + p*beta;
        err = (mult(x) - b).norm_inf()*ibm;
        if (log_error)
            std::cout << err << std::endl;
    }
    return x;
}

cnc::vec cnc::SparseMatrix::conjugate_gradient(const vec &b, scalar eps,bool log_error) const{
    return conjugate_gradient(vec(b.rowNum()),b,eps,log_error);
}

cnc::vec cnc::SparseMatrix::mult(const cnc::vec &x) const
{
    if (x.rowNum() != w)
        throw Cnc_error("mat width must be equal to vec height");
    uint srow = 0;
    vec y(h);
    if (h > 100){
        uint nb_rows_per_thread = h/nb_threads;
        uint last_row_in_thread = nb_threads*nb_rows_per_thread;
        srow = last_row_in_thread;
        std::vector<std::thread> threads;
        for (uint k = 0;k<nb_threads;k++)
            threads.push_back(std::thread(&SparseMatrix::parralel_sparse_matrix_vector_mult,this,x,std::ref(y),k*nb_rows_per_thread,(k+1)*nb_rows_per_thread));
        for (std::thread& t : threads)
            t.join();
    }
    parralel_sparse_matrix_vector_mult(x,y,srow,h);
    return y;
}

void cnc::SparseMatrix::parralel_sparse_matrix_vector_mult(const vec &x,vec& b, uint frow, uint lrow) const
{
    for (uint j = frow;j<lrow;j++){
        for (uint k = rowptr[j];k<rowptr[j+1];k++)
            b(j) += v[k]*x(colind[k]);
    }
}

cnc::scalar cnc::SparseMatrix::row_sum(uint j) const
{
    scalar s = 0;
    for (uint k = rowptr[j];k<rowptr[j+1];k++)
        s += v[k];
    return s;
}

void cnc::SparseMatrix::print() const
{
    for (uint j = 0;j<h;j++){
        if (rowptr[j] == rowptr[j+1]){
            for (uint i = 0;i<w;i++)
                std::cout << 0 << " ";
        }
        else {
            for (uint i = 0;i<colind[rowptr[j]];i++)
                std::cout << 0 << " ";
            for (uint k = rowptr[j];k<rowptr[j+1];k++){
                std::cout << v[k] << " ";
                if (rowptr[j]+1 < rowptr[j+1])
                    for (uint i = colind[k]+1;i<colind[k+1];i++)
                        std::cout << 0 << " ";
            }
            for (uint i = 1+colind[rowptr[j+1]-1];i<w;i++)
                std::cout << 0 << " ";
        }
        std::cout << std::endl;
    }
}


std::pair<cnc::smat, cnc::smat> cnc::algo::set_known_variables(const cnc::smat &M, const std::vector<uint> &id)
{
    if (!std::is_sorted(id.begin(),id.end()))
        throw Cnc_error("Coords of known variables must be sorted");
    uint m = id.size();
    uint N = M.get_height();
    uint n = N - m;
    smat M1(n),M2(m,n,false);
    using namespace algo;
    auto inM1 = [id] (uint i,uint j) {
        return !belong<uint>(id,i) && !belong<uint>(id,j);
    };
    auto inM2 = [id] (uint i,uint j) {
        return belong<uint>(id,i) && !belong<uint>(id,j);
    };
    std::vector<bool> ids(M.get_height(),false);
    for (uint i : id)
        ids[i] = true;

    for (uint j = 0;j<N;j++){
        //std::cout << j << " over " << N << std::endl;
        uint M1i = 0,M2i = 0;
        if (M.rowptr[j] == M.rowptr[j+1]){
            for (uint i = 0;i<M.w;i++){
                if (!ids[i] && !ids[j]){
                    M1i++;
                }
                else if (ids[i] && !ids[j]){
                    M2i++;
                }
            }
        }
        else {
            for (uint i = 0;i<M.colind[M.rowptr[j]];i++){
                if (!ids[i] && !ids[j]){
                    M1i++;
                }
                else if (ids[i] && !ids[j]){
                    M2i++;
                }
            }
            for (uint k = M.rowptr[j];k<M.rowptr[j+1];k++){
                uint i = M.colind[k];
                if (!ids[i] && !ids[j]){
                    M1.add_in_row(M1i,M.v[k]);
                    M1i++;
                }
                else if (ids[i] && !ids[j]){
                    M2.add_in_row(M2i,M.v[k]);
                    M2i++;
                }
                if (M.rowptr[j]+1 < M.rowptr[j+1] && uint(k+1) < M.colind.size()){
                    for (uint i = M.colind[k]+1;i<M.colind[k+1];i++){
                        //std::cout << i << std::endl;
                        if (!ids[i] && !ids[j]){
                            M1i++;
                        }
                        else if (ids[i] && !ids[j]){
                            M2i++;
                        }
                    }
                }
            }
            for (uint i = 1+M.colind[M.rowptr[j+1]-1];i<M.w;i++){
                if (!ids[i] && !ids[j]){
                    M1i++;
                }
                else if (ids[i] && !ids[j]){
                    M2i++;
                }
            }
        }
        if (!belong(id,j)){
            M1.new_row();
            M2.new_row();
        }
    }
    M1.end_construct();
    M2.end_construct();
    return {M1,M2};
}

cnc::vec cnc::algo::solve_for_kernel_with_known_variables(const cnc::smat &M, const std::vector<uint> &id, const std::vector<cnc::scalar> &v,scalar eps)
{
    if (id.size() != v.size())
        throw Cnc_error("must have as much values as known variables");
    auto P = algo::set_known_variables(M,id);
    vec B(v),X(M.get_height());
    vec Y = (P.second*B)*(-1);
    auto x = P.first.conjugate_gradient(Y,eps,true);
    uint i = 0,j=0;
    for (uint k = 0;k<M.get_height();k++){
        if (algo::belong(id,k)){
            X(k) = v[i];
            i++;
        }
        else{
            X(k) = x(j);
            j++;
        }
    }
    return X;
}

std::ostream &cnc::operator<<(std::ostream &o, const cnc::smat &M)
{
    for (uint j = 0;j<M.h;j++){
        if (M.rowptr[j] == M.rowptr[j+1]){
            for (uint i = 0;i<M.w;i++)
                o << 0 << " ";
        }
        else {
            for (uint i = 0;i<M.colind[M.rowptr[j]];i++)
                o << 0 << " ";
            for (uint k = M.rowptr[j];k<M.rowptr[j+1];k++){
                o << M.v[k] << " ";
                if (M.rowptr[j]+1 < M.rowptr[j+1] && uint(k+1) < M.colind.size())
                    for (uint i = M.colind[k]+1;i<M.colind[k+1];i++)
                        o << 0 << " ";
            }
            for (uint i = 1+M.colind[M.rowptr[j+1]-1];i<M.w;i++)
                o << 0 << " ";
        }
        o << std::endl;
    }
    return o;
}

cnc::vec cnc::algo::solve_for_kernel_with_known_variables(const std::pair<cnc::SparseMatrix, cnc::SparseMatrix> &P,uint N, const std::vector<uint> &id, const std::vector<cnc::scalar> &v, cnc::scalar eps)
{
    if (id.size() != v.size())
        throw Cnc_error("must have as much values as known variables");
    vec B(v),X(N);
    vec Y = (P.second*B)*(-1);
    auto x = P.first.conjugate_gradient(Y,eps,false);
    uint i = 0,j=0;
    for (uint k = 0;k<N;k++){
        if (algo::belong(id,k)){
            X(k) = v[i];
            i++;
        }
        else{
            X(k) = x(j);
            j++;
        }
    }
    return X;
}
