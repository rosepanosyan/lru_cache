#pragma once

#include <cstddef>
#include <list>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

/**
 * LRUCache<Key, Value>
 *
 * - O(1) average get/put/erase
 * - Implements LRU eviction policy (Least Recently Used)
 *
 * Internals:
 * - std::list keeps items ordered by recency (front = most recent)
 * - std::unordered_map maps Key -> list iterator
 */
template <class Key, class Value>
class LRUCache {
public:
    using key_type = Key;
    using mapped_type = Value;
    using value_type = std::pair<Key, Value>;

    explicit LRUCache(std::size_t capacity) : capacity_(capacity) {
        if (capacity_ == 0) {
            throw std::invalid_argument("LRUCache capacity must be > 0");
        }
    }

    std::size_t capacity() const noexcept { return capacity_; }
    std::size_t size() const noexcept { return index_.size(); }
    bool empty() const noexcept { return index_.empty(); }

    bool contains(const Key& key) const { return index_.find(key) != index_.end(); }

    /**
     * Get a value. If present:
     * - moves the key to MRU (front)
     * - returns a copy in std::optional
     */
    std::optional<Value> get(const Key& key) {
        auto it = index_.find(key);
        if (it == index_.end()) return std::nullopt;

        // move to front (MRU)
        items_.splice(items_.begin(), items_, it->second);
        return it->second->second;
    }

    /**
     * Put a key/value pair. If key exists: update + mark MRU.
     * If inserting would exceed capacity: evict LRU (back).
     */
    void put(const Key& key, Value value) {
        auto it = index_.find(key);
        if (it != index_.end()) {
            // update
            it->second->second = std::move(value);
            items_.splice(items_.begin(), items_, it->second);
            return;
        }

        // insert new (MRU at front)
        items_.emplace_front(key, std::move(value));
        index_[items_.front().first] = items_.begin();

        // evict if needed
        if (index_.size() > capacity_) {
            auto& node = items_.back();
            index_.erase(node.first);
            items_.pop_back();
        }
    }

    bool erase(const Key& key) {
        auto it = index_.find(key);
        if (it == index_.end()) return false;
        items_.erase(it->second);
        index_.erase(it);
        return true;
    }

    void clear() {
        index_.clear();
        items_.clear();
    }

    /**
     * Debug helper: list keys from MRU -> LRU.
     */
    std::vector<Key> keys_mru_to_lru() const {
        std::vector<Key> out;
        out.reserve(items_.size());
        for (const auto& kv : items_) out.push_back(kv.first);
        return out;
    }

private:
    std::size_t capacity_;
    std::list<value_type> items_;
    std::unordered_map<Key, typename std::list<value_type>::iterator> index_;
};
