#ifndef MATERIAL_H
#define MATERIAL_H
#include "graphics.h"
#include "GLWrapper.h"
#ifdef CNC_OPENGL

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

const static uint shader_type_size[6] = {1,2,3,4,9,16};
const static std::vector<std::string> type_names = {
        "float",
        "vec2",
        "vec3",
        "vec4",
        "mat3",
        "mat4"
};

enum attribute_passing_type {
    in,
    out,
    inout
};

struct shader_attribute {
    inline shader_attribute(const std::string& Name,shader_attributes_type Type,attribute_passing_type _apt){
        name = Name;
        type = Type;
        apt = _apt;
    }

    inline std::string getTypedName() const{
        return type_names[(int) type] + " " + name;
    }

    shader_attributes_type type;
    attribute_passing_type apt;
    std::string name;
};

struct shader_uniform {
    inline shader_uniform(const std::string& Name,shader_uniforms_type Type,const std::vector<float>& Values){
        name = Name;
        type = Type;
        values = Values;
    }

    inline std::string getTypedName() const{
        return type_names[(int) type] + " " + name;
    }

    shader_uniforms_type type;
    std::vector<float> values;
    std::string name;
};


class Material
{
public:
    Material();

    inline void addUniform(const shader_uniform& su){
        m_uniforms.push_back(su);
    };

    inline void addAttribute(shader_attribute sa){
        m_attributes.push_back(sa);
    };

    inline uint nbAttributes() const {
        return m_attributes.size();
    }

    inline shader_id getShaderId() const {
        return m_shaderId;
    }

    loc getAttributeLoc(uint k) const;
    loc getAttributeLoc(const std::string& name) const;

    std::string buildVertexShader();
    std::string buildFragmentShader();

    inline void setMainFragmentFunction(const std::string& mff) {
        main_fragment_function = mff;
    }

    inline void setMainVertexFunction(const std::string& mvf) {
        main_vertex_function = mvf;
    }

    inline const std::vector<shader_attribute>& getAttributes() const {
        return m_attributes;
    }

    void loadProjectors(const mat4& view_proj,const mat4& local);
    void loadCustomUniforms();

    void init();
    void draw();

private:
    std::string m_vertexShader;
    std::string m_fragmentShader;

    std::string main_fragment_function;
    std::string main_vertex_function;

    std::vector<shader_attribute> m_attributes;
    std::vector<shader_uniform> m_uniforms;

    shader_id m_shaderId;
    loc m_vpLoc;
    loc m_localLoc;

    std::vector<loc> m_customUniformsLoc;

    const std::string local_mat_name = "local_mat";
    const std::string vertex_attrib_name = "vertex";
    const std::string view_proj_mat_name = "view_proj_mat";

    const std::string shader_header = "#version 300 es\n";
    std::string precision = " highp ";
};

}

}
#endif

#endif // MATERIAL_H
