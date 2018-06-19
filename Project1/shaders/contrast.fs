#version 400 core

in vec2 textureCoords;
uniform float offset;
out vec4 out_Colour;

uniform sampler2D colourTexture;

vec4 Sepia(vec4 color )
{
vec3 SEPIA = vec3(1.2, 1.0, 0.8); 

float gray = dot(color.rgb, vec3(0.299, 0.587, 0.114));

vec3 sepiaColor = vec3(gray) * SEPIA;
    return vec4(
          sepiaColor.rgb, color.a
    );
}


void main()
{
	vec2 texCoords = textureCoords;
  	//texCoords.x += 2 * sin(texCoords.y * 4*2*3.14159 + offset) / 100;
    	//vec4 color = texture(colourTexture, texCoords);
    	//float average = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;
	//out_Colour = vec4(average, average, average, 1);	
	//out_Colour = texture(colourTexture, texCoords);

	vec4 texColor = texture2D(colourTexture, texCoords);
	
//	vec2 resolution = vec2(1280, 720);
	
//	vec2 position = (gl_FragCoord.xy / resolution.xy) - vec2(0.5);
	
//	float len = length(position);
	
//	float r = 0.5;

//	float softness = 0.5;

	//float vignette = smoothstep(r, r-softness, len);
	//texColor.rgb = mix(texColor.rgb, texColor.rgb * vignette, 0.9);
	//texColor = mix(texColor, Sepia(texColor), 1);
	out_Colour = texColor ;	
}  

