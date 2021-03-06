/**
* @file TitleScene.h
*/
#ifndef TITLESCENE_H_INCLUDED
#define TITLESCENE_H_INCLUDED
#include "../Scene.h"
#include "../Sprite.h"
#include "../Font.h"
#include <vector>

/**
* タイトル画面.
*/
class TitleScene : public Scene
{
public:
  TitleScene();
  virtual bool Initialize() override;
  virtual void Update(SceneStack&, float) override;
  virtual void Render() override;
  virtual void Finalize() override;
private:
  std::vector<Sprite> sprites;
  SpriteRenderer spriteRenderer;
  Font::Renderer fontRenderer;
};

#endif // TITLESCENE_H_INCLUDED
