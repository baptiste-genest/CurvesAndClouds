#include "simpleglmesh.h"

cnc::algo::geometry::SimpleGLMesh::SimpleGLMesh(const std::vector<QVector3D> &p, const std::vector<QVector3D> &n, const std::vector<uint> &f)
    : faces(f)
{
    vertices.resize(p.size());
    for (uint k = 0;k<p.size();k++){
        vertices[k].vpos = p[k];
        if (n.size() != 0)
            vertices[k].normal = n[k];
        vertices[k].color = QVector3D(1.f,1.f,1.f);
        vertices[k].id = k;
    }
    nb_vertices = p.size();
    nb_faces = f.size()/3;
    initialize_halfedges();
}

cnc::algo::geometry::SimpleGLMesh::SimpleGLMesh(int nb_vertex, int nb_face) :
    nb_vertices(nb_vertex),
    nb_faces(nb_face)
{
    vertices.resize(nb_vertex);
    faces.resize(nb_face*3);
}

void cnc::algo::geometry::SimpleGLMesh::set_vertex_pos(uint vertex_id, const QVector3D &v)
{
    if (vertex_id >= nb_vertices)
        throw Cnc_error("vertex id out of range");
    vertices[vertex_id].vpos =v;
}

void cnc::algo::geometry::SimpleGLMesh::set_vertex_normal(uint vertex_id, const QVector3D &v)
{
    if (vertex_id >= nb_vertices)
        throw Cnc_error("vertex id out of range");
    vertices[vertex_id].normal = v;
}

void cnc::algo::geometry::SimpleGLMesh::set_vertex_color(uint vertex_id, const QVector3D &v)
{
    if (vertex_id >= nb_vertices)
        throw Cnc_error("vertex id out of range");
    vertices[vertex_id].color = v;
}

void cnc::algo::geometry::SimpleGLMesh::set_vertex_value(uint vertex_id, cnc::scalar v)
{
    if (vertex_id >= nb_vertices)
        throw Cnc_error("vertex id out of range");
    vertices[vertex_id].value = v;
}

void cnc::algo::geometry::SimpleGLMesh::set_vertex_values(const cnc::vec &v)
{
    for (uint k = 0;k<nb_vertices;k++)
        vertices[k].value = v(k);
}

void cnc::algo::geometry::SimpleGLMesh::set_face(uint face_id, uint v1, uint v2, uint v3)
{
    if (v1 >= nb_vertices)
        throw Cnc_error("vertex id out of range");
    faces[face_id*3] = v1;
    if (v2 >= nb_vertices)
        throw Cnc_error("vertex id out of range");
    faces[face_id*3+1] = v2;
    if (v3 >= nb_vertices)
        throw Cnc_error("vertex id out of range");
    faces[face_id*3+2] = v3;
}

void cnc::algo::geometry::SimpleGLMesh::compute_normals()
{
    std::vector<QVector3D> face_normals(nb_faces);
    for (uint k = 0;k<nb_faces;k++){
        face_normals[k] = get_face_normal(k);
    }
    for (uint k = 0;k<nb_vertices;k++){
        auto F = get_faces(k);
        QVector3D avg;
        for (uint f : F)
            avg += face_normals[f]/face_area(f);
        set_vertex_normal(k,avg.normalized());
    }
}

cnc::vec cnc::algo::geometry::SimpleGLMesh::get_vertex_values() const
{
    vec sample(nb_vertices);
    for (uint k = 0;k<nb_vertices;k++)
        sample(k) = vertices[k].value;
    return sample;
}

std::vector<uint> cnc::algo::geometry::SimpleGLMesh::get_boundary_vertices() const
{
    std::vector<uint> B;
    for (halfedge* h : halfedges)
        if (h->opposite == nullptr)
            B.push_back(h->vertex_id);
    return B;
}

cnc::scalar cnc::algo::geometry::SimpleGLMesh::sum_faces_areas(cnc::algo::geometry::vref x) const
{
    scalar sa = 0;
    for (halfedge* f : vertices[x].emerging)
        sa += area_from_halfedge(f);
    return sa;
}

cnc::scalar cnc::algo::geometry::SimpleGLMesh::area_from_halfedge(cnc::algo::geometry::halfedge *h) const
{
    qvr v1 = vertices[h->vertex_id].vpos;
    qvr v2 = vertices[h->next->vertex_id].vpos;
    qvr v3 = vertices[h->next->next->vertex_id].vpos;
    return QVector3D::crossProduct(v2-v1,v3-v1).length()*0.5f;
}

