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
