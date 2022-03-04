#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;

cscalar diagoff = std::polar(1.,2*M_PI/6);

vec ctv(const cscalar& z){
    return linear_utils::vec2(z.real(),z.imag());
}

std::vector<vec> ctv(const std::vector<cscalar>& Z){
    std::vector<vec> X;
    for (const auto& z : Z)
        X.push_back(ctv(z));
    return X;
}

cscalar vtc(const vec& z){
    return cscalar(z(0),z(1));
}

cscalar integ(const cscalar& a,const cscalar& b,const cscalar& c,const cscalar& d,const cscalar& e){
    return ((a-b) + (a*(e-d) + b*(c-e))/(c-d))*std::log((e-c)/(e-d));
}

vec holomorphic_mapping(const vec& x,const std::vector<cscalar>& boundary,const std::vector<cscalar>& boundary_value){
    auto z = vtc(x);
    uint n= boundary.size();
    cscalar fx;
    for (uint i = 0;i<n;i++)
        if (std::abs(z-boundary[i]) < 1e-6)
            return ctv(boundary_value[i]);
    for (uint i = 0;i<n;i++)
        fx += integ(boundary_value[(i+1)%n],boundary_value[i],boundary[(i+1)%n],boundary[i],z);
    return ctv(fx/(cscalar(0,2*M_PI)));
}

int main(int argc, char *argv[]) {
    QApplication App(argc, argv);
    PlotWindow Window;
    Window.resize(500, 500);

    PlotTab *Tab = Window.add_tab("my first tab");
    PlotFrame *Frame = Tab->add_frame();

    int N = 20;
    int W = N;
    int H = N;

    uint HEAT_ITER = 50;

    scalar f= 1./W;
    scalar l = 2;
    range r = {-l,l};
    PlotLayer *Layer = Frame->add_grid_layer(r,r,false);

    cloud opti;
    diagoff /= diagoff.imag();
    topology::faces F;

    for (int j = 0;j<H;j++){
        for (int i = 0;i<W;i++){
            /*
    if ((i == 0 && j == 0) || (i == N-1 && j == N-1))
    continue;
    */
            if (j % 2)
                opti.add_point(ctv(cscalar(i,j-1) + diagoff)*f);
            else
                opti.add_point(ctv(cscalar(i,j))*f);
            if (i < W-1 && j<H-1){
                if (j % 2){
                    F.insert(topology::assemble_face(j*W+i,(j+1)*W+i,(j+1)*W+i+1));
                    F.insert(topology::assemble_face(j*W+i,(j+1)*W+i+1,j*W+i+1));
                }
                else {
                    F.insert(topology::assemble_face(j*W+i,j*W+i+1,(j+1)*W+i));
                    F.insert(topology::assemble_face((j+1)*W+i,j*W+i+1,(j+1)*W+i+1));
                }
            }
        }
    }
    topology::remove_vertex(F,0);
    topology::remove_vertex(F,W*H-1);
    opti.set_mean(linear_utils::vec2());
    //Layer->new_point_cloud(opti);


    if (true){
        GeometricContextRef Cref = std::make_shared<GeometricContext>(opti);
        auto& C = *Cref;
        auto D = std::make_shared<Mesh2>(Cref,F);

        D->filterFaces([](const Mesh2& M,const topology::face& F){
            const auto& C = M.getContext();
            for (const auto& e : F)
                if (C->get_vec_edge(e).norm() > std::sqrt(2)+ 1e-3)
                    return false;
            return true;
        });
        D->computeConnectivity();
        std::vector<cscalar> boundary,fb;
        for (const auto& v : D->getBoundaryVertices())
            boundary.push_back(vtc(C(v)));
        std::sort(boundary.begin(),boundary.end(),[](const cscalar& z,const cscalar& w){
            return arg(z) < arg(w);
        });
        uint B = boundary.size();
        auto kernel = [] (scalar x){
            return (1-0.5*std::exp(-x*x));
        };
        for (uint i = 0;i<B;i++){
            scalar t = 2*M_PI*scalar(i)/B;
            fb.push_back(cscalar(cos(t),sin(t)*kernel(cos(t))));
            //fb.push_back(std::cos(boundary[i]*boundary[i])*3.);
            //fb.push_back(std::polar(10.,t));
        }
        if (false){
            for (const auto& v : D->getInteriorVertices())
                C(v) = linear_utils::vec2();
            for (const auto& v : D->getBoundaryVertices())
                C(v) = holomorphic_mapping(C(v),boundary,fb);
        }
        else {
            for (const auto& v : D->getVertices())
                C(v) = holomorphic_mapping(C(v),boundary,fb);
        }
        auto V = D->getInteriorVertices();
        std::map<topology::vertex,vec> M;
        for (uint i = 0;i<HEAT_ITER;i++){
            for (const auto& v : V)
                M[v] = D->oneRingBarycenter(v);
            for (const auto& v: V)
                C(v) = M[v];
        }

        Layer->addPlot<Mesh2DDisplayer>(D);
    }

    Window.show();
    return App.exec();
}
