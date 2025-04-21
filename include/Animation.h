#pragma once

#include <cstddef>

const float BLINK_INTERVAL = 0.1f;
const float BLINK_DURATION = .5f;

enum class Animation : size_t {
    Damage,

    AnimationCount
};

constexpr size_t AnimationCount = static_cast<size_t>(Animation::AnimationCount);
