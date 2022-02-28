#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo;
using namespace cnc::algo::geometry;

cscalar diagoff = std::polar(1.,2*M_PI/6);

vec ctv(const cscalar& z){
    return linear_utils::vec2(z.real(),z.imag());
}

int main(int argc, char *argv[]) {
  QApplication App(argc, argv);
  PlotWindow Window;
  Window.resize(500, 500);

  PlotTab *T = Window.add_tab("my first tab");
  PlotFrame *F = T->add_frame();
  PlotLayer *L = F->add_layer();

  int W = 10;
  int H = 10;
  cloud opti;
  diagoff /= diagoff.imag();

  for (int i = 0;i<W;i++)
      for (int j = 0;j<H;j++){
          if (j % 2)
            opti.add_point(ctv(cscalar(i,j-1) + diagoff));
          else
            opti.add_point(ctv(cscalar(i,j)));
      }
  L->new_point_cloud(opti);

  Window.show();
  return App.exec();
}
