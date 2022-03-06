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

Variable a,b,c,d;
ValuationSystem ValuateVector(const cscalar& z,const cscalar& w){
    ValuationSystem V = {a == z.real(),
                         b == z.imag(),
                         c == w.real(),
                         d == w.imag()};
    return V;
}

int main(int argc, char *argv[])
{
    Variable x,y;
    auto E1 = x*y + cos(x);

    return 0;
}
