#include "MenuScreen.h"

Engine::MenuScreen::MenuScreen()
{
	text = NULL;
}

void Engine::MenuScreen::Init()
{
	
	// Initialize camera properties (position, zoom)
	camera.position = glm::vec2(0,0);
	camera.zoom = 1.0f;  // Default zoom level

	logoTexture = new Texture("RhythmSpace2.png");
	logo = new Sprite(logoTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());

	logo->SetScale(3.5)
		->SetPosition((game->GetSettings()->screenWidth / 2) - 270, (game->GetSettings()->screenHeight)/2 - 110);

	
	// Create a Texture
	Texture* texture = new Texture("buttonRhythmSpace.png");

	// Create Sprites
	Sprite* playSprite = (new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))
		->SetNumXFrames(2)
		->SetNumYFrames(4)
		->SetScale(5)
		->AddAnimation("normal", 0, 0)
		->AddAnimation("hover", 0, 1)
		->AddAnimation("press", 0, 1)
		->SetAnimationDuration(400);

	Sprite* exitSprite = (new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))
		->SetNumXFrames(2)
		->SetNumYFrames(4)
		->SetScale(5)
		->AddAnimation("normal", 2, 2)
		->AddAnimation("hover", 2, 3)
		->AddAnimation("press", 2, 3)
		->SetAnimationDuration(400);

	//Create Buttons
	Button* playButton = new Button(playSprite, "play");
	playButton->SetPosition((game->GetSettings()->screenWidth / 2) - (playSprite->GetScaleWidth() / 2),
		400);
	buttons.push_back(playButton);

	// Set play button into active button
	currentButtonIndex = 0;
	buttons[currentButtonIndex]->SetButtonState(Engine::ButtonState::HOVER);


	// Add input mappings
	game->GetInputManager()->AddInputMapping("next", SDLK_DOWN)
		->AddInputMapping("prev", SDLK_UP)
		->AddInputMapping("press", SDLK_RETURN)
		->AddInputMapping("exit", SDLK_BACKSPACE)
		->AddInputMapping("Zoom In", SDLK_i)
		->AddInputMapping("Zoom Out", SDLK_u);

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

	//displays score in running state
	enter_to_play = new Text("homespun.ttf", 50, game->GetDefaultTextShader());
	enter_to_play->SetScale(1)->SetColor(255, 255, 255)->SetText("PRESS ENTER TO PLAY")
		->SetPosition(((game->GetSettings()->screenWidth) / 2) / 1.34f, ((game->GetSettings()->screenHeight) / 5)/1.25f);
	
	// Create Text
	text = (new Text("homespun.ttf", 50, game->GetDefaultTextShader()))
		->SetText("BACKSPACE TO EXIT")->SetPosition(((game->GetSettings()->screenWidth) / 2)/1.3f, ((game->GetSettings()->screenHeight) / 5) / 2)->SetColor(255, 0, 0);


}


void Engine::MenuScreen::Update()
{
		
	duration1 += game->GetGameTime();
	// Increment duration with deltaTime
	duration2 += game->GetGameTime();

	// Calculate beats per second (bps)
	bps2 = (duration2 / 1000) * 1.500;

	if (bps2 >= 1) {
		// Get the fractional part of the beat to control smooth scaling during the beat
		fractionalBeat = fmod(bps2, 1.0f);

		// Check if we are in the first half (scale down) or second half (scale up) of the beat
		if (fractionalBeat < 0.9f) {
			// Scale down first
			currentScale = maxScale - (maxScale - originalScale) * (fractionalBeat / 0.9f); // Scale down over first half
		}
		else {
			// Scale up next
			currentScale = originalScale + (maxScale - originalScale) * ((fractionalBeat - 0.9f) / 0.1f); // Scale up over second half
		}

		// Apply the current scale to the logo sprite
		logo->SetScale(currentScale);
	}
	
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

		// If play button then go to Song List, exit button then exit
		if ("play" == b->GetButtonName()) {
			ScreenManager::GetInstance(game)->SetCurrentScreen("songlist");
			//music2->Stop();
			duration2 = 0;
			bps2 = 0;
			fractionalBeat = 0;
		}
		else if ("exit" == b->GetButtonName()) {
			game->SetState(Engine::State::EXIT);
		}
	}

	if (game->GetInputManager()->IsKeyReleased("exit")) {
		game->SetState(Engine::State::EXIT);
	}

	// Update All buttons
	for (Button* b : buttons) {
		b->Update(game->GetGameTime());
	}

	MoveLayer(backgrounds, 0.005f);
	MoveLayer(middlegrounds, 0.03f);
	MoveLayer(foregrounds, 0.3f);

	//camera.Follow(sprite->GetPosition());

	// Optional: Zoom control with key input
	if (game->GetInputManager()->IsKeyPressed("Zoom In")) {
		camera.SetZoom(camera.zoom + 0.007f);
	}
	if (game->GetInputManager()->IsKeyPressed("Zoom Out")) {
		camera.SetZoom(camera.zoom - 0.007f);
	}

	// Update the shader's view matrix uniform
	glUseProgram(game->GetDefaultSpriteShader()->GetId());
	glm::mat4 view = camera.GetViewMatrix(game->GetSettings()->screenWidth,game->GetSettings()->screenHeight);
	glUniformMatrix4fv(glGetUniformLocation(game->GetDefaultSpriteShader()->GetId(), "view"), 1, GL_FALSE, glm::value_ptr(view));

}

void Engine::MenuScreen::Draw()
{
	glUseProgram(game->GetDefaultSpriteShader()->GetId());

	// Set the camera's view matrix
	glm::mat4 view = camera.GetViewMatrix(game->GetSettings()->screenWidth, game->GetSettings()->screenHeight);
	glUniformMatrix4fv(glGetUniformLocation(game->GetDefaultSpriteShader()->GetId(), "view"), 1, GL_FALSE, glm::value_ptr(view));
	
	//Draw parallax background
	DrawLayer(backgrounds);
	DrawLayer(middlegrounds);
	
	// Render all buttons
	//for (Button* b : buttons) {
	//	b->Draw();
	//}

	
	text->Draw();

	enter_to_play->Draw();

	logo->Draw();
}

#pragma region Parallax Functions

void Engine::MenuScreen::MoveLayer(vector<Sprite*>& bg, float speed)
{
	for (Sprite* s : bg) {
		if (s->GetPosition().y < -game->GetSettings()->screenHeight + offset) {
			s->SetPosition(0, game->GetSettings()->screenHeight + offset - 1);
		}
		s->SetPosition(s->GetPosition().x, s->GetPosition().y - speed * game->GetGameTime());
		s->Update(game->GetGameTime());
	}
}

void Engine::MenuScreen::DrawLayer(vector<Sprite*>& bg)
{
	for (Sprite* s : bg) {
		s->Draw();
	}
}

void Engine::MenuScreen::AddToLayer(vector<Sprite*>& bg, string name)
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
