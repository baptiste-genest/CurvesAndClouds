#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::graphics;


int main(int argc, char *argv[])
{

    graphics::Material M;
    M.addUniform(shader_uniform("mat_color",u_vec3,{1.,0.,1.}));
    M.addAttribute(shader_attribute("color",a_vec3,out));
    M.addAttribute(shader_attribute("normal",a_vec3,inout));

    M.setMainVertexFunction(
"frag_normal = normal;\n"
"color = mat_color;\n"
"fragVertex = (local_mat*vec4(vertex,1)).xyz;\n"
"gl_Position = view_proj_mat*local_mat*vec4(vertex+normal,1.f);\n");
    M.setMainFragmentFunction(""
"float shade = max(dot(-frag_normal,vec3(0,0,-1)),0.f);\n"
"glFragColor = vec4(color*shade,1);"
);

    std::cout << M.buildVertexShader() << std::endl;
    std::cout << M.buildFragmentShader() << std::endl;

    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("Tree simulation");
    auto SV = T->add_3D_scene();
    auto S = SV->getScene();
    auto OP = S->add_object<graphics::Mesh>(M,"/home/eulerson/Curves-Clouds/data/bunny.obj",0.6f);
    //auto O = OP.first;

    w.show();
    return App.exec();
}
