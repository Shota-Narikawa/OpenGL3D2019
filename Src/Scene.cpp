/**
* @file Scene.cpp
*/
#include "Scene.h"
#include <iostream>

/**
* �R���X�g���N�^.
*
* @param name �V�[����.
*/
Scene::Scene(const char* name) : name(name)
{
  std::cout << "Scene Constructor: " << name << "\n";
}

/**
* �f�X�g���N�^.
*/
Scene::~Scene()
{
  Finalize();
  std::cout << "Scene Destructor: " << name << "\n";
}

/**
* �V�[����������Ԃɂ���.
*/
void Scene::Play()
{
  isActive = true;
  std::cout << "Scene Play: " << name << "\n";
}

/**
* �V�[�����~��Ԃɂ���.
*/
void Scene::Stop()
{
  isActive = false;
  std::cout << "Scene Stop: " << name << "\n";
}

/**
* �V�[����\������.
*/
void Scene::Show()
{
  isVisible = true;
  std::cout << "Scene Show: " << name << "\n";
}

/**
* �V�[�����\���ɂ���.
*/
void Scene::Hide()
{
  isVisible = false;
  std::cout << "Scene Hide: " << name << "\n";
}

/**
* �V�[�������擾����.
*
* @return �V�[����.
*/
const std::string& Scene::Name() const
{
  return name;
}

/**
* �V�[���̊�����Ԃ𒲂ׂ�.
*
* @retval true  �������Ă���.
* @retval false ��~���Ă���.
*/
bool Scene::IsActive() const
{
  return isActive;
}

/**
* �V�[���̕\����Ԃ𒲂ׂ�.
*
* @retval true  �\�����.
* @retval false ��\�����.
*/
bool Scene::IsVisible() const
{
  return isVisible;
}

/**
*
*/
SceneStack::SceneStack()
{
  stack.reserve(16);
}

/**
*
*/
void SceneStack::Push(ScenePtr p)
{
  if (!stack.empty()) {
    Current().Stop();
  }
  stack.push_back(p);
  Current().Initialize();
  Current().Play();
  std::cout << "SceneStack Push: " << p->Name() << "\n";
}

/**
*
*/
void SceneStack::Pop()
{
  if (!stack.empty()) {
    Current().Stop();
    Current().Finalize();
  }
  const std::string sceneName = stack.back()->Name();
  stack.pop_back();
  if (!stack.empty()) {
    Current().Play();
  }
  std::cout << "SceneStack Pop: " << sceneName << "\n";
}

/**
*
*/
void SceneStack::Replace(ScenePtr p)
{
  const std::string sceneName = stack.back()->Name();
  if (!stack.empty()) {
    Current().Stop();
    Current().Finalize();
    stack.pop_back();
  }
  stack.push_back(p);
  Current().Initialize();
  Current().Play();
  std::cout << "SceneStack Replace: " << sceneName << " -> " << p->Name() << "\n";
}

/**
*
*/
Scene& SceneStack::Current()
{
  return *stack.back();
}

/**
*
*/
const Scene& SceneStack::Current() const
{
  return *stack.back();
}

/**
*
*/
size_t SceneStack::Size() const
{
  return stack.size();
}

/**
*
*/
bool SceneStack::Empty() const
{
  return stack.empty();
}

/**
*
*/
void SceneStack::Update(float deltaTime)
{
  for (auto& e : stack) {
    e->Update(*this, deltaTime);
  }
}

/**
*
*/
void SceneStack::Render()
{
  for (auto& e : stack) {
    if (e->IsVisible()) {
      e->Render();
    }
  }
}