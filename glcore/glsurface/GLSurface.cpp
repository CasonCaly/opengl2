//
//  GLSurface.hpp
//  glsurface
//
//  Created by zhuang yusong on 2017/6/11.
//  Copyright © 2017年 zhuang yusong. All rights reserved.
//

#include "GLSurface.h"

void GLSurface::setInterval(const GLInterval& interval)
{
    m_divisions = interval.Divisions;
    m_upperBound = interval.UpperBound;
    m_textureCount = interval.TextureCount;
    m_slices = m_divisions - ivec2(1, 1);
}