cnc::scalar cnc::algo::geometry::SimpleGLMesh::face_area(uint f) const
{
    qvr v1 = vertices[faces[f*3]].vpos;
    qvr v2 = vertices[faces[f*3+1]].vpos;
    qvr v3 = vertices[faces[f*3+2]].vpos;
    return QVector3D::crossProduct(v2-v1,v3-v1).length()*0.5f;
}

void cnc::algo::geometry::SimpleGLMesh::set_iso_lines_color(uint nb_lines)
{
    std::vector<scalar> v = get_vertex_values().data();
    std::sort(v.begin(),v.end());
    std::vector<scalar> iso(nb_lines);
    const scalar width = 0.1f/nb_lines;
    for (uint i = 0;i<nb_lines;i++){
        iso[i] = v[i*nb_vertices/(nb_lines-1)];
    }
    iso[nb_lines - 1] = 1.f;
    for (uint k = 0;k<nb_vertices;k++){
        scalar val = vertices[k].value;
        vertices[k].color = QVector3D(val,0,1-val);
        for (uint i = 0;i<nb_lines;i++)
            if (std::abs(iso[i] - val) < width){
                vertices[k].color = QVector3D(1,1,1);
                break;
            }
    }
}

void cnc::algo::geometry::SimpleGLMesh::truncate_mesh(const std::function<bool (qvr)> &exclude_condition)
{
    std::vector<uint> new_faces;
    for (uint f = 0;f<nb_faces;f++){
        bool exclude = true;
        for (uint k = 0;k<3;k++)
            exclude &= exclude_condition(vertices[faces[f*3+k]].vpos);
        if (!exclude) {
            for (uint k = 0;k<3;k++)
                new_faces.push_back(faces[f*3+k]);
        }
    }
    std::vector<bool> vertex_keept(nb_vertices,false);
    std::vector<uint> new_ids(nb_vertices);
    for (uint i = 0;i<new_faces.size();i++)
        vertex_keept[new_faces[i]] = true;
    std::vector<VertexInfo> new_vertex;
    uint k = 0;
    for (uint i = 0;i<nb_vertices;i++)
        if (vertex_keept[i]){
            new_vertex.push_back(vertices[i]);
            new_vertex.back().emerging.clear();
            new_vertex.back().id = k;
            new_ids[i] = k;
            k++;
        }
    for (uint i = 0;i<new_faces.size();i++){
        new_faces[i] = new_ids[new_faces[i]];
    }
    std::cout << "truncated " << nb_faces - new_faces.size()/3 << " faces" << std::endl;
    nb_faces = new_faces.size()/3;
    std::cout << "truncated " << nb_vertices - new_vertex.size() << " vertices" << std::endl;
    nb_vertices = new_vertex.size();
    faces = new_faces;
    vertices = new_vertex;
    for (halfedge* h : halfedges){
        if (h != nullptr)
            delete h;
    }
    initialize_halfedges(true);
    new_vertex.clear();
    new_faces.clear();
}

bool cnc::algo::geometry::SimpleGLMesh::is_vertex_on_boundary(uint id) const
{
    const VertexInfo& v = vertices[id];
    for (halfedge* h : v.emerging)
        if (h->opposite == nullptr || h->prev->opposite == nullptr)
            return true;
    return false;
}

cnc::scalar cnc::algo::geometry::SimpleGLMesh::angle_from_incident_angle(cnc::algo::geometry::halfedge *h) const
{
    const QVector3D& p = vertices[h->prev->vertex_id].vpos;
    QVector3D a = (vertices[h->vertex_id].vpos - p).normalized();
    QVector3D b = (vertices[h->next->vertex_id].vpos - p).normalized();
    return std::acos(QVector3D::dotProduct(a,b));
}


QVector3D cnc::algo::geometry::SimpleGLMesh::get_face_normal(uint f) const
{
    uint id = f*3;
    const auto& v1 = vertices[faces[id]].vpos;
    const auto& v2 = vertices[faces[id+1]].vpos;
    const auto& v3 = vertices[faces[id+2]].vpos;
    return QVector3D::crossProduct(v2-v1,v3-v1).normalized();
}

