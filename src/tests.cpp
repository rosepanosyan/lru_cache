#include "lru_cache.h"

#include <cassert>
#include <iostream>
#include <string>

static void test_basic_eviction() {
    LRUCache<std::string, int> c(2);
    c.put("a", 1);
    c.put("b", 2);
    assert(c.size() == 2);

    auto va = c.get("a");
    assert(va && *va == 1); // a becomes MRU, b becomes LRU

    c.put("c", 3); // should evict b
    assert(!c.contains("b"));
    assert(c.contains("a"));
    assert(c.contains("c"));
}

static void test_update_moves_to_front() {
    LRUCache<std::string, int> c(2);
    c.put("a", 1);
    c.put("b", 2);

    c.put("a", 10); // update + MRU
    auto va = c.get("a");
    assert(va && *va == 10);

    c.put("c", 3); // evict LRU -> should evict b
    assert(!c.contains("b"));
}

int main() {
    test_basic_eviction();
    test_update_moves_to_front();

    std::cout << "All tests passed.\n";
    return 0;
}
