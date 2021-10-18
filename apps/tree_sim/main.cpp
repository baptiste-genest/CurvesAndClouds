#include <curvesandcloud.h>
#include "branch.h"

using namespace cnc;
using namespace cnc::graphics;


int main(int argc, char *argv[])
{
    srand(time(NULL));
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    vec3 c(109,54,21);
    c/= 255;
    Material M1 = Material::shadedUniformColor({c[0],c[1],c[2]});

    PlotTab* T = w.add_tab("Tree simulation");
    auto SV = T->add_3D_scene();
    auto S = SV->getScene();
    S->getCam()->setCamPos(vec3(-1,-5,5)*3);

    vec3 p(0.,0.,100.f);
    float dt = 0.1;

    Branch* B = new Branch();

    for (uint k = 0;k<100;k++)
        B->performLifeCycle(p,dt);

    B->buildMesh(S);
    B->log();
    /*
    const int N = 20;
    std::vector<vec3> P(N);
    for (uint k = 0;k<N;k++){
        scalar th = k*2*M_PI/(N);
        P[k] = vec3(cos(th),sin(th),0);
    }
    float r = 0.1;
    std::vector<scalar> R(N,r);
    auto OP = S->add_object<graphics::Tube>(M1,P,R,20,1);
    */
    w.show();
    return App.exec();
}

    /*
    vec3 p(0.,0.,100.f);
    float dt = 0.1;

    Branch* B = new Branch();

    for (uint k = 0;k<100;k++)
        B->performLifeCycle(p,dt);
    B->log();
    return 0;
    */
