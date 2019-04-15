/**
* @file Font.cpp
*/
#define  _CRT_SECURE_NO_WARNINGS
#include "Font.h"
#include <memory>
#include <iostream>
#include <stdio.h>

/**
* �t�H���g�`��@�\���i�[���閼�O���.
*/
namespace Font {

/**
* �t�H���g�`��I�u�W�F�N�g������������.
*
* @param maxChar   �ő�`�敶����.
*
* @retval true  ����������.
* @retval false ���������s.
*/
bool Renderer::Init(size_t maxChar)
{
  spriteRenderer.Init(maxChar, "Res/Font.vert", "Res/Font.frag");
  return true;
}

/**
* �t�H���g�t�@�C����ǂݍ���.
*
* @param filename �t�H���g�t�@�C����.
*
* @retval true  �ǂݍ��ݐ���.
* @retval false �ǂݍ��ݎ��s.
*/
bool Renderer::LoadFromFile(const char* filename)
{
  const std::unique_ptr<FILE, decltype(&fclose)> fp(fopen(filename, "r"), fclose);
  if (!fp) {
    return false;
  }

  int line = 1;
  int ret = fscanf(fp.get(), "info face=%*s size=%f bold=%*d italic=%*d charset=%*s"
    " unicode=%*d stretchH=%*d smooth=%*d aa=%*d padding=%*d,%*d,%*d,%*d spacing=%*d,%*d", &baseFontSize);
  ++line;

  glm::vec2 scale;
  ret = fscanf(fp.get(), " common lineHeight=%*d base=%*d scaleW=%f scaleH=%f pages=%*d packed=%*d", &scale.x, &scale.y);
  if (ret < 2) {
    std::cerr << "ERROR: " << filename << "�̓ǂݍ��݂Ɏ��s(line=" << line << ")\n";
    return false;
  }
  const glm::vec2 reciprocalScale = glm::vec2(1.0f / scale);
  ++line;

  std::vector<std::string> texNameList;
  for (;;) {
    char tex[128];
    ret = fscanf(fp.get(), " page id=%*d file=%127s", tex);
    if (ret < 1) {
      break;
    }
    std::string texFilename = filename;
    const size_t lastSlashIndex = texFilename.find_last_of('/', std::string::npos);
    if (lastSlashIndex == std::string::npos) {
      texFilename.clear();
    } else {
      texFilename.resize(lastSlashIndex + 1);
    }
    texFilename.append(tex + 1); // �ŏ��́u"�v�𔲂��Ēǉ�.
    texFilename.pop_back(); // �Ō�́u"�v������.
    texNameList.push_back(texFilename);
    ++line;
  }
  if (texNameList.empty()) {
    std::cerr << "ERROR: " << filename << "�̓ǂݍ��݂Ɏ��s(line=" << line << ")\n";
    return false;
  }

  int charCount;
  ret = fscanf(fp.get(), " chars count=%d", &charCount);
  if (ret < 1) {
    std::cerr << "ERROR: " << filename << "�̓ǂݍ��݂Ɏ��s(line=" << line << ")\n";
    return false;
  }
  ++line;

  fixedAdvance = 0;
  fontList.resize(65536);
  for (int i = 0; i < charCount; ++i) {
    FontInfo font;
    glm::vec2 uv;
    ret = fscanf(fp.get(), " char id=%d x=%f y=%f width=%f height=%f xoffset=%f yoffset=%f xadvance=%f page=%*d chnl=%*d", &font.id, &uv.x, &uv.y, &font.size.x, &font.size.y, &font.offset.x, &font.offset.y, &font.xadvance);
    if (ret < 8) {
      std::cerr << "ERROR: " << filename << "�̓ǂݍ��݂Ɏ��s(line=" << line << ")\n";
      return false;
    }
    uv.y = scale.y - uv.y - font.size.y; // �t�H���g�t�@�C���͍��オ���_�Ȃ̂ŁAOpenGL�̍��W�n(���������_)�ɕϊ�.
    font.uv = uv;;
    if (font.id < 65536) {
      fontList[font.id] = font;
      if (font.xadvance > fixedAdvance) {
        fixedAdvance = font.xadvance;
      }
    }
    ++line;
  }

  // �e�N�X�`����ǂݍ���.
  texList.clear();
  texList.reserve(texNameList.size());
  for (const auto& e : texNameList) {
    Texture::Image2DPtr tex = std::make_shared<Texture::Image2D>(e.c_str());
    if (!tex) {
      return false;
    }
    texList.push_back(tex);
  }
  return true;
}

/**
* �����F��ݒ肷��.
*
* @param c�����F.
*/
void Renderer::Color(const glm::vec4& c)
{
  color = c;
}

/**
*�����F���擾����.
*
* @return�����F.
*/
const glm::vec4& Renderer::Color() const
{
  return color;
}

/**
* �������ǉ�����.
*
* @param position �\���J�n���W.
* @param str      �ǉ����镶����.
*
* @retval true  �ǉ�����.
* @retval false �ǉ����s.
*/
bool Renderer::AddString(const glm::vec2& position, const wchar_t* str)
{
  glm::vec2 pos = position;
  Sprite sprite(texList.front());
  for (const wchar_t* itr = str; *itr; ++itr) {
    const FontInfo& font = fontList[*itr];
    if (font.id >= 0 && font.size.x && font.size.y) {
      const glm::vec2 baseOffset = (font.size - baseFontSize) * 0.5f * scale;
      glm::vec3 offsetedPos = glm::vec3(pos + baseOffset + font.offset * scale, 0);
      if (!propotional) {
        offsetedPos.x = pos.x;
      }
      sprite.Position(offsetedPos);
      sprite.Rectangle({ font.uv, font.size });
      sprite.Scale(scale);
      sprite.Color(color);
      if (!spriteRenderer.AddVertices(sprite)) {
        return false;
      }
    }
    pos.x += (propotional ? font.xadvance : fixedAdvance) * scale.x;
  }
  return true;
}

/**
* VBO���V�X�e���������Ƀ}�b�s���O����.
*/
void Renderer::BeginUpdate()
{
  spriteRenderer.BeginUpdate();
}

/**
* VBO�̃}�b�s���O����������.
*/
void Renderer::EndUpdate()
{
  spriteRenderer.EndUpdate();
}

/**
* �t�H���g��`�悷��.
*/
void Renderer::Draw(const glm::vec2& ss) const
{
  spriteRenderer.Draw(ss);
}

} // namespace Font
