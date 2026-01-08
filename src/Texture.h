#pragma once

#include <string>

class Texture {
public:
  explicit Texture(std::string path);
  ~Texture();

  void load();
  void bind() const;

private:
  unsigned int m_id;
  std::string m_path;

  void free() const;
};
