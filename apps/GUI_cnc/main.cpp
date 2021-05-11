#include "curvesandcloud.h"
#include "cncmainwindow.h"
#include <QApplication>

using namespace cnc;

int main(int argc, char *argv[])
{
    try {
        QApplication a(argc, argv);
        CNCMainWindow w;
        w.resize(500, 500);

        Plot_tab* start = w.add_tab("Welcome !");
        Plot_frame* hello = start->add_frame(1, 1);
        tex_stream welcomeMsg;
        welcomeMsg << "Welcome to the Curves\\&Clouds GUI!";
        hello->add_layer()->add_text_frame_from_tex_stream(welcomeMsg);

        /*cnc::cloud R = cnc::algo::parse_csv("/home/axel/development/UCBL/L2/s2/LIFAP4/curvesandclouds/data/newIris.csv");
        Plot_tab* test = w.add_tab("TEst");
        Plot_frame* PFt = test->add_frame();
        Plot_layer* Pl = PFt->add_layer();
        Pl->new_point_cloud(R)->set_axis_policy(square_max_range);*/


        w.show();
        return a.exec();
    } catch (Cnc_error& e) {
        std::cerr << e.what() << std::endl;
    }
    return (-1);
}
