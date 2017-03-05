//
//  GLApp.cpp
//  TouchCone
//
//  Created by zhuang yusong on 2017/3/5.
//  Copyright © 2017年 zhuang yusong. All rights reserved.
//
#include <stdlib.h>
#include "GLApp.h"
#include "Path.h"

void GLApp::onTouchBegan(float x, float y){
    
}

void GLApp::onTouchMove(float x, float y){

}

void GLApp::onTouchEnd(float x, float y){
}

void GLApp::initProgram(){
    GLShader vertexShader;
    vertexShader.createVertex();
    char* szVertexShader = this->readShader(Path::joinResource("Shaders/Simple.vert"));
    vertexShader.compile(szVertexShader);
    free(szVertexShader);
    
    GLShader fragmentShader;
    fragmentShader.createFragment();
    char* szFragmentShader = this->readShader(Path::joinResource("Shaders/Simple.frag"));
    fragmentShader.compile(szFragmentShader);
    free(szFragmentShader);
    
    m_glProgram.create();
    m_glProgram.attachShader(vertexShader);
    m_glProgram.attachShader(fragmentShader);
    m_glProgram.link();
    m_glProgram.use();
}

char* GLApp::readShader(const string& path){
    FILE* file = fopen(path.c_str(), "rb");
    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    char* ctn = (char*)malloc((size + 1) *sizeof(char));
    fseek(file, -size, SEEK_CUR);
    fread(ctn, 1, size, file);
    ctn[size] = 0;
    fclose(file);
    return ctn;
}
