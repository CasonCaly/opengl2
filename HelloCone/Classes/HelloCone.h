#ifndef HELLO_CONE_H
#define HELLO_CONE_H
#include <vector>
using namespace std;
#include "GLProgram.h"
#include "Quaternion.hpp"

struct Vertex {
    vec3 Position;
    vec4 Color;
};

struct Animation {
    Quaternion Start;
    Quaternion End;
    Quaternion Current;
    float Elapsed;
    float Duration;
};

class HelloCone {
    
public:
    
    HelloCone();
    
    void init();
    
    void render();

private:
    
    void applyOrtho(float maxX, float maxY);
    
    void applyRotation(float degrees);

private:
    
    vector<Vertex> m_cone;
    vector<Vertex> m_disk;
    Animation m_animation;
    GLProgram m_glProgram;
};
#endif
