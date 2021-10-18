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

cnc::graphics::loc cnc::graphics::Material::getAttributeLoc(const std::string &name) const
{
    auto f = GLWrapper::get_GL_functions();
    auto L = f->glGetAttribLocation(m_shaderId,name.c_str());
    if (L < 0){
        std::cerr << "WARNING LOC OF " << name << " WASN'T FOUND" << std::endl;
    }
    return L;
}

std::string cnc::graphics::Material::buildVertexShader()
{
    std::string shader = shader_header;
    shader += "uniform"+precision + "mat4 " + local_mat_name + ";\n";
    shader += "uniform"+precision + "mat4 " + view_proj_mat_name + ";\n";

    for (const shader_uniform& su : m_uniforms)
        if (su.dest == vertex || su.dest == vertex_fragment)
            shader += "uniform"+precision + " " + su.getTypedName() + ";\n";

    shader += "in"+precision + "vec3 vertex;\n";
    shader += "out"+precision + "vec3 fragVertex;\n";

    for (const shader_attribute& sa : m_attributes){
        if (sa.apt == in)
            shader += "in"+precision + sa.getTypedName() + ";\n";
        else if (sa.apt == out)
            shader += "out"+precision + sa.getTypedName() + ";\n";
        else {
            shader += "in"+precision + sa.getTypedName() + ";\n";
            shader += "out"+precision + type_names[(int)sa.type] + " frag_" + sa.name + ";\n";
        }
    }

    shader += "void main(){\n" + main_vertex_function + "\n}";

    return shader;
}

std::string cnc::graphics::Material::buildFragmentShader()
{
    std::string shader = shader_header;
    shader += "precision mediump float;\n";
    shader += "out"+precision + "vec4 glFragColor;\n";
    shader += "in"+precision + "vec3 fragVertex;\n";

    for (const shader_uniform& su : m_uniforms)
        if (su.dest == fragment || su.dest == vertex_fragment)
            shader += "uniform" + precision + su.getTypedName() + ";\n";

    for (const shader_attribute& sa : m_attributes){
        if (sa.apt == inout){
            shader += "in"+precision + type_names[(int)sa.type]+ " frag_" + sa.name + ";\n";
        }
        else if (sa.apt == out){
            shader += "in"+precision + sa.getTypedName() + ";\n";
        }
    }
    shader += "void main(){\n" + main_fragment_function + "\n}";

    return shader;
}

void cnc::graphics::Material::loadProjectors(const mat4 &view_proj, const mat4 &local)
{
    auto f = GLWrapper::get_GL_functions();
    f->glUseProgram(m_shaderId);
    f->glUniformMatrix4fv(m_localLoc,1,GL_FALSE,local.data());
    f->glUniformMatrix4fv(m_vpLoc,1,GL_FALSE,view_proj.data());
}

void cnc::graphics::Material::loadCustomUniforms()
{
    auto f = GLWrapper::get_GL_functions();
    f->glUseProgram(m_shaderId);
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
    for (uint k = 0;k<m_uniforms.size();k++)
        m_customUniformsLoc[k] = f->glGetUniformLocation(m_shaderId,m_uniforms[k].name.c_str());
}

cnc::graphics::Material cnc::graphics::Material::shadedUniformColor(const std::vector<float>& color)
{
    graphics::Material M;
    M.addUniform(shader_uniform("mat_color",u_vec3,fragment,color));
    M.addAttribute(shader_attribute("normal",a_vec3,inout));

    M.setMainVertexFunction(
                "frag_normal = normal;\n"
"fragVertex = (local_mat*vec4(vertex,1)).xyz;\n"
"gl_Position = view_proj_mat*local_mat*vec4(vertex,1.f);\n");
    M.setMainFragmentFunction(""
"float shade = max(dot(-frag_normal,normalize(vec3(0,1,1))),0.f);\n"
"glFragColor = vec4(mat_color*shade,1);"
);
    return M;
}

cnc::graphics::Material cnc::graphics::Material::uniformColor(const std::vector<float> &color)
{
    graphics::Material M;
    M.addUniform(shader_uniform("mat_color",u_vec3,fragment,color));

    M.setMainVertexFunction(
"gl_Position = view_proj_mat*local_mat*vec4(vertex,1.f);\n");
    M.setMainFragmentFunction(""
"glFragColor = vec4(mat_color,1);"
);
    return M;

}

#endif
