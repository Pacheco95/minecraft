#pragma once

#include <string>

class Texture2D {
public:
  explicit Texture2D(std::string path);
  ~Texture2D();

  void load();
  void bind() const;

private:
  unsigned int m_id;
  std::string m_path;

  void free() const;
};
