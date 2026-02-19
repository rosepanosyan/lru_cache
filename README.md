# lru_cache

A **header-only** templated **LRU cache** implementation in modern C++17.

## What it demonstrates
- STL-based O(1) design: `std::unordered_map + std::list`
- Clean template API with `std::optional`
- Small tests without external dependencies

## Build
```bash
mkdir -p build && cd build
cmake ..
cmake --build . -j
```

## Run demo
```bash
./lru_cache_demo --capacity 3
```

Example session:
```text
> put a 1
> put b 2
> get a
1
> dump
MRU -> LRU: a b
```

## Run tests
```bash
./lru_cache_tests
```