cnc::mat cnc::algo::geometry::SimpleGLMesh::compute_laplace_beltrami_matrix(bool weighted) const
{
    mat LB(nb_vertices);
    scalar A;
    for (uint j = 0;j<nb_vertices;j++){
        A = weighted ? 1/barycentric_area(j) : 1;
        auto N = one_ring_neighbors(j);
        for (vref n : N){
            if (n > j){
                //printf("%i %i %f \n",n,j,-cotan_weight(n,j)*A);
                LB(n,j) = -cotan_weight(j,n)*A;
                LB(j,n) = LB(n,j);
            }
        }
    }
    for (uint i = 0;i<nb_vertices;i++){
        scalar s = 0.f;
        for (uint j = 0;j<nb_vertices;j++){
            s -= LB(i,j);
        }
        LB(i,i) = s;
    }
    return LB;
}

cnc::mat cnc::algo::geometry::SimpleGLMesh::compute_uniform_laplacian_matrix() const
{
    mat LB(nb_vertices);
    for (uint i = 0;i<nb_vertices;i++){
        for (uint j = i+1;j<nb_vertices;j++){
            if (get_edge(i,j)){
                LB(i,j) = -1;
                LB(j,i) = -1;
            }
        }
    }
    for (uint i = 0;i<nb_vertices;i++){
        cnc::scalar s = 0.f;
        for (uint j = 0;j<nb_vertices;j++){
            s -= LB(i,j);
        }
        LB(i,i) = s;
    }
    return LB;
}

cnc::smat cnc::algo::geometry::SimpleGLMesh::compute_sparse_laplace_beltrami_matrix(bool weighted,bool pos) const
{
    smat LB(nb_vertices);
    scalar A;
    for (uint j = 0;j<nb_vertices;j++){
        A = (weighted ? 1/barycentric_area(j) : 1);
        auto N = one_ring_neighbors(j);
        N.push_back(j);
        std::sort(N.begin(),N.end());
        uint diag_index = 0;
        for (vref n : N){
            if (n == j){
                LB.add_in_row(n,0.);
                diag_index = LB.get_current_index();
            }
            else{
                //printf("%i %i %f \n",n,j,-cotan_weight(n,j,pos)*A);
                LB.add_in_row(n,cotan_weight(n,j,pos)*A);
            }
        }
        LB.new_row();
        LB.set_at_index(diag_index,-LB.row_sum(j));
        //std::cout << j << std::endl;
    }
    return LB;
}

cnc::smat cnc::algo::geometry::SimpleGLMesh::compute_identity_plus_dt_sparse_laplace_beltrami_matrix(cnc::scalar dt,bool weighted,bool pos) const
{
    smat LB(nb_vertices);
    scalar A;
    for (uint j = 0;j<nb_vertices;j++){
        A = (weighted ? 0.5f/barycentric_area(j) : 0.5);
        auto N = one_ring_neighbors(j);
        N.push_back(j);
        std::sort(N.begin(),N.end());
        uint diag_index = 0;
        cnc::scalar s = 0;
        for (vref n : N){
            if (n == j){
                LB.add_in_row(n,0.);
                diag_index = LB.get_current_index();
            }
            else {
                scalar w = -cotan_weight(n,j,pos)*A*dt;
                s -= w;
                LB.add_in_row(n,w);
            }
        }
        LB.new_row();
        LB.set_at_index(diag_index,1.+s);
        //std::cout << j << std::endl;
    }
    return LB;
}

cnc::smat cnc::algo::geometry::SimpleGLMesh::compute_weight_plus_dt_cot_matrix(cnc::scalar dt) const
{
    smat LB(nb_vertices);
    for (uint j = 0;j<nb_vertices;j++){
        auto N = one_ring_neighbors(j);
        N.push_back(j);
        std::sort(N.begin(),N.end());
        uint diag_index = 0;
        scalar s = 0;
        for (vref n : N){
            if (n == j){
                LB.add_in_row(n,0.);
                diag_index = LB.get_current_index();
            }
            else {
                scalar w = cotan_weight(n,j)*dt;
                s -= w;
                LB.add_in_row(n,w);
            }
        }
        LB.new_row();
        LB.set_at_index(diag_index,barycentric_area(j)+s);
        //std::cout << j << std::endl;
    }
    return LB;

}

cnc::smat cnc::algo::geometry::SimpleGLMesh::compute_mass_matrix() const
{
    smat W(nb_vertices);
    for (uint j = 0;j<nb_vertices;j++){
        W.add_in_row(j,barycentric_area(j));
        W.new_row();
    }
    return W;
}

