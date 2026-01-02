#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "Shader.h"

namespace App {

class ShaderCache {
public:
  static std::shared_ptr<Shader> get(const std::string &name);

private:
  static std::unordered_map<std::string, std::shared_ptr<Shader>> s_cache;
};

} // namespace App
