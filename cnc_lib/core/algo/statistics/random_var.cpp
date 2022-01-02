#include "random_var.h"

cnc::cloud cnc::algo::stat::random_var::sample_normal_random_vectors(const std::vector<scalar> &means, const std::vector<scalar> &var, uint N)
{
    if (means.size() != var.size())
        throw "must have as much means as variance to build random_vector";
    uint dim = means.size();

    std::vector<std::normal_distribution<scalar>> rand_var;

    cloud sample = init_empty_cloud(N,dim);

    std::random_device rd{};
    std::mt19937 gen{rd()};

    for (uint i = 0;i<dim;i++)
        rand_var.push_back(std::normal_distribution<scalar> {means[i],var[i]});

    for (uint k = 0;k<N;k++)
        for (uint j = 0;j<dim;j++)
            sample.points[k](j) = rand_var[j](gen);

    return sample;
}

cnc::cloud cnc::algo::stat::random_var::sample_gaussian_vector_by_PC(const std::vector<cnc::vec> &pc_axis, const std::vector<scalar> &var, const cnc::vec &mean, uint N)
{
    if (pc_axis.size() != var.size())
        throw Cnc_error("to sample gaussian vector from PC, each covariance vector must have an associated variance along it (#(pc_axis) != #(var))");
    std::vector<vec> A = chaskal::gram_schmidt(pc_axis);
    uint dim = pc_axis.size();
    mat O = chaskal::build_basis(A).transpose();
    mat D = mat(dim);
    for (uint i = 0;i<dim;i++)
        D.at(i,i) = std::sqrt(var[i]);
    mat B = O*D*O.transpose();
    cloud Y = sample_centered_reduced_gaussian_vector(dim,N);
    for (auto& y : Y.points)
        y = B*y + mean;
    return Y;
}

cnc::cloud cnc::algo::stat::random_var::sample_centered_reduced_gaussian_vector(uint dim, uint N)
{
    if (dim == 0)
        throw Cnc_error("Can't sample random var in dimension 0");
    std::random_device rd{};
    std::mt19937 gen{rd()};

    cloud sample = init_empty_cloud(N,dim);

    std::normal_distribution<scalar> G{0.f,1.f};

    for (uint k = 0;k<N;k++)
        for (uint j = 0;j<dim;j++)
            sample.points[k](j) = G(gen);

    return sample;
}

cnc::cloud cnc::algo::stat::random_var::sample_vector_on_unit_sphere(uint dim,uint N)
{
    if (dim == 0)
        throw Cnc_error("Can't sample random var in dimension 0");
    cloud R = sample_centered_reduced_gaussian_vector(dim,N);;
    for (vec& v : R.points)
        v.inner_normalize();
    return R;
}

cnc::cloud cnc::algo::stat::random_var::sample_uniform_in_square(uint dim, scalar half_width, uint N)
{
    if (dim == 0)
        throw Cnc_error("Can't sample random var in dimension 0");
    cloud S = init_empty_cloud(N,dim);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<scalar> dis(-half_width,half_width);
    for (uint i = 0;i<N;i++)
        for (uint k = 0;k<dim;k++)
            S.points[i](k) = dis(gen);
    return S;
}

cnc::scalar cnc::algo::stat::random_var::random_scalar(scalar a, scalar b)
{
    if (b < a)
        throw Cnc_error("Can't generate random between a and b if b < a");
    static constexpr int N = RAND_MAX;
    static constexpr scalar ratio = 1.0/N;
    return (b-a)*(scalar(rand()%N)*ratio) + a;
}

cnc::mat cnc::algo::stat::random_var::random_mat(cnc::scalar lb, cnc::scalar ub, uint n)
{
    return random_mat(lb,ub,n,n);
}

cnc::mat cnc::algo::stat::random_var::random_mat(cnc::scalar lb, cnc::scalar ub, uint h, uint w)
{
    mat R(h,w);
    for (uint j = 0;j<h;j++)
        for (uint i = 0;i<w;i++)
            R(i,j) = random_scalar(lb,ub);

    return R;
}

