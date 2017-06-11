//
//  GLSurface.hpp
//  glsurface
//
//  Created by zhuang yusong on 2017/6/11.
//  Copyright © 2017年 zhuang yusong. All rights reserved.
//

#ifndef GLSurface_hpp
#define GLSurface_hpp

#include <stdio.h>

class GLSurface{
    
public:
    
    virtual int getVertexCount() {};
    
    virtual int getLineIndexCount() {};
    
    virtual int getTriangleIndexCount() {};
    
    virtual void generateVertices(vector<float>& vertices) {};
    
    virtual void generateLineIndices(vector<unsigned short>& indices){};
    
    virtual void generateTriangleIndices(vector<unsigned short>& indices){};
    
    virtual ~GLSurface() {}
};

#endif /* GLSurface_hpp */
