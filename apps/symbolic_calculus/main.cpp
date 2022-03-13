#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::symbolic;

struct netnode {
    std::vector<Variable> weights;
    Variable biais;
    Expression output;

    netnode() {}

    netnode(uint previous_layer_size,const std::vector<Variable>& inputs) :
        weights(previous_layer_size)
    {
        Expression C;
        for (uint i = 0;i<previous_layer_size;i++)
            C = C + inputs[i]*weights[i];
        output = sigmoid(C + biais);

        std::cout << output.print() << std::endl;
    }
};

struct network {
    std::vector<std::vector<netnode>> layers;
    std::vector<std::vector<Variable>> netVariables;
    std::vector<Expression> outs;
    network(const std::vector<uint>& heights){
        uint W = heights.size();
        for (const auto& h : heights)
            netVariables.push_back(std::vector<Variable>(h));
        for (uint i = 1;i<W;i++){
            for (uint j = 0;j<heights[i];j++){
                layers[j][i] = netnode(heights[i-1],netVariables[i-1]);
            }
        }
        for (uint i = W-1;i<W;i++){
            for (uint j = 0;j<heights[i];j++){
                layers[j][i] = netnode(heights[i-1],netVariables[i-1]);
            }
        }
    }
};

using Tensor = std::vector<smat>;
smat invert_diag_smat(const smat& S){
    uint n = S.getHeight();
    smat iM(n,n);
    for (uint i = 0;i<n;i++)
        iM(i,i) = pow(S(i,i),-1).simplify();
    return iM;
}


Tensor getChristoffelSymbols(const smat& g){
    auto ig = invert_diag_smat(g);
    uint N = g.getHeight();
    Tensor gamma(N,smat(N,N));
    for (uint i = 0;i<N;i++)
        for (uint k = 0;k<N;k++)
            for (uint l = 0;l<N;l++){
                for (uint m = 0;m<N;m++)
                    gamma[i](l,k) = gamma[i](l,k) + ig(m,i)*(g(k,m)(l) + g(l,m)(k) - g(l,k)(m));
                gamma[i](l,k) = (0.5*gamma[i](l,k)).simplify();
            }
    return gamma;
}


int main()
{
    using namespace std;
    smat g(3,3);
    Variable r,th,phi;
    scalar m = 0.5;
    auto schwartz = (r-2*m)/(r);
    g(0,0) = 1.;
    g(1,1) = pow(r,2);
    g(2,2) = pow(sin(th)*r,2);
    auto G = getChristoffelSymbols(g);
    for (const auto& Gk : G){
        std::cout << Gk.print() << std::endl;
    }
    /*
    uint N = 2;
    for (uint i = 0;i<N;i++)
        for (uint k = 0;k<N;k++)
            for (uint l = 0;l<N;l++){
                std::cout << G[i](k,l).simplify() << std::endl;
            }
            */
    return 0;
}
