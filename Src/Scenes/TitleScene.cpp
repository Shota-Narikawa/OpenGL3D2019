/**
* @file TitleScene.cpp
*/
#include "TitleScene.h"
#include "MainGameScene.h"
#include "../GLFWEW.h"

const glm::vec3 positions[4] = { { -1, 1, 0 }, { -1, -1, 0 }, { 1, -1, 0 }, { 1, 1, 0 } };
const glm::vec4 colors[4] = { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } };
const glm::vec2 texCoords[4] = { { 0, 1 }, { 0, 0 }, { 1, 0 }, { 1, 1 } };
const GLushort indices[] = { 0, 1, 2, 2, 3, 0 };

/**
* �R���X�g���N�^.
*/
TitleScene::TitleScene() : Scene("TitleScene")
{
}

/**
* �V�[��������������.
*
* @retval true  ����������.
* @retval false ���������s. �Q�[���i�s�s�ɂ��A�v���O�������I�����邱��.
*/
bool TitleScene::Initialize()
{
  vbo.Create(GL_ARRAY_BUFFER, sizeof(positions) + sizeof(colors) + sizeof(texCoords));
  int offset = 0;
  vbo.BufferSubData(offset, sizeof(positions), positions);
  offset += sizeof(positions);
  vbo.BufferSubData(offset, sizeof(colors), colors);
  offset += sizeof(colors);
  vbo.BufferSubData(offset, sizeof(texCoords), texCoords);

  ibo.Create(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices);
  vao.Create(vbo.Id(), ibo.Id());
  tex = std::make_unique<Texture::Image2D>("Res/Title.tga");
  program = Shader::Cache::Instance().Create("Res/Sprite.vert", "Res/Sprite.frag");
  return true;
}

/**
* �V�[�����X�V����.
*
* @param sceneStack �V�[������I�u�W�F�N�g.
* @param deltaTime  �O��̍X�V����̌o�ߎ���(�b).
*/
void TitleScene::Update(SceneStack& sceneStack, float deltaTime)
{
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  if (window.KeyDown(GLFW_KEY_ENTER)) {
    sceneStack.Replace(std::make_shared<MainGameScene>());
  }
}

/**
* �V�[����`�悷��.
*/
void TitleScene::Render()
{
  vao.Bind();
  vao.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
  vao.VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), sizeof(positions));
  vao.VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), sizeof(positions) + sizeof(colors));

  tex->Bind(0);

  program->Use();
  program->SetViewProjectionMatrix(glm::mat4(1));
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, reinterpret_cast<const GLvoid*>(0));

  program->Unuse();
  tex->Unbind(0);
  vao.Unbind();
}

/**
* �V�[����j������.
*/
void TitleScene::Finalize()
{
}
