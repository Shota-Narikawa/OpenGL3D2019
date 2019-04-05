/**
* @file StatusScene.cpp
*/
#include "StatusScene.h"
#include "../GLFWEW.h"

/**
* �R���X�g���N�^.
*/
StatusScene::StatusScene() : Scene("StatusScene")
{
}

/**
* �V�[��������������.
*
* @retval true  ����������.
* @retval false ���������s. �Q�[���i�s�s�ɂ��A�v���O�������I�����邱��.
*/
bool StatusScene::Initialize()
{
  return true;
}

/**
* �V�[�����X�V����.
*
* @param sceneStack �V�[������I�u�W�F�N�g.
* @param deltaTime  �O��̍X�V����̌o�ߎ���(�b).
*/
void StatusScene::Update(SceneStack& sceneStack, float deltaTime)
{
  GLFWEW::Window& window = GLFWEW::Window::Instance();
  if (window.KeyDown(GLFW_KEY_ENTER)) {
    sceneStack.Pop();
  }
}

/**
* �V�[����`�悷��.
*/
void StatusScene::Render()
{
}

/**
* �V�[����j������.
*/
void StatusScene::Finalize()
{
}

