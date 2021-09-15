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

const static uint shader_type_size[6] = {1,2,3,4,9,16};
const static std::vector<std::string> type_names = {
        "float",
        "vec2",
        "vec3",
        "vec4",
        "mat3",
        "mat4"
};

struct shader_attribute {
    inline shader_attribute(const std::string& Name,shader_attributes_type Type,bool isVarying){
        name = Name;
        type = Type;
        varying = isVarying;
    }

    inline std::string getTypedName() const{
        return type_names[(int) type] + " " + name;
    }

    shader_attributes_type type;
    bool varying = false;
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

    loc getAttributeLoc(uint k) const;

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

    void init();

private:
    std::string m_vertexShader;
    std::string m_fragmentShader;

    std::string main_fragment_function;
    std::string main_vertex_function;

    std::vector<shader_attribute> m_attributes;
    std::vector<shader_uniform> m_uniforms;

    shader_id m_shaderId;

    const std::string shader_header = "#version 300 es\n";
};

}

}

#endif // MATERIAL_H
