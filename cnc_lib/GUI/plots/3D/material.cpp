#include "material.h"

cnc::graphics::Material::Material()
{

}

cnc::graphics::loc cnc::graphics::Material::getAttributeLoc(uint k) const
{
    auto f = GLWrapper::get_GL_functions();
    return f->glGetAttribLocation(m_shaderId,m_attributes[k].name.c_str());
}

std::string cnc::graphics::Material::buildVertexShader()
{
    std::string shader = shader_header;
    shader += "precision mediump float;\n";

    for (const shader_uniform& su : m_uniforms)
        shader += "uniform " + su.getTypedName() + ";\n";

    for (const shader_attribute& sa : m_attributes){
        shader += "in " + sa.getTypedName() + ";\n";
        if (sa.varying)
            shader += "out " + type_names[(int)sa.type] + " v_" + sa.name + ";\n";

    }

    shader += "uniform mat4 local_proj;\nuniform mat4 view_proj;\n";

    shader += "void main(){\n" + main_vertex_function + "\n}";

    return shader;
}

std::string cnc::graphics::Material::buildFragmentShader()
{
    std::string shader = shader_header;
    shader += "out vec4 glFragColor;\n";

    for (const shader_attribute& sa : m_attributes){
        if (sa.varying)
            shader += "in " + type_names[(int)sa.type]+ " v_" + sa.name + ";\n";
    }

    shader += "void main(){\n" + main_fragment_function + "\n}";

    return shader;
}

void cnc::graphics::Material::init()
{
    m_shaderId = GLWrapper::makeShaderProgram(buildVertexShader(),buildFragmentShader());
}
