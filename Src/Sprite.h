/**
* @file Sprite.h
*/
#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED
#include "BufferObject.h"
#include "Texture.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <vector>

/**
* ��`�\����.
*/
struct Rect
{
  glm::vec2 origin; ///< �������_.
  glm::vec2 size; ///< �c���̕�.
};

/**
* �X�v���C�g�N���X.
*/
class Sprite
{
public:
  Sprite() = default;
  explicit Sprite(const Texture::Image2DPtr&);
  virtual ~Sprite() = default;
  Sprite(const Sprite&) = default;
  Sprite& operator=(const Sprite&) = default;

  void Position(const glm::vec3& p) { position = p; }
  const glm::vec3& Position() const { return position; }
  void Color(const glm::vec4& c) { color = c; }
  const glm::vec4& Color() const { return color; }
  void Rectangle(const Rect& r) { rect = r; }
  const Rect& Rectangle() const { return rect; }
  const Texture::Image2DPtr& Texture() const { return texture; }
  void Texture(const Texture::Image2DPtr& tex);

private:
  glm::vec3 position;
  glm::vec4 color = glm::vec4(1);
  Rect rect = { glm::vec2(0, 0), glm::vec2(1, 1) };

  Texture::Image2DPtr texture;
};

/**
* �X�v���C�g�`��N���X.
*/
class SpriteRenderer
{
public:
  SpriteRenderer() = default;
  ~SpriteRenderer() = default;
  SpriteRenderer(const SpriteRenderer&) = delete;
  SpriteRenderer& operator=(const SpriteRenderer&) = delete;

  bool Init(size_t maxSpriteCount);
  void BeginUpdate();
  bool AddVertices(const Sprite&);
  void EndUpdate();
  void Draw(const glm::vec2&) const;
  void ClearDrawData();

private:
  BufferObject vbo;
  BufferObject ibo;
  VertexArrayObject vao;
  Shader::ProgramPtr program;

  GLsizei vboUsed = 0;            ///< VBO�Ɋi�[����Ă��钸�_��.
  struct Vertex* pVBO = nullptr;  ///< VBO�ւ̃|�C���^.

  struct DrawData {
    size_t count;
    size_t offset;
    Texture::Image2DPtr texture;
  };
  std::vector<DrawData> drawDataList;
};

#endif // SPRITE_H_INCLUDED