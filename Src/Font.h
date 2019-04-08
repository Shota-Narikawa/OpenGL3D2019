/**
* @file Font.h
*/
#ifndef FONT_H_INCLUDED
#define FONT_H_INCLUDED
#include "Sprite.h"
#include <vector>
#include <string>

namespace Font {

/**
* �r�b�g�}�b�v�t�H���g�`��N���X.
*/
class Renderer
{
public:
  Renderer() = default;
  ~Renderer() = default;
  Renderer(const Renderer&) = delete;
  Renderer& operator=(const Renderer&) = delete;

  bool Init(size_t maxChar);
  bool LoadFromFile(const char* filename);

  void Scale(const glm::vec2& s) { scale = s; }
  const glm::vec2& Scale() const { return scale; }
  void Color(const glm::vec4& c);
  const glm::vec4& Color() const;

  void BeginUpdate();
  bool AddString(const glm::vec2& position, const wchar_t* str);
  void EndUpdate();
  void Draw(const glm::vec2& ss) const;

private:
  SpriteRenderer spriteRenderer;
  std::vector<Texture::Image2DPtr> texList;

  glm::vec2 scale = glm::vec2(1); ///< �t�H���g��`�悷��Ƃ��̊g�嗦.
  glm::vec4 color = glm::vec4(1); ///< �t�H���g��`�悷��Ƃ��̐F.
  bool propotional = true;
  float fixedAdvance = 0;

  /// �t�H���g���.
  struct FontInfo {
    int id = -1;        ///< �����R�[�h.
    int page = 0;       ///< �t�H���g���܂܂��摜�̔ԍ�.
    glm::vec2 uv[2]; ///< �t�H���g�摜�̃e�N�X�`�����W.
    glm::vec2 size;     ///< �t�H���g�摜�̕\���T�C�Y.
    glm::vec2 offset;   ///< �\���ʒu�����炷����.
    float xadvance = 0; ///< �J�[�\����i�߂鋗��.
  };
  std::vector<FontInfo> fontList; ///< �t�H���g�ʒu���̃��X�g.
};

} // namespace Font

#endif // FONT_H_INCLUDED
