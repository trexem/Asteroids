#pragma once
#ifndef __ANIMATION_H_
#define __ANIMATION_H_

#include <cstddef>

const float BLINK_INTERVAL = 0.1f;
const float BLINK_DURATION = .5f;

enum class Animation : size_t {
    Damage,

    AnimationCount
};

constexpr size_t AnimationCount = static_cast<size_t>(Animation::AnimationCount);

#endif // !__ANIMATION_H_
