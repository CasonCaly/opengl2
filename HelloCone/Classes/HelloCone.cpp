#include <cmath>
#include "HelloCone.h"


HelloCone::HelloCone()
{
	m_rotationAngle = 0.0f;
	m_lastRotationAngle = 0.0f;
	m_previousX = 0.0f;
	m_previousY = 0.0f;
	m_bodyIndexCount = 0;
}

void HelloCone::init()
{
    
    glEnable(GL_DEPTH_TEST);
    
    const float coneRadius = 0.5f;
    const float coneHeight = 1.866f;
	const int coneSlices = 40;

	//{
	//    // Allocate space for the cone vertices.
	//    m_cone.resize((coneSlices + 1) * 2);
	//    
	//    // Initialize the vertices of the triangle strip.
	//    vector<Vertex>::iterator vertex = m_cone.begin();
	//    const float dtheta = TwoPi / coneSlices;
	//    for (float theta = 0; vertex != m_cone.end(); theta += dtheta) {
	//        
	//        // Grayscale gradient
	//        float brightness = abs(sin(theta));
	//        vec4 color(brightness, brightness, brightness, 1);
	//        
	//        // Apex vertex
	//        vertex->Position = vec3(0, 1, 0);
	//        vertex->Color = color;
	//        vertex++;
	//        
	//        // Rim vertex
	//        vertex->Position.x = coneRadius * cos(theta);
	//        vertex->Position.y = 1 - coneHeight;
	//        vertex->Position.z = coneRadius * sin(theta);
	//        vertex->Color = color;
	//        vertex++;
	//    }
	//}
	//
	//{
	//    // Allocate space for the disk vertices.
	//    m_disk.resize(coneSlices + 2);
	//    
	//    // Initialize the center vertex of the triangle fan.
	//    vector<Vertex>::iterator vertex = m_disk.begin();
	//    vertex->Color = vec4(0.75, 0.75, 0.75, 1);
	//    vertex->Position.x = 0;
	//    vertex->Position.y = 1 - coneHeight;
	//    vertex->Position.z = 0;
	//    vertex++;
	//    
	//    // Initialize the rim vertices of the triangle fan.
	//    const float dtheta = TwoPi / coneSlices;
	//    for (float theta = 0; vertex != m_disk.end(); theta += dtheta) {
	//        vertex->Color = vec4(0.75, 0.75, 0.75, 1);
	//        vertex->Position.x = coneRadius * cos(theta);
	//        vertex->Position.y = 1 - coneHeight;
	//        vertex->Position.z = coneRadius * sin(theta);
	//        vertex++;
	//    }
	//}
	{
		int vertexCount = coneSlices * 2 + 1;
		m_bodyIndexCount = coneSlices * 3;
		m_diskIndexCount = coneSlices * 3;

		vector<Vertex> cone;
		cone.resize(vertexCount);
		float dtheta = TwoPi / coneSlices;
		vector<Vertex>::iterator begin = cone.begin();
		for (float theta = 0.0f; begin != cone.end() - 1; theta += dtheta)
		{
			float bright = abs(sin(theta));
			vec4 color = vec4(bright, bright, bright, 1.0f);

			begin->Position = vec3(0, 1, 0);
			begin->Color = color;
			begin++;

			begin->Position.x = cos(theta) * coneRadius;
			begin->Position.y = 1.0f - coneHeight;
			begin->Position.z = sin(theta) * coneRadius;
			begin->Color = color;
			begin++;
		}
		//圆锥底部
		begin->Position = vec3(0, 1 - coneHeight, 0);
		begin->Color = vec4(1.0f, 1.0f, 1.0f, 1.0f);

		//将顶点数据和vbo绑定
		m_vertexBuffer.gen();
		m_vertexBuffer.bind(GL_ARRAY_BUFFER);
		m_vertexBuffer.setData(
					GL_ARRAY_BUFFER, 
					cone.size() * sizeof(Vertex),
					&cone[0],
					GL_STATIC_DRAW);

		//生成圆锥索引
		vector<GLubyte> coneIndices(m_bodyIndexCount + m_diskIndexCount);
		vector<GLubyte>::iterator index = coneIndices.begin();
		for (int i = 0; i < 2 * coneSlices; i += 2)
		{
			*index++ = i;
			*index++ = (i + 1) % (2 * coneSlices);
			*index++ = (i + 3) % (2 * coneSlices);
		}

		int diskCenterIndex = vertexCount - 1;
		//生成圆锥底盘索引
		for (int i = 0; i < 2 * coneSlices; i += 2)
		{
			*index++ = (i + 1) % (2 * coneSlices);
			*index++ = (i + 3) % (2 * coneSlices);
			*index++ = diskCenterIndex;
		}

		//将圆锥索引和vbo绑定
		m_indexBuffer.gen();
		m_indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		m_indexBuffer.setData(GL_ELEMENT_ARRAY_BUFFER,
			coneIndices.size() * sizeof(GLubyte),
			&coneIndices[0],
			GL_STATIC_DRAW);


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
    
	mat4 rotation = mat4::Rotate(m_rotationAngle, vec3(1.0f, 0.0f, 0.0f));
    mat4 translation = mat4::Translate(0, 0, -7);
    
    // Set the model-view matrix.
    GLUniform* modelviewUniform = m_glProgram.getUniform("Modelview");
    mat4 modelviewMatrix = rotation * translation;
	modelviewUniform->matrix4fv(1, 0, modelviewMatrix.Pointer());

	//// Draw the cone.
	//{
	//    GLsizei stride = sizeof(Vertex);
	//    const GLvoid* pCoords = &m_cone[0].Position.x;
	//    const GLvoid* pColors = &m_cone[0].Color.x;
	//    positionSlot->vertexAttribPointer(3, GL_FLOAT, GL_FALSE, stride, pCoords);
	//    colorSlot->vertexAttribPointer(4, GL_FLOAT, GL_FALSE, stride, pColors);
	//    glDrawArrays(GL_TRIANGLE_STRIP, 0, (GLsizei)m_cone.size());
	//}
	//
	//// Draw the disk that caps off the base of the cone.
	//{
	//    GLsizei stride = sizeof(Vertex);
	//    const GLvoid* pCoords = &m_disk[0].Position.x;
	//    const GLvoid* pColors = &m_disk[0].Color.x;
	//    positionSlot->vertexAttribPointer(3, GL_FLOAT, GL_FALSE, stride, pCoords);
	//    colorSlot->vertexAttribPointer(4, GL_FLOAT, GL_FALSE, stride, pColors);
	//    glDrawArrays(GL_TRIANGLE_FAN, 0, (GLsizei)m_disk.size());
	//}
	//

	{
		m_indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
		m_vertexBuffer.bind(GL_ARRAY_BUFFER);

		positionSlot->vertexAttribPointer(3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
		colorSlot->vertexAttribPointer(4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(vec3));

		positionSlot->enableVertexAttribArray();
		colorSlot->enableVertexAttribArray();

		glDrawElements(GL_TRIANGLES, m_bodyIndexCount, GL_UNSIGNED_BYTE, 0);
		glDrawElements(GL_TRIANGLES, m_diskIndexCount, GL_UNSIGNED_BYTE, (GLvoid*)(m_bodyIndexCount * sizeof(GLubyte)));
	}

    positionSlot->disableVertexAttribArray();
    colorSlot->disableVertexAttribArray();
}

void HelloCone::onTouchBegan(float x, float y)
{
	m_previousX = x;
	m_previousY = y;
	m_rotationAngle = m_lastRotationAngle;
}

void HelloCone::onTouchMove(float x, float y)
{
	float detha = x - m_previousX;
	float anglePerPix = 30/15; //30度没15像素
	m_rotationAngle = m_lastRotationAngle - (detha * anglePerPix);
}

void HelloCone::onTouchEnd(float x, float y)
{
	m_lastRotationAngle = m_rotationAngle;
}