#pragma once

#include <vector>
#include <unordered_map>
#include <memory>

namespace ENG {

template<typename T>
using Vec_t = std::vector<T>;

template<typename K, typename V>
using Hash_t = std::unordered_map<K, V>;

template<typename T>
using UPTR_t = std::unique_ptr<T>;

using EntID_t   = std::size_t;
using CmpType_t = std::size_t;

}
