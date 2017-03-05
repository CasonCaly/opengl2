#include <cmath>
#include "TouchCone.h"

TouchCone::TouchCone(){
    m_rotationAngle = 0.0f;
    m_scale = 1.0f;
    m_previousX = 0.0f;
    m_previousY = 0.0f;
}

void TouchCone::init(){
    
    glEnable(GL_DEPTH_TEST);
    
    m_pivotPoint = ivec2(320 / 2, 480 / 2);
    
    const float coneRadius = 0.5f;
    const float coneHeight = 1.866f;
    const int coneSlices = 40;
    const float dtheta = TwoPi / coneSlices;
    const int vertexCount = coneSlices * 2 + 1;
    const int diskCenterIndex = vertexCount - 1;
    
    m_bodyIndexCount = coneSlices * 3;
    m_diskIndexCount = coneSlices * 3;
    
    vector<Vertex> coneVertices(vertexCount);
    vector<Vertex>::iterator vertex = coneVertices.begin();
    
    // Cone's body
    for (float theta = 0; vertex != coneVertices.end() - 1; theta += dtheta) {
        
        // Grayscale gradient
        float brightness = abs(sin(theta));
        vec4 color(brightness, brightness, brightness, 1);
        
        // Apex vertex
        vertex->Position = vec3(0, 1, 0);
        vertex->Color = color;
        vertex++;
        
        // Rim vertex
        vertex->Position.x = coneRadius * cos(theta);
        vertex->Position.y = 1 - coneHeight;
        vertex->Position.z = coneRadius * sin(theta);
        vertex->Color = color;
        vertex++;
    }
    
    // Disk center
    vertex->Position = vec3(0, 1 - coneHeight, 0);
    vertex->Color = vec4(1, 1, 1, 1);
    
    // Create the VBO for the vertices.
    glGenBuffers(1, &m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 coneVertices.size() * sizeof(coneVertices[0]),
                 &coneVertices[0],
                 GL_STATIC_DRAW);
    
    vector<GLubyte> coneIndices(m_bodyIndexCount + m_diskIndexCount);
    vector<GLubyte>::iterator index = coneIndices.begin();
    
    // Body triangles
    for (int i = 0; i < coneSlices * 2; i += 2) {
        *index++ = i;
        *index++ = (i + 1) % (2 * coneSlices);
        *index++ = (i + 3) % (2 * coneSlices);
    }
    
    // Disk triangles
    for (int i = 1; i < coneSlices * 2 + 1; i += 2) {
        *index++ = diskCenterIndex;
        *index++ = i;
        *index++ = (i + 2) % (2 * coneSlices);
    }
    
    // Create the VBO for the indices.
    glGenBuffers(1, &m_indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 coneIndices.size() * sizeof(coneIndices[0]),
                 &coneIndices[0],
                 GL_STATIC_DRAW);
    
    mat4 projectionMatrix = mat4::Frustum(-1.6f, 1.6, -2.4, 2.4, 5, 10);
    GLUniform* projectionUniform = m_glProgram.getUniform("Projection");
    projectionUniform->matrix4fv( 1, 0, projectionMatrix.Pointer());
}


void TouchCone::render(){

    GLAttribute* positionSlot = m_glProgram.getAttribute("Position");
    GLAttribute* colorSlot = m_glProgram.getAttribute("SourceColor");
    
    mat4 rotation = mat4::Rotate(m_rotationAngle);
    mat4 scale = mat4::Scale(m_scale);
    mat4 translation = mat4::Translate(0, 0, -7);
    
    GLUniform* modelviewUniform = m_glProgram.getUniform("Modelview");
    mat4 modelviewMatrix = scale * rotation * translation;
    
    GLsizei stride = sizeof(Vertex);
    const GLvoid* colorOffset = (GLvoid*) sizeof(vec3);
    
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    modelviewUniform->matrix4fv(1, 0, modelviewMatrix.Pointer());
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
    positionSlot->vertexAttribPointer(3, GL_FLOAT, GL_FALSE, stride, 0);
    colorSlot->vertexAttribPointer(4, GL_FLOAT, GL_FALSE, stride, colorOffset);
    positionSlot->enableVertexAttribArray();
    
    const GLvoid* bodyOffset = 0;
    const GLvoid* diskOffset = (GLvoid*) m_bodyIndexCount;
    
    colorSlot->enableVertexAttribArray();
    glDrawElements(GL_LINES, m_bodyIndexCount, GL_UNSIGNED_BYTE, bodyOffset);
    colorSlot->disableVertexAttribArray();
    colorSlot->vertexAttrib4f(1, 1, 1, 1);
    glDrawElements(GL_LINES, m_diskIndexCount, GL_UNSIGNED_BYTE, diskOffset);
    
    positionSlot->disableVertexAttribArray();
}

void TouchCone::onTouchBegan(float x, float y){
    m_scale = 1.5f;
    m_previousX = x;
    m_previousY = y;
}

void TouchCone::onTouchMove(float x, float y){
     m_scale = 1.5f;
    
    vec2 direction = vec2(x - m_previousX, y - m_previousY).Normalized();
    
    // Flip the Y axis because pixel coords increase towards the bottom.
    direction.y = -direction.y;
    
    m_rotationAngle = std::acos(direction.y) * 180.0f / 3.14159f;
    if (direction.x > 0)
        m_rotationAngle = -m_rotationAngle;
    
    m_previousX = x;
    m_previousY = y;
}

void TouchCone::onTouchEnd(float x, float y){
     m_scale = 1.0f;
}

