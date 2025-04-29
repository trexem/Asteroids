#pragma once

#include <algorithm>
#include <array>
#include <cstddef>
#include <random>
#include <vector>

#include "utils.hpp"

enum class WeaponAbilities : size_t {
    LaserGun,
    Rocket,
    Laser,
    Explosives,
    GravitySaws,

    WeaponAbilitiesCount
};

constexpr float abilitiesCooldowns[static_cast<size_t>(WeaponAbilities::WeaponAbilitiesCount)][10] {
    //Levels 0     1     2       3      4      5      6      7      8      9
    {   0.50f, 0.50f, 0.50f, 0.50f, 0.50f, 0.45f, 0.45f, 0.45f, 0.45f, 0.35f},  //LaserGun
    {   1.50f, 1.50f, 1.40f, 1.40f, 1.40f, 1.30f, 1.30f, 1.20f, 1.20f, 1.00f},  //Rocket
    {   3.25f, 3.25f, 3.20f, 3.20f, 3.20f, 3.10f, 3.10f, 3.10f, 3.10f, 3.0f},  //Laser
    {   1.25f, 1.25f, 1.20f, 1.20f, 1.20f, 1.10f, 1.10f, 1.10f, 1.10f, 1.00f},  //Explosives
    {   3.25f, 3.25f, 3.20f, 3.20f, 3.20f, 3.10f, 3.10f, 3.10f, 3.10f, 3.05f},  //GravitySaws
};

constexpr int abilitiesProjectileCount[static_cast<size_t>(WeaponAbilities::WeaponAbilitiesCount)][10] {
    //Levels 0     1     2       3      4      5      6      7      8      9
    {       1,     2,    2,      3,     3,     3,     4,     4,     4,     5},  //LaserGun
    {       1,     2,    2,      3,     3,     3,     4,     4,     4,     5},  //Rocket
    {       1,     1,    1,      1,     1,     1,     1,     1,     1,     1},  //Laser
    {       1,     2,    2,      3,     3,     3,     4,     4,     4,     5},  //Explosives
    {       1,     2,    2,      3,     3,     3,     4,     4,     4,     5},  //GravitySaws
};

constexpr float abilitiesProjectileSpeed[static_cast<size_t>(WeaponAbilities::WeaponAbilitiesCount)][10] {
    //Levels 0     1     2       3      4      5      6      7      8      9
    {     1000, 1000, 1000,   1000,  1000,  1000,  1000,  1000,  1000,  1000},  //LaserGun
    {      100,  100,  200,    200,   400,   400,   600,   600,   800,  1000},  //Rocket
    {        0,    0,    0,      0,     0,     0,     0,     0,     0,     0},  //Laser
    {        0,    0,    0,      0,     0,     0,     0,     0,     0,     0},  //Explosives
    {      TAU,  TAU,  TAU,    TAU,   TAU,   TAU,   TAU,   TAU,   TAU,   TAU},  //GravitySaw
};

constexpr float abilitiesDamage[static_cast<size_t>(WeaponAbilities::WeaponAbilitiesCount)][10] {
    //Levels 0     1     2       3      4      5      6      7      8      9
    {       10,   10,   15,     15,    20,    20,    20,    25,    30,    40},  //LaserGun
    {       25,   25,   25,  36.5f, 36.5f, 36.5f,    50,    50, 62.5f,    75},  //Rocket
    {       10,   10,   15,     15,    20,    20,    20,    25,    30,    40},  //Laser
    {       25,   25,   25,  36.5f, 36.5f, 36.5f,    50,    50, 62.5f,    75},  //Explosives
    {       10,   10,   15,     15,    20,    20,    20,    25,    30,    40},
};

constexpr float abilitiesSize[static_cast<size_t>(WeaponAbilities::WeaponAbilitiesCount)][10]{
    //Levels 0     1     2       3      4      5      6      7      8      9
    {        1,    1,    1,      1,     1,     1,     1,     1,     1,     1},  //LaserGun
    {        1,    1,    1,      1,     1,     1,     1,     1,     1,     1},  //Rocket
    {        1,1.05f,1.05f,   1.1f,  1.1f, 1.15f, 1.15f, 1.15f,  1.2f, 1.25f},
    {        1,    1,    1,      1,     1,     1,     1,     1,     1,     1},
    {        1,    1,    1,      1,     1,     1,     1,     1,     1,     1},
};

constexpr double abilitiesLifeTime[static_cast<size_t>(WeaponAbilities::WeaponAbilitiesCount)][10]{
    //Levels 0     1     2       3      4      5      6      7      8      9
    {        0,    0,    0,      0,     0,     0,     0,     0,     0,     0},  //LaserGun
    {     2.0f, 2.0f, 2.0f,   2.0f,  2.0f,  2.0f,  2.0f,  2.0f,  2.0f,  2.0f},  //Rocket
    {     1.0f, 1.0f, 1.0f,   1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f},
    {     2.0f, 2.0f, 2.0f,   2.5f,  2.5f,  2.5f,  2.5f,  2.5f,  2.5f,  3.0f},
    {     1.0f, 1.0f, 1.2f,   1.2f,  1.4f,  1.4f,  1.7f,  1.7f,  1.7f,  2.0f},  //GravitySaw
};

constexpr std::array<const char*, static_cast<size_t>(WeaponAbilities::WeaponAbilitiesCount)> weaponIconLocation = {
    "data/img/laserGunIcon.bmp",
    "data/img/rocketIcon.bmp",
    "data/img/laserIcon.bmp",
    "data/img/explosivesIcon.bmp",
    "data/img/gravitySawIcon.bmp",
    
};

constexpr uint8_t maxWeaponLevel[static_cast<size_t>(WeaponAbilities::WeaponAbilitiesCount)] = {
    9,  // LaserGun
    9,  // Rocket
    9,  // Laser
    9,  // Explosives
    9   // GravitySaws
};

constexpr std::array<bool, static_cast<size_t>(WeaponAbilities::WeaponAbilitiesCount)> abilityBursts = {
    false, false, false, true, false
};
