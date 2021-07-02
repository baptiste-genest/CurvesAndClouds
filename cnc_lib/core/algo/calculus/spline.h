#ifndef SPLINE_H
#define SPLINE_H

#include "calculus.h"

namespace cnc {

namespace algo {

namespace calculus {

class Spline
{
public:
    Spline(const scalar_function_1D& f,const nodes& n,scalar fp0,scalar fp1);

    scalar operator()(scalar x) const;
    operator scalar_function_1D()const;
private:
    nodes N;
    scalar fp0, fp1;
    uint size;
    std::vector<std::array<scalar,4>> weights;

    void compute_weights(const scalar_function_1D& f);
    void compute_third_derivatives();
};


}

}

}

#endif // SPLINE_H
