#include "statistics.h"

cnc::vec cnc::algo::stat::compute_mean(const cnc::cloud& P)
{
    if (P.size() == 0)
        throw Cnc_error("can't compute mean of a void cloud");
    uint N = P.points.size();
    const scalar scaling = 1.f/(scalar)N;
    vec M(P.points[0].rowNum());
    for (const vec& v : P.points)
        M += v;
    return M*scaling;
}

cnc::cloud cnc::algo::stat::init_empty_cloud(uint N, uint dim)
{
    if (dim == 0)
        throw Cnc_error("the dimension of the cloud can't be 0");
    cnc::cloud C;
    for (uint i = 0;i<N;i++)
        C.points.push_back(cnc::vec(dim));

    return C;
}

cnc::cloud cnc::algo::stat::project_into_2D(const cnc::cloud & C)
{
    if (C.size() == 0)
        throw Cnc_error("Can't project void cloud in 2D");
    if (C[0].rowNum() == 2)
        return C;
    projector P = cnc::algo::stat::get_PCA_projector(C,2);
    return C.apply_to_copy(P);
}

cnc::cloud cnc::algo::stat::sample_points_on_1D_function(const cnc::algo::calculus::scalar_function_1D &f, const cnc::algo::calculus::nodes &X)
{
    cnc::cloud R = init_empty_cloud(X.size(),2);
    for (uint i = 0;i<X.size();i++){
        R.points[i](0) = X[i];
        R.points[i](1) = f(X[i]);
    }
    return R;
}

cnc::cloud cnc::operator+(const cnc::cloud &a, const cnc::cloud &b)
{
    cloud c = a;
    c.points.insert(c.points.end(),b.points.begin(),b.points.end());
    return c;
}

std::vector<cnc::cloud> cnc::algo::stat::compute_clusters_by_k_means(const cnc::cloud & I, const uint k)
{
    uint dim = I.points[0].rowNum(),N = I.points.size();
    std::vector<vec> k_means(k);
    for (uint i = 0;i<k;i++)
        k_means[i] = vec(dim);

    std::vector<std::vector<uint>> clusters_id(k);

    const static uint MAX_NB_ITER = 1000;

    const static scalar eps = 1e-6;
    scalar dmean = 2*eps;
    for (uint iter = 0;dmean > eps && iter < MAX_NB_ITER;iter++) {
        std::vector<vec> avg_pos_in_voronoi(k);
        for (uint i = 0;i<k;i++){
            clusters_id[i].clear();
            avg_pos_in_voronoi[i] = vec(dim);
        }

        for (uint i = 0;i<N;i++){
            std::vector<scalar> dist_to_mean(k);
            for (uint j = 0;j<k;j++){
                dist_to_mean[j] = (I.points[i] - k_means[j]).norm();
            }
            uint arg_min = std::distance(dist_to_mean.begin(),std::min_element(dist_to_mean.begin(),dist_to_mean.end()));
            avg_pos_in_voronoi[arg_min] += I.points[i];
            clusters_id[arg_min].push_back(i);
        }

        dmean = 0.f;
        vec new_pos(dim);
        for (uint i = 0;i<k;i++){
            if (clusters_id[i].size()){
                new_pos = avg_pos_in_voronoi[i] * (1.f/clusters_id[i].size());
                dmean += (k_means[i] - new_pos).norm();
                k_means[i] = new_pos;
            }
        }
    }

    std::vector<cnc::cloud> clusters(k);
    for (uint i = 0;i<k;i++){
        for (uint id : clusters_id[i]){
            clusters[i].add_point(I.points[id]);
        }
    }
    return clusters;
}

cnc::mat cnc::algo::stat::compute_covariance_matrix(const cnc::cloud &c)
{
    cnc::cloud centered = center(c);
    uint dim = c.points[0].rowNum(), N = c.points.size();
    scalar scale_factor = 1.f/(N-1);
    mat C(dim,dim);

    for (uint j = 0;j<dim;j++){
        for (uint i = j;i<dim;i++){
            for (uint k = 0;k<N;k++){
                C.at(i,j) += centered.points[k](i)*centered.points[k](j);
            }
            C.at(i,j) *= scale_factor;
        }
    }

    for (uint j = 1;j<dim;j++)
        for (uint i = 0;i<j;i++)
            C.at(i,j) = C.ix(j,i);
    return C;
}

cnc::cloud cnc::center(const cnc::cloud &c)
{
    cnc::vec M = cnc::algo::stat::compute_mean(c)*(-1.f);
    cloud C = c;
    for (cnc::vec& p : C.points)
        p += M;
    return C;
}

std::vector<cnc::vec> cnc::algo::stat::compute_PCA(const cloud &X)
{
    return compute_PCA(X,X[0].rowNum());
}

