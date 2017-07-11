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

	GLUniform* m_projectionUniform;//投影矩阵
	GLUniform* m_modelviewUniform;//模型视图矩阵
	GLUniform* m_normalMatrixUniform;//法线矩阵
	GLUniform* m_lightPositionUniform;//光源位置
	GLUniform* m_ambientMaterialUniform;//环境光颜色值
	GLUniform* m_specularMaterialUniform;//镜面光材质
	GLUniform* m_shininessUniform;//发光?

	GLAttribute* m_positionSlot;//顶点属性
	GLAttribute* m_colorSlot;
	GLAttribute* m_normalSlot;//法线属性
	GLAttribute* m_diffuseMaterialSlot;//漫反射属性
};

#endif