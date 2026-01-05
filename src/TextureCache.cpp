#include "TextureCache.h"

namespace App {

std::shared_ptr<Texture2D> TextureCache::get(const std::string &path) {
  if (const auto entry = s_cache.find(path); entry != s_cache.end()) {
    return entry->second;
  }

  auto texture = std::make_shared<Texture2D>(path);

  s_cache.insert({path, texture});

  return texture;
}

} // namespace App
