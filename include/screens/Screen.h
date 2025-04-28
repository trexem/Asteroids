#pragma once

#include "ClickMessage.h"
#include "EntityManager.h"
#include "EntityHandle.h"
#include "GameStateManager.h"
#include "MessageManager.h"
#include "MouseMotionMessage.h"

#include <cstdint>
#include <functional>
#include <memory>
#include <SDL3/SDL.h>
#include <unordered_set>

class Screen : public std::enable_shared_from_this<Screen> {
    std::unordered_set<uint32_t> hoveredEntities;
    std::vector<std::function<void()>> unsubscribers;
protected:
    EntityManager* eManager;
public:
    Screen(EntityManager* eM);
    ~Screen();
    void initSubscriptions();
    void clearSubscriptions();
    virtual void create(EntityManager* eManager, SDL_Renderer* renderer) = 0;
    virtual void destroy(EntityManager* eManager) = 0;
    virtual void handleMouseHover(std::shared_ptr<MouseMotionMessage> msg) = 0;
    virtual void handleMouseClick(std::shared_ptr<ClickMessage> msg) = 0;
    virtual void update(EntityManager* eManager, SDL_Renderer* renderer) = 0;
    void handleHover(uint32_t eID, FPair pos, std::function<void()> callback);
    void handleClick(uint32_t eID, FPair pos, std::function<void()> callback);

    float hoveredOffset = 5.0f;
};
