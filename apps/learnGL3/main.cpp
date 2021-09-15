#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::graphics;

void print(const QVector4D& x){
    for (uint k = 0;k<4;k++)
        std::cout << x[k] << std::endl;
}

int main(int argc, char *argv[])
{
    Material M;
    M.addAttribute({"normal",shader_attributes_type::a_vec3,true});
    M.setMainVertexFunction("v_normal = normal;");
    M.setMainFragmentFunction("float light = max(dot(normal,light),0);\n"
    "glFragColor = vec4(vec3(1,0,0)*light,1);");

    std::cout << M.buildVertexShader() << std::endl<< std::endl;
    std::cout << M.buildFragmentShader() << std::endl;
    return 0;

    QApplication App(argc,argv);

    std::vector<QVector3D> points(4);
    float x = 0.5;
    points[0][1] = x;
    points[1][0] = x;
    points[1][1] = x;
    points[3][0] = x;
    points[3][2] = 0.1;
    QVector3D offset(-x,-x,0);
    for (uint k = 0;k<4;k++)
        points[k] += offset/2;


    PlotWindow W;W.resize(500,500);
    auto SV = W.add_tab("3D scene")->add_3D_scene();
    auto S = SV->getScene();
    graphics::Camera* C = S->getCam();
    print(C->getViewMatrix()*points[0]);
    std::pair<graphics::Object*,graphics::Quad*> OQ = S->add_object<graphics::Quad>(points);

    W.show();

    return App.exec();
}
