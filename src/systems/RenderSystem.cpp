#include "RenderSystem.h"
#include "SettingsManager.h"
#include "GUI.h"
#include "Fonts.h"
#include "TextureManager.h"

RenderSystem::RenderSystem(SDL_Window* window, const char * name, Camera* camera)
    : renderer(std::make_shared<Renderer>(window, name)), camera(camera) {
    renderer->changeColor(0x00, 0x00, 0x00, 0xFF);
    screenTexture = SDL_CreateTexture(renderer->getRenderer(), SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET, 1920, 1080);
    if (!screenTexture) {
        std::cerr << "Failed to create screen texture: " << SDL_GetError() << std::endl;
    }
}

SDL_Renderer* RenderSystem::getRenderer() {
	return renderer->getRenderer();
}

void RenderSystem::render(EntityManager& eMgr) {
    renderer->clear();
    SDL_SetRenderTarget(renderer->getRenderer(), screenTexture);
    SDL_SetRenderDrawColor(renderer->getRenderer(), 0, 0, 0, 255);
    renderer->clear();
    drawGameEntities(eMgr);
    drawGUI(eMgr);
    renderer->render();
    SDL_SetRenderTarget(renderer->getRenderer(), nullptr);
    FPair screenSize = SettingsManager::instance().currentScreenSize;
    FPair res = SettingsManager::instance().currentResolution->second;
    FPair pos = SettingsManager::instance().screenPos;
    
    //SDL_Log("Rendering to resolution: %fx%f", res.x, res.y);
    //SDL_Log("Rendering to screen pos: %fx%f", pos.x, pos.y);
    //SDL_Log("Scren size: %fx%f", screenSize.x, screenSize.y);
    SDL_FRect screenRect = {pos.x, pos.y, res.x, res.y};
    SDL_RenderTexture(renderer->getRenderer(), screenTexture, nullptr, &screenRect);
    renderer->render();
    frame++;
    // saveRendererToImage();
}


//Used for debugging purposes but might be useful in the future to save actual screenshots (?)
void RenderSystem::saveRendererToImage() {
    int width, height;
    SDL_GetCurrentRenderOutputSize(renderer->getRenderer(), &width, &height);
    SDL_Surface* surface = SDL_RenderReadPixels(renderer->getRenderer(), NULL);
    // Save the surface to a BMP file (you can use SDL_SaveBMP or SDL_Image for PNG, JPG, etc.)
    if (!SDL_SaveBMP(surface, "screenshot.bmp")) {
        std::cerr << "SDL_SaveBMP failed: " << SDL_GetError() << std::endl;
    } else {
        std::cout << "Saved screenshot as screenshot.bmp" << std::endl;
    }

    // Clean up the surface
    SDL_DestroySurface(surface);
}

