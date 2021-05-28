#include "quadtree.h"

cnc::algo::geometry::Quadtree::Quadtree(const range& X,const range& Y,uint factor,Quadtree* p) : x_range(X) , y_range(Y) , parent(p)
{
    if (!is_sorted(X))
        throw Cnc_error("Can't build quadtree with X range unsorted");
    if (!is_sorted(Y))
        throw Cnc_error("Can't build quadtree with Y range unsorted");
    if (X.second - X.first < eps)
        throw Cnc_error("Can't build quadtree with X range space below epsilon");
    if (Y.second - Y.first < eps)
        throw Cnc_error("Can't build quadtree with Y range space below epsilon");

    for (uint k = 0;k<4;k++)
        leafs[k] = nullptr;
    val = nullptr;
    scale_factor = factor;
    mx = (x_range.first + x_range.second)*0.5;
    my = (y_range.first + y_range.second)*0.5;
}

cnc::algo::geometry::Quadtree::Quadtree(const cnc::range &X, const cnc::range &Y) : Quadtree(X,Y,0,nullptr)
{}

cnc::algo::geometry::Quadtree::~Quadtree()
{
    for (uint k = 0;k<4;k++)
        delete leafs[k];
    val = nullptr;
}

void cnc::algo::geometry::Quadtree::insert(const cnc::vec *x)
{
    if (find(*x,eps) == nullptr)
        inner_insert(x);
}

void cnc::algo::geometry::Quadtree::inner_insert(const cnc::vec *x,uint iter)
{
    if (iter > 10)
        throw Cnc_error("Probable error with insertion");
    if (is_empty()){
        val = x;
        return;
    }
    create_leafs();
    if (val != nullptr){
        quadtree_loc::location vc = get_corner(*val);
        leafs[vc]->inner_insert(val,iter+1);
        val = nullptr;
    }
    quadtree_loc::location xc = get_corner(*x);
    leafs[xc]->inner_insert(x,iter+1);
}

const cnc::algo::geometry::Quadtree *cnc::algo::geometry::Quadtree::find(const cnc::vec &x, cnc::scalar eps) const
{
    if (is_empty())
        return nullptr;
    else if (is_leaf())
        return (x.distance(*val) < eps) ? this : nullptr;
    else
        return leafs[get_corner(x)]->find(x,eps);
}

cnc::algo::geometry::quadtree_loc::location cnc::algo::geometry::Quadtree::get_corner(const cnc::vec &p) const
{
    using namespace quadtree_loc;
    const scalar& x = p(0),y = p(1);
    if (x <= mx && y <= my)
        return lower_left;
    if (x >= mx && y <= my)
        return lower_right;
    if (x <= mx && y >= my)
        return upper_left;
    return upper_right;
}

bool cnc::algo::geometry::Quadtree::check_adjacency(const cnc::algo::geometry::Quadtree *q) const
{
    return inter_type(x_range,q->x_range) + inter_type(y_range,q->y_range) > 2;
}

std::vector<cnc::algo::geometry::Quadtree *> cnc::algo::geometry::Quadtree::get_empty_leafs()
{
    std::vector<Quadtree*> EL;
    get_empty_leafs(EL);
    return EL;
}

std::vector<cnc::algo::geometry::Quadtree *> cnc::algo::geometry::Quadtree::get_full_leafs()
{
    std::vector<Quadtree*> FL;
    get_full_leafs(FL);
    return FL;
}

std::vector<cnc::algo::geometry::Quadtree *> cnc::algo::geometry::Quadtree::get_leafs()
{
    std::vector<Quadtree*> L;
    get_leafs(L);
    return L;
}

bool cnc::algo::geometry::Quadtree::in_x_range(cnc::scalar x) const
{
    return (x >= x_range.first) && (x <= x_range.second);
}

bool cnc::algo::geometry::Quadtree::in_y_range(cnc::scalar y) const
{
    return (y >= y_range.first) && (y <= y_range.second);
}

bool cnc::algo::geometry::Quadtree::in_ranges(const cnc::vec &x) const
{
    return in_x_range(x(0)) && in_y_range(x(1));
}

void cnc::algo::geometry::Quadtree::subdivide()
{
    create_leafs();
    if (val != nullptr){
        vec* tmp = (vec*)val;
        val = nullptr;
        insert(tmp);
    }
}

cnc::algo::geometry::quadtree_loc::direction cnc::algo::geometry::Quadtree::get_adjacency_dir(cnc::algo::geometry::Quadtree *o) const
{
    constexpr static scalar eps = 1e-12;
    set_type X = inter_type(x_range,o->x_range);
    if (X == continuous){
        if (std::abs(y_range.second - o->y_range.first) < eps)
            return quadtree_loc::top;
        return quadtree_loc::bottom;
    }
    else {
        if (std::abs(x_range.second - o->x_range.first) < eps)
            return quadtree_loc::right;
        return quadtree_loc::left;
    }
}

std::vector<cnc::vec> cnc::algo::geometry::Quadtree::get_corners() const
{
    std::vector<vec> C(4);
    scalar X[2] = {x_range.first,x_range.second};
    scalar Y[2] = {y_range.first,y_range.second};
    C[0] = vec({X[0],Y[1]});
    C[1] = vec({X[1],Y[1]});
    C[2] = vec({X[1],Y[0]});
    C[3] = vec({X[0],Y[0]});
    return C;
}

