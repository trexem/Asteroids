#include "Button.h"
#include "MetaStats.hpp"

class UpgradeButton : public Button {
public:
    UpgradeButton(EntityManager* em, const std::string& label, FPair pos, FPair size,
        Texture* texture, SDL_Renderer* renderer, UpgradeType type, uint32_t parent = 0);
    std::string costText;
    Texture costTexture;
    uint32_t costId;
    int level, lastlevel, maxLevel;
    UpgradeType type;

    void destroy(EntityManager* em);
    void updateCost(EntityManager* em);
};
