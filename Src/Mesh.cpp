/**
* @file Mesh.h
*/
#include "Mesh.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>
#include <math.h>
#include <iostream>

namespace Mesh {

/**
* ���b�V���o�b�t�@������������.
*
* @param vboSize VBO�̃o�C�g�T�C�Y.
* @param iboSize IBO�̃o�C�g�T�C�Y.
*
* @retval true  ����������.
* @retval false ���������s.
*/
bool Buffer::Init(GLsizeiptr vboSize, GLsizeiptr iboSize)
{
  if (!vbo.Create(GL_ARRAY_BUFFER, vboSize)) {
    return false;
  }
  if (!ibo.Create(GL_ELEMENT_ARRAY_BUFFER, iboSize)) {
    return false;
  }
  if (!vao.Create(vbo.Id(), ibo.Id())) {
    return false;
  }
  vao.Bind();
  vao.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, position));
  vao.VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, color));
  vao.VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, texCoord));
  vao.VertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, normal));
  vao.Unbind();
  vboEnd = 0;
  iboEnd = 0;
  meshes.reserve(100);
  return true;
}

/**
* ���_�f�[�^��ǉ�����.
*
* @param data �ǉ�����f�[�^�̃|�C���^.
* @param size �ǉ�����f�[�^�̃o�C�g��.
*
* @return �f�[�^��ǉ������ʒu.
*         ���_�A�g���r���[�g��offset�p�����[�^�Ƃ��Ďg�����Ƃ��ł���.
*/
GLintptr Buffer::AddVertexData(const void* data, size_t size)
{
  vbo.BufferSubData(vboEnd, size, data);
  const GLintptr tmp = vboEnd;
  vboEnd += size;
  return tmp;
}

/**
* �C���f�b�N�X�f�[�^��ǉ�����.
*
* @param data �ǉ�����f�[�^�̃|�C���^.
* @param size �ǉ�����f�[�^�̃o�C�g��.
*
* @return �f�[�^��ǉ������ʒu.
*         �v���~�e�B�u��indices�p�����[�^�Ƃ��Ďg�����Ƃ��ł���.
*/
GLintptr Buffer::AddIndexData(const void* data, size_t size)
{
  ibo.BufferSubData(iboEnd, size, data);
  const GLintptr tmp = iboEnd;
  iboEnd += size;
  return tmp;
}

/**
* ���b�V����ǉ�����.
*
* @param mesh �ǉ����郁�b�V��.
*/
void Buffer::AddMesh(const Mesh& mesh)
{
  meshes.emplace(mesh.name, mesh);
  std::cout << "Mesh::Buffer: ���b�V��'" << mesh.name << "'��ǉ�.\n";
}

/**
* ���b�V�����擾����.
*
* @param meshName �擾���������b�V���̖��O.
*
* @return meshName�Ɠ������O�������b�V��.
*/
const Mesh& Buffer::GetMesh(const char* meshName) const
{
  const auto itr = meshes.find(meshName);
  if (itr == meshes.end()) {
    static const Mesh dummy;
    return dummy;
  }
  return itr->second;
}

/**
* �~�Ղ��쐬����.
*
* @param name     �~�Ղ̃��b�V����.
* @param segments �~�Ղ̉~���̕�����.
*/
void Buffer::CreateCircle(const char* name, size_t segments)
{
  if (segments < 3) {
    return;
  }

  std::vector<Vertex> vertices;
  vertices.reserve(segments + 1);

  Vertex center;
  center.position = glm::vec3(0);
  center.color = glm::vec4(1);
  center.texCoord = glm::vec2(0.5f);
  center.normal = glm::vec3(0, 1, 0);
  vertices.push_back(center);

  const float seg = static_cast<float>(segments);
  for (float i = 0; i < seg; ++i) {
    const glm::mat4 m = glm::rotate(glm::mat4(1), i / seg * glm::two_pi<float>(), glm::vec3(0, 1, 0));
    Vertex v;
    v.position = m * glm::vec4(1, 0, 0, 1);
    v.color = glm::vec4(1);
    v.texCoord = glm::vec2(v.position.x, v.position.z) * 0.5f + 0.5f;
    v.normal = glm::vec3(0, 1, 0);
    vertices.push_back(v);
  }
  const size_t vOffset = AddVertexData(vertices.data(), vertices.size() * sizeof(Vertex));
  
  std::vector<GLushort> indices;
  indices.reserve(segments * 3);
  for (GLushort i = 0; i < segments - 1; ++i) {
    indices.push_back(0);
    indices.push_back(i + 1);
    indices.push_back(i + 2);
  }
  indices.push_back(0);
  indices.push_back(static_cast<GLushort>(segments));
  indices.push_back(1);
  const size_t iOffset = AddIndexData(indices.data(), indices.size() * sizeof(GLushort));

  Mesh mesh;
  mesh.name = name;
  mesh.count = static_cast<GLsizei>(indices.size());
  mesh.indices = reinterpret_cast<const GLvoid*>(iOffset);
  mesh.baseVertex = vOffset / sizeof(Vertex);
  AddMesh(mesh);
}

void Buffer::Bind()
{
  vao.Bind();
}

void Buffer::Unbind()
{
  vao.Unbind();
}

} // namespace Mesh