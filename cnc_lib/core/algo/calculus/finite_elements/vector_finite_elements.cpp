#include "vector_finite_elements.h"

cnc::algo::FEM::VectorFiniteElementSolver::VectorFiniteElementSolver(cnc::algo::geometry::MeshRef M, cnc::algo::FEM::verticesSubSet dirichlet) : FiniteElementSolver(M,dirichlet)
{

}

cnc::algo::FEM::LinearElasticity::LinearElasticity(cnc::algo::geometry::MeshRef M, cnc::algo::FEM::verticesSubSet dirichlet, const cnc::algo::calculus::vector_function &f) : VectorFiniteElementSolver(M,dirichlet),force(f)
{

}

void cnc::algo::FEM::LinearElasticity::ComputeSolution()
{
    uint N=2*valuedID.size();
    assemble_matrices();
    SMB sum = tensor_term;
    sum *= mu/lambda;
    sum += div_term;
    sum *= lambda;
    auto S = algo::IterativeSolvers::ConjugateGradient(sum,force_term,vec(N),1e-20);
    for (const auto& vid : valuedID)
        V[vid.first] = linear_utils::vec2(S(2*vid.second),S(2*vid.second+1));
    for (const auto& v : dirichletBorder)
        V[v] = linear_utils::vec2();
}

cnc::vec cnc::algo::FEM::LinearElasticity::getSolutionNorm() const
{
    vec N(MR->nbVertices());
    for (const auto& vid : valuedID)
        N(vid.first) = V.at(vid.first).norm();
    return N;
}

void cnc::algo::FEM::LinearElasticity::setCoefficients(cnc::scalar l, cnc::scalar m)
{
    lambda = l;
    mu = m;
}

cnc::algo::calculus::vector_function cnc::algo::FEM::LinearElasticity::vec_basis(cnc::algo::topology::vertex v, const cnc::algo::topology::face &f, uint coord)
{
    auto b = B[v][f];
    if (coord == 0)
        return [b] (const vec& x){
            return linear_utils::vec2(b(x),0);
        };
    else
        return [b] (const vec& x){
            return linear_utils::vec2(0,b(x));
        };
}

cnc::scalar cnc::algo::FEM::LinearElasticity::dot(const cnc::mat &A, const cnc::mat &B)
{
    scalar s = 0;
    for (uint j = 0;j<2;j++)
        for (uint i = 0;i<2;i++)
            s += A(i,j)*B(i,j);
    return s;
}

cnc::mat cnc::algo::FEM::LinearElasticity::vector_field_gradient(const cnc::vec &grad, uint coord)
{
    mat G(2);
    for (uint j = 0;j<2;j++)
        G(coord,j) = grad(j);
    return G;
}

cnc::mat cnc::algo::FEM::LinearElasticity::deform_tensor(const cnc::vec &grad, uint coord)
{
    const auto& g = vector_field_gradient(grad,coord);
    return (g + g.transpose())*0.5;
}

void cnc::algo::FEM::LinearElasticity::assemble_matrices()
{
    const auto& M = *MR;
    const auto& C = *M.getContext();
    uint N = 2*valuedID.size();
    tensor_term = SMB(N,true);
    div_term = SMB(N,true);
    for (const auto& e : M.edges()){
        if (!isValuedVertex(e[0]) || !isValuedVertex(e[1]))
            continue;
        for (uint j = 0;j<2;j++){
            for (uint i = 0;i<2;i++){
                scalar S1 = 0;
                scalar S2 = 0;
                for (const auto& f : M.getAdjacentFaces(e)){
                    const auto& G1 = BGrad[e[0]][f];
                    const auto& G2 = BGrad[e[1]][f];
                    scalar A = C.faceArea(f);
                    S1 += 2*dot(deform_tensor(G1,j),deform_tensor(G2,i))*A;
                    S2 += G1(j)*G2(i)*A;
                }
                tensor_term(2*valuedID[e[0]]+i,2*valuedID[e[1]]+j) = S1;
                div_term(2*valuedID[e[0]]+i,2*valuedID[e[1]]+j) = S2;
            }
        }
    }
    for (const auto& vid : valuedID){
        auto v = vid.first;
        for (uint j = 0;j<2;j++){
            for (uint i = 0;i<2;i++){
                scalar S1 = 0;
                scalar S2 = 0;
                for (const auto& psi : BGrad[v]){
                    const auto& g = psi.second;
                    scalar A = C.faceArea(psi.first);
                    S1 += 2*dot(deform_tensor(g,j),deform_tensor(g,i))*A;
                    S2 += g(j)*g(i)*A;
                }
                tensor_term(2*vid.second+j,2*vid.second+i) = S1;
                div_term(2*vid.second+j,2*vid.second+i) = S2;
            }
        }
    }
    force_term = vec(N);
    for (const auto& vid : valuedID){
        auto v = vid.first;
        for (const auto& f : B[v]){
            auto m = C.midPoint(f.first);
            for (uint i = 0;i<2;i++)
                force_term(2*vid.second+i) += force(m)(i)*f.second(m)*C.faceArea(f.first);
        }
    }
}