void cnc::algo::geometry::Quadtree::get_empty_leafs(std::vector<cnc::algo::geometry::Quadtree *> &L)
{
    if (is_empty())
        L.push_back(this);
    else if (!is_leaf())
        for (uint k = 0;k<4;k++)
            leafs[k]->get_empty_leafs(L);
}

void cnc::algo::geometry::Quadtree::get_full_leafs(std::vector<cnc::algo::geometry::Quadtree *> &L)
{
    if (is_leaf() && val != nullptr)
        L.push_back(this);
    else if (!is_leaf())
        for (uint k = 0;k<4;k++)
            leafs[k]->get_full_leafs(L);
}

void cnc::algo::geometry::Quadtree::get_leafs(std::vector<cnc::algo::geometry::Quadtree *> &L)
{
    if (is_leaf())
        L.push_back(this);
    else
        for (uint k = 0;k<4;k++)
            leafs[k]->get_leafs(L);
}

bool cnc::algo::geometry::Quadtree::is_empty() const {
    bool empty = (val == nullptr) && is_leaf();
    return empty;
}

bool cnc::algo::geometry::Quadtree::is_leaf() const
{
    bool leaf = true;
    for (uint k = 0;k<4;k++)
        leaf &= leafs[k] == nullptr;
    return leaf;
}

void cnc::algo::geometry::Quadtree::print(uint pads) const
{
    using namespace quadtree_loc;
    const static std::string pad = " ";
    const static std::string loc_names[4] = {"LL","LR","UL","UR"};
    std::string padding;
    for (uint k = 0;k<pads;k++)
        padding += pad;
    location l;
    std::cout << padding;
    printf("[%f,%f]x[%f,%f] ",x_range.first,x_range.second,y_range.first,y_range.second);
    if (val != nullptr)
        std::cout << "val: \n" << *val;
    std::cout << std::endl;
    if (is_leaf())
        return;
    for (uint k = 0;k<4;k++){
        l = static_cast<location>(k);
        std::cout << padding << loc_names[k] << ":" << std::endl;
        leafs[l]->print(pads+1);
    }
}

std::vector<std::vector<cnc::vec> > cnc::algo::geometry::Quadtree::get_lines() const
{
    std::vector<std::vector<cnc::vec> > L;
    get_lines(L);
    return L;
}

std::vector<cnc::algo::geometry::Quadtree *> cnc::algo::geometry::Quadtree::get_adjacents_cells() const
{
    std::vector<Quadtree*> neighbors;
    std::vector<Quadtree*> visited;
    parent->get_adjacents_cells(this,neighbors,visited);
    return neighbors;
}

uint cnc::algo::geometry::Quadtree::get_deep() const
{
    if (parent == nullptr)
        return 0;
    return 1+ parent->get_deep();
}

uint cnc::algo::geometry::Quadtree::get_height() const
{
    if (is_leaf())
        return 1;
    uint h = 0;
    for (uint k = 0;k<4;k++)
        h = std::max(h,leafs[k]->get_height());
    return 1+h;
}

void cnc::algo::geometry::Quadtree::get_adjacents_cells(const cnc::algo::geometry::Quadtree *q,std::vector<cnc::algo::geometry::Quadtree *> &L,std::vector<Quadtree*>& V)
{
    V.push_back(this);
    if (check_adjacency(q)){
        if (is_leaf())
            L.push_back(this);
        else
            for (uint k = 0;k<4;k++)
                if (leafs[k] != q && leafs[k]->check_adjacency(q) && (std::find(V.begin(),V.end(),leafs[k]) == V.end()))
                    leafs[k]->get_adjacents_cells(q,L,V);
    }
    if (parent != nullptr)
        if (std::find(V.begin(),V.end(),parent) == V.end())
            parent->get_adjacents_cells(q,L,V);
}

void cnc::algo::geometry::Quadtree::get_lines(std::vector<std::vector<cnc::vec> > &L) const
{
    if (!is_leaf()){
        L.push_back({vec({x_range.first,my}),vec({x_range.second,my})});
        L.push_back({vec({mx,y_range.first}),vec({mx,y_range.second})});
        for (uint k = 0;k<4;k++){
            quadtree_loc::location l = static_cast<quadtree_loc::location>(k);
            leafs[l]->get_lines(L);
        }
    }
}

void cnc::algo::geometry::Quadtree::create_leafs()
{
    using namespace quadtree_loc;
    range NX,NY;
    location l;
    for (uint k = 0;k<4;k++){
        if (leafs[k] == nullptr){
            l = static_cast<location>(k);
            switch (l) {
            case lower_left:
                NX = {x_range.first,mx};
                NY = {y_range.first,my};
                break;
            case lower_right:
                NX = {mx,x_range.second};
                NY = {y_range.first,my};
                break;
            case upper_left:
                NX = {x_range.first,mx};
                NY = {my,y_range.second};
                break;
            case upper_right:
                NX = {mx,x_range.second};
                NY = {my,y_range.second};
                break;
            }
            leafs[k] = new Quadtree(NX,NY,scale_factor+1,this);
        }
    }
}
