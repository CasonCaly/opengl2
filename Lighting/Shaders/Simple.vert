attribute vec4 Position;
attribute vec4 Normal;//��������
attribute vec4 DiffuseMaterial;//���������

uniform mat4 Projection;
uniform mat4 Modelview;
uniform mat4 NormalMatrix;

unifrom vec3 LightPosition;
uniform vec3 AmbientMaterial;//���������
uniform vec3 SpecularMaterial;//��������
uniform float Shininess;//

attribute vec4 SourceColor;
varying vec4 DestinationColor;


void main(void)
{
    DestinationColor = SourceColor;
    gl_Position = Projection * Modelview * Position;
}
