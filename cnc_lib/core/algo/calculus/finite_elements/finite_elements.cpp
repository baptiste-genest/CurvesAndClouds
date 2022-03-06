#include "finite_elements.h"
int cnc::algo::FEM::FiniteElementSolver::interiorId(cnc::algo::topology::vertex v) const
{
    const auto& IV = MR->getInteriorVertices();
    auto it = IV.find(v);
    if (it == IV.end())
        return -1;
    return std::distance(IV.begin(),it);
}

bool cnc::algo::FEM::FiniteElementSolver::isValuedVertex(cnc::algo::topology::vertex v) const
{
    return dirichletBorder.find(v) == dirichletBorder.end();
}

const cnc::algo::geometry::Mesh2 &cnc::algo::FEM::FiniteElementSolver::getMesh() const
{
    return *MR;
}
cnc::algo::FEM::FiniteElementSolver::FiniteElementSolver(cnc::algo::geometry::MeshRef mesh, const cnc::algo::FEM::verticesSubSet &Dirichlet) : MR(mesh), dirichletBorder(Dirichlet)
{
    MR->computeConnectivity();
    uint i = 0;
    for (auto v : mesh->getInteriorVertices()){
        valuedID[v] = i;
        i++;
    }
    for (auto v : mesh->getBoundaryVertices()){
        if (Dirichlet.find(v) == Dirichlet.end()){
            valuedID[v] = i;
            i++;
        }
    }
    buildP1Basis();
}

cnc::algo::FEM::FiniteElementSolver::FiniteElementSolver(cnc::algo::geometry::MeshRef mesh) : FiniteElementSolver(mesh,mesh->getBoundaryVertices())
{
}

cnc::algo::topology::vertex cnc::algo::FEM::FiniteElementSolver::ClosestInteriorPoint(const cnc::vec &x) const
{
    const auto& C = *(*MR).getContext();
    const auto& IV = (*MR).getInteriorVertices();
    auto cp = *std::min_element(IV.begin(),IV.end(),[C,x](topology::vertex a,topology::vertex b){
        return C(a).distance2(x) < C(b).distance2(x);
    });
    return cp;
}

cnc::algo::topology::face cnc::algo::FEM::FiniteElementSolver::OneRingContainer(cnc::algo::topology::vertex center, const cnc::vec &x,bool& contained) const
{
    contained = true;
    for (const auto& f : MR->getOneRingFaces(center))
        if (MR->getContext()->insideFace(f,x))
            return f;
    contained = false;
    return topology::face();
}

void cnc::algo::FEM::FiniteElementSolver::buildP1Basis()
{
    auto& M = *MR;
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
            if (isValuedVertex(V[j])){
                B[V[j]][f] = [L] (const vec& x){
                    return L(0)*x(0) + L(1)*x(1) + L(2);
                };
                BGrad[V[j]][f] = vec({L(0),L(1)});
            }
        }
    }
}

cnc::algo::FEM::ScalarFiniteElementSolver::ScalarFiniteElementSolver(cnc::algo::geometry::MeshRef mesh) : FiniteElementSolver(mesh)
{
    buildRigidityMatrix();
}

void cnc::algo::FEM::ScalarFiniteElementSolver::buildRigidityMatrix()
{
    const auto& M = *MR;
    const auto& C = *M.getContext();
    RigidityMatrix = SMB(valuedID.size(),true);
    for (const auto& e : M.edges()){
        scalar S = 0;
        if (!isValuedVertex(e[0]) || !isValuedVertex(e[1]))
            continue;
        for (const auto& f : M.getAdjacentFaces(e)){
            const auto& G1 = BGrad[e[0]][f];
            const auto& G2 = BGrad[e[1]][f];
            S += G1.scalar_product(G2)*C.faceArea(f);
        }
        RigidityMatrix(valuedID[e[0]],valuedID[e[1]]) = S;
    }
    for (const auto& vid : valuedID){
        auto v = vid.first;
        scalar SG = 0;
        for (const auto& psi : BGrad[v]){
            scalar mu = C.faceArea(psi.first);
            SG += psi.second.norm2()*mu;
        }
        RigidityMatrix(vid.second,vid.second) = SG;
    }
}

cnc::scalar cnc::algo::FEM::ScalarFiniteElementSolver::sampleSolution(const cnc::vec &x) const
{
    auto cp = ClosestInteriorPoint(x);
    bool in;
    auto f = OneRingContainer(cp,x,in);
    if (!in)
        return 0.;
    return sampleSolutionOnFace(x,f);
}

cnc::scalar cnc::algo::FEM::ScalarFiniteElementSolver::sampleSolutionOnFace(const cnc::vec &x, const cnc::algo::topology::face &f) const
{
    scalar val = 0;
    for (auto v : topology::get_indexed_vertices(f)){
        int id = interiorId(v);
        if (id > 0)
            val += B.at(v).at(f)(x)*solutionValueAtVertex(id);
    }
    return val;
}

