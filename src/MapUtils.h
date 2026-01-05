#pragma once

#include <unordered_map>

template <typename K, typename V> V &upsert(std::unordered_map<K, V> &map, const K key, const auto inserter) {
  if (const auto entry = map.find(key); entry != map.end()) {
    return entry->second;
  }

  map.insert({key, inserter()});

  return map[key];
}
