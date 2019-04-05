/**
* @file MainGameScene.h
*/
#ifndef MAINGAMESCENE_H_INCLUDED
#define MAINGAMESCENE_H_INCLUDED
#include "../Scene.h"

/**
* ���C���Q�[�����.
*/
class MainGameScene : public Scene
{
public:
  MainGameScene();
  virtual bool Initialize() override;
  virtual void Update(SceneStack&, float) override;
  virtual void Render() override;
  virtual void Finalize() override;
};

#endif // MAINGAMESCENE_H_INCLUDED
