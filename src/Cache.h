#pragma once

#include <unordered_map>
#include <optional>
#include <concepts>
#include <utility>
#include <stdexcept>

// C++23 Cache Class
template <typename Key, typename Value>
  requires std::movable<Value> && std::equality_comparable<Key> // C++20/23 Concepts
class Cache {
public:
  // 1. Constructors & Destructor
  Cache() = default;
  ~Cache() = default;

  // 2. Disable Copy Semantics (Non-copyable)
  Cache(const Cache &) = delete;
  Cache &operator=(const Cache &) = delete;

  // 3. Enable Move Semantics
  Cache(Cache &&) noexcept = default;
  Cache &operator=(Cache &&) noexcept = default;

  // 4. Query (Get)
  // Returns a pointer to the value if found, nullptr otherwise.
  // Using a pointer avoids copying the value on return.
  [[nodiscard]] const Value *get(const Key &key) const;

  // 5. Set (Update Only)
  // Updates the value only if the key already exists.
  // Returns true if updated, false if key was not found.
  bool set(const Key &key, Value value);

  // 6. Upsert (Insert or Update)
  // Inserts the value if key is new, or overwrites if key exists.
  void upsert(Key key, Value value);

  // Utility: Check size
  [[nodiscard]] size_t size() const noexcept;

  // Utility: Clear cache
  void clear() noexcept;

private:
  std::unordered_map<Key, Value> m_store;
};

template <typename Key, typename Value>
  requires std::movable<Value> && std::equality_comparable<Key>

const Value *Cache<Key, Value>::get(const Key &key) const {
  if (auto it = m_store.find(key); it != m_store.end()) {
    return &it->second;
  }
  return nullptr;
}

template <typename Key, typename Value>
  requires std::movable<Value> && std::equality_comparable<Key>

bool Cache<Key, Value>::set(const Key &key, Value value) {
  if (auto it = m_store.find(key); it != m_store.end()) {
    it->second = std::move(value);
    return true;
  }
  return false;
}

template <typename Key, typename Value>
  requires std::movable<Value> && std::equality_comparable<Key>
void Cache<Key, Value>::upsert(Key key, Value value) {
  m_store.insert_or_assign(std::move(key), std::move(value));
}

template <typename Key, typename Value>
  requires std::movable<Value> && std::equality_comparable<Key>

size_t Cache<Key, Value>::size() const noexcept {
  return m_store.size();
}

template <typename Key, typename Value>
  requires std::movable<Value> && std::equality_comparable<Key>
void Cache<Key, Value>::clear() noexcept {
  m_store.clear();
}
