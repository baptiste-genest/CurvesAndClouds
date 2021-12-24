#include "meshgeneration.h"

cnc::algo::geometry::Mesh2 cnc::algo::geometry::mesh_generation::Delaunay(const cnc::cloud &G,bool proj2D)
{
    if (G.size() < 3)
        throw "Convex hull only for more than 2 points";
    if (G.size() == 3){
        GeometricContextRef C = std::make_shared<GeometricContext>(G);
        Mesh2 H(C);
        H.F.insert(topology::assemble_face({0,1},2));
        H.induce_from_faces();
        return H;
    }
    static auto parabola_proj = [] (const vec& x){
        return vec({x(0),x(1),x.norm2()});
    };
    auto X = G;
    X.apply(parabola_proj);
    GeometricContextRef C = std::make_shared<GeometricContext>(std::move(X));
    auto H = giftWrapping(C);
    static auto lower_hull = [] (const Mesh2& M,const topology::face& F){
        return M.getNormal(F)(2) < 0;
    };
    H.filterFaces(lower_hull);
    if (proj2D){
        static auto proj2D = [] (const vec& x){
            return vec({x(0),x(1)});
        };
        H.G.get()->points.apply(proj2D);
        H.N.clear();
    }
    return H;
}
