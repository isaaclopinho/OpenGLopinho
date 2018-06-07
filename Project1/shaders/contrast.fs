#version 400 core

in vec2 textureCoords;
uniform float offset;
out vec4 out_Colour;

uniform sampler2D colourTexture;


void main()
{
   // out_Colour = texture(colourTexture, textureCoords);
   // float average = 0.2126 * out_Colour.r + 0.7152 * out_Colour.g + 0.0722 * out_Colour.b;
    //out_Colour = vec4(average, average, average, 1.0);

	vec2 texcoord = textureCoords;
  //texcoord.x += sin(texcoord.y * 4*2*3.14159 + offset) / 100;
  out_Colour = texture2D(colourTexture, texcoord);
}  