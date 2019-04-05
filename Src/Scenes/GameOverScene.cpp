/**
* @file GameOverScene.cpp
*/
#include "../GLFWEW.h"
#include "GameOverScene.h"
#include "TitleScene.h"

/**
* �R���X�g���N�^.
*/
GameOverScene::GameOverScene() : Scene("GameOverScene")
{
}

/**
* �V�[��������������.
*
* @retval true  ����������.
* @retval false ���������s. �Q�[���i�s�s�ɂ��A�v���O�������I�����邱��.
*/
bool GameOverScene::Initialize()
{
  return true;
}

/**
* �V�[�����X�V����.
*
* @param sceneStack �V�[������I�u�W�F�N�g.
* @param deltaTime  �O��̍X�V����̌o�ߎ���(�b).
*/
void GameOverScene::Update(SceneStack& sceneStack, float deltaTime)
{
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  if (window.KeyDown(GLFW_KEY_ENTER)) {
    sceneStack.Replace(std::make_shared<TitleScene>());
  }
}
/**
* �V�[����`�悷��.
*/
void GameOverScene::Render()
{
}

/**
* �V�[����j������.
*/
void GameOverScene::Finalize()
{
}
