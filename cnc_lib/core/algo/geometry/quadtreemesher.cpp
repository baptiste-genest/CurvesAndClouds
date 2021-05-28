#include "quadtreemesher.h"

cnc::algo::geometry::QuadtreeMesher::QuadtreeMesher(){
    simplex_head = nullptr;
    simplex_tail = nullptr;
}

cnc::algo::geometry::QuadtreeMesher::~QuadtreeMesher()
{
    for (vec* a : avg)
        delete a;
    for (vec* b : boundaries)
        delete b;
    for (vec* p : mesh_points)
        delete p;
    delete Q;
}

void cnc::algo::geometry::QuadtreeMesher::add_to_chain(const cnc::vec &x)
{
    boundaries.push_back(new vec(x));
    vec* head = boundaries.back();
    if (simplex_tail == nullptr){
        simplex_tail = head;
        simplex_head = head;
    }
    else {
        boundaries_edge.push_back({simplex_head,head});
        simplex_head = head;
    }
}

void cnc::algo::geometry::QuadtreeMesher::complete_chain()
{
    boundaries_edge.push_back({simplex_head,simplex_tail});
    simplex_head = nullptr;
    simplex_tail = nullptr;
}

std::vector<std::vector<cnc::vec>> cnc::algo::geometry::QuadtreeMesher::get_edges() const
{
    std::vector<std::vector<cnc::vec>> R;
    for (const edge& e : boundaries_edge)
        R.push_back({*e.a,*e.b});
    return R;
}

std::vector<cnc::algo::geometry::Quadtree *> cnc::algo::geometry::QuadtreeMesher::get_empty_leafs_with_multiple_edges() const
{
    std::vector<Quadtree*> L = Q->get_empty_leafs();
    std::vector<Quadtree*> R;
    for (Quadtree* l : L){
        uint i = 0;
        for (const edge& e : boundaries_edge)
            if (is_edge_in_box(l,e))
                i++;
        if (i >= 2)
            R.push_back(l);
    }
    return R;
}

void cnc::algo::geometry::QuadtreeMesher::split_leafs_by_type()
{
    leafs_by_type[FULL_LEAF] = Q->get_full_leafs();
    std::vector<Quadtree*> E = Q->get_empty_leafs();
    for (Quadtree* l : E){
        bool interior = true;
        for (const edge& e : boundaries_edge)
            if (is_edge_in_box(l,e)){
                interior = false;
                leafs_by_type[BORDER_LEAF].push_back(l);
                break;
            }
        if (interior)
            leafs_by_type[VOID_LEAF].push_back(l);
    }
}

void cnc::algo::geometry::QuadtreeMesher::balance_quadtree()
{
    std::vector<Quadtree*> L = Q->get_leafs();
    balance_quadtree(L);
}

void cnc::algo::geometry::QuadtreeMesher::balance_quadtree(const std::vector<cnc::algo::geometry::Quadtree *> &L)
{
    std::vector<Quadtree*> to_check;
    for (Quadtree* l : L){
        std::vector<Quadtree*> neighbors = l->get_adjacents_cells();
        bool sub_divide = false;
        for (Quadtree* n : neighbors){
            int scale_diff = int(n->scale_factor) - int(l->scale_factor);
            if ( scale_diff > 1){
                l->subdivide();
                for (uint k = 0;k<4;k++)
                    to_check.push_back(l->leafs[k]);
                sub_divide = true;
            }
        }
        if (sub_divide){
            for (Quadtree* n : neighbors)
                to_check.push_back(n);
        }
    }
    if (to_check.size())
        balance_quadtree(to_check);
}

