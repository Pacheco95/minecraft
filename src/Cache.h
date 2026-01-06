#pragma once

#include <unordered_map>
#include <sstream>
#include <string>

constexpr auto sep = "_";

template <class T> class Cache {
public:
  template <class... CtorArgs> std::shared_ptr<T> &get(CtorArgs... args) {
    std::ostringstream oss;
    bool first = true;

    ((oss << (first ? "" : sep) << args, first = false), ...);

    const std::string key = std::string(typeid(T).name()) + sep + oss.str();

    if (const auto entry = m_cache.find(key); entry != m_cache.end()) {
      return entry->second;
    }

    m_cache.insert({key, std::make_shared<T>(std::forward<CtorArgs>(args)...)});

    return m_cache[key];
  }

private:
  std::unordered_map<std::string, std::shared_ptr<T>> m_cache{};
};
