attribute vec4 Position;
attribute vec3 Normal;//法线向量，
attribute vec3 DiffuseMaterial;//漫反射材质
attribute vec4 SourceColor;
attribute vec2 TextureCoord;

uniform mat4 Projection;//投影矩阵
uniform mat4 Modelview;//模型矩阵
uniform mat3 NormalMatrix;//法线矩阵

uniform vec3 LightPosition;//光源位置
uniform vec3 AmbientMaterial;//环境光材质
uniform vec3 SpecularMaterial;//镜面光材质
uniform float Shininess;//镜面指数

varying vec4 DestinationColor;
varying vec2 TextureCoordOut;

void main(void)
{
    vec3 N = NormalMatrix * Normal;//先将顶点进行矩阵转换，这样就不受缩放影响
    vec3 L = normalize(LightPosition);//对光源进行单位矩阵话，这样才能在算镜面光的时候算出正确的半角向量
    vec3 E = vec3(0, 0, 1);
    vec3 H = normalize(L + E);//算镜面光的时候需要用到半角向量
    
    float df = max(0.0, dot(N, L));//漫反射系数，介于0和1之间。采用朗伯体反射来进行计算
    float sf = max(0.0, dot(N, H));//镜面反射系数，介于0和1之间。采用半角向量和法线的夹角的余弦值。
    sf = pow(sf, Shininess);
    
    vec3 color = AmbientMaterial + df * DiffuseMaterial + sf * SpecularMaterial;
    DestinationColor = vec4(color, 1);
    gl_Position = Projection * Modelview * Position;
    TextureCoordOut = TextureCoord;
}
