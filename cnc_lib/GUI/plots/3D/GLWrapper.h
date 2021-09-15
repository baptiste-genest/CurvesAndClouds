#ifndef GLWRAPPER_H
#define GLWRAPPER_H
#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif


#include <QOpenGLFunctions>
#include "cnc_error.h"
#include "graphics.h"
#include <iostream>
#include <fstream>

namespace cnc {

namespace graphics {

namespace GLWrapper {

inline QOpenGLFunctions* get_GL_functions(){
    auto f = QOpenGLContext::currentContext()->functions();
    if (f == nullptr)
        throw Cnc_error("3D/OpenGL : couldn't load GL functions");
    return f;
}

template<typename type>
loc create_vbo(const std::vector<type>& values,int vbo_type,int usage) {
    auto f = get_GL_functions();
    loc id;
    f->glGenBuffers(1,&id);
    f->glBindBuffer(vbo_type,id);
    f->glBufferData(vbo_type,values.size()*sizeof(type),values.data(),usage);
    f->glBindBuffer(vbo_type,0);

    return id;
}

shader_id compileShader(index shaderType,const GLchar* source);

shader_id makeShaderProgramFromFiles(const std::string& vertex_loc,const std::string& frag_loc);
shader_id makeShaderProgram(const std::string& vertexShader,const std::string& fragShader);



}

}

}

#endif // GLWRAPPER_H
