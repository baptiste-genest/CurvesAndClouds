#include <curvesandcloud.h>
#include <GUI/plots/3D/primitives/quad.h>

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
    points[3][2] = x;

    SceneViewer W;
    auto S = W.getScene();
    S->add_object<graphics::Quad>(points);

    W.show();

    return App.exec();
}