cnc::scalar cnc::algo::geometry::SimpleGLMesh::mean_curvature(cnc::algo::geometry::vref x) const
{
    auto f = [] (const VertexInfo* x){
        return x->vpos;
    };
    return laplace_beltrami_operator<QVector3D>(x,f).length()*0.5f;
}

cnc::scalar cnc::algo::geometry::SimpleGLMesh::gaussian_curvature(cnc::algo::geometry::vref x) const
{
    const static scalar tau = 2*M_PI,itau = 1/tau;
    scalar th_sum = 0;
    for (halfedge* h : vertices[x].emerging)
        th_sum += angle_from_incident_angle(h);
    return (tau - th_sum)*itau;
}

std::vector<QVector3D> cnc::algo::geometry::SimpleGLMesh::gradient_per_triangle() const
{
    std::vector<QVector3D> gradient(nb_faces);

    for (uint f = 0;f<nb_faces;f++){
        QVector3D N = get_face_normal(f);
        halfedge* h = halfedges[f*3];
        for (uint k = 0;k<3;k++){
            QVector3D e = vertices[h->vertex_id].vpos - vertices[h->prev->vertex_id].vpos;
            gradient[f] = QVector3D::crossProduct(N,e)*vertices[h->next->vertex_id].value;
            h = h->next;
        }
        gradient[f] *= 0.5f/face_area(f);
    }
    return gradient;
}

std::vector<QVector3D> cnc::algo::geometry::SimpleGLMesh::gradient_per_triangle(const vec &v) const
{
    std::vector<QVector3D> gradient(nb_faces);

    for (uint f = 0;f<nb_faces;f++){
        QVector3D N = get_face_normal(f);
        halfedge* h = halfedges[f*3];
        for (uint k = 0;k<3;k++){
            QVector3D e = vertices[h->next->vertex_id].vpos - vertices[h->vertex_id].vpos;
            gradient[f] += QVector3D::crossProduct(N,e)*v(h->prev->vertex_id);
            h = h->next;
        }
        gradient[f] *= 0.5f/face_area(f);
    }
    return gradient;
}

cnc::vec cnc::algo::geometry::SimpleGLMesh::integrated_divergence_per_vertex(const std::vector<QVector3D> &v,bool pos_cotan) const
{
    vec div(nb_vertices);
    for (uint k = 0;k<nb_vertices;k++) {
        for (halfedge* e : vertices[k].emerging){
            qvr X = v[e->face_id];
            QVector3D e1 = vertices[e->vertex_id].vpos - vertices[e->prev->vertex_id].vpos;
            QVector3D e2 = vertices[e->next->vertex_id].vpos - vertices[e->prev->vertex_id].vpos;
            div(k) += (cotan_weight(e,pos_cotan)*QVector3D::dotProduct(e2,X) + cotan_weight(e->next,pos_cotan)*QVector3D::dotProduct(e1,X))*0.5f;
        }
    }
    return div;
}

cnc::scalar cnc::algo::geometry::SimpleGLMesh::mean_spacing() const
{
    scalar ms = 0;
    for (uint k = 0;k<nb_vertices;k++){
        auto N = one_ring_neighbors(k);
        scalar s = 0;
        for (uint n : N)
            s += (vertices[k].vpos - vertices[n].vpos).length();
        ms += s/N.size();
    }
    return ms/nb_vertices;
}

cnc::vec cnc::algo::geometry::SimpleGLMesh::get_geodesic_distance_from_vertex(uint vertex_id) const
{
    scalar h = mean_spacing();
    scalar t = h*h*80,eps = 1e-10;

    auto M_tLcot = compute_weight_plus_dt_cot_matrix(-t);

    vec v(get_nb_vertices());
    v(vertex_id) = 1.f;

    vec u = M_tLcot.conjugate_gradient(v,eps,true);
    auto grad = gradient_per_triangle(u);

    std::vector<QVector3D> X(get_nb_faces());
    for (uint k = 0;k<get_nb_faces();k++)
        X[k] = -grad[k].normalized();

    auto div = integrated_divergence_per_vertex(X,false);

    auto Lcot = compute_sparse_laplace_beltrami_matrix(false);
    auto dist = Lcot.conjugate_gradient(div,0.1,true);
    return dist.scale_from_0_to_1();
}

cnc::algo::geometry::SimpleGLMesh::~SimpleGLMesh()
{
    for (href h: halfedges)
        delete h;
}

