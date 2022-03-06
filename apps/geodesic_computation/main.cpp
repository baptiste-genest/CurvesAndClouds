#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::symbolic;



int main(int argc, char *argv[]) {
    smat g(2,2);
    Variable r,th;
    svec map(2);
    map(0) = r*cos(th);
    map(1) = r*sin(th);

    vec x = linear_utils::vec2(1.,M_PI*0.5);
    std::cout << map(x) << std::endl;

    return 0;
    QApplication App(argc, argv);
    PlotWindow Window;
    Window.resize(500, 500);

    PlotTab *T = Window.add_tab("my first tab");
    PlotFrame *F = T->add_frame();
    PlotLayer *L = F->add_layer();

    tex_stream ts;
    ts << "My first plot!" << tex::endl;

    L->add_text_frame_from_tex_stream(ts);

    Window.show();
    return App.exec();
}
