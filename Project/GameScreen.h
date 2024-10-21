#ifndef GAMESCREEN_H
#define GAMESCREEN_H

#include "Screen.h"
#include "Turtle.h"
#include "ScreenManager.h"
#include "Game.h"
#include "Setting.h"
#include "Texture.h"
#include "Sprite.h"
#include "Music.h"
#include "Sound.h"
#include "Text.h"
#include "Input.h"
#include "Bullet.h"
//#include "Shader.h"
#include <vector>
#include <GLFW/glfw3.h>

namespace Engine {
	class GameScreen :
		public Engine::Screen
	{
	public:
		enum class GameState
		{
			RUNNING,
			RESET,
			GAME_OVER,
			FINISH
		};
		GameState gstate;

		GameScreen();
		void Init();
		void Update();
		void Draw();

	private:
	////////////////////
		Engine::Sprite* CreateSprite(Texture* texture);
		void SpawnObjects();
		vector<Turtle*> objects;
		float spawnDuration = 0, maxSpawnTime = 0, numObjectsInPool = 0, numObjectPerSpawn = 0;
	////////////////////

		////////////Sprites////////////
		Engine::Texture* texture_N1 = NULL;

		Engine::Sprite* sprite = NULL;

		Engine::Texture* texture_bg = NULL;

		Engine::Sprite* bg = NULL;


		////////////Music & Sound////////////
		Music* music = NULL;

		Sound* hit = NULL;

		//Sound* metronome = NULL;

		float duration;

		int bps;

		int previousBps;


		////////////Parallax////////////
		vector<Sprite*> backgrounds, middlegrounds, foregrounds;

		void MoveLayer(vector<Sprite*>& bg, float speed);

		void DrawLayer(vector<Sprite*>& bg);

		void AddToLayer(vector<Sprite*>& bg, string name);

		float offset = 0;

		/////////////Shader////////////
		//Shader* shader;
		//glm::mat4 view;


		///////////Bullets////////////
		Engine::Texture* texture = NULL, * bulletTexture = NULL;

		vector<Bullet*> inUseBullets;

		vector<Bullet*> readyBullets;

		float yVelocity = 0, gravity = 0, timeInterval = 0, xBulletVel = 0;

		bool jump = false, toTop = true;

		void SpawnBullets();



		//////////Obstacle///////////
		vector<Sprite*> platforms;

		void SpawnObstacle(float xPosition);

		void GenerateObstaclePattern();

		int previousEmptySection = -1;


		//////////Enemies////////////



		////////////Others////////////
		int score = 1000;
		Text* text = NULL;

	};
}

#endif