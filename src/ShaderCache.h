#pragma once

#include <memory>
#include <unordered_map>
#include <string>

#include "Shader.h"

namespace App {

class ShaderCache {
public:
  std::shared_ptr<Shader> get(const std::string &name);

private:
  std::unordered_map<std::string, std::shared_ptr<Shader>> s_cache{};
};

} // namespace App
