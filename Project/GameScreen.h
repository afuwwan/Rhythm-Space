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

		////////////Sprites////////////
		Engine::Texture* texture_N1 = NULL;

		Engine::Sprite* sprite = NULL;

		Engine::Texture* texture_bg = NULL;

		Engine::Sprite* bg = NULL;

		Engine::Texture* texture2 = NULL;

		Engine::Sprite* sprite2 = NULL;


		////////////Music & Sound////////////
		Music* music = NULL;

		Sound* hit = NULL;

		Music* music2 = NULL;


		//Sound* metronome = NULL;

		float duration;

		int bps;

		int previousBps;

		float duration2;

		int bps2;

		int previousBps2;


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

		void ResetBullets();



		//////////Obstacle///////////
		vector<Sprite*> platforms;

		void SpawnObstacle(float xPosition);

		void GenerateObstaclePattern();

		void ResetObstacleGeneration();

		int previousEmptySection = -1;


		//////////Enemies////////////
		vector<Sprite*> enemies;

		void SpawnEnemies(float xPosition);

		void GenerateEnemyPattern();

		void ResetEnemyGeneration();

		bool enemySpawned = false;


		////////////Others////////////
		int score = 1000;
		Text* text1 = NULL;

	};
}

#endif