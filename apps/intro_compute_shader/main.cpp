#include <curvesandcloud.h>

using namespace cnc;


int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setVersion(4,3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    auto C = QOpenGLContext::currentContext();
    C->functions()->initializeOpenGLFunctions();
    QOpenGLContext::currentContext()->setFormat(format);
    ComputeShader cs;
    cs.init();
    return 0;
}
