#include "finite_elements.h"

cnc::algo::FEM::FiniteElementSolver::FiniteElementSolver(cnc::algo::geometry::MeshRef mesh) : MR(mesh),M(*MR)
{
    buildP1Basis();
}

void cnc::algo::FEM::FiniteElementSolver::buildRigidityMatrix()
{
    const auto& C = *M.getContext();
    const auto& IV = M.getInteriorVertices();
    RigidityMatrix = SMB(IV.size(),true);
    for (const auto& e : M.interiorEdges()){
        std::map<int,int> ids;
        bool interior = true;
        for (uint i = 0;i<2;i++){
            auto it = std::find(IV.begin(),IV.end(),e[i]);
            if (it == IV.end())
                interior = false;
            else
                ids[e[i]] = std::distance(IV.begin(),it);
        }
        if (interior){
            scalar S = 0;
            for (const auto& f : M.getAdjacentFaces(e)){
                const auto& G1 = BGrad[e[0]][f];
                const auto& G2 = BGrad[e[1]][f];
                S += G1.scalar_product(G2)*C.faceArea(f);
            }
            RigidityMatrix(ids[e[0]],ids[e[1]]) = S;
        }
    }
    uint id = 0;
    for (auto it = M.getInteriorVertices().begin();it != M.getInteriorVertices().end();++it){
        scalar SG = 0;
        for (const auto& psi : BGrad[*it]){
            scalar mu = C.faceArea(psi.first);
            SG += psi.second.norm2()*mu;
        }
        RigidityMatrix(id,id) = SG;
        id++;
    }
}

cnc::scalar cnc::algo::FEM::FiniteElementSolver::sampleSolution(const cnc::vec &x) const
{
    auto cp = ClosestInteriorPoint(x);
    bool in;
    auto f = OneRingContainer(cp,x,in);
    if (!in)
        return 0.;
    return sampleSolutionOnFace(x,f);
}

cnc::scalar cnc::algo::FEM::FiniteElementSolver::sampleSolutionOnFace(const cnc::vec &x, const cnc::algo::topology::face &f) const
{
    scalar val = 0;
    for (auto v : topology::get_indexed_vertices(f)){
        int id = interiorId(v);
        if (id > 0)
            val += B.at(v).at(f)(x)*solutionValueAtVertex(id);
    }
    return val;
}

cnc::vec cnc::algo::FEM::FiniteElementSolver::sampleSolutionGradient(const cnc::vec &x) const
{
    auto cp = ClosestInteriorPoint(x);
    bool in;
    auto f = OneRingContainer(cp,x,in);
    if (!in)
        return vec(2);
    vec g = vec(2);
    for (auto v : topology::get_indexed_vertices(f)){
        int id = interiorId(v);
        if (id > 0)
            g += BGrad.at(v).at(f)*solutionValueAtVertex(id);
    }
    return g;

}

cnc::vec cnc::algo::FEM::FiniteElementSolver::SolutionGradientOnFace(const cnc::algo::topology::face &f) const
{
    vec g = vec(2);
    for (auto v : topology::get_indexed_vertices(f)){
        int id = interiorId(v);
        if (id > 0)
            g += BGrad.at(v).at(f)*solutionValueAtVertex(id);
    }
    return g;
}

cnc::range cnc::algo::FEM::FiniteElementSolver::SolutionValueRange() const
{
    auto v = solutionVector().data();
    return {*std::min_element(v.begin(),v.end()),*std::max_element(v.begin(),v.end())};
}

cnc::algo::topology::vertex cnc::algo::FEM::FiniteElementSolver::ClosestInteriorPoint(const cnc::vec &x) const
{
    const auto& C = *M.getContext();
    const auto& IV = M.getInteriorVertices();
    auto cp = *std::min_element(IV.begin(),IV.end(),[C,x](topology::vertex a,topology::vertex b){
        return C(a).distance2(x) < C(b).distance2(x);
    });
    return cp;
}

cnc::algo::topology::face cnc::algo::FEM::FiniteElementSolver::OneRingContainer(cnc::algo::topology::vertex center, const cnc::vec &x,bool& contained) const
{
    contained = true;
    for (const auto& f : M.getOneRingFaces(center))
        if (M.getContext()->insideFace(f,x))
            return f;
    contained = false;
    return topology::face();
}

int cnc::algo::FEM::FiniteElementSolver::interiorId(cnc::algo::topology::vertex v) const
{
    const auto& IV = M.getInteriorVertices();
    auto it = IV.find(v);
    if (it == IV.end())
        return -1;
    return std::distance(IV.begin(),it);
}

const cnc::algo::geometry::Mesh2 &cnc::algo::FEM::FiniteElementSolver::getMesh() const
{
    return M;
}

