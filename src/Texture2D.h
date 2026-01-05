#pragma once

#include <string>

enum TextureIndex {
  DIFFUSE,
  HEIGHT,
  SPECULAR,
  ROUGHNESS,
  AMBIENT_OCCLUSION,
};

class Texture2D {
public:
  explicit Texture2D(std::string path);
  ~Texture2D();

  void load(TextureIndex textureIndex);
  [[nodiscard]] unsigned int getId() const;
  [[nodiscard]] const std::string &getPath() const;

private:
  unsigned int m_id;
  std::string m_path;

  void free() const;
};
