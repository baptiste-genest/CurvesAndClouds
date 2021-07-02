#include "spline.h"

cnc::algo::calculus::Spline::Spline(const scalar_function_1D& f,const nodes &n, scalar _fp0, scalar _fp1):
    N(n),fp0(_fp0),fp1(_fp1)
{
    size = N.size();
    weights.resize(size);

    compute_weights(f);
}

cnc::scalar cnc::algo::calculus::Spline::operator()(scalar x) const
{
    int node_id = get_node_number(N,x);
    if (node_id == -1)
        return weights[0][0];
    if (node_id == -2)
        return weights.back()[0];
    scalar X = x - N[node_id];
    scalar Xp = 1.;
    scalar I = 0;
    for (uint k = 0;k<4;k++){
        I += Xp*weights[node_id][k];
        Xp *= X;
    }
    return I;
}

void cnc::algo::calculus::Spline::compute_weights(const scalar_function_1D& f)
{
    auto S = sample(f,N);
    for (uint k = 0;k<size;k++)
        weights[k][0] = S[k];
    weights[0][1] = fp0;
    weights.back()[1] = fp1;

    vec Spp(size);
    mat D(size,size);
    /*
    for (uint k = 0;k<size;k++)
        D(k,k)
        */


    compute_third_derivatives();
}

void cnc::algo::calculus::Spline::compute_third_derivatives()
{
    for (uint k = 0;k<size-1;k++)
        weights[k][3] = (weights[k+1][2] - weights[k][2])/(N[k+1]-N[k]);
}
