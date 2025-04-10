#include "Screen.h"

Screen::Screen(EntityManager* eM) : eManager(eM) {}

void Screen::initSubscriptions() {
    auto weak_this = weak_from_this();
    // Subscribe to MouseMotionMessages
    unsubscribers.emplace_back(
        MessageManager::getInstance().subscribe<MouseMotionMessage>(
            [weak_this](auto msg) {
                if (auto shared_this = weak_this.lock()) {
                    shared_this->handleMouseHover(msg);
                }
            }
        )
    );
    // Subscribe to ClickMessages
    unsubscribers.emplace_back(
        MessageManager::getInstance().subscribe<ClickMessage>(
            [weak_this](auto msg) {
                if (auto shared_this = weak_this.lock()) {
                    shared_this->handleMouseClick(msg);
                }
            }
        )
    );
}

void Screen::clearSubscriptions() {
    std::cout << "Clearing " << unsubscribers.size() << " subscriptions\n";
    for (auto& unsub : unsubscribers) {
        unsub();
    }
    unsubscribers.clear();
}

Screen::~Screen() {
    std::cout << "Screen destroyed\n";
}

void Screen::handleHover(uint32_t eID, FPair pos, std::function<void()> callback) {
    CollisionComponent colComp = eManager->getComponentData<CollisionComponent>(eID);
    TransformComponent trComp = eManager->getComponentData<TransformComponent>(eID);
    RenderComponent render = eManager->getComponentData<RenderComponent>(eID);

    bool isHovered = (pos.x > colComp.position.x && pos.x < colComp.position.x + colComp.width &&
        pos.y > colComp.position.y && pos.y < colComp.position.y + colComp.height
    );
    auto search = hoveredEntities.find(eID);
    bool contained = search != hoveredEntities.end();
    if (isHovered) {
        if (!contained) {
            trComp.position = colComp.position;
            render.size = colComp.width / render.texture->getWidth();

            eManager->setComponentData<TransformComponent>(eID, trComp);
            eManager->setComponentData<RenderComponent>(eID, render);
            
            hoveredEntities.insert(eID);
        }
        if (callback) {
            callback();
        }
    } else {
        if (contained) {
            trComp.position += hoveredOffset;
            render.size = 1.0f;

            eManager->setComponentData<TransformComponent>(eID, trComp);
            eManager->setComponentData<RenderComponent>(eID, render);
            hoveredEntities.erase(eID);
        }
    }
}

void Screen::handleClick(uint32_t eID, FPair pos, std::function<void()> callback) {
    CollisionComponent colComp = eManager->getComponentData<CollisionComponent>(eID);
    if (pos.x > colComp.position.x && pos.x < colComp.position.x + colComp.width &&
        pos.y > colComp.position.y && pos.y < colComp.position.y + colComp.height
    ) {
        if (callback) {
            callback();
        }
    }
}