void RenderSystem::drawGameEntities(EntityManager& eM) {
    const float margin = 64.0f;
    const SDL_FRect cameraView = {
        camera->position.x - margin,
        camera->position.y - margin,
        GUI::screenWidth + 2 * margin,
        GUI::screenHeight + 2 * margin
    };
    for (uint32_t eID : eM.getEntitiesWithComponent(ComponentType::Render)) {
        if (eM.isMarkedForDestruction(eID)) continue;
        if (! eM.hasComponent<GUIComponent>(eID)) {
            RenderComponent rComp = eM.getComponentData<RenderComponent>(eID);
            TransformComponent trComp = eM.getComponentData<TransformComponent>(eID);
            // CollisionComponent* cComp = eM.getComponentDataPtr<CollisionComponent>(eID);
            FPair position;
            position.x = trComp.position.x - camera->position.x;
            position.y = trComp.position.y - camera->position.y;
            const int w = rComp.isStretched ? rComp.exactSize.x : rComp.texture->getWidth();
            const int h = rComp.isStretched ? rComp.exactSize.y : rComp.texture->getHeight();

            // Check if the entity is outside the camera view (with margin)
            if (trComp.position.x + w < cameraView.x || 
                trComp.position.x > cameraView.x + cameraView.w ||
                trComp.position.y + h < cameraView.y || 
                trComp.position.y > cameraView.y + cameraView.h) {
                continue; // Skip rendering
            }
            
            SDL_FPoint pivot = getPivotFromRotationPoint(
                rComp.texture->rotationPoint, w, h);
            rComp.texture->setAlphaMod(rComp.visibility);
            if (rComp.isStretched) {
                rComp.texture->renderEx(
                    static_cast<int>(position.x),
                    static_cast<int>(position.y),
                    nullptr,
                    static_cast<int>(trComp.rotDegrees),
                    &pivot,
                    SDL_FLIP_NONE,
                    rComp.exactSize,
                    rComp.color
                );
            } else {
                rComp.texture->renderEx(
                    static_cast<int>(position.x),
                    static_cast<int>(position.y),
                    nullptr,
                    static_cast<int>(trComp.rotDegrees),
                    &pivot,
                    SDL_FLIP_NONE,
                    rComp.size,
                    rComp.color
                );
            }
            // Debug oriented bounding box (no circles though)
            // if (cComp.shape == Shape::Rectangle) {
            //     TransformComponent a = trComp;
            //     a.position = position;
            //     auto corners = getCorners(a, cComp);
            //     renderer->drawDebugLine(corners[3].x, corners[3].y, corners[0].x, corners[0].y, Colors::Experience);
            //     for (int i = 0; i < 3; i++) {
            //         renderer->drawDebugLine(corners[i].x, corners[i].y, corners[i+1].x, corners[i+1].y, Colors::Experience);
            //     }
            // } else if (cComp.shape == Shape::Circle) {
            //     TransformComponent a = trComp;
            //     FPair circleCenter = {
            //         a.position.x + cComp.position.x + cComp.radius - camera->position.x,
            //         a.position.y + cComp.position.y + cComp.radius - camera->position.y
            //     };
            //     renderer->drawDebugCircle(circleCenter.x, circleCenter.y, cComp.radius, Colors::Experience);
            // }
        }
    }
}

void RenderSystem::drawGUI(EntityManager& eM) {
    for (uint32_t eID : eM.getEntitiesWithComponent(ComponentType::GUI)) {
        if (eM.isMarkedForDestruction(eID)) continue;
        RenderComponent* rComp = eM.getComponentDataPtr<RenderComponent>(eID);
        if (rComp == nullptr) continue;
        FPair pos {0.0f};
        double rot = 0;
        if (! eM.hasComponent<GUIComponent>(eID)) {
            TransformComponent trComp = eM.getComponentData<TransformComponent>(eID);
            pos = trComp.position;
            rot = trComp.rotDegrees;
        } else {
            GUIComponent* guiComp = eM.getComponentDataPtr<GUIComponent>(eID);
            if (guiComp->parent != 0) {
                auto parentGui = eM.getComponentDataPtr<GUIComponent>(guiComp->parent);
                pos = parentGui->pos + guiComp->pos;
            } else {
                pos = guiComp->pos;
            }
        }
        rComp->texture->setAlphaMod(rComp->visibility);
        if (eM.hasComponent<NineGridComponent>(eID)) {
            const NineGridComponent& grid = eM.getComponentData<NineGridComponent>(eID);
            SDL_FRect dst = { pos.x, pos.y, rComp->exactSize.x, rComp->exactSize.y };
            rComp->texture->render9Grid(
                nullptr,                // full texture
                grid.left, grid.right,
                grid.top, grid.bottom,
                0.0f,                   // no scaling of corners
                &dst
            );
        } else {
            rComp->texture->renderEx(
                static_cast<int>(pos.x),
                static_cast<int>(pos.y),
                nullptr,
                static_cast<int>(rot),
                nullptr,
                SDL_FLIP_NONE,
                rComp->size,
                rComp->color
            );
        }
    }
}
