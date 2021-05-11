#include <curvesandcloud.h>

using namespace cnc;


int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    Plot_window w; w.resize(500,500);

    {
        Plot_tab* T = w.add_tab("ONE TAB");
        Plot_frame* F= T->add_frame();
        F->set_nb_layer_per_second(0.5f);

        tex_stream fs;
        fs << "A frame" << tex::endl;

        int plot_count = 0;
        for (int k = 0;k<3;k++){
            Plot_layer* L = F->add_layer();
            tex_stream ls = fs;
            ls << "Layer nb: " << k << tex::endl;
            tex_stream ps = ls;
            ps << "plot nb: " << plot_count <<  "   plot nb: " << plot_count+1 << tex::endl;
            L->add_text_frame_from_tex_stream(ps);
            plot_count+=2;
        }

        T->add_frame()->add_layer()->add_text_frame("Another frame");
    }

    Plot_tab* T2 = w.add_tab("Frame system");
    {
        T2->add_frame()->add_layer()->add_text_frame("frame at (0,0) of size (1,1)");
        T2->add_frame_at(2,0,2,1)->add_layer()->add_text_frame("frame at (2,0) of size (2,1)");
        T2->add_frame_at(0,1,1,2)->add_layer()->add_text_frame("frame at (0,1) of size (1,2)");
        T2->add_frame_at(1,1,2,2)->add_layer()->add_text_frame("frame at (1,1) of size (2,2)");
    }
    w.show();
    return a.exec();
}
