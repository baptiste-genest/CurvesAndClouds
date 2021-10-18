#include "compute_shader.h"

cnc::ComputeShader::ComputeShader()
{
}

void cnc::ComputeShader::init(){
    /*----------- Create Texture -----------*/
    glGenTextures(1,&textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 640, 480, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D,0);

    /*----------- Create Compute Shader -----------*/
    id = cnc::graphics::GLWrapper::compileShader(GL_COMPUTE_SHADER,getSource().c_str());
    programId = glCreateProgram();

    glAttachShader(programId,id);
    glLinkProgram(programId);
    //glDeleteShader(programId);

    glUseProgram(programId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glBindImageTexture(0, textureId, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(40, 30, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void cnc::ComputeShader::compute()
{

}

std::string cnc::ComputeShader::getSource()
{
    return "#version 430\n"
"uniform image2D img_output;\n"
"void main(){\n"
"	ivec2 coord = ivec2(gl_GlobalInvocationID);\n"
"vec4 pixel;\n"
"if ( ((gl_WorkGroupID.x & 1u) != 1u) != ((gl_WorkGroupID.y & 1u) == 1u))\n"
"pixel = vec4(1.0,.5,.0,1.0);\n"
"else\n"
"pixel = vec4(.0,.5,1.,1.);\n"
"	imageStore(img_output,coords,pixel);}\n";
}

