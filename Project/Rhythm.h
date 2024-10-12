#ifndef RHYTHM_H
#define RHYTHM_H

#include "Game.h"
#include "Setting.h"
#include "Texture.h"
#include "Sprite.h"
#include "Music.h"
#include "Sound.h"
#include "Text.h"
#include "Input.h"
#include <vector>
#include <GLFW/glfw3.h>

namespace Engine {
	class Rhythm :public Engine::Game
	{
	public:
		Rhythm(Setting* setting);
		~Rhythm();
		virtual void Init();
		virtual void Update();
		virtual void Render();

	private:

		////////////Sprites////////////
		Engine::Texture* texture_N1 = NULL;

		Engine::Sprite* note1 = NULL;

		Engine::Texture* texture_bg = NULL;

		Engine::Sprite* bg = NULL;


		////////////Music & Sound////////////
		Music* music = NULL;

		Sound* hit = NULL;

		//Sound* metronome = NULL;

		float duration;

		int bps;


		////////////Parallax////////////
		vector<Sprite*> backgrounds, middlegrounds, foregrounds;

		void MoveLayer(vector<Sprite*>& bg, float speed);

		void DrawLayer(vector<Sprite*>& bg);

		void AddToLayer(vector<Sprite*>& bg, string name);

		float offset = 0;


	};
}

#endif



