#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <random>
#include <vector>

enum class ShipAbilities : size_t{
    LaserGun,
    Rocket,
    Laser,
    Explosives,
    GravitySaws,
    PickupRadius,

    ShipAbilitiesCount
};

constexpr float abilitiesCooldowns[static_cast<size_t>(ShipAbilities::ShipAbilitiesCount)][10] {
    //Levels 0     1     2       3      4      5      6      7      8      9
    {   0.50f, 0.50f, 0.50f, 0.50f, 0.50f, 0.45f, 0.45f, 0.45f, 0.45f, 0.35f},  //LaserGun
    {   1.25f, 1.25f, 1.20f, 1.20f, 1.20f, 1.10f, 1.10f, 1.10f, 1.10f, 1.00f},  //Rocket
    {   0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f, 0.25f},  //Laser
    {   1.25f, 1.25f, 1.20f, 1.20f, 1.20f, 1.10f, 1.10f, 1.10f, 1.10f, 1.00f},  //Explosives
    {   0.25f, 0.25f, 0.20f, 0.20f, 0.20f, 0.10f, 0.10f, 0.10f, 0.10f, 0.05f},  //GravitySaws
    {   0.25f, 0.25f, 0.20f, 0.20f, 0.20f, 0.10f, 0.10f, 0.10f, 0.10f, 0.05f}   //PickupRadius
};

constexpr int abilitiesProjectileCount[static_cast<size_t>(ShipAbilities::ShipAbilitiesCount)][10] {
    //Levels 0     1     2       3      4      5      6      7      8      9
    {       1,     2,    2,      3,     3,     3,     4,     4,     4,     5},  //LaserGun
    {       1,     2,    2,      3,     3,     3,     4,     4,     4,     5},  //Rocket
    {       1,     2,    2,      3,     3,     3,     4,     4,     4,     5},  //Laser
    {       1,     2,    2,      3,     3,     3,     4,     4,     4,     5},  //Explosives
    {       1,     2,    2,      3,     3,     3,     4,     4,     4,     5},  //GravitySaws
    {       1,     2,    2,      3,     3,     3,     4,     4,     4,     5}   //PickupRadius
};

constexpr float abilitiesProjectileSpeed[static_cast<size_t>(ShipAbilities::ShipAbilitiesCount)][10] {
    //Levels 0     1     2       3      4      5      6      7      8      9
    {     1000, 1000, 1000,   1000,  1000,  1000,  1000,  1000,  1000,  1000},  //LaserGun
    {     1000, 1000, 1000,   1000,  1000,  1000,  1000,  1000,  1000,  1000},  //Rocket
    {     1000, 1000, 1000,   1000,  1000,  1000,  1000,  1000,  1000,  1000},
    {     1000, 1000, 1000,   1000,  1000,  1000,  1000,  1000,  1000,  1000},
    {     1000, 1000, 1000,   1000,  1000,  1000,  1000,  1000,  1000,  1000},
    {     1000, 1000, 1000,   1000,  1000,  1000,  1000,  1000,  1000,  1000}
};

constexpr float abilitiesDamage[static_cast<size_t>(ShipAbilities::ShipAbilitiesCount)][10] {
    //Levels 0     1     2       3      4      5      6      7      8      9
    {       10,   10,   15,     15,    20,    20,    20,    25,    30,    40},  //LaserGun
    {       10,   10,   15,     15,    20,    20,    20,    25,    30,    40},  //Rocket
    {       10,   10,   15,     15,    20,    20,    20,    25,    30,    40},
    {       10,   10,   15,     15,    20,    20,    20,    25,    30,    40},
    {       10,   10,   15,     15,    20,    20,    20,    25,    30,    40},
    {       10,   10,   15,     15,    20,    20,    20,    25,    30,    40}
};

constexpr float abilitiesSize[static_cast<size_t>(ShipAbilities::ShipAbilitiesCount)][10]{
    //Levels 0     1     2       3      4      5      6      7      8      9
    {        1,    1,    1,      1,     1,     1,     1,     1,     1,     1},  //LaserGun
    {        1,    1,    1,      1,     1,     1,     1,     1,     1,     1},  //Rocket
    {        1,    1,    1,      1,     1,     1,     1,     1,     1,     1},
    {        1,    1,    1,      1,     1,     1,     1,     1,     1,     1},
    {        1,    1,    1,      1,     1,     1,     1,     1,     1,     1},
    {      200,  250,  300,    350,   400,   500,   600,   700,   800,   1000}   //PickUpRadius
};

constexpr std::array<const char*, static_cast<size_t>(ShipAbilities::ShipAbilitiesCount)> abilityIconLocation = {
    "data/img/laserGunIcon.bmp",
    "data/img/rocketIcon.bmp",
    "data/img/laserIcon.bmp",
    "data/img/explosivesIcon.bmp",
    "data/img/gravitySawIcon.bmp",
    "data/img/pickupRangeIcon.bmp"
};
