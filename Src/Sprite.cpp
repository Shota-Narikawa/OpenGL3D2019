/**
* @file Sprite.cpp
*/
#include "Sprite.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

/**
* ���_�f�[�^�^.
*/
struct Vertex
{
  glm::vec3 position; ///< ���W
  glm::vec4 color; ///< �F
  glm::vec2 texCoord; ///< �e�N�X�`�����W
};

/**
* Sprite�R���X�g���N�^.
*/
Sprite::Sprite(const Texture::Image2DPtr& tex) :
  texture(tex),
  rect(Rect{ glm::vec2(), glm::vec2(tex->Width(), tex->Height()) })
{
}

/**
* �`��Ɏg�p����e�N�X�`�����w�肷��.
*
* @param tex �`��Ɏg�p����e�N�X�`��.
*/
void Sprite::Texture(const Texture::Image2DPtr& tex)
{
  texture = tex;
  Rectangle(Rect{ glm::vec2(0), glm::vec2(tex->Width(), tex->Height()) });
}

/**
* �X�v���C�g�`��N���X������������.
*
* @param maxSpriteCount �`��\�ȍő�X�v���C�g��.
*
* @retval true  ����������.
* @retval false ���������s.
*/
bool SpriteRenderer::Init(size_t maxSpriteCount, const char* vsPath, const char* fsPath)
{
  vbo.Create(GL_ARRAY_BUFFER, sizeof(Vertex) * maxSpriteCount * 4, nullptr, GL_STREAM_DRAW);
  std::vector<GLushort> indices;
  indices.resize(maxSpriteCount * 6);
  for (GLushort i = 0; i < maxSpriteCount; ++i) {
    indices[i * 6 + 0] = (i * 4) + 0;
    indices[i * 6 + 1] = (i * 4) + 1;
    indices[i * 6 + 2] = (i * 4) + 2;
    indices[i * 6 + 3] = (i * 4) + 2;
    indices[i * 6 + 4] = (i * 4) + 3;
    indices[i * 6 + 5] = (i * 4) + 0;
  }
  ibo.Create(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), indices.data());
  vao.Create(vbo.Id(), ibo.Id());
  vao.Bind();
  vao.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position));
  vao.VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, color));
  vao.VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texCoord));
  vao.Unbind();
  program = Shader::Cache::Instance().Create(vsPath, fsPath);

  vboUsed = 0;
  pVBO = nullptr;

  drawDataList.reserve(128);

  if (!vbo.Id() || !ibo.Id() || !vao.Id() || program->IsNull()) {
    return false;
  }
  return true;
}

/**
* ���_�f�[�^�̍쐬���J�n����.
*/
void SpriteRenderer::BeginUpdate()
{
  glBindBuffer(GL_ARRAY_BUFFER, vbo.Id());
  pVBO = static_cast<Vertex*>(glMapBufferRange(GL_ARRAY_BUFFER, 0, vbo.Size(), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT));
  vboUsed = 0;
  ClearDrawData();
}

/**
* ���_�f�[�^��ǉ�����.
*
* @param sprite ���_�f�[�^�̌��ɂȂ�X�v���C�g.
*
* @retval true  �ǉ�����.
* @retval false ���_�o�b�t�@�����t�Œǉ��ł��Ȃ�.
*/
bool SpriteRenderer::AddVertices(const Sprite& sprite)
{
  if (!pVBO || vboUsed >= vbo.Size()) {
    return false;
  }
  const Texture::Image2DPtr& texture = sprite.Texture();
  const glm::vec2 textureSize(texture->Width(), texture->Height());
  const glm::vec2 reciprocalSize(glm::vec2(1) / textureSize);

  Rect rect = sprite.Rectangle();
  rect.origin *= reciprocalSize;
  rect.size *= reciprocalSize;
  const glm::vec2 halfSize = sprite.Rectangle().size * 0.5f;
  const glm::mat4 matT = glm::translate(glm::mat4(1), sprite.Position());
  const glm::mat4 matR = glm::rotate(glm::mat4(1), sprite.Rotation(), glm::vec3(0, 0, 1));
  const glm::mat4 matS = glm::scale(glm::mat4(1), glm::vec3(sprite.Scale(), 1));
  const glm::mat4 transform = matT * matR * matS;

  pVBO[0].position = transform * glm::vec4(-halfSize.x, -halfSize.y, 0, 1);
  pVBO[0].color = sprite.Color();
  pVBO[0].texCoord = rect.origin;

  pVBO[1].position = transform * glm::vec4(halfSize.x, -halfSize.y, 0, 1);
  pVBO[1].color = sprite.Color();
  pVBO[1].texCoord = glm::vec2(rect.origin.x + rect.size.x, rect.origin.y);

  pVBO[2].position = transform * glm::vec4(halfSize.x, halfSize.y, 0, 1);
  pVBO[2].color = sprite.Color();
  pVBO[2].texCoord = rect.origin + rect.size;

  pVBO[3].position = transform * glm::vec4(-halfSize.x, halfSize.y, 0, 1);
  pVBO[3].color = sprite.Color();
  pVBO[3].texCoord = glm::vec2(rect.origin.x, rect.origin.y + rect.size.y);

  pVBO += 4;
  vboUsed += sizeof(Vertex) * 4;

  if (drawDataList.empty()) {
    drawDataList.push_back({ 6, 0, texture });
  } else {
    auto& data = drawDataList.back();
    if (data.texture == texture) {
      data.count += 6;
    } else {
      drawDataList.push_back({ 6, data.offset + data.count * sizeof(GLushort), texture });
    }
  }
  
  return true;
}

/**
* ���_�f�[�^�̍쐬���I������.
*/
void SpriteRenderer::EndUpdate()
{
  glUnmapBuffer(GL_ARRAY_BUFFER);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  pVBO = nullptr;
}

/**
* �X�v���C�g��`�悷��.
*
* @param texture    �`��Ɏg�p����e�N�X�`��.
* @param screenSize ��ʃT�C�Y.
*/
void SpriteRenderer::Draw(const glm::vec2& screenSize) const
{
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  vao.Bind();
  program->Use();

  // ���s���e�A���_�͉�ʂ̒��S.
  const glm::vec2 halfScreenSize = screenSize * 0.5f;
  const glm::mat4x4 matProj = glm::ortho(-halfScreenSize.x, halfScreenSize.x, -halfScreenSize.y, halfScreenSize.y, 1.0f, 1000.0f);
  const glm::mat4x4 matView = glm::lookAt(glm::vec3(0, 0, 100), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  program->SetViewProjectionMatrix(matProj * matView);

  for (const auto& data : drawDataList) {
    data.texture->Bind(0);
    glDrawElements(GL_TRIANGLES, data.count, GL_UNSIGNED_SHORT, reinterpret_cast<const GLvoid*>(data.offset));
    data.texture->Unbind(0);
  }

  program->Unuse();
  vao.Unbind();
}

/**
* �X�v���C�g�`��f�[�^����������.
*/
void SpriteRenderer::ClearDrawData()
{
  drawDataList.clear();
}