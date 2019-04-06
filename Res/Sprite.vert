/**
* @file Sprite.vert
*/
#version 410

layout(location=0) in vec3 vPosition;
layout(location=1) in vec4 vColor;
layout(location=2) in vec2 vTexCoord;

layout(location=0) out vec4 outColor;
layout(location=1) out vec2 outTexCoord;

uniform mat4x4 matVP;

/**
* �X�v���C�g�p���_�V�F�[�_�[.
*/
void main()
{
  outColor = vColor;
  outTexCoord = vTexCoord;
  gl_Position = matVP * vec4(vPosition, 1.0);
}