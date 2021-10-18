#ifndef COMPUTE_SHADER_H
#define COMPUTE_SHADER_H

#include <QOpenGLFunctions>
#include "../plots/3D/GLWrapper.h"
#include <QOpenGLFunctions_4_3_Core>

namespace cnc {

class ComputeShader : public QOpenGLFunctions_4_3_Core
{
public:
    ComputeShader();

    void init();
    void compute();
private:
    std::string getSource();

    std::string source;
    GLuint id;
    GLuint programId;
    GLuint textureId;
};

}

#endif // COMPUTE_SHADER_H
