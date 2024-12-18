#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include "Screen.h"
#include "Game.h"
#include "Setting.h"
#include "Texture.h"
#include "Sprite.h"
#include "Button.h"
#include "Text.h"
#include "Music.h"
#include "Sound.h"
#include <vector>
#include "ScreenManager.h"
#include "Camera.h"

namespace Engine {
	class LoadingScreen :
		public Engine::Screen
	{
	public:
		LoadingScreen();
		void Init();
		void Update();
		void Draw();
	private:

		Camera camera;

		float duration = 0.0f;

		float loadingStartTime = 0.0f;

		Engine::Texture* texture = NULL;
		Engine::Texture* bgtexture = NULL;
		Engine::Sprite* sprite = NULL;
		Engine::Sprite* sprite2 = NULL;

	};
}

#endif