bool cnc::algo::geometry::QuadtreeMesher::is_edge_in_box(const cnc::algo::geometry::Quadtree *Q, const cnc::algo::geometry::edge &e) const
{
    const vec& e1 = *e.a,e2 = *e.b;
    vec E = e2-e1;
    if (Q->in_ranges(e1) || Q->in_ranges(e2))
        return true;
    /*
    if (std::abs(E(0)) < 1e-12)
        return Q->in_x_range(E(0));
    if (std::abs(E(1)) < 1e-12)
        return Q->in_y_range(E(1));
    */
    scalar X[2] = {Q->x_range.first,Q->x_range.second};
    scalar Y[2] = {Q->y_range.first,Q->y_range.second};

    vec et;
    for (uint k = 0;k<2;k++){
        scalar tx = (X[k] - e1(0))/E(0);
        scalar iy = e1(1) + E(1)*tx;
        if (Q->in_y_range(iy)  && tx <= 1 && tx >= 0 )
            return true;
    }

    for (uint k = 0;k<2;k++){
        scalar ty = (Y[k] - e1(1))/E(1);
        scalar ix = e1(0) + E(0)*ty;
        if (Q->in_x_range(ix) && ty <= 1 && ty >= 0 )
            return true;
    }
    return false;
}

std::array<cnc::algo::geometry::edge,2> cnc::algo::geometry::QuadtreeMesher::get_edges_by_corner(const cnc::vec *x) const
{
    std::array<edge,2> E;
    uint i = 0;
    for (const edge& e: boundaries_edge){
        if (e.a == x || e.b == x){
            E[i] = e;
            i++;
            if (i == 2)
                return E;
        }
    }
    throw Cnc_error("couldn't find 2 edges associated with a corner");
}

cnc::vec cnc::algo::geometry::QuadtreeMesher::compute_quad_edge_intersection(const cnc::algo::geometry::Quadtree *q, const cnc::algo::geometry::edge &e) const
{
    const vec& e1 = *e.a,e2 = *e.b;
    vec E = e2-e1;
    scalar X[2] = {q->x_range.first,q->x_range.second};
    scalar Y[2] = {q->y_range.first,q->y_range.second};

    vec et;
    for (uint k = 0;k<2;k++){
        scalar tx = (X[k] - e1(0))/E(0);
        scalar iy = e1(1) + E(1)*tx;
        if (q->in_y_range(iy)  && tx <= 1 && tx >= 0 )
            return e1 + E*tx;
    }

    for (uint k = 0;k<2;k++){
        scalar ty = (Y[k] - e1(1))/E(1);
        scalar ix = e1(0) + E(0)*ty;
        if (q->in_x_range(ix) && ty <= 1 && ty >= 0 )
            return e1 + E*ty;
    }
    throw Cnc_error("Couldn't find intersection for edge");
}

std::array<cnc::vec,2> cnc::algo::geometry::QuadtreeMesher::compute_quad_edge_intersection_border(const cnc::algo::geometry::Quadtree *q, const cnc::algo::geometry::edge &e) const
{
    std::array<vec,2> R;
    const vec& e1 = *e.a,e2 = *e.b;
    vec E = e2-e1;
    scalar X[2] = {q->x_range.first,q->x_range.second};
    scalar Y[2] = {q->y_range.first,q->y_range.second};

    uint i = 0;

    vec et;
    for (uint k = 0;k<2;k++){
        scalar tx = (X[k] - e1(0))/E(0);
        scalar iy = e1(1) + E(1)*tx;
        if (q->in_y_range(iy)  && tx <= 1 && tx >= 0 ){
            R[i] = e1 + E*tx;
            i++;
            if (i == 2)
                return R;
        }
    }

    for (uint k = 0;k<2;k++){
        scalar ty = (Y[k] - e1(1))/E(1);
        scalar ix = e1(0) + E(0)*ty;
        if (q->in_x_range(ix) && ty <= 1 && ty >= 0 ){
            R[i] = e1 + E*ty;
            i++;
            if (i == 2)
                return R;
        }
    }
    throw Cnc_error("Couldn't find intersection for edge at border");
}

