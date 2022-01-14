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
using verticesSubSet = topology::vertices;
using idMapper = std::map<vertex,uint>;

class FiniteElementSolver{
public:
    FiniteElementSolver(geometry::MeshRef);
    FiniteElementSolver(geometry::MeshRef,const verticesSubSet& Dirichlet);
    virtual void ComputeSolution() = 0;
    const geometry::Mesh2& getMesh() const;
    topology::vertex ClosestInteriorPoint(const vec& x)const;
    topology::face OneRingContainer(topology::vertex center,const vec& x,bool& contained) const;
    int interiorId(topology::vertex v) const;
    bool isValuedVertex(topology::vertex v) const;
protected:
    verticesSubSet dirichletBorder;
    idMapper valuedID;
    void buildP1Basis();
    geometry::MeshRef MR;
    P1Base B;
    P1BaseGradient BGrad;
};

class ScalarFiniteElementSolver : public FiniteElementSolver{
public:
    ScalarFiniteElementSolver(geometry::MeshRef M);
    scalar sampleSolution(const vec& x) const;
    scalar sampleSolutionOnFace(const vec& x,const topology::face& f) const;
    vec sampleSolutionGradient(const vec& x) const;
    vec SolutionGradientOnFace(const topology::face& f) const;
    range SolutionValueRange() const;
    vec fullSolutionVector() const;
    virtual scalar solutionValueAtVertex(uint interior_id) const = 0;
    virtual const vec& solutionVector() const = 0;
    friend class cnc::FEMDisplayer;
protected:
    SMB computeMassMatrix() const;
    void buildRigidityMatrix();
    SMB RigidityMatrix;
};


class PoissonEquation : public ScalarFiniteElementSolver{
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

class MultiLayerSolver : public ScalarFiniteElementSolver {
public:
    MultiLayerSolver(geometry::MeshRef M) : ScalarFiniteElementSolver(M) {}
    virtual void ComputeSolution() override = 0;
    scalar solutionValueAtVertex(uint interior_id) const override{
        return solution[cursor](interior_id);
    }
    const vec& solutionVector() const override {
        return solution[cursor];
    }
    void setCursor(uint c) {
        cursor = c;
    }
protected:
    std::vector<vec> solution;
    uint cursor = 0;
};

class LaplaceEigenFunctions : public MultiLayerSolver {
public:
    LaplaceEigenFunctions(geometry::MeshRef M,uint n);
    void ComputeSolution() override;
    scalar getEigenValue() const{
        return eigenvalues[cursor];
    }
    std::vector<vec> foo_test();

private:
    vec Bh;
    std::vector<scalar> eigenvalues;
    uint n;
};

class WaveEquation : public MultiLayerSolver {
public:
    WaveEquation(geometry::MeshRef M,uint n,scalar dt);
    void setInitialConditions(const vec& U0,const vec& V0);
    void ComputeSolution() override;
private:
    void assemble_matricies();
    smat M,L,K;
    scalar dt,idt2;
    uint n;
    scalar delta = 0.5;
    scalar theta = delta*0.5+0.01;
    scalar l1 = 0.5 + delta - 2 * theta;
    scalar l2 = 0.5 - delta + theta;
};

}
}
}

#endif // FINITE_ELEMENTS_H
