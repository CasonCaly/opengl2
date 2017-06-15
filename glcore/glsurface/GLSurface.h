//
//  GLSurface.hpp
//  glsurface
//
//  Created by zhuang yusong on 2017/6/11.
//  Copyright © 2017年 zhuang yusong. All rights reserved.
//

#ifndef GLSurface_hpp
#define GLSurface_hpp

#include <vector>
using namespace std;
#include "GLInterval.h"

class GLCORE GLSurface{
    
public:
    
	virtual int getVertexCount();

	virtual int getLineIndexCount();

	virtual int getTriangleIndexCount();
    
    virtual void generateVertices(vector<float>& vertices);
    
    virtual void generateLineIndices(vector<unsigned short>& indices);
    
    virtual void generateTriangleIndices(vector<unsigned short>& indices);
    
	virtual ~GLSurface();
  
protected:
    
	vec2 computeDomain(float x, float y);

    void setInterval(const GLInterval& interval);
    
    virtual vec3 evaluate(const vec2& domain);
    
    virtual bool invertNormal(const vec2& domain) const { return false; }
    
protected:
    ivec2 m_slices;
    ivec2 m_divisions;
    vec2 m_upperBound;
    vec2 m_textureCount;
};

#endif /* GLSurface_hpp */
