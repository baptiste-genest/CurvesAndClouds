#include <curvesandcloud.h>

using namespace cnc;

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);

    std::vector<QVector3D> points(4);
    float x = 0.5;
    points[0][1] = x;
    points[1][0] = x;
    points[1][1] = x;
    points[3][0] = x;

    PlotWindow W;W.resize(500,500);
    auto SV = W.add_tab("3D scene")->add_3D_scene();
    auto S = SV->getScene();
    std::pair<graphics::Object*,graphics::Quad*> OQ = S->add_object<graphics::Quad>(points);

    W.show();

    return App.exec();
}
