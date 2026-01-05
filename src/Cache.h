#pragma once

#include "MapUtils.h"

#include <memory>
#include <unordered_map>

template <class Key, class Value> class Cache {
public:
  std::shared_ptr<Value> get(const Key &key);

private:
  std::unordered_map<Key, std::shared_ptr<Value>> s_cache;
};

template <class Key, class Value> std::shared_ptr<Value> Cache<Key, Value>::get(const Key &key) {
  return upsert(s_cache, key, [key] { return std::make_shared<Value>(key); });
}