std::vector<cnc::vec> cnc::algo::stat::compute_PCA(const cnc::cloud &X, uint n,bool unit_length)
{
    if (X.size() == 0)
        throw Cnc_error("can't compute PCA on empty cloud");
    if (n > X[0].rowNum())
        throw Cnc_error("can't project cloud in higher dimension");
    mat C = cnc::algo::stat::compute_covariance_matrix(X);
    auto eigens = C.lanczos(n);
    std::vector<cnc::vec> PCA(n);
    for (uint i = 0;i<PCA.size();i++){
        if (unit_length)
            PCA[i] = eigens[i].vector;
        else
            PCA[i] = eigens[i].vector*eigens[i].value;
    }
    return PCA;
}

cnc::algo::stat::norm cnc::algo::stat::build_wasserstein_norm(uint p)
{
    return [p] (const cnc::vec& v){
        scalar p_norm = 0.f;
        for (uint k = 0;k<v.rowNum();k++)
            p_norm += std::pow(std::abs(v(k)),p);
        return std::pow(p_norm,1.f/p);
    };
}

cnc::cloud::cloud(const std::vector<cscalar> &p)
{
    for (const auto& z : p)
        points.push_back(linear_utils::vec2(z.real(),z.imag()));
}

void cnc::cloud::set_mean(const cnc::vec &new_mean)
{
    vec old_mean = cnc::algo::stat::compute_mean(*this);
    vec offset = new_mean - old_mean;
    for (uint k = 0;k<size();k++)
        points[k] += offset;
}

cnc::cloud cnc::cloud::subsample(scalar f) const
{
    cloud sub;
    uint step = 1./f;
    for (uint k = 0;k<size();k+= step)
        sub.add_point((*this)[k]);
    return sub;
}


cnc::mat cnc::algo::stat::compute_kernel_matrix(const cnc::cloud &c, const cnc::algo::stat::kernel &k)
{
    auto centered = center(c);
    uint d = c.size();
    mat K(d,d);
    for (uint j = 0;j<d;j++)
        for (uint i = j;i<d;i++){
            K(i,j) = k(centered[i],centered[j]);
            K(j,i) = K(i,j);
        }
    return K;
}

cnc::algo::stat::projector cnc::algo::stat::get_KPCA_projector(const cnc::cloud &c, const cnc::algo::stat::kernel &ker, uint dim)
{
    if (c.size() == 0)
        throw Cnc_error("can't compute KPCA on empty cloud");
    if (dim > c[0].rowNum())
        throw Cnc_error("can't project cloud in higher dimension");
    mat K = compute_kernel_matrix(c,ker);
    auto eigen = K.lanczos(dim);
    std::vector<vec> kernel_vectors(dim);
    for (uint i = 0;i<dim;i++)
        kernel_vectors[i] = eigen[i].vector;
    uint d = c.size();
    return [kernel_vectors,c,ker,d,dim] (const vec& x){
        vec proj(dim);
        for (uint j = 0;j<dim;j++)
            for (uint i = 0;i<d;i++){
                proj(j) += kernel_vectors[j](i)*ker(c[i],x);
            }
        return proj;
    };
}

cnc::algo::stat::projector cnc::algo::stat::get_PCA_projector(const cnc::cloud &c, uint dim)
{
    if (c.size() == 0)
        throw Cnc_error("can't compute KPCA on empty cloud");
    if (dim > c[0].rowNum())
        throw Cnc_error("can't project cloud in higher dimension");
    auto A = compute_PCA(c,dim,true);
    return [A,dim] (const vec& x) {
        vec P(dim);
        for (uint k = 0;k<dim;k++)
            P(k) = A[k].scalar_product(x);
        return P;
    };
}

cnc::cloud cnc::algo::stat::project_by_min_dist(const cnc::cloud &c, uint dim)
{
    uint N = c.size(),d1 = c[0].rowNum();
    if (d1 == dim)
        return c;
    if (d1 < dim)
        throw Cnc_error("can't MD project in higher dimension");
    mat D(N,N);
    for (uint j = 0;j<N;j++)
        for (uint i = j+1;i<N;i++){
            D(i,j) = (c[i]-c[j]).norm();
            D(j,i) = D(i,j);
        }
    calculus::scalar_function dist = [N,D,dim] (const vec& X) {
        scalar cum_dist = 0.f;
        std::vector<vec> p(N,vec(dim));
        for (uint j = 0;j<N;j++)
            for (uint i = 0;i<dim;i++)
                p[j](i) = X(j*dim +i);
        for (uint j = 0;j<N;j++)
            for (uint i = 0;i<N;i++){
                if (i != j)
                    cum_dist += D(i,j) - (p[j] - p[i]).norm();
            }
        return cum_dist;
    };
    vec pc(N*dim);
    pc = algo::calculus::optimization::gradient_descent_fixed_step(dist,pc,1e-1,1,100);
    cloud R = init_empty_cloud(N,dim);
    for (uint j = 0;j<N;j++)
        for (uint i = 0;i<dim;i++)
            R[j](i) = pc(j*dim + i);
    return R;
}

cnc::cloud cnc::algo::stat::project_on_plane(const cnc::cloud &c, const cnc::vec &n, cnc::scalar val)
{
    cloud p = c;
    for (vec& x : p.points)
        x = x + n*(val-x.scalar_product(n));
    return p;
}
