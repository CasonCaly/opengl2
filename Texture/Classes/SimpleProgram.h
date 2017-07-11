#ifndef Normal_program_h
#define Normal_program_h
#include "glclass/GLProgram.h"
#include "math/Matrix.hpp"

class GLSurface;
class SimpleProgram : public GLProgram
{
public:

	void init();

	void useWith(GLSurface* surface, mat4& translation);

protected:

	GLUniform* m_projectionUniform;//ͶӰ����
	GLUniform* m_modelviewUniform;//ģ����ͼ����
	GLUniform* m_normalMatrixUniform;//���߾���
	GLUniform* m_lightPositionUniform;//��Դλ��
	GLUniform* m_ambientMaterialUniform;//��������ɫֵ
	GLUniform* m_specularMaterialUniform;//��������
	GLUniform* m_shininessUniform;//����?

	GLAttribute* m_positionSlot;//��������
	GLAttribute* m_colorSlot;
	GLAttribute* m_normalSlot;//��������
	GLAttribute* m_diffuseMaterialSlot;//����������
};

#endif