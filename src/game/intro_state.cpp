#include "intro_state.h"

using namespace ldjam;
ldjam::IntroState ldjam::IntroState::s_instance;

void ldjam::IntroState::Init(EngineRuntime& rt)
{
    rt.textureManager.Load("anime", LD_TEXTURE_DIR "test.png");
    
    testSprite.texture = rt.textureManager["anime"];
    testSprite.position = glm::ivec2(10, 10);
    testSprite.size = glm::ivec2(100, 100);
}

void ldjam::IntroState::Cleanup(EngineRuntime& rt)
{

}

void ldjam::IntroState::Pause()
{
}

void ldjam::IntroState::Resume()
{

}

void ldjam::IntroState::HandleEvents(GameStateManager *game, EngineRuntime& rt)
{
    if (KeyDown(rt, SDL_SCANCODE_ESCAPE))
        game->Quit();
}

void ldjam::IntroState::Update(GameStateManager *game, EngineRuntime& rt, float dt)
{

}

void ldjam::IntroState::Render(GameStateManager *game, EngineRuntime& rt)
{

}
