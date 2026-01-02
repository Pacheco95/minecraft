#include "ShaderCache.h"

namespace App {

std::shared_ptr<Shader> ShaderCache::get(const std::string &name) {
  if (const auto entry = s_cache.find(name); entry != s_cache.end()) {
    return entry->second;
  }

  auto shader = std::make_shared<Shader>(name);

  s_cache.insert({name, shader});

  return shader;
}

std::unordered_map<std::string, std::shared_ptr<Shader>> ShaderCache::s_cache = {};

} // namespace App
