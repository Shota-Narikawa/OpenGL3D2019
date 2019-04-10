/**
* @file TitleScene.cpp
*/
#include "TitleScene.h"
#include "MainGameScene.h"
#include "../GLFWEW.h"
#include <glm/gtc/matrix_transform.hpp>

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
  spriteRenderer.Init(1000, "Res/Sprite.vert", "Res/Sprite.frag");
  fontRenderer.Init(1000);
  fontRenderer.LoadFromFile("Res/font.fnt");
  Sprite spr(std::make_shared<Texture::Image2D>("Res/Title.tga"));
  sprites.push_back(spr);
  meshBuffer.Init(sizeof(Mesh::Vertex) * 10000, sizeof(GLushort) * 30000);
  meshBuffer.CreateCircle("Circle", 8);

  progMesh = Shader::Cache::Instance().Create("Res/Mesh.vert", "Res/Mesh.frag");
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
  fontRenderer.BeginUpdate();
  fontRenderer.AddString(glm::vec2(0, 0), L"�^�C�g�����");
  fontRenderer.EndUpdate();
}

/**
* �V�[����`�悷��.
*/
void TitleScene::Render()
{
  const GLFWEW::Window& window = GLFWEW::Window::Instance();
  const glm::vec2 screenSize(window.Width(), window.Height());
//  spriteRenderer.Draw(screenSize);
//  fontRenderer.Draw(screenSize);

  glDisable(GL_CULL_FACE);

  meshBuffer.Bind();
  progMesh->Use();

  const glm::mat4 matView = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  const float aspectRatio = static_cast<float>(window.Width()) / static_cast<float>(window.Height());
  const glm::mat4 matProj = glm::perspective(glm::radians(30.0f), aspectRatio, 1.0f, 1000.0f);
  const glm::mat4 matModel = glm::scale(glm::mat4(1), glm::vec3(1));
  progMesh->SetViewProjectionMatrix(matProj * matView * matModel);

  const Mesh::Mesh& mesh = meshBuffer.GetMesh("Circle");
  glDrawElementsBaseVertex(mesh.mode, mesh.count, mesh.type, mesh.indices, mesh.baseVertex);

  progMesh->Unuse();
  meshBuffer.Unbind();
}

/**
* �V�[����j������.
*/
void TitleScene::Finalize()
{
  sprites.clear();
}
