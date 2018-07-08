#version 140

in vec2 textureCoords;

out vec4 out_Color;

uniform sampler2D guiTexture;

uniform float constant;

void main(void){
	out_Color = vec4(texture(guiTexture,textureCoords).rgb, texture(guiTexture,textureCoords).a * constant);

}