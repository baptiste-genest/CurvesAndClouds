#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::graphics;


int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    Material M1 = Material::shadedUniformColor({1,0,0});
    Material M2 = Material::shadedUniformColor({0,0,1});

    PlotTab* T = w.add_tab("Tree simulation");
    auto SV = T->add_3D_scene();
    auto S = SV->getScene();
    S->getCam()->setCamPos(vec3(0,0,5));
    {
        auto OP = S->add_object<graphics::Mesh>(M1,"/home/eulerson/Curves-Clouds/data/bunny.obj",0.6f);
        OP.first->setObjectPos(vec3(-0.4,0,0));
    }
    {
        auto OP = S->add_object<graphics::Mesh>(M2,"/home/eulerson/Curves-Clouds/data/bunny.obj",0.6f);
        OP.first->setObjectPos(vec3(0.6,0,0));
    }

    w.show();
    return App.exec();
}
