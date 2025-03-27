#pragma once
#ifndef __ABILITIES_H_
#define __ABILITIES_H_

#include <cstddef>

enum class ShipAbilities : size_t{
    LaserGun,
    Rocket,

    ShipAbilitiesCount
};

constexpr float abilitiesCooldowns[static_cast<size_t>(ShipAbilities::ShipAbilitiesCount)][10] {
    //Levels 0     1     2       3      4      5      6      7      8      9
    {   0.25f, 0.25f, 0.20f, 0.20f, 0.20f, 0.10f, 0.10f, 0.10f, 0.10f, 0.05f},  //LaserGun
    {   0.25f, 0.25f, 0.20f, 0.20f, 0.20f, 0.10f, 0.10f, 0.10f, 0.10f, 0.05f}  //Rocket
};

constexpr int abilitiesProjectileCount[static_cast<size_t>(ShipAbilities::ShipAbilitiesCount)][10] {
    //Levels 0     1     2       3      4      5      6      7      8      9
    {       1,     2,    2,      3,     3,     3,     4,     4,     4,     5},  //LaserGun
    {       1,     2,    2,      3,     3,     3,     4,     4,     4,     5} //Rocket
};

constexpr float abilitiesProjectileSpeed[static_cast<size_t>(ShipAbilities::ShipAbilitiesCount)][10] {
    //Levels 0     1     2       3      4      5      6      7      8      9
    {     1000, 1000, 1000,   1000,  1000,  1000,  1000,  1000,  1000,  1000},  //LaserGun
    {   0.25f, 0.25f, 0.20f, 0.20f, 0.20f, 0.10f, 0.10f, 0.10f, 0.10f, 0.05f}  //Rocket
};

constexpr float abilitiesDamage[static_cast<size_t>(ShipAbilities::ShipAbilitiesCount)][10] {
    //Levels 0     1     2       3      4      5      6      7      8      9
    {       10,   10,   15,     15,    20,    20,    20,    25,    30,    40},  //LaserGun
    {   0.25f, 0.25f, 0.20f, 0.20f, 0.20f, 0.10f, 0.10f, 0.10f, 0.10f, 0.05f}  //Rocket
};

#endif // !__ABILITIES_H_
