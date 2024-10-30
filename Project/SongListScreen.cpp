#include "SongListScreen.h"

Engine::SongListScreen::SongListScreen()
{
	text = NULL;
}

void Engine::SongListScreen::Init()
{

	// Create a Texture
	Texture* texture = new Texture("songList.png");

	// Create Sprites
	Sprite* song1 = (new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))
		->SetNumXFrames(2)
		->SetNumYFrames(2)
		->SetScale(5)
		->AddAnimation("normal", 0, 0)
		->AddAnimation("hover", 0, 1)
		->AddAnimation("press", 1, 1)
		->SetAnimationDuration(400);

	Sprite* song2 = (new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))
		->SetNumXFrames(2)
		->SetNumYFrames(2)
		->SetScale(5)
		->AddAnimation("normal", 2, 2)
		->AddAnimation("hover", 2, 3)
		->AddAnimation("press", 3, 3)
		->SetAnimationDuration(400);

	//Create Buttons
	Button* song1Button = new Button(song1, "song1");
	song1Button->SetPosition((game->GetSettings()->screenWidth / 2) - (song1->GetScaleWidth() / 2),
		400);
	buttons.push_back(song1Button);

	Button* song2Button = new Button(song2, "song2");
	song2Button->SetPosition((game->GetSettings()->screenWidth / 2) - (song2->GetScaleWidth() / 2),
		250);
	buttons.push_back(song2Button);

	// Set play button into active button
	currentButtonIndex = 0;
	buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);

	// Create Text
	text = (new Text("8-bit Arcade In.ttf", 100, game->GetDefaultTextShader()))
		->SetText("Song List")->SetPosition((game->GetSettings()->screenWidth / 2) - 270, game->GetSettings()->screenHeight - 100)->SetColor(235, 229, 52);

	// Add input mappings
	game->GetInputManager()->AddInputMapping("next", SDLK_DOWN)
		->AddInputMapping("prev", SDLK_UP)
		->AddInputMapping("press", SDLK_RETURN);

	music2 = (new Music("Shirobon-Regain-Control.ogg"))->SetVolume(30)->Play(true);

	for (int i = 0; i <= 2; i++) {
		AddToLayer(backgrounds, "spc0" + to_string(i) + ".png");
	}
	for (int i = 3; i <= 4; i++) {
		AddToLayer(middlegrounds, "spc0" + to_string(i) + ".png");
	}
	for (int i = 5; i <= 5; i++) {
		AddToLayer(foregrounds, "spc0" + to_string(i) + ".png");
	}

	offset = 2;

}


void Engine::SongListScreen::Update()
{
	// Set background
	game->SetBackgroundColor(52, 155, 235);

	if (game->GetInputManager()->IsKeyReleased("next")) {
		// Set previous button to normal state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
		// Next Button
		currentButtonIndex = (currentButtonIndex < (int)buttons.size() - 1) ? currentButtonIndex + 1 : currentButtonIndex;
		// Set current button to hover state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
	}

	if (game->GetInputManager()->IsKeyReleased("prev")) {
		// Set previous button to normal state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::NORMAL);
		// Prev Button
		currentButtonIndex = currentButtonIndex > 0 ? currentButtonIndex - 1 : currentButtonIndex;
		// Set current button to hover state
		buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);
	}

	if (game->GetInputManager()->IsKeyReleased("press")) {
		// Set current button to press state
		Button* b = buttons[currentButtonIndex];
		b->SetButtonState(Engine::ButtonState::PRESS);

		b->GetSprite()->PlayAnim("press");

		// If play button then go to InGame, exit button then exit
		if ("song1" == b->GetButtonName()) {
			ScreenManager::GetInstance(game)->SetCurrentScreen("ingame");
			music2->Stop();
		}
		else if ("song2" == b->GetButtonName()) {
			ScreenManager::GetInstance(game)->SetCurrentScreen("ingame2");
			music2->Stop();

		}
	}

	// Update All buttons
	for (Button* b : buttons) {
		b->Update(game->GetGameTime());
	}

	MoveLayer(backgrounds, 0.005f);
	MoveLayer(middlegrounds, 0.03f);
	MoveLayer(foregrounds, 0.3f);

}

void Engine::SongListScreen::Draw()
{
	//Draw parallax background
	DrawLayer(backgrounds);
	DrawLayer(middlegrounds);

	// Render all buttons
	for (Button* b : buttons) {
		b->Draw();
	}
	// Render title 
	text->Draw();
}

#pragma region Parallax Functions

void Engine::SongListScreen::MoveLayer(vector<Sprite*>& bg, float speed)
{
	for (Sprite* s : bg) {
		if (s->GetPosition().y < -game->GetSettings()->screenHeight + offset) {
			s->SetPosition(0, game->GetSettings()->screenHeight + offset - 1);
		}
		s->SetPosition(s->GetPosition().x, s->GetPosition().y - speed * game->GetGameTime());
		s->Update(game->GetGameTime());
	}
}

void Engine::SongListScreen::DrawLayer(vector<Sprite*>& bg)
{
	for (Sprite* s : bg) {
		s->Draw();
	}
}

void Engine::SongListScreen::AddToLayer(vector<Sprite*>& bg, string name)
{
	Texture* texture = new Texture(name);

	Sprite* s = new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	s->SetSize(game->GetSettings()->screenWidth, game->GetSettings()->screenHeight + offset);
	bg.push_back(s);

	Sprite* s2 = new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	s2->SetSize(game->GetSettings()->screenWidth, game->GetSettings()->screenHeight + offset)->SetPosition(0, game->GetSettings()->screenHeight + offset - 1);
	bg.push_back(s2);
}

#pragma endregion
