/**
* @file Mesh.h
*/
#ifndef MESH_H_INCLUDED
#define MESH_H_INCLUDED
#include <GL/glew.h>
#include "BufferObject.h"
#include <glm/glm.hpp>
#include <vector>
#include <unordered_map>
#include <string>

namespace Mesh {

// ���_�f�[�^.
struct Vertex
{
  glm::vec3 position;
  glm::vec4 color;
  glm::vec2 texCoord;
  glm::vec3 normal;
};

// ���_����.
struct VertexAttribute
{
  GLuint index = GL_INVALID_INDEX;
  GLint size = 0;
  GLenum type = GL_FLOAT;
  GLsizei stride = 0;
  size_t offset = 0;
};

/**
* �`�悷�郁�b�V���f�[�^.
*
* Primitve���܂Ƃ߂�����.
*/
struct Mesh
{
  std::string name;
  GLenum mode = GL_TRIANGLES;
  GLsizei count = 0;
  GLenum type = GL_UNSIGNED_SHORT;
  const GLvoid* indices = 0;
  GLint baseVertex = 0;
};

/**
* �S�Ă̕`��f�[�^��ێ�����N���X.
*
* ���b�V���̓o�^���@:
* - ���_�f�[�^�ƃC���f�b�N�X�f�[�^��p�ӂ���.
* - Mesh�I�u�W�F�N�g��p�ӂ��A���b�V���������߂�Mesh::name�ɐݒ肷��.
* - AddVertexData()�Œ��_�̍��W�A�e�N�X�`�����W�A�F�Ȃǂ�ݒ肵�A�߂�l��Vertex�T�C�Y�Ŋ������l��baseVertex�ɐݒ肷��.
* - AddIndexData()�ŃC���f�b�N�X�f�[�^��ݒ肵�A�߂�l��Mesh::indices�ɐݒ肷��.
* - Mesh��mode, count, type�����o�[�ɓK�؂Ȓl��ݒ肷��.
*   ���Ƃ��΍��W�𒸓_�A�g���r���[�g��0�ԖڂƂ��Ďw�肷��ꍇ�AMesh::attributes[0]::offset�ɐݒ肷��.
* - ���_�A�g���r���[�g��index, size, type, stride�ɓK�؂Ȓl��ݒ肷��.
* - AddMesh()�Ń��b�V����o�^����.
*/
class Buffer
{
public:
  Buffer() = default;
  ~Buffer() = default;

  bool Init(GLsizeiptr vboSize, GLsizeiptr iboSize);
  GLintptr AddVertexData(const void* data, size_t size);
  GLintptr AddIndexData(const void* data, size_t size);
  void AddMesh(const Mesh&);
  const Mesh& GetMesh(const char* meshName) const;
  void Bind();
  void Unbind();

  void CreateCircle(const char* name, size_t segments);

private:
  BufferObject vbo;
  BufferObject ibo;
  VertexArrayObject vao;
  GLintptr vboEnd = 0;
  GLintptr iboEnd = 0;
  std::unordered_map<std::string, Mesh> meshes;
};

} // namespace Mesh

#endif // MESH_H_INCLUDED