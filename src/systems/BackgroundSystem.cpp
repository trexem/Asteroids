#include "BackgroundSystem.h"

BackgroundSystem::BackgroundSystem(EntityManager& eManager, SDL_Renderer* renderer) {
    starTexture.m_renderer = renderer;
    starTexture.loadFromFile("data/img/star1.bmp");
    createBackground(eManager);
}

void BackgroundSystem::update(EntityManager& eManager) {
    uint32_t player = eManager.getEntitiesWithComponent(ComponentType::Player)[0];
    TransformComponent playerTr = eManager.getComponentData<TransformComponent>(player);

    FPair delta = playerTr.position - lastPlayerPos;
    lastPlayerPos = playerTr.position;

    constexpr float MIN_POS = -5000.0f;
    constexpr float MAX_POS =  5000.0f;
    constexpr float RANGE = MAX_POS - MIN_POS;

    for (uint32_t e : eManager.getEntitiesWithComponent(ComponentType::Background)) {
        TransformComponent tComp = eManager.getComponentData<TransformComponent>(e);
        BackgroundComponent bComp = eManager.getComponentData<BackgroundComponent>(e);
        
        tComp.position -= delta * bComp.parallaxFactor;

        if (tComp.position.x < lastPlayerPos.x + MIN_POS) tComp.position.x += RANGE;
        if (tComp.position.x > lastPlayerPos.x + MAX_POS) tComp.position.x -= RANGE;
        if (tComp.position.y < lastPlayerPos.y + MIN_POS) tComp.position.y += RANGE;
        if (tComp.position.y > lastPlayerPos.y + MAX_POS) tComp.position.y -= RANGE;

        eManager.setComponentData(e, tComp);
    }
}

void BackgroundSystem::createBackground(EntityManager& eManager) {
    constexpr float MAX_DISTANCE = 5000.0f;
    for (int i = 0; i < 200; ++i) {
        FPair pos = {
            randFloat(-MAX_DISTANCE, MAX_DISTANCE),
            randFloat(-MAX_DISTANCE, MAX_DISTANCE)
        };
        float parallax = randFloat(0.01f, .1f);
        uint32_t e = eManager.createEntity();
        EntityHandle eHandle {e, &eManager};
        eHandle.add<TransformComponent>();
        eHandle.add<RenderComponent>();
        eHandle.add<BackgroundComponent>();
        TypeComponent type = EntityType::Background;
        TransformComponent tComp;
        tComp.position = pos;
        BackgroundComponent bComp = parallax;
        RenderComponent rComp;
        rComp.texture = &starTexture;
        eHandle.set(type);
        eHandle.set(tComp);
        eHandle.set(bComp);
        eHandle.set(rComp);
    }
}
