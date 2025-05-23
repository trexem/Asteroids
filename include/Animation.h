#pragma once

#include <cstddef>
#include <vector>

class Texture;

const float BLINK_INTERVAL = 0.1f;
const float BLINK_DURATION = .2f;

enum class Animation : size_t {
    Idle,
    Damage,

    None
};

struct AnimationState {
    std::vector<Texture*> frames;
    float frameDuration = 0.1f; // duration per frame
    bool looping = false;
};


constexpr size_t AnimationCount = static_cast<size_t>(Animation::None);
