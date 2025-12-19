#pragma once
#include "Engine/Math/Public/Math.h"
#include <memory>
#include <vector>

template<typename T>
using OcPtr = std::shared_ptr<T>;

template<typename T>
using OcWeakPtr = std::weak_ptr<T>;

template<typename T1, typename T2>
using OcPair = std::pair<T1, T2>;

template<typename T, typename... Args>
OcPtr<T> OcMakeShared(Args&&... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

template<typename T>
using OcArray = std::vector<T>;
