#include <curvesandcloud.h>

using namespace cnc;


int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    Plot_window w; w.resize(500,500);

    Plot_tab* T = w.add_tab("my first tab");
    Plot_frame* F= T->add_frame();
    Plot_layer* L = F->add_layer();

    tex_stream ts;
    ts << "My first plot!" << tex::endl;

    L->add_text_frame_from_tex_stream(ts);

    w.show();
    return a.exec();
}
