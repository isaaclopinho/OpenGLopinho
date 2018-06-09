#version 400 core

layout (location = 0) in vec3 in_position;
layout (location = 3) in ivec4 BoneIDs;
layout (location = 4) in vec4 pesos;

const int MAX_BONES = 100;
uniform mat4 mvpMatrix;
uniform mat4 gBones[MAX_BONES];
uniform int animated;

void main(void){

	mat4 BoneTransform = mat4(1.0);
	
	if(animated == 1){
		BoneTransform = gBones[BoneIDs[0]] * pesos[0];
		BoneTransform += gBones[BoneIDs[1]] * pesos[1];
		BoneTransform += gBones[BoneIDs[2]] * pesos[2];
		BoneTransform += gBones[BoneIDs[3]] * pesos[3];
	}

	vec4 transform = BoneTransform * vec4(in_position, 1.0);

	gl_Position = mvpMatrix * transform ;

}