#include "vector_calculus.h"
using namespace cnc;


cnc::algo::calculus::vector_function_2D cnc::algo::vector_calculus::build_gradient(const cnc::algo::calculus::scalar_function_2D &f,scalar dx)
{
    return [f,dx] (scalar x,scalar y){
        scalar v = f(x,y);
        return cnc::vec({f(x+dx,y) - v,f(x,y+dx)-v})*(1.f/dx);
    };
}

cnc::algo::calculus::scalar_function_2D cnc::algo::vector_calculus::build_divergence(const cnc::algo::calculus::vector_function_2D &f, scalar dx)
{
    return [f,dx] (scalar x,scalar y){
        cnc::vec v = f(x,y);
        return ((f(x+dx,y)-v)(0)+(f(x,y+dx)-v)(1))*(1.f/dx);
    };
}

cnc::algo::calculus::scalar_function_2D cnc::algo::vector_calculus::build_rot_norm(const cnc::algo::calculus::vector_function_2D &f, scalar dx)
{
    return [f,dx] (scalar x,scalar y){
        cnc::vec v = f(x,y);
        return ((f(x+dx,y)(1) - v(1) - f(x,y+dx)(0) + v(0))*(1.f/dx));
    };
}

cnc::algo::vector_calculus::Vector2DArray::Vector2DArray(const cnc::algo::calculus::nodes &X, const cnc::algo::calculus::nodes &Y, const cnc::algo::calculus::vector_function_2D &V)
{
    w = X.size();
    h = Y.size();
    value.resize(w*h);
    for (uint j = 0;j<h;j++)
        for (uint i = 0;i<w;i++)
            (*this)(i,j) = V(X[i],Y[j]);
}

cnc::algo::calculus::vector_function_2D cnc::algo::vector_calculus::Vector2DArray::get_interpolator(const cnc::range &x, const cnc::range &y) const {
    auto X = calculus::build_range_mapper(x,{0,w-1});
    auto Y = calculus::build_range_mapper(y,{0,h-1});
    auto W = w;
    scalar bx = x.second,by = y.second;
    auto V = value;
    return [X,Y,V,W,bx,by] (scalar x,scalar y){
        if (x > bx)
            return vec(2);
        if (y > by)
            return vec(2);
        scalar px = X(x),py = Y(y);
        uint ix = (uint)px,iy = (uint)py;
        scalar tx = px - ix,ty = py-iy;
        const vec& v00 = V[iy*W + ix];
        const vec& v10 = V[iy*W + ix+1];
        const vec& v01 = V[(iy+1)*W + ix];
        const vec& v11 = V[(iy+1)*W + ix+1];
        vec R1 = v00*(1-tx) + v10*tx;
        vec R2 = v01*(1-tx) + v11*tx;
        return R1*(1-ty) + R2*ty;
    };
}

cnc::algo::vector_calculus::Vector2DArray cnc::algo::vector_calculus::Vector2DArray::operator+(const cnc::algo::vector_calculus::Vector2DArray &O) const
{
    if (w != O.w)
        throw Cnc_error("vector arrays to add must have same width");
    if (h != O.h)
        throw Cnc_error("vector arrays to add must have same height");
    Vector2DArray R(w,h);
    for (uint j = 0;j<R.value.size();j++)
        R.value[j] = value[j] + O.value[j];
    return R;
}

cnc::algo::vector_calculus::Vector2DArray cnc::algo::vector_calculus::Vector2DArray::operator-(const cnc::algo::vector_calculus::Vector2DArray &O) const
{
    if (w != O.w)
        throw Cnc_error("vector arrays to sub must have same width");
    if (h != O.h)
        throw Cnc_error("vector arrays to sub must have same height");
    Vector2DArray R(w,h);
    for (uint j = 0;j<R.value.size();j++)
        R.value[j] = value[j] + O.value[j]*(-1.f);
    return R;

}

cnc::algo::vector_calculus::Vector2DArray cnc::algo::vector_calculus::Vector2DArray::operator*(scalar l) const
{
    Vector2DArray R(w,h);
    for (uint j = 0;j<R.value.size();j++)
        R.value[j] = value[j]*l;
    return R;
}

cnc::vec cnc::algo::vector_calculus::Vector2DArray::get_vec_form_divergence() const
{
    vec R(w*h);
    for (uint j = 0;j<h-1;j++)
        for (uint i = 0;i<w-1;i++){
            R(j*w+i) = ix(i+1,j)(0) - ix(i,j)(0) + ix(i,j+1)(1) - ix(i,j)(1);
        }
    for (uint j = 0;j<h;j++)
        R(j*w+w-1) = R(j*w+w-2);
    for (uint i = 0;i<w-1;i++)
        R((h-1)*w+i) = R((h-2)*w+i);
    R(w*(h-1) + w-1) += R((h-2)*w+w-2);
    R(w*(h-1) + w-1) *= 0.5f;
    return R;
}

cnc::algo::vector_calculus::Vector2DArray cnc::algo::vector_calculus::Vector2DArray::build_gradient_from_matrix(const cnc::mat &M)
{
    Vector2DArray R(M.colNum(),M.rowNum());
    for (uint j = 0;j<M.rowNum()-1;j++){
        for (uint i = 0;i<M.colNum()-1;i++){
            R(i,j)(0) = M(i+1,j) - M(i,j);
            R(i,j)(1) = M(i,j+1) - M(i,j);
        }
    }
    for (uint j = 0;j<M.rowNum()-1;j++)
        R(M.colNum()-1,j) = R(M.colNum()-2,j);
    for (uint i = 0;i<M.colNum()-1;i++)
        R(i,M.rowNum()-1) = R(i,M.rowNum()-2);
    return R;
}

std::pair<cnc::algo::vector_calculus::Vector2DArray, cnc::algo::vector_calculus::Vector2DArray> cnc::algo::vector_calculus::Vector2DArray::get_helmoltz_hodge_decomposition() const
{
    uint N = w*h,x,y,k;
    mat L(N,N);
    for (uint j = 0;j<N;j++){
        k = 0;
        x = j%w;
        y = j/w;
        if (x > 0){
            L(j-1,j) = -1.f;
            k++;
        }
        if (x < w-1){
            L(j+1,j) = -1.f;
            k++;
        }
        if (y > 0){
            L(j-w,j) = -1.f;
            k++;
        }
        if (y < w-1){
            L(j+w,j) = -1.f;
            k++;
        }
        L(j,j) = 4;
    }

    vec Div = get_vec_form_divergence();

    auto S = L.solve(Div,1e-4);
    auto A = mat(h,w,S.get_values());

    Vector2DArray grad = build_gradient_from_matrix(A);
    const Vector2DArray& V = *this;

    auto U = V - grad;

    return {U,grad};
}

cnc::algo::calculus::vector_function cnc::algo::vector_calculus::build_gradient(const cnc::algo::calculus::scalar_function &f,uint dim, scalar dx)
{
    scalar idx = 1.f/dx;
    auto DX = [dim,dx] (uint k){
        vec d(dim);
        d(k) = dx;
        return d;
    };
    return [f,idx,dim,DX] (const vec& x){
        vec G(dim);
        scalar v = f(x);
        for (uint k = 0;k<dim;k++)
            G(k) = f(x + DX(k)) - v;
        return G*idx;
    };
}


algo::calculus::scalar_function algo::vector_calculus::differentiate(const calculus::scalar_function &f, const calculus::vector_function &X, scalar dx)
{
    return [f,X,dx] (const vec& x){
        return (f(x+ X(x)*dx) - f(x))/dx;
    };
}