cnc::vec cnc::algo::FEM::ScalarFiniteElementSolver::sampleSolutionGradient(const cnc::vec &x) const
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

cnc::vec cnc::algo::FEM::ScalarFiniteElementSolver::SolutionGradientOnFace(const cnc::algo::topology::face &f) const
{
    vec g = vec(2);
    for (auto v : topology::get_indexed_vertices(f)){
        int id = interiorId(v);
        if (id > 0)
            g += BGrad.at(v).at(f)*solutionValueAtVertex(id);
    }
    return g;
}

cnc::range cnc::algo::FEM::ScalarFiniteElementSolver::SolutionValueRange() const
{
    auto v = solutionVector().data();
    return {*std::min_element(v.begin(),v.end()),*std::max_element(v.begin(),v.end())};
}
cnc::vec cnc::algo::FEM::ScalarFiniteElementSolver::fullSolutionVector() const
{
    vec S(MR->nbVertices());
    for (const auto& v : valuedID)
        S(v.first) = solutionVector()(v.second);
    return S;
}

cnc::sparse::SMB cnc::algo::FEM::ScalarFiniteElementSolver::computeMassMatrix() const
{
    const geometry::Mesh2& M = *MR;
    const auto& C = *M.getContext();
    SMB MassMatrix(valuedID.size(),true);
    for (const auto& e : M.edges()){
        if (!isValuedVertex(e[0]) || !isValuedVertex(e[1]))
            continue;
        scalar S = 0;
        for (const auto& f : M.getAdjacentFaces(e)){
            const auto& G1 = B.at(e[0]).at(f);
            const auto& G2 = B.at(e[1]).at(f);
            scalar s = 0;
            for (const auto& me : f){
                auto m = C.midPoint(me);
                s += G1(m)*G2(m)*0.33333;
            }
            S += s*C.faceArea(f);
        }
        MassMatrix(valuedID.at(e[0]),valuedID.at(e[1])) = S;
    }
    for (const auto& v : valuedID){
        scalar SG = 0;
        for (const auto& psi : B.at(v.first)){
            scalar s = 0;
            for (const auto& me : psi.first){
                auto m = C.midPoint(me);
                s += psi.second(m)*psi.second(m)*0.33333;
            }
            SG += s*C.faceArea(psi.first);
        }
        MassMatrix(v.second,v.second) = SG;
    }
    return MassMatrix;
}
cnc::algo::FEM::PoissonEquation::PoissonEquation(cnc::algo::geometry::MeshRef mesh,const calculus::scalar_function& potential) : ScalarFiniteElementSolver(mesh), f(potential)
{
}

void cnc::algo::FEM::PoissonEquation::ComputeSolution()
{
    const auto& M = *MR;
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


cnc::algo::FEM::LaplaceEigenFunctions::LaplaceEigenFunctions(cnc::algo::geometry::MeshRef mesh, uint N) : MultiLayerSolver(mesh) , n(N)
{

}

void cnc::algo::FEM::LaplaceEigenFunctions::ComputeSolution()
{
    auto RM = RigidityMatrix.DenseMatrix();
    auto MM = ScalarFiniteElementSolver::computeMassMatrix().DenseMatrix();
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

std::vector<cnc::vec> cnc::algo::FEM::LaplaceEigenFunctions::foo_test()
{
    std::vector<cnc::vec> EV;
    auto RM = RigidityMatrix.DenseMatrix();
    auto MM = ScalarFiniteElementSolver::computeMassMatrix().DenseMatrix();
    auto L = algo::Cholesky(MM);
    auto ILR = algo::Cholesky(RM).invert_triangular(LOWER);
    auto IRM = ILR.transpose()*ILR;
    auto K = L.transpose()*IRM*L;


    return EV;
}

cnc::algo::FEM::WaveEquation::WaveEquation(cnc::algo::geometry::MeshRef mesh, uint nb, cnc::scalar t):MultiLayerSolver(mesh),dt(t),n(nb)
{
    assemble_matricies();
}

void cnc::algo::FEM::WaveEquation::setInitialConditions(const cnc::vec &U0, const cnc::vec &V0)
{
    solution.push_back(U0);
    solution.push_back(U0+V0*dt);
}

void cnc::algo::FEM::WaveEquation::ComputeSolution()
{
    uint i = solution.size()-1;
    const auto& Un = solution[i];
    const auto& Up = solution[i-1];
    auto b = M*(Un*2-Up)*idt2 - K*(Un*l1 + Up*l2);
    solution.push_back(IterativeSolvers::ConjugateGradient(L,b,Un,1e-30));
    cursor = i+1;
}

void cnc::algo::FEM::WaveEquation::assemble_matricies()
{
    idt2 = 1./(dt*dt);
    auto SM = computeMassMatrix();
    const auto& SK = RigidityMatrix;
    auto SL = SM;
    SL *= idt2/theta;
    SL += SK;
    SL *= theta;
    M= smat(SM);
    L = smat(SL);
    K = smat(SK);

}
