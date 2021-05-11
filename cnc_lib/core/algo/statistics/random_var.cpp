#include "random_var.h"

cnc::cloud cnc::algo::stat::random_var::sample_normal_random_vectors(const std::vector<float> &means, const std::vector<float> &var, uint N)
{
    if (means.size() != var.size())
        throw "must have as much means as variance to build random_vector";
    uint dim = means.size();

    std::vector<std::normal_distribution<float>> rand_var;

    cloud sample = init_empty_cloud(N,dim);

    std::random_device rd{};
    std::mt19937 gen{rd()};

    for (uint i = 0;i<dim;i++)
        rand_var.push_back(std::normal_distribution<float> {means[i],var[i]});

    for (uint k = 0;k<N;k++)
        for (uint j = 0;j<dim;j++)
            sample.points[k](j) = rand_var[j](gen);

    return sample;
}

cnc::cloud cnc::algo::stat::random_var::sample_gaussian_vector_by_PC(const std::vector<cnc::vec> &pc_axis, const std::vector<float> &var, const cnc::vec &mean, uint N)
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

    std::normal_distribution<float> G{0.f,1.f};

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

cnc::cloud cnc::algo::stat::random_var::sample_uniform_in_square(uint dim, float half_width, uint N)
{
    if (dim == 0)
        throw Cnc_error("Can't sample random var in dimension 0");
    cloud S = init_empty_cloud(N,dim);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-half_width,half_width);
    for (uint i = 0;i<N;i++)
        for (uint k = 0;k<dim;k++)
            S.points[i](k) = dis(gen);
    return S;
}

float cnc::algo::stat::random_var::random_float(float a, float b)
{
    if (b < a)
        throw Cnc_error("Can't generate random between a and b if b < a");
    static constexpr int N = 1e6;
    return (b-a)*float(rand()%N)/N + a;
}
