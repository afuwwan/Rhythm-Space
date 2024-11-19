#ifndef RHYTHMSPACE_H
#define RHYTHMSPACE_H

#include "Game.h"
#include "Setting.h"
#include "MenuScreen.h"
#include "GameScreen.h"
#include "SongListScreen.h"
#include "GameScreen2.h"
#include "GameScreen3.h"
#include "TutorialScreen.h"
#include "LoadingScreen.h"


namespace Engine {
	class RhythmSpace :public Engine::Game
	{
	public:
		RhythmSpace(Setting* setting);
		~RhythmSpace();
		virtual void Init();
		virtual void Update();
		virtual void Render();
	};
}

#endif

