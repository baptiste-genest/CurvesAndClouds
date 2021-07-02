#include "geodesy.h"


geodesy build_pather(const mapper &M)
{
    return [M] (const vec& a,const vec& x){
        mat U = M(a);
        mat P = chaskal::Id<scalar>(4) - U*(U.transpose()*U).invert()*U.transpose();
        return P*x;
    };
}