void cnc::algo::FEM::FiniteElementSolver::buildP1Basis()
{
    M.computeConnectivity();
    mat A(3),IA(3);
    for (uint j = 0;j<3;j++)
        A(2,j) = 1;
    const auto& C = *M.getContext();
    vec basis[3] = {vec({1.,0.,0.}),vec({0.,1.,0.}),vec({0.,0.,1.})};
    for (const auto& f : M.faces()){
        auto V = topology::get_indexed_vertices(f);
        for (uint j = 0;j<3;j++){
            A(0,j) = C(V[j])(0);
            A(1,j) = C(V[j])(1);
        }
        IA = A.invert();
        for (uint j = 0;j<3;j++){
            vec L = IA*basis[j];
            B[V[j]][f] = [L] (const vec& x){
                return L(0)*x(0) + L(1)*x(1) + L(2);
            };
            BGrad[V[j]][f] = vec({L(0),L(1)});
        }
    }
    buildRigidityMatrix();
}

cnc::algo::FEM::PoissonEquation::PoissonEquation(cnc::algo::geometry::MeshRef mesh,const calculus::scalar_function& potential) : FiniteElementSolver(mesh), f(potential)
{
}

void cnc::algo::FEM::PoissonEquation::ComputeSolution()
{
    const auto& C = *M.getContext();
    Bh = vec(M.getInteriorVertices().size());
    uint id = 0;
    for (auto it = M.getInteriorVertices().begin();it != M.getInteriorVertices().end();++it){
        scalar quadrature = 0;
        for (const auto& psi : BGrad[*it]){
            auto m = C.midPoint(psi.first);
            quadrature += B[*it][psi.first](m)*f(m)*C.faceArea(psi.first);
        }
        Bh(id) = quadrature;
        id++;
    }
    solution = algo::IterativeSolvers::ConjugateGradient(RigidityMatrix,Bh,vec(Bh.size()),1e-20);
}

cnc::scalar cnc::algo::FEM::PoissonEquation::solutionValueAtVertex(uint interior_id) const
{
    return solution(interior_id);
}

const cnc::vec &cnc::algo::FEM::PoissonEquation::solutionVector() const
{
    return solution;
}


cnc::algo::FEM::LaplaceEigenFunctions::LaplaceEigenFunctions(cnc::algo::geometry::MeshRef mesh, uint N) : FiniteElementSolver(mesh) , n(N)
{

}

void cnc::algo::FEM::LaplaceEigenFunctions::ComputeSolution()
{
    const auto& C = *M.getContext();
    const auto& IV = M.getInteriorVertices();
    SMB MassMatrix(IV.size(),true);
    for (const auto& e : M.interiorEdges()){
        int ids[2];
        bool interior = true;
        for (uint i = 0;i<2;i++){
            auto it = std::find(IV.begin(),IV.end(),e[i]);
            if (it == IV.end())
                interior = false;
            else
                ids[i] = std::distance(IV.begin(),it);
        }
        if (interior){
            scalar S = 0;
            for (const auto& f : M.getAdjacentFaces(e)){
                const auto& G1 = B[e[0]][f];
                const auto& G2 = B[e[1]][f];
                scalar s = 0;
                for (const auto& me : f){
                    auto m = C.midPoint(me);
                    s += G1(m)*G2(m)*0.33333;
                }
                S += s*C.faceArea(f);
            }
            MassMatrix(ids[0],ids[1]) = S;
        }
    }
    uint id = 0;
    for (auto it = IV.begin();it != IV.end();++it){
        scalar SG = 0;
        for (const auto& psi : B[*it]){
            scalar s = 0;
            for (const auto& me : psi.first){
                auto m = C.midPoint(me);
                s += psi.second(m)*psi.second(m)*0.33333;
            }
            SG += s*C.faceArea(psi.first);
        }
        MassMatrix(id,id) = SG;
        id++;
    }
    auto RM = RigidityMatrix.DenseMatrix();
    auto MM = MassMatrix.DenseMatrix();
    auto L = algo::Cholesky(MM);
    auto ILR = algo::Cholesky(RM).invert_triangular(LOWER);
    auto IRM = ILR.transpose()*ILR;
    auto K = L.transpose()*IRM*L;
    auto eigen = K.lanczos(n);
    solution.resize(n);
    eigenvalues.resize(n);
    for (uint i = 0;i<n;i++){
        solution[i] = L.transpose().upper_solve(eigen[i].vector).normalize();
        eigenvalues[i] = 1./(K*solution[i]).norm();
    }
}


cnc::vec cnc::algo::FEM::LaplaceEigenFunctions::fullSolutionVector() const
{
    vec S(M.nbVertices());
    uint i = 0;
    for (const auto& v : M.getInteriorVertices()){
        S(v) = solution[cursor](i);
        i++;
    }
    return S;
}
