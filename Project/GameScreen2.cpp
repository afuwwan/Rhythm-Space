#include "GameScreen2.h"


Engine::GameScreen2::GameScreen2()
{
	delete texture_N1;
	delete sprite;
}

void Engine::GameScreen2::Init()
{

#pragma region Sprite and Background

	// Init Notes Sprite 
	texture_N1 = new Texture("ship.png");
	sprite = new Sprite(texture_N1, game->GetDefaultSpriteShader(), game->GetDefaultQuad());

	texture_bg = new Texture("Space.png");
	bg = (new Sprite(texture_bg, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetSize((float)game->GetSettings()->screenWidth, (float)game->GetSettings()->screenHeight);

	sprite->SetScale(0.3f)
		->SetPosition(game->GetSettings()->screenWidth / 2.15f, (game->GetSettings()->screenHeight / 12) - 50.0f)
		->SetBoundingBoxSize(65, 65);

	texture2 = new Texture("RotationPoint.png");
	sprite2 = new Sprite(texture2, game->GetDefaultSpriteShader(), game->GetDefaultQuad());

	sprite2->SetPosition((game->GetSettings()->screenWidth / 2) - 12, (game->GetSettings()->screenHeight / 2) - 12)
		->SetSize(25, 25);



#pragma endregion

#pragma region Camera init

	//cam_follow = new Game(defaultProjection);

#pragma endregion

#pragma region Score init

	text1 = new Text("homespun.ttf", 100, game->GetDefaultTextShader());
	text1->SetScale(1)->SetColor(255, 255, 255)
		->SetPosition((game->GetSettings()->screenWidth / 4) * 3.25, ((game->GetSettings()->screenHeight / 4) - 50) - (text1->GetFontSize() * text1->GetScale()));


#pragma endregion

#pragma region Music and Sound init

	music = (new Music("Bossfight-Milky-Ways.ogg"))->SetVolume(45)/*->Play(false)*/;
	music2 = (new Music("Shirobon-Regain-Control.ogg"))->SetVolume(45)/*->Play(false)*/;

	//metronome = (new Sound("beep.ogg"))->SetVolume(70)->Play(true);

	//hit = (new Sound(""))->SetVolume(70);

#pragma endregion

#pragma region Input Init

	game->GetInputManager()->AddInputMapping("Move Right", SDLK_d)
		->AddInputMapping("Move Left", SDLK_a)
		->AddInputMapping("Move Center", SDLK_s)
		->AddInputMapping("Attack", SDL_BUTTON_LEFT)
		->AddInputMapping("Avoid", SDLK_SPACE)
		->AddInputMapping("mainmenu", SDLK_ESCAPE)
		->AddInputMapping("mainmenu", SDL_CONTROLLER_BUTTON_Y);

#pragma endregion

#pragma region Parallax Init

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

#pragma endregion

#pragma region Obstacle init

#pragma endregion

#pragma region Bullet Init

	//Add bullets
	bulletTexture = new Texture("projectiles.png");
	int bulletNum = 10000;
	for (int i = 0; i < bulletNum; i++) {
		Sprite* bs = (new Sprite(bulletTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetNumXFrames(3)
			->SetNumYFrames(2)
			->SetScale(0.07)
			->SetBoundingBoxSize((0.7 * 1) / 1, (0.7 * 1) / 1)
			->AddAnimation("idle", 1, 1)
			->PlayAnim("idle");
		readyBullets.push_back(new Bullet(bs));
	}

#pragma endregion

	GameState2 gstate;

}

void Engine::GameScreen2::Update()
{

	if (gstate2 == GameState2::RUNNING)
	{

		if (game->GetInputManager()->IsKeyReleased("mainmenu")) {
			ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");
			music->Stop();

			ResetVariables();

		}

#pragma region Score Handling

		text1->SetText(std::to_string(score));

		//if score below 0 game is over
		if (score < 0)
		{
			gstate2 = GameState2::GAME_OVER;			
			score = 0;
			music->Stop();
			return;
		}

#pragma endregion

#pragma region Music Handling
		std::cout << "duration : " << duration / 1000 << std::endl;


		if (duration / 1000 >= 1.8) //add slight delay to make sound match obstacle spawning
		{
			if (music->IsPlaying() == false)
			{
				music->Play(true);
				music->IsPlaying() == true;
			}
		}

		//if game time exceeds 259 seconds game state is at finish
		if (duration / 1000 >= 259.0f)
		{
			music->Stop();
			gstate2 = GameState2::FINISH;
			return;
		}



		if (game->GetInputManager()->IsKeyReleased("mainmenu"))
		{
			ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");
			music->Stop();
			music2->Play(true);
		}

		if (game->GetInputManager()->IsKeyReleased("Avoid"))
		{
			gstate2 == GameState2::GAME_OVER;
			return;
		}

#pragma endregion

#pragma region Movement

		vec2 oldMonsterPos = sprite->GetPosition();
		float x = oldMonsterPos.x, y = oldMonsterPos.y;
		if (game->GetInputManager()->IsKeyPressed("Move Right")) {
			x = (game->GetSettings()->screenWidth / 2.15) + 200;
			//note1->PlayAnim("walk")->SetFlipHorizontal(false)->SetRotation(0);
		}
		if (game->GetInputManager()->IsKeyPressed("Move Left")) {
			x = (game->GetSettings()->screenWidth / 2.15) - 200;
			//note1->PlayAnim("walk")->SetFlipHorizontal(true)->SetRotation(0);
		}
		if (game->GetInputManager()->IsKeyPressed("Move Center")) {
			x = game->GetSettings()->screenWidth / 2.15;
			//note1->PlayAnim("walk")->SetFlipHorizontal(true)->SetRotation(0);
		}
		if (game->GetInputManager()->IsKeyPressed("Avoid")) {
			x = game->GetSettings()->screenWidth / 2.15;
			// Remove Bounding Box
			// Plays Jumping Animation
		}

		sprite->Update(game->GetGameTime());
		sprite->PlayAnim("idle");

		sprite->SetPosition(x, y);

#pragma endregion



#pragma region Obstacle and Enemies Mapping

		//float duration;
		duration += game->GetGameTime();

		//1 beat = 1 / 2.6 of a second
		bps = (duration / 1000) * 2.6;

		// Obstacle pattern
		if (duration / 1000 < 36)
		{
			if (floor(bps) > previousBps) {
				previousBps = floor(bps);  // Update the previous BPS value
				//SpawnBullets();

				// Check if the beat count is the equivalent of 4
				if (previousBps % 4 == 0) {

					GenerateObstaclePattern();

				}

				if (previousBps % 3 == 0)
				{
					GenerateEnemyPattern();
				}

			}
		}

		std::cout << "Beats : " << bps << std::endl;

#pragma endregion

#pragma region Obstacle Handling

		//Obstacle behaviour
		for (auto it = platforms.begin(); it != platforms.end();)
		{

			Sprite* obstacle = *it;

			//
			if (obstacle)
			{
				float y_ = obstacle->GetPosition().y;

				y_ -= 1.5f * game->GetGameTime();  // Adjust obstacle speed as needed

				obstacle->SetPosition(obstacle->GetPosition().x, y_)->Update(game->GetGameTime());

				if (obstacle->GetBoundingBox()->CollideWith(sprite->GetBoundingBox()))
				{
					//gstate = GameState::GAME_OVER;
					//return;
					it = platforms.erase(it); // Remove obstacle on collision
					score -= 50;
					//text->SetText("Score: " + std::to_string(score));
				}
				else if (y_ <= -300)
				{
					it = platforms.erase(it);  // Remove obstacle when it's off-screen
				}
				else
				{
					++it; // Only increment iterator if obstacle isn't erased
				}

				// Hide obstacle when it reaches the defined screen height
				if (y_ == (game->GetSettings()->screenHeight * 0) + 300)
				{
					obstacle->SetBoundingBoxSize(0, 0);
				}

				// Rotate meteor
				int spinT = duration / 70;

				if (spinT == 360)
				{
					spinT = 0;
				}

				obstacle->SetRotation(spinT);
			}
			else
			{
				// If obstacle is null, erase it and move to the next
				it = platforms.erase(it);
			}
		}

#pragma endregion

		MoveLayer(backgrounds, 0.01f);
		MoveLayer(middlegrounds, 0.06f);
		MoveLayer(foregrounds, 0.6f);



	}
	else if (gstate2 == GameState2::FINISH)
	{
		//music->Stop();

		MoveLayer(backgrounds, 0.0f);
		MoveLayer(middlegrounds, 0.0f);
		MoveLayer(foregrounds, 0.0f);

		//escape untuk ke menu screen

		//reset semua variabel

		if (game->GetInputManager()->IsKeyReleased("mainmenu")) {
			ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");
			music->Stop();
			ResetVariables();
		}



		std::cout << "Game state is at finish" << std::endl;
	}
	else if (gstate2 == GameState2::GAME_OVER)
	{
		if (game->GetInputManager()->IsKeyReleased("mainmenu"))
		{
			music->Stop();

			ResetVariables();

			ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");
			music2->Play(true);


		}

		MoveLayer(backgrounds, 0.0f);
		MoveLayer(middlegrounds, 0.0f);
		MoveLayer(foregrounds, 0.0f);

		//escape untuk ke menuscreen

		//jika restart reset semua variabel
		//gstate == GameState::RESET;


		std::cout << "Game state is at game over" << std::endl;
	}
	else if (gstate2 == GameState2::RESET)
	{
		ResetVariables();

		std::cout << "Game resets" << std::endl;

		//gstate == GameState::RUNNING;
	}

}

void Engine::GameScreen2::Draw()
{

#pragma region Camera Render

	//vec2 notePosition = note1->GetPosition();

	//glm::vec3 cameraPosition(notePosition.x, notePosition.y, 1.0f); // Camera positioned behind the scene
	//glm::mat4 view = glm::lookAt(cameraPosition, glm::vec3(notePosition.x, notePosition.y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	//shader->Use();
	//shader->setMat4(view, "view");

	//glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(note1->GetPosition().x), 0.0f, static_cast<GLfloat>(note1->GetPosition().y), -1.0f, 1.0f);
	//glm::mat4 view = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);

#pragma endregion

	//Draw Background Sprite
	bg->Draw();

	//Draw parallax background
	DrawLayer(backgrounds);
	DrawLayer(middlegrounds);

	//Obstacle
	for (Sprite* s : platforms)
	{
		s->Draw();
		std::cout << "obstacle sprite is drawn" << std::endl;

	}

	//Bullets
	for (Bullet* b : inUseBullets)
	{
		b->Draw();
	}

	//Draw Note Sprite
	sprite->Draw();

	for (Sprite* e : enemies)
	{
		e->Draw();
		std::cout << "enemy sprite is drawn" << std::endl;
	}

	DrawLayer(foregrounds);

	//Score
	text1->Draw();

	//Draw Rotation Point
	sprite2->Draw();


}

void Engine::GameScreen2::ResetVariables()
{

	inUseBullets.clear();
	platforms.clear();
	enemies.clear();
	score = 500;
	duration = 0;
	previousBps = 0;
	bps = 0;

}

#pragma region Obstacle Spawning

void Engine::GameScreen2::SpawnObstacle(float xPosition)
{

	Texture* platformTexture = new Texture("meteor.png");
	vec2 start = vec2(xPosition, game->GetSettings()->screenHeight);  // Starting position (top of the screen)

	Sprite* platformSprite = new Sprite(platformTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	platformSprite->SetSize(100, 100)->SetPosition(start.x, start.y);
	platformSprite->SetBoundingBoxSize(platformSprite->GetScaleWidth() - (3.5 * platformSprite->GetScale()), platformSprite->GetScaleHeight() - 50);



	platforms.push_back(platformSprite);  // Add the new obstacle to the list
}

void Engine::GameScreen2::GenerateObstaclePattern()
{
	int emptySection;

	do
	{
		emptySection = rand() % 3;
	} while (emptySection == previousEmptySection);

	previousEmptySection = emptySection;

	std::cout << emptySection << std::endl;

	float leftPositionX = (game->GetSettings()->screenWidth / 2.15) - 200;    // Position for the left lane
	float centerPositionX = game->GetSettings()->screenWidth / 2.15;  // Position for the center lane
	float rightPositionX = (game->GetSettings()->screenWidth / 2.15) + 200;  // Position for the right lane

	// Spawn obstacle in the left section if it's not empty
	if (emptySection != 0)
	{
		SpawnObstacle(leftPositionX);
	}

	// Spawn obstacle in the center section if it's not empty
	if (emptySection != 1)
	{
		SpawnObstacle(centerPositionX);
	}

	// Spawn obstacle in the right section if it's not empty
	if (emptySection != 2)
	{
		SpawnObstacle(rightPositionX);
	}

}




#pragma endregion

#pragma region Enemies (EvilShip) Handling

//Small EvilShip
void Engine::GameScreen2::SpawnEnemies(float xPosition) {
	Texture* enemyTexture = new Texture("evilShip.png");
	vec2 start = vec2(xPosition, game->GetSettings()->screenHeight);  // Starting position (top of screen)

	Sprite* enemySprite = new Sprite(enemyTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	enemySprite->SetSize(100, 100)->SetPosition(start.x, start.y)->SetFlipVertical(true);
	enemySprite->SetBoundingBoxSize(enemySprite->GetScaleWidth() - (3.5 * enemySprite->GetScale()), enemySprite->GetScaleHeight() - 50);

	enemies.push_back(enemySprite);  // Add enemy ship to the list
}

void Engine::GameScreen2::GenerateEnemyPattern() {
	float randomX = rand() % (int)((game->GetSettings()->screenWidth));  // Random X position
	SpawnEnemies(randomX);  // Spawn an enemy at the random X position
}

//Mid EvilShip
void Engine::GameScreen2::SpawnEnemieslv2(float xPosition) {
	Texture* enemyTexture2 = new Texture("evilShip.png");
	vec2 start = vec2(xPosition, game->GetSettings()->screenHeight);  // Starting position (top of screen)

	Sprite* enemySprite2 = new Sprite(enemyTexture2, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	enemySprite2->SetSize(100, 100)->SetPosition(start.x, start.y)->SetFlipVertical(true);
	enemySprite2->SetBoundingBoxSize(enemySprite2->GetScaleWidth() - (3.5 * enemySprite2->GetScale()), enemySprite2->GetScaleHeight() - 50);

	enemies2.push_back(enemySprite2);  // Add enemy ship to the list
}

void Engine::GameScreen2::GenerateEnemylv2Pattern() {
	float randomX = rand() % (int)((game->GetSettings()->screenWidth));  // Random X position
	SpawnEnemieslv2(randomX);  // Spawn an enemy at the random X position
}

//Large Evil Ship
void Engine::GameScreen2::SpawnEnemieslv3(float xPosition) {
	Texture* enemyTexture3 = new Texture("evilShip.png");
	vec2 start = vec2(xPosition, game->GetSettings()->screenHeight);  // Starting position (top of screen)

	Sprite* enemySprite3 = new Sprite(enemyTexture3, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	enemySprite3->SetSize(100, 100)->SetPosition(start.x, start.y)->SetFlipVertical(true);
	enemySprite3->SetBoundingBoxSize(enemySprite3->GetScaleWidth() - (3.5 * enemySprite3->GetScale()), enemySprite3->GetScaleHeight() - 50);

	enemies3.push_back(enemySprite3);  // Add enemy ship to the list
}

void Engine::GameScreen2::GenerateEnemylv3Pattern() {
	float randomX = rand() % (int)((game->GetSettings()->screenWidth));  // Random X position
	SpawnEnemieslv3(randomX);  // Spawn an enemy at the random X position
}



#pragma endregion

#pragma region Parallax Functions

void Engine::GameScreen2::MoveLayer(vector<Sprite*>& bg, float speed)
{
	for (Sprite* s : bg) {
		if (s->GetPosition().y < -game->GetSettings()->screenHeight + offset) {
			s->SetPosition(0, game->GetSettings()->screenHeight + offset - 1);
		}
		s->SetPosition(s->GetPosition().x, s->GetPosition().y - speed * game->GetGameTime());
		s->Update(game->GetGameTime());
	}
}

void Engine::GameScreen2::DrawLayer(vector<Sprite*>& bg)
{
	for (Sprite* s : bg) {
		s->Draw();
	}
}

void Engine::GameScreen2::AddToLayer(vector<Sprite*>& bg, string name)
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

#pragma region Bullet Spawn

void Engine::GameScreen2::SpawnBullets()
{
	if (timeInterval >= 250) {
		if (readyBullets.empty()) {
			return;
		}
		Bullet* b = readyBullets.back();
		readyBullets.pop_back();

		// Get spaceship's dimensions
		float spriteWidth = sprite->GetScaleWidth();
		float spriteHeight = sprite->GetScaleHeight();

		// Calculate the spawn position at the center bottom (0.5, 1) of the sprite
		float bulletStartX = sprite->GetPosition().x + (spriteWidth / 2.75f);
		float bulletStartY = sprite->GetPosition().y + (spriteHeight / 2.75f);

		// Set the bullet's initial position to the (0.5, 1) texture coordinate
		b->SetPosition(bulletStartX, bulletStartY);

		// Get the angle of the spaceship (sprite rotation)
		float angle = glm::radians(sprite->GetRotation() + 90);

#pragma region spacehip angle logic

		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);

		//float dx = mouseX - note1->GetPosition().x;
		//float dy = mouseY - note1->GetPosition().y;

		//float angle = atan2(dy, dx) * (180 / M_PI) - 90;

		glm::vec2 mousePos = { mouseX, mouseY };

		glm::vec2 screenCenter(game->GetSettings()->screenWidth / 2.0f, game->GetSettings()->screenHeight / 2.0f);

		glm::vec2 mouseDirection = mousePos - screenCenter;

		if (glm::length(mouseDirection) == 0.f)
		{
			mouseDirection = { 1,0 };
		}
		else
		{
			mouseDirection = normalize(mouseDirection);
		}

#pragma endregion

		//initiates the spaceship angle for bullets 
		float spaceshipAngle = atan2(mouseDirection.y, -mouseDirection.x) * (180 / M_PI) + 90;

		// Calculate x and y velocity based on the angle
		float speed = 0.75f;  // Adjust this as needed
		b->xVelocity = cos(angle) * speed;
		b->yVelocity = sin(angle) * speed;

		//sets bullets rotation the same as ships angle
		b->SetRotation(spaceshipAngle);

		inUseBullets.push_back(b);
		timeInterval = 0;

	}

}

#pragma endregion


