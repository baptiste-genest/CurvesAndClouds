#ifndef MESHGENERATION_H
#define MESHGENERATION_H

#include "mesh2.h"
#include "convex_hull.h"
#include "convexpolygon.h"
#include "../statistics/random_var.h"
#include "../../algo/data_structures/interval_tree.h"
#include <queue>

namespace cnc {
namespace algo {
namespace geometry {

struct mesh_generation {
    static Mesh2 Delaunay(const cloud& X,bool proj2D = true);
    static Mesh2 BowyerWatson(const cloud& X);
    static Mesh2 BowyerWatsonWIP(const cloud& X);
    static Mesh2 LloydRelaxation(const ConvexPolygon& B,uint N);
    static Mesh2 LaplacianRelaxation(const std::vector<ConvexPolygon>& B,const ShapePredicate& P,uint N);
    static Mesh2 FromBoundaryMesh(cloud B,scalar treshold,const ShapePredicate& P);
    static MeshRef Refinement(const cloud& B,const ShapePredicate& P,scalar h);
    using RBST = data_structure::IntervalTree<face>;
private:
    static void split_long_edges(Mesh2& M,scalar h);
    static void collapse_short_edges(Mesh2& M,scalar l,scalar h);
    static void equalize_valences(Mesh2& M);
    static void relaxation(Mesh2& M);
    static std::array<cnc::vec,3> super_triangle(const cloud& X);
    static bool inCircle(const vec& x,const vec& center,scalar r);
    static void insertVertexInDelaunay(Mesh2& M,vertex x);

    using circumMap = std::map<topology::face,circum_data,topology::faceComp>;

    /*
    struct RBSTnode{
        scalar v;
        bool upper;
        RBSTnode* L = nullptr;
        RBSTnode* R = nullptr;
        face f;
        ~RBSTnode(){
            delete L;
            delete R;
        }
    };
    */

public:
    /*
    class RBST {
    public:
        void insert(const face& f,range r);
        topology::faces get(scalar x,const faces& t) const;
        ~RBST(){
            delete head;
        }
    private:
        void get(RBSTnode* n,topology::faces& f,scalar x,const faces& t) const;
        void insert_right(RBSTnode** n,const face& f,scalar x);
        void insert_left(RBSTnode** n,const face& f,scalar x);
        RBSTnode* head = nullptr;
    };
    */
    static void insertCircum(RBST& RX,RBST& RY,const face& f,const GeometricContext& C,circumMap& M);
    static topology::faces nonDelaunayTriangles(const vec& x,const RBST& X,const RBST& Y,const circumMap& M,const faces& T);

};
}
}
}

#endif // MESHGENERATION_H
