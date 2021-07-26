#include <curvesandcloud.h>

using namespace cnc;

cloud generate_fondamental_hoop(){
    constexpr static uint N = 100;
    cloud H;
    auto th = algo::calculus::build_range_mapper({0,N},{0,2*M_PI});
    for (uint k = 0;k<N;k++)
        H.add_point(vec({cos(th(k)),sin(th(k)),0}));
    return H;
}

std::array<vec,3> gen_spherical_triangle(scalar gamma){
    constexpr static scalar alpha = M_PI/2,beta = M_PI/3;
    std::array<vec,3> corners;
    corners[0] = vec({0.,0.,1.});
    scalar th1 = std::acos((cos(gamma) + cos(alpha)*cos(beta))/(sin(alpha)*sin(beta)));
    corners[1] = algo::geometry::Rx(th1)*corners[0];
    scalar th2 = std::asin(sin(th1)*sin(beta)/sin(gamma));
    corners[2] = algo::geometry::Ry(th2)*corners[0];
    return corners;
}

vec proj(const vec& x){
    return vec({x(0),x(1)});
}

std::array<mat,3> reflexions(const std::array<vec,3>& Tr){
    using namespace algo::geometry;
    std::array<vec,3> normals;
    for (uint k = 0;k<3;k++){
        normals[k] = cross(Tr[(k+1)%3],Tr[(k+2)%3]).normalize();
        if (normals[k].scalar_product(Tr[k]) < 0)
            normals[k] *= -1.;
     }


    for (uint k = 0;k<3;k++){
        std::cout << "float3 p" << k+1 << " = float3(";
        for (uint i = 0;i<3;i++)
            std::cout << Tr[k](i) << (i<2 ? "," : ");");
        std::cout << std::endl;
    }
    for (uint k = 0;k<3;k++){
        std::cout << "float3 n" << k+1 << " = float3(";
        for (uint i = 0;i<3;i++)
            std::cout << normals[k](i) << (i<2 ? "," : ");");
        std::cout << std::endl;
    }

    std::array<mat,3> reflex;
    for (uint k = 0;k<3;k++){
        reflex[k] = chaskal::Id<scalar>(3) - mat(normals[k])*normals[k].transpose()*2.;
    }
    return reflex;
}

int main(int argc, char *argv[])
{
    /*
    cloud Hf = generate_fondamental_hoop();

    cloud nucleus = Hf;

    const uint N = 5,nb = N*2;
    const scalar th = M_PI/N;
    for (uint k = 0;k<nb;k++){
        nucleus += Hf.apply_to_copy(geometry::Rx(th*k));
    }
    cloud pave = nucleus + nucleus.apply_to_copy(geometry::Ry(M_PI/2)) + nucleus.apply_to_copy(geometry::Rz(M_PI/2));
    algo::export_cloud("/home/eulerson/paving.data",pave);
    return 0;
    */

    auto Tr = gen_spherical_triangle(M_PI/5);
    cloud triangle;
    triangle.points = std::vector<vec>(Tr.begin(),Tr.end());

    auto rlfx = reflexions(Tr);

    cloud octa;
    vec p = Tr[2];
    for (uint k = 0;k<40;k++){
        octa.add_point(p);
        p = rlfx[k%3]*p;
    }
    algo::export_cloud("/home/eulerson/paving.data",octa);
    return 0;


    QApplication a(argc,argv);
    PlotWindow w; w.resize(500,500);


    PlotTab* T = w.add_tab("sphere paving");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();
    //L->new_point_cloud(pave.apply_to_copy(proj));

    w.show();
    return a.exec();
}
