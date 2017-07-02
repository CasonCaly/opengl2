attribute vec4 Position;
attribute vec4 Normal;//法线向量，
attribute vec4 DiffuseMaterial;//漫反射材质

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat4 NormalMatrix;

uniform vec3 LightPosition;
uniform vec3 AmbientMaterial;//环境光材质
uniform vec3 SpecularMaterial;//镜面光材质
uniform float Shininess;//

attribute vec4 SourceColor;
varying vec4 DestinationColor;


void main(void)
{
    DestinationColor = SourceColor;
    gl_Position = Projection * Modelview * Position;
}
