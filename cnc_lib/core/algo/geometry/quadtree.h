#ifndef QUADTREE_H
#define QUADTREE_H

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

#include "core/algo/algo.h"
#include "core/algo/linear_algebra/lin_alg.h"
#include "cnc_error.h"
#include "cnc_types.h"

namespace cnc {

namespace algo {

namespace geometry {


namespace quadtree_loc {
    enum location {
        lower_left,
        lower_right,
        upper_left,
        upper_right
    };
    enum direction {
        top,
        right,
        bottom,
        left
    };
}

class Quadtree
{
public:
    Quadtree(const range& X,const range& Y);
    ~Quadtree();

    void insert(const vec* x);

    const Quadtree* find(const vec& x,scalar eps) const;

    void print(uint pads = 0) const ;
    std::vector<std::vector<vec>> get_lines() const;
    std::vector<Quadtree*> get_adjacents_cells() const;
    uint get_deep() const;
    uint get_height() const;
    std::vector<Quadtree*> get_empty_leafs();
    std::vector<Quadtree*> get_full_leafs();
    std::vector<Quadtree*> get_leafs();

    bool in_x_range(scalar x) const ;
    bool in_y_range(scalar y) const ;
    bool in_ranges(const vec& x) const;

    inline const Quadtree* get_parent() const {
        return parent;
    }

    inline vec get_mid_point() const {
        return vec({mx,my});
    }

    void subdivide();

    quadtree_loc::direction get_adjacency_dir(Quadtree* o) const;

    std::vector<vec> get_corners() const;

private:
    void inner_insert(const vec* x,uint iter = 0);
    bool check_adjacency(const Quadtree* q) const;
    void get_empty_leafs(std::vector<Quadtree*>& L);
    void get_full_leafs(std::vector<Quadtree*>& L);
    void get_leafs(std::vector<Quadtree*>& L);
    void get_adjacents_cells(const Quadtree* x,std::vector<Quadtree*>& L,std::vector<Quadtree*>& V);
    quadtree_loc::location get_corner(const vec& x) const;

    inline bool is_empty() const;
    inline bool is_leaf() const;

    Quadtree(const range& X,const range& Y,uint factor,Quadtree* p);
    void get_lines(std::vector<std::vector<vec>>& L) const;

    void create_leafs();

    range x_range;
    range y_range;
    scalar mx,my;
    uint scale_factor;

    std::array<Quadtree*,4> leafs;
    Quadtree* parent;

    constexpr static scalar eps = 1e-15;

    const vec* val;

    friend class QuadtreeMesher;
};

}
}
}

#endif // QUADTREE_H
