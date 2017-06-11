//
//  GLCone.cpp
//  glcore
//
//  Created by zhuang yusong on 2017/6/11.
//  Copyright © 2017年 zhuang yusong. All rights reserved.
//

#include "GLCone.h"

GLCone::GLCone(float height, float radius) : m_height(height), m_radius(radius)
{
    GLInterval interval = { ivec2(20, 20), vec2(TwoPi, 1), vec2(30, 20) };
    this->setInterval(interval);
}

vec3 GLCone::evaluate(const vec2& domain)
{
    float u = domain.x, v = domain.y;
    float x = m_radius * (1 - v) * cos(u);
    float y = m_height * (v);
    float z = m_radius * (1 - v) * -sin(u);
    return vec3(x, y, z);
}
