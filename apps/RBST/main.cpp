#include <curvesandcloudCore.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;

void print(const topology::faces& F){
    std::cout << "FACES ";
    for (const auto& f : F){
        std::cout << " : ";
        for (vertex v : get_vertices(f))
            std::cout << v << ' ';
    }
    std::cout << std::endl;
}

int main()
{
    face f1 = topology::assemble_face(0,1,2);
    face f2 = topology::assemble_face(1,2,3);
    face f3 = topology::assemble_face(2,3,4);
    faces F = {f1,f2,f3};
    mesh_generation::RBST T;
    T.insert(f2,{0,2});
    T.insert(f1,{-1,1});
    T.insert(f3,{-4,4});
    print(T.get(0.5,F));
    print(T.get(-0.5,F));
    print(T.get(1.5,F));
    print(T.get(3,F));
    print(T.get(-3,F));

    return 0;
}
