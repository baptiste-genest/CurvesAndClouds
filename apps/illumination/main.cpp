#include <curvesandcloud.h>

using namespace cnc;
using namespace std;
using namespace std::chrono;

const uint N = 50;
const uint T = N*N+1;
const uint W = 100;

uint cr(uint x){
    if (x < 0)
        return 0;
    if (x > 255)
        return 255;
    return x;
}

algo_GUI::texture_function get_plotter(const vec& f){
    return [f] (uint x,uint y) {
        auto M = algo::calculus::build_range_mapper({0,W-1},{0,N-1});
        auto IM = [M] (scalar x) {return std::round(M(x));};
        uint index = IM(y)*(N)+IM(x);
        uint v = cr(std::abs(f(index))*255);
        return QColor::fromRgb(v,v,v);
    };
}


const vec c = vec({0.,0.,0.5});
const scalar r = 0.1;
scalar visibility(const vec& x,const vec& y){
    vec w = x-c;
    vec d = y-x;
    scalar a = d.scalar_product(d);
    scalar b = 2*w.scalar_product(d);
    scalar c = w.scalar_product(w)-r*r;
    scalar delta = b*b-4*a*c;
    if (delta < 0)
        return 1.;
    scalar t1 = (sqrt(delta)-b)*0.5/a;
    scalar t2 = (-sqrt(delta)-b)*0.5/a;
    if (((t1 > 0) && (t1 < 1)) || ((t2 > 0) && (t2 < 1)))
        return 0.;
    return 1.;
}

mat build_system(){
    std::vector<vec> mesh(T,vec(3));
    scalar dx = 1./N;
    vec source_pos = vec({0.,0.,1});
    mesh.back() = source_pos;
    auto M = algo::calculus::build_range_mapper({0,N-1},{-1.,1.});
    for (uint j = 0;j<N;j++){
        for (uint i = 0;i<N;i++){
            mesh[j*N+i] = vec({M(i),M(j),0.});
            mesh[j*N+i].plain_log = true;
        }
    }
    vec up({0.,0.,1.});
    vec nm = -up,ns = up;
    mat S(T,T);
    vec d,ni,nj;
    for (uint j = 0;j<T;j++){
        for (uint i = j+1;i<T;i++){
            ni = (i == T-1) ? ns : nm;
            nj = (j == T-1) ? ns : nm;
            d = mesh[j]-mesh[i];
            scalar id2 = 1./d.scalar_product(d);
            S(i,j) = dx*d.scalar_product(ni)*d.scalar_product(nj)*id2*id2*visibility(mesh[j],mesh[i]);
            S(j,i) = S(i,j);
        }
    }
    return S;
}

int main(int argc, char *argv[])
{
    vec source(T);source(T-1) = 10.;
    scalar rho = 0.9;
    mat S = build_system();
    mat A = chaskal::Id<scalar>(T) - S*rho;

    auto start = high_resolution_clock::now();

    vec f = A.solve(source);

    auto stop = high_resolution_clock::now();    // Get starting timepoint

    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "Time taken by function: "
             << duration.count() << " microseconds" << endl;

    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("illumination");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();

    L->new_figure_from_texturing(W,W,get_plotter(f));
    w.show();
    return App.exec();
}
