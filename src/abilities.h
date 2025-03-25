#pragma once
#ifndef __ABILITIES_H_
#define __ABILITIES_H_

enum ShipAbilities {
    LaserGun,
    Rocket,

    ShipAbilitiesCount
};

constexpr float weaponCooldowns[static_cast<size_t>(ShipAbilities::ShipAbilitiesCount)][10] {
    //Levels 0     1     2       3      4      5      6      7      8      9
    {   0.25f, 0.25f, 0.20f, 0.20f, 0.20f, 0.10f, 0.10f, 0.10f, 0.10f, 0.05f},  //LaserGun
    {   0.25f, 0.25f, 0.20f, 0.20f, 0.20f, 0.10f, 0.10f, 0.10f, 0.10f, 0.05f}  //Rocket
};

constexpr int weaponProjectileCount[static_cast<size_t>(ShipAbilities::ShipAbilitiesCount)][10] {
    //Levels 0     1     2       3      4      5      6      7      8      9
    {       1,     2,    2,      3,     3,     3,     4,     4,     4,     5},  //LaserGun
    {       1,     2,    2,      3,     3,     3,     4,     4,     4,     5} //Rocket
};

#endif // !__ABILITIES_H_
