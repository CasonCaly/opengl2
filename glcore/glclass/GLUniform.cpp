//
//  GLUniform.cpp
//  HelloArrow
//
//  Created by zhuang yusong on 2017/3/5.
//  Copyright © 2017年 zhuang yusong. All rights reserved.
//

#include "GLUniform.h"

GLUniform::GLUniform(GLuint program, GLuint uniform){
    m_program = program;
    m_unifrom = uniform;
}

void GLUniform::matrix4fv(GLsizei count, GLboolean transpose, const GLfloat *value){
    glUniformMatrix4fv(m_unifrom, count, transpose, value);
}