std::vector<cnc::vec *> cnc::algo::geometry::QuadtreeMesher::get_contact_points(Quadtree* l,uint &config)
{
    using namespace quadtree_loc;
    uint N = l->scale_factor;
    std::vector<Quadtree*> Adj = l->get_adjacents_cells();
    std::vector<vec> points = l->get_corners();
    std::vector<vec*> points_address(8,nullptr);
    for (uint k = 0;k<4;k++)
        points_address[2*k] = get_point_adress(points[k]);
    uint id;
    config = 0;
    vec p;
    for (Quadtree* a : Adj){
        if (a->scale_factor > N){
            direction d = l->get_adjacency_dir(a);
            id =int(d)*2+1;
            if (points_address[id] == nullptr){
                p = (points[d] + points[(int(d)+1)%4])*0.5;
                points_address[id] = get_point_adress(p);
                config += 1 << (3-int(d));
            }
        }
    }
    return points_address;
}

std::array<std::vector<cnc::vec *>,2> cnc::algo::geometry::QuadtreeMesher::get_chunks(const std::vector<cnc::vec *> &points, const std::pair<cnc::vec *, cnc::vec *> &intersections)
{
    std::array<std::vector<vec*>,2> R;
    R[0] = {intersections.first,intersections.second};
    R[1] = {intersections.first,intersections.second};
    uint a = 0 ,b = 0;
    for (uint k = 0;k<points.size();k++){
        if (points[k] == intersections.first)
            a = k;
        else if (points[k] == intersections.second)
            b = k;
    }
    if (a > b){
        uint tmp = a;
        a = b;
        b = tmp;
    }
    for (uint k = 0;k<points.size();k++){
        if (k > a && k < b)
            R[0].push_back(points[k]);
        else if (k != a && k != b)
            R[1].push_back(points[k]);
    }
    return R;
}

cnc::vec* cnc::algo::geometry::QuadtreeMesher::get_avg(const std::vector<cnc::vec *> &P)
{
    vec a(2);
    for (uint k = 0;k<P.size();k++)
        a += *P[k];
    a *= scalar(1)/P.size();

    vec* A = new vec(a);
    avg.push_back(A);
    return A;
}

void cnc::algo::geometry::QuadtreeMesher::add_to_mesh(const cnc::vec &x)
{
    for (const vec* m : mesh_points)
        if (m->distance(x) < eps){
            return;
        }
    mesh_points.push_back(new vec(x));
}

void cnc::algo::geometry::QuadtreeMesher::compute_mesh_points()
{
    std::vector<Quadtree*> L = Q->get_leafs();
    for (const Quadtree* l : L){
        std::vector<vec> tmp = l->get_corners();
        for (uint k = 0;k<4;k++)
            add_to_mesh(tmp[k]);
    }
    for (const Quadtree* l : leafs_by_type[FULL_LEAF]){
        std::array<edge,2> edges = get_edges_by_corner(l->val);
        std::vector<vec*> intersections;
        for (const edge& e : edges){
            vec i = compute_quad_edge_intersection(l,e);
            add_to_mesh(i);
            intersections.push_back(get_point_adress(i));
        }
        edge_intersections.push_back({l,{intersections[0],intersections[1]}});
    }
}

void cnc::algo::geometry::QuadtreeMesher::compute_interior_mesh(cnc::algo::geometry::Quadtree *l)
{
    uint config;
    std::vector<vec*> points_address = get_contact_points(l,config);
    make_interior_triangles(points_address,config);
}

void cnc::algo::geometry::QuadtreeMesher::compute_corner_mesh(cnc::algo::geometry::Quadtree *l)
{
    uint config;
    std::vector<vec*> points_address = get_contact_points(l,config);
    std::vector<vec*> at_border;
    for (const auto& pa : points_address)
        if (pa != nullptr)
            at_border.push_back(pa);
    for (const auto& x : edge_intersections)
        if (x.first == l){
            at_border.push_back(x.second.first);
            at_border.push_back(x.second.second);
            break;
        }
    std::vector<vec*> final_points = sort_counterclockwise(*l->val,at_border);
    uint n_final = final_points.size();
    for (uint k = 0;k<n_final;k++){
        mesh.push_back(Triangle(final_points[k],(vec*)l->val,final_points[(k+1)%n_final]));
    }
}