cnc::cmat cnc::algo::stat::random_var::random_complex_mat(cnc::scalar lb, cnc::scalar ub, uint h, uint w)
{
    cmat R(h,w);
    for (uint j = 0;j<h;j++)
        for (uint i = 0;i<w;i++)
            R(i,j) = complex_scalar(random_scalar(lb,ub),random_scalar(lb,ub));

    return R;
}

cnc::cmat cnc::algo::stat::random_var::random_complex_mat(cnc::scalar lb, cnc::scalar ub, uint n)
{
    return random_complex_mat(lb,ub,n,n);
}

cnc::complex_scalar cnc::algo::stat::random_var::random_complex_scalar(cnc::scalar a, cnc::scalar b)
{
    return complex_scalar(random_scalar(a,b),random_scalar(a,b));
}

cnc::cloud cnc::algo::stat::random_var::PoissonSamplingInSphere(cnc::scalar R, cnc::scalar r, uint dim, uint N, cnc::vec center)
{
    if (center.rowNum() == 1)
        center = vec(dim);
    auto belong = [R,center] (const vec& x){
        return x.distance2(center) < R*R;
    };
    std::vector<vec> P;
    for (uint k = 0;k<N;k++){
        auto x = random_vec_in_sphere(R,dim);
        if (std::find_if(P.begin(),P.end(),[x,r](const vec& p){return x.distance2(p) < r*r;}) == P.end())
            P.push_back(x);
    }
    return cloud(std::move(P));
    /*
    uint S = 0.5*R/r;
    uint NB = S*S;
    std::cout << "MAX N "  << NB << std::endl;
    vec offset = -ones(dim)*R;
    //int rn = std::sqrt(N);
    cloud P;
    std::vector<bool> I(NB,false);
    for (uint k = 0;k<std::min(N,NB);k++){
        int i = rand()%S,j = rand()%S;
        scalar x = scalar(i)/S;
        scalar y = scalar(j)/S;
        vec p = vec({x*2.*R,y*2.*R}) + offset + random_vec(0,r,dim);
        if (belong(p) && !I[j*S+i]){
            P.add_point(p);
            I[j*S+i] = true;
        }
    }
    return P;
    */
}

cnc::vec cnc::algo::stat::random_var::random_vec(cnc::scalar a, cnc::scalar b, uint N)
{
    vec X(N);
    for (uint i = 0;i<N;i++)
        X(i) = random_scalar(a,b);
    return X;
}

cnc::vec cnc::algo::stat::random_var::random_vec_in_sphere(cnc::scalar R, uint dim)
{
    auto x = random_vec(-R,R,dim);
    while (x.norm2() > R*R) {
        x = random_vec(-R,R,dim);
    }
    return x;
}

cnc::cloud cnc::algo::stat::random_var::PseudoRandomSequenceR2(uint N)
{
    constexpr static scalar g = 1.32471795724474602596;
    constexpr static scalar a1 = 1.0/g;
    constexpr static scalar a2 = 1.0/(g*g);
    cloud X = stat::init_empty_cloud(N,2);
    scalar tmp;
    for (int i = 0;i<(int)N;i++){
        X[i](0) = std::modf(0.5+a1*i,&tmp);
        X[i](1) = std::modf(0.5+a2*i,&tmp);
    }
    return X;
}

uint cnc::algo::stat::random_var::weightedChoice(const cnc::vec &W)
{
    scalar sum_of_weight = 0;
    for(uint i=0; i<W.size(); i++) {
       sum_of_weight += W(i);
    }
    scalar rnd = random_scalar(0,sum_of_weight);
    for(uint i=0; i<W.size(); i++) {
      if(rnd < W(i))
        return i;
      rnd -= W(i);
    }
    throw Cnc_error("could pick weighted choice");
}
