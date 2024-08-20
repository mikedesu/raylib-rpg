#pragma once

#include <raylib.h>
#include <string>

using std::string;

class PopupManager {

public:
  PopupManager();
  ~PopupManager();

  void draw(const float x, const float y);
  void render(string s);
  void zero_alpha();
  void set_fontsize(const int size);
  const int get_fontsize() const;

private:
  RenderTexture target;
  float alpha = 1.0f;
  int fontsize = 24;
};
