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
	sprite = new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	sprite->SetNumXFrames(1)->SetNumYFrames(4)
		->AddAnimation("normal", 0, 3)
		->SetScale(10)
		->SetAnimationDuration(150)
		->SetPosition(game->GetSettings()->screenWidth / 2, game->GetSettings()->screenHeight / 2)
		->PlayAnim("normal");

	game->SetBackgroundColor(0, 0, 0);

}

void Engine::LoadingScreen::Update()
{
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

	
	sprite->Draw();

}


