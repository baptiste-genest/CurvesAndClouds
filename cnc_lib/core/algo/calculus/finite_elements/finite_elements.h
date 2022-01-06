#ifndef FINITE_ELEMENTS_H
#define FINITE_ELEMENTS_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "../../geometry/meshgeneration.h"
#include "../../linear_algebra/sparse_matrix_builder.h"
#include "../../linear_algebra/iterative_solvers.h"

namespace cnc{

class FEMDisplayer;
namespace algo {
namespace FEM {


using BarycentricFunction = std::function<scalar(const vec&)>;
using BarycentricFunctionGrad = vec;
using P1Base = std::map<topology::vertex,std::map<topology::face,BarycentricFunction,topology::faceComp>>;
using P1BaseGradient = std::map<topology::vertex,std::map<topology::face,BarycentricFunctionGrad,topology::faceComp>>;

class FiniteElementSolver{
public:
    FiniteElementSolver(geometry::MeshRef M);
    virtual void ComputeSolution() = 0;
    void buildRigidityMatrix();
    scalar sampleSolution(const vec& x) const;
    scalar sampleSolutionOnFace(const vec& x,const topology::face& f) const;
    vec sampleSolutionGradient(const vec& x) const;
    vec SolutionGradientOnFace(const topology::face& f) const;
    range SolutionValueRange() const;
    topology::vertex ClosestInteriorPoint(const vec& x)const;
    topology::face OneRingContainer(topology::vertex center,const vec& x,bool& contained) const;
    int interiorId(topology::vertex v) const;
    virtual scalar solutionValueAtVertex(uint interior_id) const = 0;
    virtual const vec& solutionVector() const = 0;
    const geometry::Mesh2& getMesh() const;
    friend class cnc::FEMDisplayer;
protected:
    void buildP1Basis();
    P1Base B;
    P1BaseGradient BGrad;
    SMB RigidityMatrix;
    geometry::MeshRef MR;
    geometry::Mesh2& M;
};


class PoissonEquation : public FiniteElementSolver{
public:
    PoissonEquation(geometry::MeshRef M,const calculus::scalar_function& f);
    void ComputeSolution() override;
    scalar solutionValueAtVertex(uint interior_id) const override;
    virtual const vec& solutionVector() const override;
private:
    vec Bh;
    vec solution;
    calculus::scalar_function f;
};

class LaplaceEigenFunctions : public FiniteElementSolver {
public:
    LaplaceEigenFunctions(geometry::MeshRef M,uint n);
    void ComputeSolution() override;
    scalar solutionValueAtVertex(uint interior_id) const override{
        return solution[cursor](interior_id);
    }
    virtual const vec& solutionVector() const override {
        return solution[cursor];
    }
    vec fullSolutionVector() const;
    scalar getEigenValue() const{
        return eigenvalues[cursor];
    }
    void setCursor(uint c) {
        cursor = c;
    }
private:
    vec Bh;
    std::vector<vec> solution;
    std::vector<scalar> eigenvalues;
    uint n;
    uint cursor = 0;
};

}
}
}

#endif // FINITE_ELEMENTS_H
