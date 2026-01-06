#pragma once

#include <unordered_map>
#include <sstream>
#include <string>

constexpr auto sep = " ";

template <class T> class Cache {
public:
  template <class... CtorArgs> std::shared_ptr<T> &get(CtorArgs... args) {
    std::ostringstream oss;

    ((oss << sep << args), ...);

    const std::string key = std::string(typeid(T).name()) + oss.str();

    if (const auto entry = m_cache.find(key); entry != m_cache.end()) {
      return entry->second;
    }

    m_cache.insert({key, std::make_shared<T>(std::forward<CtorArgs>(args)...)});

    return m_cache[key];
  }

private:
  std::unordered_map<std::string, std::shared_ptr<T>> m_cache{};
};
