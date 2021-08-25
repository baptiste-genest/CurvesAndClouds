/**
  * @file statistics.h
  *
  * All algorithms and types related to statistical analysis, mainly for point clouds
  *
*/
#ifndef STATISTICS_H
#define STATISTICS_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "core/algo/algo.h"
#include "../lin_alg.h"
#include "core/algo/calculus/calculus.h"
#include "core/algo/calculus/optimization.h"


namespace cnc {

/**
 * @brief cloud of N-dimensionnal points
 */
struct cloud{
    std::vector<vec> points;

    uint size() const{
        return points.size();
    }

    vec& operator[](uint i) {
        if (i >= size())
            throw Cnc_error("index out of cloud range");
        return points[i];
    }

    vec operator[](uint i) const {
        if (i >= size())
            throw Cnc_error("index out of cloud range");
        return points[i];
    }

    /**
     * @brief operator += concatenates two clouds
     * @param C cloud to concatenate with
     */
    void operator+=(const cloud& C){
        for (uint k = 0;k<C.size();k++)
            add_point(C[k]);
    }

    /**
     * @brief add_point add single point to the cloud
     * @param x point to add (must have the same dimension than the cloud)
     */
    void add_point(const cnc::vec& x){
        if (points.size() == 0 || points[0].rowNum() == x.rowNum())
            points.push_back(x);
        else
            throw Cnc_error("can't add point of dim "+ std::to_string(x.rowNum()) + " to a cloud of dim " + std::to_string(points[0].rowNum()));
    }

    /**
    * @brief set_mean translate cloud so its mean is the given one
    * @param new_mean
    */
    void set_mean(const cnc::vec& new_mean);

    /**
     * @brief apply a function v to all points of the cloud
     * @param v function to apply
     */
    inline void apply(const algo::calculus::vector_function& v){
        for (vec& p : points)
            p = v(p);
    }
    inline void apply(const mat& v){
        for (vec& p : points)
            p = v*p;
    }

    /**
     * @brief apply_to_copy duplicate cloud and apply to the duplicata a given function v
     * @param v function to apply to the duplicata
     * @return duplicata
     */
    inline cloud apply_to_copy(const algo::calculus::vector_function& v) const {
        cloud D; D.points = points;
        for (vec& p : D.points)
            p = v(p);
        return D;
    }
    inline cloud apply_to_copy(const mat& v) const {
        cloud D; D.points = points;
        for (vec& p : D.points)
            p = v*p;
        return D;
    }
};
/**
 * @brief operator + merge operator, creates a new cloud with all the points of A and B
 * @param a
 * @param b
 * @return
 */
cloud operator+(const cloud& a,const cloud& b);

/**
 * @brief center returns the centered version of a point cloud
 * @param c cloud
 * @return centered cloud
 */
cloud center(const cloud& c);

namespace algo {

/**
 * @brief namespace containing all the algorithms relative to statistical analysis
 */
namespace stat {

typedef std::function<scalar(const vec&)> norm;

/**
 * @brief build_wasserstein_norm builds p norm
 * @param p
 * @return p-norm
 */
norm build_wasserstein_norm(uint p);

/**
 * @brief sample_points_on_1D_function builds a cloud with points at (x,f(x)) with x : X
 * @param f function to sample
 * @param X	nodes where sample
 * @return sample cloud
 */
cloud sample_points_on_1D_function(const calculus::scalar_function_1D& f, const calculus::nodes& X);

/**
 * @brief project_into_2D reduce dimensionnality of the point cloud to 2
 * @return new point cloud
 */
cloud project_into_2D(const cloud&);

/**
 * @brief init_empty_cloud initialize a point cloud with given size in given dimension
 * @param N number of point in cloud
 * @param dim dimensions of each point
 * @return point cloud
 */
cloud init_empty_cloud(uint N,uint dim);

/**
 * @brief compute_mean compute the mean value of a given set of point in N-space
 * @return mean value
 */
vec compute_mean(const cloud&);

/**
 * @brief compute_clusters_by_k_means compute k clusters by k-means algorithms
 * @param c cloud to clusterize
 * @param k number of clusters
 * @return vector of clusters
 */
std::vector<cloud> compute_clusters_by_k_means(const cloud& c,uint k);

/**
 * @brief get_covariance_matrix compute covariance matrix of a point cloud
 * @param c cloud
 * @return covariance matrix
 */
mat compute_covariance_matrix(const cloud& c);

/**
 * @brief compute_PCA compute principal component analysis, returns axis with length as associated variance
 * @param X sample
 * @return priciple axis of the cloud
 */
std::vector<vec> compute_PCA(const cloud& X);
/**
 * @brief compute_PCA compute principal component analysis for only n axis
 * @param X sample
 * @param n number of axis
 * @param unit_length if true, eigenvectors has norm 1 else same length than the associated eigenvalue
 * @return n priciple axis of the cloud with greatest variance
 */
std::vector<vec> compute_PCA(const cloud& X,uint n,bool unit_length = false);

/**
 * @brief projector function that maps a vector to a set (used in KPCA for exemple)
 */
typedef std::function<vec(const vec&)> projector;

/**
 * @brief get_PCA_projector construct the projector on the PCA components of the given cloud (
 * @param c base cloud to compute PCA on
 * @param dim dimension of the projection
 * @return projector function
 */
projector get_PCA_projector(const cloud& c,uint dim = 2);

/**
 * @brief kernel functions
 */
typedef std::function<scalar(const vec&,const vec&)> kernel;


/**
 * @brief compute_kernel_matrix given a kernel k and a cloud c, return matrix K_ij = k(c_i,c_j)
 * @param c cloud to compute kernel on
 * @param k kernel
 * @return kernel matrix
 */
mat compute_kernel_matrix(const cloud& c,const kernel& k);

/**
 * @brief get_KPCA_projector construct the projector on the Kernel-PCA of the cloud c, associated with kernel ker to reduce dimension to dim
 * @param c cloud to analyse
 * @param ker kernel
 * @param dim dimension of the kernel space
 * @return projector
 */
projector get_KPCA_projector(const cloud& c,const kernel& ker, uint dim = 2);

cloud project_on_plane(const cloud& c,const vec& n,scalar val = 0);

// WIP
cloud project_by_min_dist(const cloud& c,uint dim = 2);

}

}

}


#endif // STATISTICS_H
