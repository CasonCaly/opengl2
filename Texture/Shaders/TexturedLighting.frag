varying vec4 DestinationColor;
varying vec2 TextureCoordOut;
uniform sampler2D Sampler;
uniform sampler2D Sampler2;

void main(void)
{
    vec4 tex1 = texture2D(Sampler, TextureCoordOut);
    vec4 tex2 = texture2D(Sampler2, TextureCoordOut);
    gl_FragColor = mix(tex1, tex2, 0.3) *DestinationColor;//texture2D(Sampler, TextureCoordOut) * DestinationColor;
}