void cnc::algo::geometry::QuadtreeMesher::compute_border_mesh(cnc::algo::geometry::Quadtree *l)
{
    uint config;
    std::vector<vec*> points_address = get_contact_points(l,config);
    std::vector<vec*> at_border;
    std::pair<vec*,vec*> inter;
    for (const auto& pa : points_address)
        if (pa != nullptr)
            at_border.push_back(pa);
    for (const edge& e : boundaries_edge){
        if (is_edge_in_box(l,e)){
            auto I = compute_quad_edge_intersection_border(l,e);
            vec* I1 = new vec(I[0]); avg.push_back(I1);
            vec* I2 = new vec(I[1]); avg.push_back(I2);
            inter.first = I1;
            inter.second = I2;
            at_border.push_back(I1);
            at_border.push_back(I2);
            break;
        }
    }
    std::vector<vec*> sorted_points = sort_counterclockwise(l->get_mid_point(),at_border);
    std::array<std::vector<vec*>,2> chunks = get_chunks(sorted_points,inter);
    for (uint i = 0;i<2;i++){
        if (chunks[i].size() > 3){
            vec* avg = get_avg(chunks[i]);
            chunks[i] = sort_counterclockwise(*avg,chunks[i]);
            uint n = chunks[i].size();
            for (uint k = 0;k<n;k++)
                mesh.push_back(Triangle(chunks[i][k],avg,chunks[i][(k+1)%n]));
        }
        else
            mesh.push_back(Triangle(chunks[i][0],chunks[i][1],chunks[i][2]));
    }

}

void cnc::algo::geometry::QuadtreeMesher::subdivide_for_edges()
{
    auto L = Q->get_empty_leafs();
    for (Quadtree* l : L){
        uint i = 0;
        for (const edge& e : boundaries_edge)
            if (is_edge_in_box(l,e))
                i++;
        if (i > 1){
            l->subdivide();
        }
    }
}

cnc::vec *cnc::algo::geometry::QuadtreeMesher::get_point_adress(const cnc::vec &x)
{
    for (uint k = 0;k<mesh_points.size();k++)
        if (mesh_points[k]->distance(x) < eps)
            return mesh_points[k];
    throw Cnc_error("Couldn't find point in QuadtreeMesher");
}

std::vector<cnc::vec *> cnc::algo::geometry::QuadtreeMesher::sort_counterclockwise(const cnc::vec &center, const std::vector<cnc::vec *> &points) const
{
    uint N = points.size();
    std::vector<cnc::vec*> R(N);
    std::vector<std::pair<scalar,uint>> dots(N);
    for (uint k = 0;k<N;k++){
        vec n = (*points[k] - center).normalize();
        dots[k] = {atan2(n(1),n(0)),k};
    }

    auto cmp = [] (const std::pair<scalar,uint>& u,const std::pair<scalar,uint>& v) {
        return u.first < v.first;
    };

    std::sort(dots.begin(),dots.end(),cmp);
    for (uint k = 0;k<N;k++)
        R[k] = points[dots[k].second];

    return R;
}



bool cnc::algo::geometry::QuadtreeMesher::is_triangle_in_shape(const cnc::algo::geometry::Triangle &T)
{
    vec M = T.get_mid_point();
    scalar th = algo::stat::random_var::random_scalar(0,2*M_PI);
    vec a({cos(th),sin(th)});
    uint nb = 0;
    for (const edge& e : boundaries_edge)
        if (ray_edge_intersection(M,a,e) > 0)
            nb++;
    return nb%2;
}

