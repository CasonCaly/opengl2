#include <cmath>
#include "HelloCone.h"

HelloCone::HelloCone(){

}

void HelloCone::init(){
    
    glEnable(GL_DEPTH_TEST);
    
    const float coneRadius = 0.5f;
    const float coneHeight = 1.866f;
    const int coneSlices = 40;
    
    {
        // Allocate space for the cone vertices.
        m_cone.resize((coneSlices + 1) * 2);
        
        // Initialize the vertices of the triangle strip.
        vector<Vertex>::iterator vertex = m_cone.begin();
        const float dtheta = TwoPi / coneSlices;
        for (float theta = 0; vertex != m_cone.end(); theta += dtheta) {
            
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
    }
    
    {
        // Allocate space for the disk vertices.
        m_disk.resize(coneSlices + 2);
        
        // Initialize the center vertex of the triangle fan.
        vector<Vertex>::iterator vertex = m_disk.begin();
        vertex->Color = vec4(0.75, 0.75, 0.75, 1);
        vertex->Position.x = 0;
        vertex->Position.y = 1 - coneHeight;
        vertex->Position.z = 0;
        vertex++;
        
        // Initialize the rim vertices of the triangle fan.
        const float dtheta = TwoPi / coneSlices;
        for (float theta = 0; vertex != m_disk.end(); theta += dtheta) {
            vertex->Color = vec4(0.75, 0.75, 0.75, 1);
            vertex->Position.x = coneRadius * cos(theta);
            vertex->Position.y = 1 - coneHeight;
            vertex->Position.z = coneRadius * sin(theta);
            vertex++;
        }
    }

    mat4 projectionMatrix = mat4::Frustum(-1.6f, 1.6f, -2.4f, 2.4f, 5, 10);
    GLUniform* projectionUniform = m_glProgram.getUniform("Projection");
    projectionUniform->matrix4fv( 1, 0, projectionMatrix.Pointer());
}


void HelloCone::render(){
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    GLAttribute* positionSlot = m_glProgram.getAttribute("Position");
    GLAttribute* colorSlot = m_glProgram.getAttribute("SourceColor");
    
    positionSlot->enableVertexAttribArray();
    colorSlot->enableVertexAttribArray();
    
    mat4 rotation(m_animation.Current.ToMatrix());
    mat4 translation = mat4::Translate(0, 0, -7);
    
    // Set the model-view matrix.
    GLUniform* modelviewUniform = m_glProgram.getUniform("Modelview");
    mat4 modelviewMatrix = rotation * translation;
    modelviewUniform->matrix4fv(1, 0, modelviewMatrix.Pointer());
    
    // Draw the cone.
    {
        GLsizei stride = sizeof(Vertex);
        const GLvoid* pCoords = &m_cone[0].Position.x;
        const GLvoid* pColors = &m_cone[0].Color.x;
        positionSlot->vertexAttribPointer(3, GL_FLOAT, GL_FALSE, stride, pCoords);
        colorSlot->vertexAttribPointer(4, GL_FLOAT, GL_FALSE, stride, pColors);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)m_cone.size());
    }
    
    // Draw the disk that caps off the base of the cone.
    {
        GLsizei stride = sizeof(Vertex);
        const GLvoid* pCoords = &m_disk[0].Position.x;
        const GLvoid* pColors = &m_disk[0].Color.x;
        positionSlot->vertexAttribPointer(3, GL_FLOAT, GL_FALSE, stride, pCoords);
        colorSlot->vertexAttribPointer(4, GL_FLOAT, GL_FALSE, stride, pColors);
        glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)m_disk.size());
    }
    
    positionSlot->disableVertexAttribArray();
    colorSlot->disableVertexAttribArray();
}

