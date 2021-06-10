#include <curvesandcloud.h>

using namespace cnc;

uint pick(const std::array<scalar,4>& probs){
    scalar t = algo::stat::random_var::random_scalar(0.,1.);
    scalar cumulated = 0;
    for (uint k = 0;k<4;k++){
        cumulated += probs[k];
        if (t < cumulated)
            return k;
    }
    throw Cnc_error("random picking error");
}

int main(int argc, char *argv[])
{
    std::array<mat,4> transforms;
    transforms[0] = mat(3,3,{
                            0,0,0,
                            0,0.16,0,
                            0,0,1
                        });
    transforms[1] = mat(3,3,{
                            0.85,0.04,0,
                            -0.04,0.85,1.60,
                            0,0,1
                        });
    transforms[2] = mat(3,3,{
                            0.2,-0.26,0,
                            0.23,0.22,1.6,
                            0,0,1
                        });
    transforms[3] = mat(3,3,{
                            -0.15,0.28,0,
                            0.26,0.24,0.44,
                            0,0,1
                        });

    std::array<scalar,4> probabilities =
    {0.01,0.85,0.07,0.07};

    uint N = 2000;

    vec x({0.,0.,1.});

    cloud fern = algo::stat::init_empty_cloud(N,2);

    for (uint k = 0;k<N;k++){
        uint p = pick(probabilities);
        x = transforms[p]*x;
        fern[k](0) = x(0);
        fern[k](1) = x(1);
    }

    QApplication a(argc,argv);
    PlotWindow w; w.resize(500,500);

    PlotTab* T = w.add_tab("my first tab");
    PlotFrame* F= T->add_frame();
    PlotLayer* L = F->add_layer();

    //range r{-2,2};
    L->new_point_cloud(fern,1);

    w.show();
    return a.exec();
}
