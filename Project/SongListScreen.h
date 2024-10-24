//#ifndef MENUSCREEN_H
//#define MENUSCREEN_H
//
//#include "Screen.h"
//#include "Game.h"
//#include "Setting.h"
//#include "Texture.h"
//#include "Sprite.h"
//#include "Button.h"
//#include "Text.h"
//#include "Music.h"
//#include "Sound.h"
//#include <vector>
//#include "ScreenManager.h"
//
//namespace Engine {
//	class MenuScreen :
//		public Engine::Screen
//	{
//	public:
//		MenuScreen();
//		void Init();
//		void Update();
//		void Draw();
//	private:
//		vector<Button*> buttons;
//		Text* text;
//		int currentButtonIndex = 0;
//		Music* music = NULL;
//		Music* music2 = NULL;
//
//		vector<Sprite*> backgrounds, middlegrounds, foregrounds;
//		void MoveLayer(vector<Sprite*>& bg, float speed);
//		void DrawLayer(vector<Sprite*>& bg);
//		void AddToLayer(vector<Sprite*>& bg, string name);
//		float offset = 1;
//
//	};
//}
//
//#endif