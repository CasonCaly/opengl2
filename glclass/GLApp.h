//
//  GLApp.hpp
//  TouchCone
//
//  Created by zhuang yusong on 2017/3/5.
//  Copyright © 2017年 zhuang yusong. All rights reserved.
//

#ifndef GLApp_hpp
#define GLApp_hpp

#include <string>
#include <vector>
#include <iostream>
using namespace std;
#include "GLProgram.h"
#include "Quaternion.hpp"

class GLApp{
  
public:
    
    virtual void onTouchBegan(float x, float y);
    
    virtual void onTouchMove(float x, float y);
    
    virtual void onTouchEnd(float x, float y);
    
    virtual void initProgram();
    
protected:
    
    char* readShader(const string& path);
    
protected:
    
    GLProgram m_glProgram;
};


#endif /* GLApp_hpp */
