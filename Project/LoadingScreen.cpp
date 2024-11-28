#include "LoadingScreen.h"

Engine::LoadingScreen::LoadingScreen()
{
	delete texture;
	delete sprite;
}

void Engine::LoadingScreen::Init()
{
	
	// Initialize camera properties (position, zoom)
	camera.position = glm::vec2(0, 0);
	camera.zoom = 1.0f;  // Default zoom level

	texture = new Texture("Loading.png");
	bgtexture = new Texture("spc01.png");
	sprite2 = new Sprite(bgtexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	sprite = new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	sprite->SetNumXFrames(1)->SetNumYFrames(4)
		->AddAnimation("normal", 0, 3)
		->SetScale(10)
		->SetAnimationDuration(350)
		->SetPosition(game->GetSettings()->screenWidth / 2.7, game->GetSettings()->screenHeight / 2.15)
		->PlayAnim("normal");

	sprite2->SetNumXFrames(1)->SetNumYFrames(1)
		->AddAnimation("normal", 0, 0)
		->SetAnimationDuration(350)
		->PlayAnim("normal");

}

void Engine::LoadingScreen::Update()
{
	
	std::cout << game->selectedSong << std::endl;


	loadingStartTime += game->GetGameTime();

	
	
	if (loadingStartTime / 1000 >= 4.0f) {

		// Switch based on the selected song
		if (game->selectedSong == 1) 
		{
			loadingStartTime = 0.0f;
			ScreenManager::GetInstance(game)->SetCurrentScreen("ingame");
		}
		else if (game->selectedSong == 2) 
		{
			loadingStartTime = 0.0f;
			ScreenManager::GetInstance(game)->SetCurrentScreen("ingame2");
		}
	}

	game->SetBackgroundColor(0, 0, 0);

	sprite->Update(game->GetGameTime());

	// Update the shader's view matrix uniform
	glUseProgram(game->GetDefaultSpriteShader()->GetId());
	glm::mat4 view = camera.GetViewMatrix(game->GetSettings()->screenWidth, game->GetSettings()->screenHeight);
	glUniformMatrix4fv(glGetUniformLocation(game->GetDefaultSpriteShader()->GetId(), "view"), 1, GL_FALSE, glm::value_ptr(view));


}

void Engine::LoadingScreen::Draw()
{
	glUseProgram(game->GetDefaultSpriteShader()->GetId());

	// Set the camera's view matrix
	glm::mat4 view = camera.GetViewMatrix(game->GetSettings()->screenWidth, game->GetSettings()->screenHeight);
	glUniformMatrix4fv(glGetUniformLocation(game->GetDefaultSpriteShader()->GetId(), "view"), 1, GL_FALSE, glm::value_ptr(view));

	sprite2->Draw();
	sprite->Draw();

}


