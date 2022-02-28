#include <curvesandcloud.h>

using namespace cnc;

int main(int argc, char *argv[]) {
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
