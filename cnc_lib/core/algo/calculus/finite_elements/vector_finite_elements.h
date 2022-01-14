#ifndef VECTOR_FINITE_ELEMENTS_H
#define VECTOR_FINITE_ELEMENTS_H

#include "finite_elements.h"

namespace cnc{
namespace algo {
namespace FEM {

using namespace geometry;

using VectorSample = std::map<vertex,vec>;

class VectorFiniteElementSolver : public FiniteElementSolver {
protected:
    VectorFiniteElementSolver(MeshRef M,verticesSubSet dirichlet);
};

class LinearElasticity : public VectorFiniteElementSolver {
public:
    LinearElasticity(MeshRef M,verticesSubSet dirichlet,const calculus::vector_function& f);
    void ComputeSolution() override;
    const VectorSample& getSolution() const {
        return V;
    }
    vec getSolutionNorm() const;
    void setCoefficients(scalar l,scalar m);
    void setLambda(scalar l){
        lambda = l;
    }
    void setMu(scalar m){
        mu = m;
    }
protected:
    calculus::vector_function vec_basis(vertex v,const topology::face& f,uint coord);
    VectorSample V;
    void assemble_matrices();
    scalar lambda=1,mu=1;
    SMB tensor_term,div_term;
    vec force_term;
    static scalar dot(const mat& A,const mat& B);
    static mat vector_field_gradient(const vec& grad,uint coord);
    static mat deform_tensor(const vec& grad,uint coord);

    calculus::vector_function force;
};

}}}

#endif // VECTOR_FINITE_ELEMENTS_H
