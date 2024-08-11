#pragma once

#include <raylib.h>
#include <string>

using std::string;

class PopupManager {

public:
  PopupManager();
  ~PopupManager();

  // void print(string s);
  // void draw();
  void draw(const float x, const float y);
  void render(string s);
  void zero_alpha();

private:
  RenderTexture target;

  float alpha = 1.0f;
};
