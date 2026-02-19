#pragma once

#include "lru_cache.h"

#include <mutex>
#include <optional>

/**
 * ThreadSafeLRUCache - minimal mutex wrapper around LRUCache.
 * Demonstrates how to provide a safe API boundary for concurrent access.
 */
template <class Key, class Value>
class ThreadSafeLRUCache {
public:
    explicit ThreadSafeLRUCache(std::size_t capacity) : cache_(capacity) {}

    std::optional<Value> get(const Key& key) {
        std::lock_guard<std::mutex> lock(mtx_);
        return cache_.get(key);
    }

    void put(const Key& key, Value value) {
        std::lock_guard<std::mutex> lock(mtx_);
        cache_.put(key, std::move(value));
    }

    bool erase(const Key& key) {
        std::lock_guard<std::mutex> lock(mtx_);
        return cache_.erase(key);
    }

    std::size_t size() const {
        std::lock_guard<std::mutex> lock(mtx_);
        return cache_.size();
    }

private:
    mutable std::mutex mtx_;
    LRUCache<Key, Value> cache_;
};
