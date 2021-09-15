#include "GUI/plots/3D/GLWrapper.h"

cnc::graphics::shader_id cnc::graphics::GLWrapper::makeShaderProgramFromFiles(const std::string &vertex_loc, const std::string &frag_loc){
    std::string vertexShader,fragShader;
    {
        std::ifstream in(vertexShader);
        std::string contents((std::istreambuf_iterator<char>(in)),
                 std::istreambuf_iterator<char>());
        vertexShader = contents.c_str();
    }
    {
        std::ifstream in(fragShader);
        std::string contents((std::istreambuf_iterator<char>(in)),
                 std::istreambuf_iterator<char>());
        fragShader = contents.c_str();
    }
    return makeShaderProgram(vertexShader,fragShader);
}

cnc::graphics::shader_id cnc::graphics::GLWrapper::compileShader(cnc::graphics::index shaderType, const GLchar *source)
{
    auto f = get_GL_functions();
    // compiler le source
    GLint shader = f->glCreateShader(shaderType);
    if (shader == 0) return 0;

    f->glShaderSource(shader, 1, &source, NULL);
    f->glCompileShader(shader);

    // vérifier l'état
    GLint status;
    f->glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (status == GL_TRUE) return shader;

    // il y a eu une erreur, afficher le log
    int infologLength = 0;
    f->glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infologLength);
    if (infologLength > 1) {
        int charsWritten  = 0;
        GLchar *infoLog = (char *)malloc(infologLength);
        f->glGetShaderInfoLog(shader, infologLength, &charsWritten, infoLog);
        if (status == GL_FALSE) {
            std::cerr << infoLog;
        }
        free(infoLog);
        // quitter si pb
        if (status == GL_FALSE)
            throw Cnc_error("couldn't compile sharder");
    }
    return shader;
}

cnc::graphics::shader_id cnc::graphics::GLWrapper::makeShaderProgram(const std::string &vertexShader, const std::string &fragShader)
{
    auto f = get_GL_functions();
    // compiler les shaders séparément
    GLint vertexShaderId   = compileShader(GL_VERTEX_SHADER,   vertexShader.c_str());
    GLint fragmentShaderId = compileShader(GL_FRAGMENT_SHADER, fragShader.c_str());

    // créer un programme et ajouter les deux shaders
    GLint program = f->glCreateProgram();
    f->glAttachShader(program, vertexShaderId);
    f->glAttachShader(program, fragmentShaderId);

    // forcer "glVertex" à être l'attribut 0 (pb si c'est pas comme ça qu'elle s'appelle)
    // voir http://www.opengl.org/wiki/Vertex_Attribute, pour éviter le bug de l'attribut 0 pas lié
    //f->glBindAttribLocation(program, 0, "glVertex");
    //f->glBindAttribLocation(program, 1, "color");

    // lier le programme
    f->glLinkProgram(program);

    /*
    // vérifier l'état
    GLint status;
    f->glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {

        // il y a eu une erreur, afficher le log
        int infologLength = 0;
        int charsWritten  = 0;
        f->glGetProgramiv(program, GL_INFO_LOG_LENGTH,&infologLength);
        if (infologLength > 1) {
            GLchar *infoLog = (char *)malloc(infologLength);
            f->glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog);
            std::cerr << infoLog << std::endl;
            free(infoLog);
        }

        // quitter par une exception
        throw Cnc_error("couldn't link shader");
    }
    */

    return program;
}
