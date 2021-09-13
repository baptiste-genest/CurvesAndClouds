#include "material.h"

std::string cnc::graphics::Material::buildVertexShader()
{
    std::string shader = shader_header;
    shader += "precision mediump float;\n";

    for (const shader_uniform& su : m_uniforms)
        shader += "uniform " + type_names[(int)su.type] + " " + su.name + ";\n";

    for (const shader_attribute& sa : m_attributes)
        shader += "in " + type_names[(int)sa.type]+ " " + sa.name + ";\n";

    shader += "uniform mat4 local_proj;\n uniform mat4 view_proj;\n";

    shader += "void main(){\n";

    shader += "}";
    return shader;
}

std::string cnc::graphics::Material::buildFragmentShader()
{
    std::string shader = shader_header;
    shader += "out vec4 glFragColor;\n";

    shader += "void main(){\n";

    shader += "}";
    return shader;
}
