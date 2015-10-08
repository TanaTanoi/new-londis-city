#version 120
varying vec3 position;
varying vec3 normal;
varying vec2 vTextureCoord0;
uniform samplerCube skybox;

void main()
{
	gl_FragColor.rgb = textureCube(skybox, position).rgb;
}