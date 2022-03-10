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


int main()
{
    using namespace std;
    Variable x,y;
    auto E1 = cos(x) + cos(y) + cos(x);
    auto E2 = cos(x) + cos(x) + cos(y);
    auto E3 = sin(x);
    cout << E1.print() << std::endl;
    cout << E2.print() << std::endl;
    cout << (E1 == E2) << std::endl;
    cout << (E1 == E3) << std::endl;

    return 0;
}