cnc::vec cnc::algo::geometry::SimpleGLMesh::test()
{
    vec C(nb_vertices);
    for (uint k = 0;k<nb_vertices;k++)
        C(k) = algo::clamp<cnc::scalar>(mean_curvature(k),0.f,20.f);
        //C(k) = mean_curvature(k);
    return C;
}

cnc::scalar cnc::algo::geometry::SimpleGLMesh::barycentric_area(vref vr) const
{
    return sum_faces_areas(vr)/3.f;
}

cnc::scalar cnc::algo::geometry::SimpleGLMesh::cotan_weight(vref a,vref b,bool pos) const
{
    halfedge* e = get_edge(a,b);
    if (a == b)
        throw Cnc_error("cotan : same edge");
    if (e->opposite == nullptr)
        return 1;
    halfedge* eo = e->opposite;
    if (!((eo->vertex_id == a && eo->prev->vertex_id == b) || (eo->vertex_id == b && eo->prev->vertex_id == a)))
        throw Cnc_error("cotan : bad opposing connection");
    if (e->next->vertex_id == eo->next->vertex_id)
        throw Cnc_error("cotan : wrong vertices");
    qvr pi = vertices[a].vpos,pj = vertices[b].vpos;
    if (pos){
        scalar w = 0.f;
        scalar e1 = (pi-pj).length(),
                e2 = (pi - vertices[e->next->vertex_id].vpos).length(),
                e3 = (pj - vertices[e->next->vertex_id].vpos).length();
        scalar cos_alpha = std::fabs((e3*e3 + e2*e2 - e1*e1))/(2.0f*e3*e2);
        scalar e4 = (pi - vertices[eo->next->vertex_id].vpos).length(),
                e5 = (pj - vertices[eo->next->vertex_id].vpos).length();
        scalar cos_beta = std::fabs((e4*e4 + e5*e5 - e1*e1))/(2.0f*e4*e5);
        scalar cotan1 = cos_alpha/std::sqrt(1.0f - cos_alpha*cos_alpha);
        scalar cotan2 = cos_beta/std::sqrt(1.0f - cos_beta*cos_beta);
        w = (cotan1 + cotan2)*0.5f;
        if (std::isnan(w))
            return 0.f;
        const cnc::scalar eps = 1e-6,
                maxcot = std::cos(eps)/std::sin(eps);
        if (w >= maxcot)
            return maxcot;
        if (w < 0)
            throw Cnc_error("wut?");
        return w;
    }
    else {
        qvr pa = vertices[e->next->vertex_id].vpos;
        qvr pb = vertices[eo->next->vertex_id].vpos;
        scalar cotan1 = QVector3D::dotProduct(pi-pa,pj-pa)/QVector3D::crossProduct(pi-pa,pj-pa).length();
        scalar cotan2 = QVector3D::dotProduct(pi-pb,pj-pb)/QVector3D::crossProduct(pi-pb,pj-pb).length();
        scalar w = (cotan1 + cotan2)*0.5f;
        if (std::isnan(w))
            return 0;
        if (w > 1e6)
            return 1e6;
        return w;
    }
}

cnc::scalar cnc::algo::geometry::SimpleGLMesh::cotan_weight(cnc::algo::geometry::halfedge *h,bool pos) const
{
    if (pos) {
        scalar e3 = (vertices[h->vertex_id].vpos - vertices[h->prev->vertex_id].vpos).length();//a
        scalar e2 = (vertices[h->vertex_id].vpos - vertices[h->next->vertex_id].vpos).length();//b
        scalar e1 = (vertices[h->next->vertex_id].vpos - vertices[h->prev->vertex_id].vpos).length();//c
        scalar cos = fabs((e3*e3 + e2*e2 - e1*e1) / (2.0f*e3*e2));
        scalar w = cos / sqrt(1.0f - cos * cos);
        if (std::isnan(w))
            return 0.f;
        const cnc::scalar eps = 1e-6,
                maxcot = std::cos(eps)/std::sin(eps);
        if (w >= maxcot)
            return maxcot;
        return w;
    }
    else {
        qvr in = vertices[h->vertex_id].vpos;//a
        qvr a =  vertices[h->next->vertex_id].vpos;//b
        qvr b = vertices[h->prev->vertex_id].vpos;//c
        scalar w = QVector3D::dotProduct(a-in,b-in)/QVector3D::crossProduct(a-in,b-in).length();
        if (std::isnan(w))
            return 0;
        if (w > 1e6)
            return 1e6;
        return w;
    }
}

