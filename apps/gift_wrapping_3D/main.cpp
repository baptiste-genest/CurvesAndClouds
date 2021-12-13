#include <curvesandcloud.h>

using namespace cnc;

int main(int argc, char *argv[])
{
    QApplication App(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();

    tex_stream ts;
    ts << "My first plot!" << tex::endl;

    L->add_text_frame_from_tex_stream(ts);

    w.show();
    return App.exec();
}
