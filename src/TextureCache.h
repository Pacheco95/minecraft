#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "Texture2D.h"

namespace App {

class TextureCache {
public:
  std::shared_ptr<Texture2D> get(const std::string &path);

private:
  std::unordered_map<std::string, std::shared_ptr<Texture2D>> s_cache{};
};

} // namespace App
