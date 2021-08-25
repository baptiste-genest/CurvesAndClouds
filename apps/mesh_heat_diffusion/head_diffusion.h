#ifndef HEAD_DIFFUSION_H
#define HEAD_DIFFUSION_H

#include <QObject>
#include <curvesandcloud.h>

using namespace cnc;
using namespace cnc::algo::geometry;

class heat_diffusion : public QObject
{
    Q_OBJECT
public:
    heat_diffusion(SimpleGLMesh* m,GeometryEngine* ge);
    inline ~heat_diffusion() {
        delete LB;
    }
private:
    smat* LB;
    smat* W;
    uint N;
    vec v;
    SimpleGLMesh* M;
    GeometryEngine* GE;
    const float dt = -1e-2;
    const float heat_max = 3.f;
public slots:
    void compute();
};
#endif // HEAD_DIFFUSION_H
