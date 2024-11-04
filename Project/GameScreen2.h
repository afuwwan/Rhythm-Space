#ifndef GAMESCREEN2_H
#define GAMESCREEN2_H

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
	class GameScreen2 :
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

		GameScreen2();
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

		Engine::Texture* texture_gov = NULL;

		Engine::Sprite* gov = NULL;

		Engine::Texture* texture_you = NULL;

		Engine::Sprite* you = NULL;

		Engine::Texture* texture_survived = NULL;

		Engine::Sprite* survived = NULL;


		////////////Music & Sound////////////
		Music* music = NULL;

		Sound* hit = NULL;

		Music* music2 = NULL;

		Music* finish_music = NULL;

		Music* gov_music = NULL;

		//Sound* metronome = NULL;

		float duration;

		int bps;

		int previousBps;

		float duration2;

		int bps2;

		int previousBps2;

		float finish_duration;


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

		//float y_;


		//////////Enemies////////////
		vector<Sprite*> enemies;

		void SpawnEnemies(float xPosition);

		void GenerateEnemyPattern();

		vector<Sprite*> enemies2;

		void SpawnEnemieslv2(float xPosition);

		void GenerateEnemylv2Pattern();

		int enemielv2health;

		vector<Sprite*> enemies3;

		void SpawnEnemieslv3(float xPosition);

		void GenerateEnemylv3Pattern();

		bool enemySpawned = false;


		////////////Others////////////
		int score = 500;

		Text* text1 = NULL;

		Text* score_finish = NULL;

		Text* back_txt = NULL;

		Text* reset_txt = NULL;

		float x_score;

		void ResetVariables();

	};
}

#endif