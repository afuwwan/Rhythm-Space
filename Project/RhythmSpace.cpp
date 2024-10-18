#include "RhythmSpace.h"

Engine::RhythmSpace::RhythmSpace(Setting* setting) :Engine::Game(setting)
{
	setting->windowTitle = "RhythmSpace";
}

Engine::RhythmSpace::~RhythmSpace()
{
}

void Engine::RhythmSpace::Init()
{
	Engine::ScreenManager::GetInstance(this)->AddScreen("ingame", new GameScreen())
		->AddScreen("mainmenu", new MenuScreen())->SetCurrentScreen("mainmenu");

}

void Engine::RhythmSpace::Update()
{
	Engine::ScreenManager::GetInstance(this)->Update();
}

void Engine::RhythmSpace::Render()
{
	Engine::ScreenManager::GetInstance(this)->Draw();
}