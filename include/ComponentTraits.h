#pragma once

#include "Components.h"

template <typename T>
struct ComponentTraits {
    static constexpr ComponentType type = ComponentType::Count;
};

template <> struct ComponentTraits<TransformComponent> {
    static constexpr ComponentType type = ComponentType::Transform;
};

template <> struct ComponentTraits<PhysicsComponent> {
    static constexpr ComponentType type = ComponentType::Physics;
};

template <> struct ComponentTraits<CollisionComponent> {
    static constexpr ComponentType type = ComponentType::Collision;
};

template <> struct ComponentTraits<RenderComponent> {
    static constexpr ComponentType type = ComponentType::Render;
};

template <> struct ComponentTraits<HealthComponent> {
    static constexpr ComponentType type = ComponentType::Health;
};

template <> struct ComponentTraits<DamageComponent> {
    static constexpr ComponentType type = ComponentType::Damage;
};

template <> struct ComponentTraits<PlayerComponent> {
    static constexpr ComponentType type = ComponentType::Player;
};

template <> struct ComponentTraits<StatsComponent> {
    static constexpr ComponentType type = ComponentType::Stats;
};

template <> struct ComponentTraits<MovementComponent> {
    static constexpr ComponentType type = ComponentType::Movement;
};

template <> struct ComponentTraits<TypeComponent> {
    static constexpr ComponentType type = ComponentType::Type;
};

template <> struct ComponentTraits<AnimationComponent> {
    static constexpr ComponentType type = ComponentType::Animation;
};

template <> struct ComponentTraits<GUIComponent> {
    static constexpr ComponentType type = ComponentType::GUI;
};

template <> struct ComponentTraits<PickupComponent> {
    static constexpr ComponentType type = ComponentType::Pickup;
};

template <> struct ComponentTraits<LifeTimeComponent> {
    static constexpr ComponentType type = ComponentType::LifeTime;
};

template <> struct ComponentTraits<OrbitComponent> {
    static constexpr ComponentType type = ComponentType::Orbit;
};

template <> struct ComponentTraits<FollowComponent> {
    static constexpr ComponentType type = ComponentType::Follow;
};

template <> struct ComponentTraits<BackgroundComponent> {
    static constexpr ComponentType type = ComponentType::Background;
};

template <> struct ComponentTraits<LabelComponent> {
    static constexpr ComponentType type = ComponentType::Label;
};

template <> struct ComponentTraits<ButtonComponent> {
    static constexpr ComponentType type = ComponentType::Button;
};

template <> struct ComponentTraits<SliderComponent> {
    static constexpr ComponentType type = ComponentType::Slider;
};

template <> struct ComponentTraits<ContainerComponent> {
    static constexpr ComponentType type = ComponentType::Container;
};

template <> struct ComponentTraits<GUIStateComponent> {
    static constexpr ComponentType type = ComponentType::GUIState;
};

template <> struct ComponentTraits<ClickCallbackComponent> {
    static constexpr ComponentType type = ComponentType::ClickCallback;
};

template <> struct ComponentTraits<TooltipComponent> {
    static constexpr ComponentType type = ComponentType::Tooltip;
};

template <> struct ComponentTraits<NineGridComponent> {
    static constexpr ComponentType type = ComponentType::NineGrid;
};

template <> struct ComponentTraits<TouchJoystickComponent> {
    static constexpr ComponentType type = ComponentType::TouchJoystick;
};

template <> struct ComponentTraits<ProximityTrackerComponent> {
    static constexpr ComponentType type = ComponentType::ProximityTracker;
};

template <> struct ComponentTraits<LightningComponent> {
    static constexpr ComponentType type = ComponentType::Lightning;
};