void cnc::algo::geometry::QuadtreeMesher::purge_outside_mesh()
{
    std::vector<Triangle> M;
    for (const auto& t : mesh)
        if (is_triangle_in_shape(t))
            M.push_back(t);
    mesh = M;
}

cnc::scalar cnc::algo::geometry::QuadtreeMesher::ray_edge_intersection(const cnc::vec &a, const cnc::vec &b, const cnc::algo::geometry::edge &e) const
{
    mat A(2,2);
    const vec& c = *e.a;
    vec d = *e.b - *e.a;
    A(0,0) = b(0);A(1,0) = -d(0);
    A(0,1) = b(1);A(1,1) = -d(1);
    vec B = c-a;
    vec X = A.solve(B,1e-10);
    if (X(1) > 0 && X(1) < 1)
        return X(0);
    return -1;
}



void cnc::algo::geometry::QuadtreeMesher::build_mesh()
{
    std::vector<vec> p(boundaries.size());
    for (uint k = 0;k<boundaries.size();k++)
        p[k] = *boundaries[k];
    std::vector<range> bounds = get_min_max_range(p);
    scale_range(bounds[0],1.2);
    scale_range(bounds[1],1.2);
    bounds[0].second *= 0.98764;
    bounds[1].second *= 0.992;
    Q = new Quadtree(bounds[0],bounds[1]);

    for (vec* v : boundaries){
        Q->insert(v);
    }

    subdivide_for_edges();
    balance_quadtree();
    split_leafs_by_type();
    compute_mesh_points();
    for (Quadtree* il : leafs_by_type[VOID_LEAF]){
            compute_interior_mesh(il);
        }
    for (Quadtree* bl : leafs_by_type[BORDER_LEAF]){
            compute_border_mesh(bl);
        }
    for (Quadtree* pl : leafs_by_type[FULL_LEAF]){
            compute_corner_mesh(pl);
        }
    purge_outside_mesh();

}

std::vector<cnc::algo::geometry::Triangle> cnc::algo::geometry::QuadtreeMesher::get_mesh() const
{
    return mesh;
}

void cnc::algo::geometry::QuadtreeMesher::make_interior_triangles(const std::vector<cnc::vec *> &points, uint config)
{
    constexpr static uint L = 4;
    std::vector<std::vector<uint>> T;
    int permutation_order = 0;
    if (config == 0){
        T.push_back({0,2,4});
        T.push_back({0,4,6});
    }
    else if ((permutation_order = find_cyclic_permutation(10,config,L)) >= 0){
        T.push_back({0,1,5});
        T.push_back({1,2,5});
        T.push_back({2,4,5});
        T.push_back({5,6,0});
    }
    else if ((permutation_order = find_cyclic_permutation(8,config,L)) >= 0){
        T.push_back({0,1,6});
        T.push_back({1,2,4});
        T.push_back({1,4,6});
    }
    else if ((permutation_order = find_cyclic_permutation(11,config,L)) >= 0){
        T.push_back({0,1,7});
        T.push_back({1,2,5});
        T.push_back({2,4,5});
        T.push_back({5,6,7});
        T.push_back({5,7,1});
    }
    else if ((permutation_order = find_cyclic_permutation(12,config,L)) >= 0){
        T.push_back({0,1,6});
        T.push_back({1,2,3});
        T.push_back({3,4,6});
        T.push_back({1,3,6});
    }
    else if ((permutation_order = find_cyclic_permutation(15,config,L)) >= 0){
        T.push_back({0,1,7});
        T.push_back({1,2,3});
        T.push_back({3,4,5});
        T.push_back({5,6,7});
        T.push_back({1,3,5});
        T.push_back({1,5,7});
    }
    else
        throw Cnc_error("error while building interior quadtree mesh : no cyclic config founded");
    for (const auto& t: T){
        mesh.push_back(Triangle(
                points[(t[0] + 2*permutation_order)%8],
                points[(t[1] + 2*permutation_order)%8],
                points[(t[2] + 2*permutation_order)%8]));
    }
}
