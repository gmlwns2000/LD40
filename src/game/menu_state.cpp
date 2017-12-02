#include "menu_state.h"

using namespace ldjam;
ldjam::MenuState ldjam::MenuState::s_instance;

void ldjam::MenuState::Init(EngineRuntime& rt)
{
    rt.textureManager.Load("anime", LD_TEXTURE_DIR "test.png");
    
    testSprite.SetTexture(rt.textureManager["anime"]);
    testSprite.position = glm::ivec2(0, 0);
    testSprite.size = glm::ivec2(50, 50);
}

void ldjam::MenuState::Cleanup(EngineRuntime& rt)
{
}

void ldjam::MenuState::Pause()
{
}

void ldjam::MenuState::Resume()
{

}

void ldjam::MenuState::HandleEvents(GameStateManager *game, EngineRuntime& rt)
{
    if (KeyDown(rt, SDL_SCANCODE_ESCAPE))
        game->Quit();
}

void ldjam::MenuState::Update(GameStateManager *game, EngineRuntime& rt, float dt)
{
    // testSprite.rotation += 20.00 * dt;
    testSprite.position = rt.mousePosition;
}

void ldjam::MenuState::Render(GameStateManager *game, EngineRuntime& rt)
{
    RenderSprite(rt, testSprite);
}
