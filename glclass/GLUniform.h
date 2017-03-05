//
//  GLUniform.hpp
//  HelloArrow
//
//  Created by zhuang yusong on 2017/3/5.
//  Copyright © 2017年 zhuang yusong. All rights reserved.
//

#ifndef GLUniform_hpp
#define GLUniform_hpp

#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>

class GLUniform{
    
public:
    
    GLUniform(GLuint program, GLuint uniform);
    
    void matrix4fv(GLsizei count, GLboolean transpose, const GLfloat *value);
    
protected:
    
    GLuint m_unifrom;
    
    GLuint m_program;
};


#endif /* GLUniform_hpp */
