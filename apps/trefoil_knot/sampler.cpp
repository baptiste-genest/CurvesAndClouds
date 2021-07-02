#include "sampler.h"

sampler::sampler(const geodesy &f,scalar _dt, uint _N)
    :
      pather(f),dt(_dt),N(_N)
{

}

cloud sampler::sample(const vec &v0)
{
    vec X = v0;
    cloud R;
    R.points.push_back(v0);
    uint nb = 1;
    vec o = algo::stat::random_var::sample_vector_on_unit_sphere(4,1)[0];
    for (uint k = 0;k<N;k++){
        vec a = pather(X,o);
        if (a.norm() < 1e-3)
            o = algo::stat::random_var::sample_vector_on_unit_sphere(4,1)[0];
        else{
            if (R.size() > 1)
                if (a.scalar_product(X-R[nb-2]) < 0)
                    a *= -1.;
            X = X + a.normalize()*dt;
            if (X.distance(R.points.back()) > 1e-2){
                R.add_point(X);
                nb++;
            }
        }
    }
    return R;
}
