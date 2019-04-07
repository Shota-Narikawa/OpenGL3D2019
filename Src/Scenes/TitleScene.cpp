/**
* @file TitleScene.cpp
*/
#include "TitleScene.h"
#include "MainGameScene.h"
#include "../GLFWEW.h"

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
  spriteRenderer.Init(1000);
  Sprite spr(std::make_shared<Texture::Image2D>("Res/Title.tga"));
  sprites.push_back(spr);
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
  spriteRenderer.BeginUpdate();
  for (const auto& e : sprites) {
    spriteRenderer.AddVertices(e);
  }
  spriteRenderer.EndUpdate();
}

/**
* �V�[����`�悷��.
*/
void TitleScene::Render()
{
  const GLFWEW::Window& window = GLFWEW::Window::Instance();
  spriteRenderer.Draw({ window.Width(), window.Height() });
}

/**
* �V�[����j������.
*/
void TitleScene::Finalize()
{
  sprites.clear();
}
