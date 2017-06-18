#include <cmath>
#include "Interfaces.hpp"
#include "SimpleWireframe.h"

struct Drawable {
    GLuint VertexBuffer;
    GLuint IndexBuffer;
    int IndexCount;
};

SimpleWireframe::SimpleWireframe()
{
//	m_rotationAngle = 0.0f;
//	m_lastRotationAngle = 0.0f;
//	m_previousX = 0.0f;
//	m_previousY = 0.0f;
//	m_bodyIndexCount = 0;
}

void SimpleWireframe::init()
{
    vector<ISurface*> surfaces(1);
    //m_cone = new GLCone(3, 1, ivec2(4, 4));
    
    vector<float> vertices;
    //m_cone->generateVertices(vertices);

    m_sphere = new GLSphere(2, ivec2(20, 20));
    m_sphere->generateVertices(vertices);
    
    m_glSurface = m_sphere;
    
    m_vertexBuffer.gen();
    m_vertexBuffer.bind(GL_ARRAY_BUFFER);
    m_vertexBuffer.setData(GL_ARRAY_BUFFER,
                 vertices.size() * sizeof(vertices[0]),
                 &vertices[0],
                 GL_STATIC_DRAW);
    
    // Create a new VBO for the indices if needed.
    int indexCount = m_glSurface->getLineIndexCount();
    vector<GLushort> indices(indexCount);
    m_glSurface->generateLineIndices(indices);
    
    m_indexBuffer.gen();
    m_indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
    m_indexBuffer.setData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(GLushort),
                        &indices[0],
                        GL_STATIC_DRAW);
    
    
    m_positionSlot = m_glProgram.getAttribute("Position");
    m_colorSlot = m_glProgram.getAttribute("SourceColor");
    
    m_positionSlot->enableVertexAttribArray();
    m_translation = mat4::Translate(0, 0, -7);
    m_projectionUniform = m_glProgram.getUniform("Projection");
    m_modelviewUniform = m_glProgram.getUniform("Modelview");
}


void SimpleWireframe::render()
{
    glClearColor(0.5f, 0.5f, 0.5f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    
    int visualIndex = 0;
    
    // Set the viewport transform.
//    ivec2 size = visual->ViewportSize;
//    ivec2 lowerLeft = visual->LowerLeft;
//    glViewport(lowerLeft.x, lowerLeft.y, size.x, size.y);
    
    // Set the model-view transform.
//    Quaternion Orientation;
    mat4 rotation = mat4::Rotate(m_rotationAngle, vec3(0.0f, 1.0f, 0.0f));
    //mat4 rotation = Orientation.ToMatrix();
    mat4 modelview = rotation * m_translation;
    m_modelviewUniform->matrix4fv(1, 0, modelview.Pointer());
    
    // Set the projection transform.
    float h = 4.0f * 480 / 320;
    mat4 projectionMatrix = mat4::Frustum(-2, 2, -h / 2, h / 2, 5, 10);
    m_projectionUniform->matrix4fv(1, 0, projectionMatrix.Pointer());
    
    // Set the color.
    vec3 color = vec3(1, 1, 0.5f);
    m_colorSlot->vertexAttrib4f(color.x, color.y, color.z, 1);
    
    // Draw the wireframe.
    int stride = sizeof(vec3);

    m_vertexBuffer.bind(GL_ARRAY_BUFFER);
    m_positionSlot->vertexAttribPointer(3, GL_FLOAT, GL_FALSE, stride, 0);
    m_indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
    glDrawElements(GL_LINES, m_glSurface->getLineIndexCount(), GL_UNSIGNED_SHORT, 0);
}

void SimpleWireframe::onTouchBegan(float x, float y)
{
	m_previousX = x;
	m_previousY = y;
	m_rotationAngle = m_lastRotationAngle;
}

void SimpleWireframe::onTouchMove(float x, float y)
{
	float detha = x - m_previousX;
	float anglePerPix = 30/15; //30¶ÈÃ»15ÏñËØ
	m_rotationAngle = m_lastRotationAngle - (detha * anglePerPix);
}

void SimpleWireframe::onTouchEnd(float x, float y)
{
	m_lastRotationAngle = m_rotationAngle;
}
