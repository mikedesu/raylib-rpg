#pragma once

#include <raylib.h>
#include <string>
// #include <memory>

using std::string;
// using std::shared_ptr;

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

  // Font &font;
};