std::vector<uint> cnc::algo::geometry::SimpleGLMesh::one_ring_neighbors(uint x) const
{
    std::vector<vref> N;
    for (href h : vertices[x].emerging){
        if (h->vertex_id == x || h->prev->vertex_id != x){
            std::cout << x << ' ' << h->prev->vertex_id << std::endl;
            throw Cnc_error("emergence error");
        }
        if (!algo::belong(N,h->vertex_id))
            N.push_back(h->vertex_id);
        if (!algo::belong(N,h->next->vertex_id))
            N.push_back(h->next->vertex_id);
    }
    return N;
}

void cnc::algo::geometry::SimpleGLMesh::export_as_obj(std::string out_file) const
{
    algo::export_mesh_as_obj(this,out_file);
}

void cnc::algo::geometry::SimpleGLMesh::set_halfedge(int HE_id, int face_id)
{
    halfedges[HE_id]->vertex_id = faces[HE_id];
    halfedges[HE_id]->next = halfedges[pos_mod(HE_id+1-face_id,3)+face_id];
    halfedges[HE_id]->prev = halfedges[pos_mod(HE_id-1-face_id,3)+face_id];
    halfedges[HE_id]->face_id = face_id/3;
    vertices[halfedges[HE_id]->vertex_id].emerging.push_back(halfedges[HE_id]->next);
}

void cnc::algo::geometry::SimpleGLMesh::initialize_halfedges(bool opposites)
{
    halfedges.resize(faces.size());
    uint id;
    for (uint i = 0;i<nb_faces;i++){
        id = i*3;
        for (uint k = 0;k<3;k++)
            halfedges[id+k] = new halfedge;
    }
    for (uint i = 0;i<nb_faces;i++){
        id = i*3;
        for (uint k = 0;k<3;k++)
            set_halfedge(id+k,id);
    }
    if (opposites)
        set_opposites();
}

std::vector<uint> cnc::algo::geometry::SimpleGLMesh::get_faces(uint vid) const
{
    std::vector<uint> F;
    for (uint k = 0;k<vertices[vid].emerging.size();k++)
        F.push_back(vertices[vid].emerging[k]->face_id);
    return F;
}

void cnc::algo::geometry::SimpleGLMesh::set_opposites()
{
    uint common[2];
    for (uint k = 0;k<halfedges.size();k++){
        href h = halfedges[k];
        uint pv = h->prev->vertex_id;
        for (href r : vertices[pv].emerging){
            if (r->face_id != h->face_id)
                if (share_edge(h->face_id,r->face_id,common)){
                    vref v1 = common[0];
                    vref v2 = common[1];
                    if ((v1 == pv && h->vertex_id == v2) || (v2 == pv && h->vertex_id == v1)){
                        href common_edge = get_edge(v1,v2,r->face_id);
                        common_edge->opposite = h;
                        h->opposite = common_edge;
                        break;
                    }
                }
        }
    }
}

cnc::algo::geometry::halfedge *cnc::algo::geometry::SimpleGLMesh::get_edge(uint a, uint b,uint face) const
{
    halfedge* h = halfedges[face*3];
    while (!((h->vertex_id == a && h->prev->vertex_id == b) || ((h->vertex_id == b && h->prev->vertex_id == a))))
        h = h->next;
    return h;
}

cnc::algo::geometry::halfedge *cnc::algo::geometry::SimpleGLMesh::get_edge(uint a, uint b) const
{
    for (halfedge* h : vertices[a].emerging){
        if (h->vertex_id == b)
            return h;
        if (h->next->next->vertex_id == a && h->next->vertex_id == b)
            return h->next->next;
    }
    return nullptr;
}

bool cnc::algo::geometry::SimpleGLMesh::are_well_oriented(cnc::algo::geometry::halfedge *h1, cnc::algo::geometry::halfedge *h2) const
{
    /*
    if (h1->opposite != h2 || h1 != h2->opposite)
        throw Cnc_error("halfedges aren't opposite");
    return h1->vertex != h2->vertex && h1->prev->vertex == h2->vertex;
    */
    return true;
}

bool cnc::algo::geometry::SimpleGLMesh::share_edge(uint f1, uint f2,uint edge[]) const
{
    uint id1 = f1*3,id2 = f2*3,count = 0;
    for (uint i = 0;i<3;i++){
        for (uint j = 0;j<3;j++){
            if (faces[id1+i] == faces[id2 + j]){
                edge[count] = faces[id1+i];
                count++;
            }
        }
    }
    if (count == 2)
        return true;
    return false;
}
