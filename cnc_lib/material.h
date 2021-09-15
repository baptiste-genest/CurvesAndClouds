#ifndef MATERIAL_H
#define MATERIAL_H
#include "graphics.h"
#include "GLWrapper.h"

#if defined(CNC_LIB)
#  define CNC_LIB_EXPORT Q_DECL_EXPORT
#else
#  define CNC_LIB_EXPORT Q_DECL_IMPORT
#endif

namespace cnc {

namespace graphics {


enum shader_attributes_type {
    a_float,
    a_vec2,
    a_vec3,
    a_vec4
};


enum shader_uniforms_type {
    u_float,
    u_vec2,
    u_vec3,
    u_vec4,
    u_mat3,
    u_mat4
};

struct shader_attribute {
    std::string name;
    shader_attributes_type type;
    std::vector<float> values;
};

struct shader_uniform {
    std::string name;
    shader_uniforms_type type;
    std::vector<float> values;
};


class Material
{
public:
    Material();

    inline void addUniform(const shader_uniform& su){
        m_uniforms.push_back(su);
    };

    inline void addAttribute(const shader_attribute& sa){
        m_attributes.push_back(sa);
    };

    std::string buildVertexShader();
    std::string buildFragmentShader();

private:
    std::string m_vertexShader;
    std::string m_fragmentShader;

    std::string main_fragment_function;
    std::string main_vertex_function;

    std::vector<shader_attribute> m_attributes;
    std::vector<shader_uniform> m_uniforms;

    const std::string shader_header = "#version 300 es\n ";

    const std::vector<std::string> type_names = {
        "float",
        "vec2",
        "vec3",
        "vec4",
        "mat3",
        "mat4"
    };
};

}

}

#endif // MATERIAL_H
