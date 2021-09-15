#include "material.h"
#ifdef CNC_OPENGL

cnc::graphics::Material::Material()
{

}

cnc::graphics::loc cnc::graphics::Material::getAttributeLoc(uint k) const
{
    auto f = GLWrapper::get_GL_functions();
    return f->glGetAttribLocation(m_shaderId,m_attributes[k].name.c_str());
}

cnc::graphics::loc cnc::graphics::Material::getVertexLoc() const
{
    auto f = GLWrapper::get_GL_functions();
    return f->glGetAttribLocation(m_shaderId,vertex_attrib_name.c_str());
}

std::string cnc::graphics::Material::buildVertexShader()
{
    std::string shader = shader_header;
    shader += "precision mediump float;\n";
    shader += "in vec3 vertex;\n";
    shader += "out vec3 fragVertex;\n";

    shader += "uniform mat4 " + local_mat_name + ";\n";
    shader += "uniform mat4 " + view_proj_mat_name + ";\n";

    for (const shader_uniform& su : m_uniforms)
        shader += "uniform " + su.getTypedName() + ";\n";

    for (const shader_attribute& sa : m_attributes){
        shader += "in " + sa.getTypedName() + ";\n";
        if (sa.varying)
            shader += "out " + type_names[(int)sa.type] + " v_" + sa.name + ";\n";

    }

    shader += "void main(){\n" + main_vertex_function + "\n}";

    return shader;
}

std::string cnc::graphics::Material::buildFragmentShader()
{
    std::string shader = shader_header;
    shader += "out vec4 glFragColor;\n";
    shader += "in vec3 fragVertex;\n";

    for (const shader_attribute& sa : m_attributes){
        if (sa.varying)
            shader += "in " + type_names[(int)sa.type]+ " v_" + sa.name + ";\n";
    }

    shader += "void main(){\n" + main_fragment_function + "\n}";

    return shader;
}

void cnc::graphics::Material::loadProjectors(const mat4 &view_proj, const mat4 &local)
{
    auto f = GLWrapper::get_GL_functions();
    f->glUniformMatrix4fv(m_localLoc,1,GL_FALSE,local.data());
    f->glUniformMatrix4fv(m_vpLoc,1,GL_FALSE,view_proj.data());
}

void cnc::graphics::Material::loadCustomUniforms()
{
    auto f = GLWrapper::get_GL_functions();
    for (uint k = 0;k<m_uniforms.size();k++){
        loc l = m_customUniformsLoc[k];
        const auto & u = m_uniforms[k];
        switch (u.type) {
            case u_float :
                f->glUniform1f(l,u.values[0]);
                break;
            case u_vec2 :
                f->glUniform2fv(l,1,u.values.data());
                break;
            case u_vec3 :
                f->glUniform3fv(l,1,u.values.data());
                break;
            case u_vec4 :
                f->glUniform4fv(l,1,u.values.data());
                break;
            case u_mat3 :
                f->glUniformMatrix3fv(l,1,GL_FALSE,u.values.data());
                break;
            case u_mat4 :
                f->glUniformMatrix4fv(l,1,GL_FALSE,u.values.data());
                break;
        }
    }
}

void cnc::graphics::Material::init()
{
    auto f = GLWrapper::get_GL_functions();
    m_shaderId = GLWrapper::makeShaderProgram(buildVertexShader(),buildFragmentShader());
    m_localLoc = f->glGetUniformLocation(m_shaderId,local_mat_name.c_str());
    m_vpLoc = f->glGetUniformLocation(m_shaderId,view_proj_mat_name.c_str());

    m_customUniformsLoc.resize(m_uniforms.size());
    for (uint k = 0;k<m_uniforms.size();k++){
        m_customUniformsLoc[k] = f->glGetUniformLocation(m_shaderId,m_uniforms[k].name.c_str());
    }
}

#endif
