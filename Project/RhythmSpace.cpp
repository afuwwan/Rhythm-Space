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
		->AddScreen("ingame2", new GameScreen2())
		->AddScreen("ingame3", new GameScreen3())
		->AddScreen("mainmenu", new MenuScreen())
		->AddScreen("songlist", new SongListScreen())
		->AddScreen("loading", new LoadingScreen())
		->AddScreen("tutorial", new TutorialScreen())
		->SetCurrentScreen("mainmenu");

}

void Engine::RhythmSpace::Update()
{
	Engine::ScreenManager::GetInstance(this)->Update();
}

void Engine::RhythmSpace::Render()
{
	Engine::ScreenManager::GetInstance(this)->Draw();
}