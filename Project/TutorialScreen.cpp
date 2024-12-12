#include "TutorialScreen.h"

Engine::TutorialScreen::TutorialScreen()
{
	delete texture_N1;
	delete sprite;
}

// Define the LoadTimestamps function
std::vector<float> Engine::TutorialScreen::LoadTimestamps(const std::string& filename) {
	std::vector<float> timestamps;
	std::ifstream file(filename);
	float timestamp;

	// Read each line as a timestamp
	while (file >> timestamp) {
		timestamps.push_back(timestamp);
	}
	file.close();

	// Reverse the vector to get chronological order
	std::reverse(timestamps.begin(), timestamps.end());
	return timestamps;
}

void Engine::TutorialScreen::Init()
{

#pragma region Sprite and Background

	// Init Notes Sprite 
	texture_N1 = new Texture("ship.png");
	sprite = new Sprite(texture_N1, game->GetDefaultSpriteShader(), game->GetDefaultQuad());

	texture_bg = new Texture("Space.png");
	bg = (new Sprite(texture_bg, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetSize((float)game->GetSettings()->screenWidth, (float)game->GetSettings()->screenHeight);

	//spaceship
	sprite->SetScale(0.3f)
		->SetPosition(game->GetSettings()->screenWidth / 2.15f, (game->GetSettings()->screenHeight / 12) - 50.0f)
		->SetBoundingBoxSize(65, 65);

	texture2 = new Texture("RotationPoint.png");
	sprite2 = new Sprite(texture2, game->GetDefaultSpriteShader(), game->GetDefaultQuad());

	//rotation point
	sprite2->SetPosition((game->GetSettings()->screenWidth / 2) - 12, (game->GetSettings()->screenHeight / 2) - 12)
		->SetSize(25, 25);

	//gameover sprite
	texture_gov = new Texture("GameOver.png");
	gov = (new Sprite(texture_gov, game->GetDefaultSpriteShader(), game->GetDefaultQuad()));

	gov->SetScale(3.0f)
		->SetNumXFrames(6)
		->SetNumYFrames(1)
		->AddAnimation("idle", 0, 5)
		->PlayAnim("idle")
		->SetPosition((game->GetSettings()->screenWidth) * 2, (game->GetSettings()->screenHeight) * 1.5f)
		->SetAnimationDuration(200);


	texture_you = new Texture("You.png");
	you = (new Sprite(texture_you, game->GetDefaultSpriteShader(), game->GetDefaultQuad()));

	texture_survived = new Texture("Survived.png");
	survived = (new Sprite(texture_survived, game->GetDefaultSpriteShader(), game->GetDefaultQuad()));

	you->SetScale(2.5f)
		->SetPosition((game->GetSettings()->screenWidth) - ((game->GetSettings()->screenWidth) * 2), (game->GetSettings()->screenHeight) / 2);

	survived->SetScale(2.5f)
		->SetPosition((game->GetSettings()->screenWidth) - ((game->GetSettings()->screenWidth) * 2), (game->GetSettings()->screenHeight) / 3);

#pragma endregion

#pragma region Text init

	//displays score in running state
	reset_txt = new Text("homespun.ttf", 40, game->GetDefaultTextShader());
	reset_txt->SetScale(1)->SetColor(255, 255, 255)->SetText("PRESS R TO PLAY AGAIN")
		->SetPosition(((game->GetSettings()->screenWidth) / 2) / 1.25f, ((game->GetSettings()->screenHeight) / 5) / 1.25f);

	//displays score in running state
	back_txt = new Text("homespun.ttf", 40, game->GetDefaultTextShader());
	back_txt->SetScale(1)->SetColor(255, 0, 0)->SetText("PRESS Esc TO EXIT")
		->SetPosition(((game->GetSettings()->screenWidth) / 2) / 1.2f, ((game->GetSettings()->screenHeight) / 5) / 2);



#pragma endregion

#pragma region Camera init

	// Initialize camera properties (position, zoom)
	camera.position = glm::vec2(0, 0);
	camera.zoom = 1.0228f;  // Default zoom level

#pragma endregion

#pragma region Score init

	//displays score in running state
	text1 = new Text("homespun.ttf", 100, game->GetDefaultTextShader());
	text1->SetScale(1)->SetColor(255, 255, 255)
		->SetPosition((game->GetSettings()->screenWidth / 4) * 3.25, ((game->GetSettings()->screenHeight / 4) - 50) - (text1->GetFontSize() * text1->GetScale()));

	score_finish = new Text("homespun.ttf", 100, game->GetDefaultTextShader());
	score_finish->SetScale(1)->SetColor(255, 255, 255)
		->SetPosition((game->GetSettings()->screenWidth) - ((game->GetSettings()->screenWidth) * 2), ((game->GetSettings()->screenHeight) / 4) * 1.3f);

#pragma endregion

#pragma region Music and Sound init

	music = (new Music("Shirobon-On-The-Run.ogg"))->SetVolume(45)/*->Play(false)*/;
	music2 = (new Music("Shirobon-Regain-Control.ogg"))->SetVolume(45)/*->Play(false)*/;

	finish_music = (new Music("PIXL-Sugar-Rush-Challenge-Loop.ogg"))->SetVolume(45);

	gov_music = (new Music("Kubbi - Formed by Glaciers (Game Over Theme).ogg"))->SetVolume(150);

	//metronome = (new Sound("beep.ogg"))->SetVolume(70)->Play(true);

	hit = (new Sound("explode.wav"))->SetVolume(70);

#pragma endregion

#pragma region Input Init

	game->GetInputManager()->AddInputMapping("Move Right", SDLK_d)
		->AddInputMapping("Move Left", SDLK_a)
		->AddInputMapping("Move Center", SDLK_s)
		->AddInputMapping("Attack", SDL_BUTTON_LEFT)
		->AddInputMapping("Null", SDL_BUTTON_RIGHT)
		->AddInputMapping("Avoid", SDLK_SPACE)
		->AddInputMapping("mainmenu", SDLK_ESCAPE)
		->AddInputMapping("mainmenu", SDLK_o)
		->AddInputMapping("Reset", SDLK_r)
		->AddInputMapping("Finish", SDLK_f)
		->AddInputMapping("GameOver", SDLK_g)
		->AddInputMapping("Zoom In", SDLK_i)
		->AddInputMapping("Zoom Out", SDLK_o);

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

	//Texture* platformTexture = new Texture("cathy.png");
	//vec2 start = vec2(game->GetSettings()->screenWidth / 2.15, game->GetSettings()->screenHeight);
	//for (int i = 0; i < 1; i++) {
	//	Sprite* platformSprite = new Sprite(platformTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	//	platformSprite->SetSize(100, 100)->SetPosition(start.x, start.y);
	//	platformSprite->SetBoundingBoxSize(platformSprite->GetScaleWidth() - (3.5 * platformSprite->GetScale()), platformSprite->GetScaleHeight() - 10);
	//	platforms.push_back(platformSprite);
	//}

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

	spawnTimestamps = LoadTimestamps("Shirobon-On-The-Run.txt");
	currentTimestampIndex = 0; // Start at the first timestamp
	// Debug: Print all timestamps to confirm loading
	for (float time : spawnTimestamps) {
		std::cout << "Loaded timestamp: " << time << std::endl;
	}
}

void Engine::TutorialScreen::Update()
{
#pragma region State Debugging

	if (game->GetInputManager()->IsKeyPressed("Finish"))
	{
		music->Stop();
		gov_music->Stop();
		gstate = GameState::FINISH;
	}

	if (game->GetInputManager()->IsKeyPressed("GameOver"))
	{
		music->Stop();
		finish_music->Stop();
		gstate = GameState::GAME_OVER;
	}

#pragma endregion


	if (gstate == GameState::RUNNING)
	{

		if (game->GetInputManager()->IsKeyReleased("mainmenu")) {
			ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");
			music->Stop();

			ResetVariables();

		}

#pragma region Score Handling

		text1->SetText(std::to_string(score))
			->SetPosition((game->GetSettings()->screenWidth / 4) * 3.25, ((game->GetSettings()->screenHeight / 4) - 50) - (text1->GetFontSize() * text1->GetScale()));

		//if score below 0 game is over
		if (score <= 0)
		{
			score = 0;
			music->Stop();
			hit->Play(false);
			gstate = GameState::GAME_OVER;
			return;
		}

		//if game time exceeds 258 seconds game state is at finish
		if (duration / 1000 >= 258.0f)
		{
			music->Stop();
			gstate = GameState::FINISH;
			return;
		}



#pragma endregion

#pragma region Music Handling
		std::cout << "duration : " << duration / 1000 << std::endl;


		if (duration / 1000 >= 2.0f) //add slight delay to make sound match obstacle spawning
		{
			if (music->IsPlaying() == false)
			{
				music->Play(true);
				music->IsPlaying() == true;
			}
		}


		if (game->GetInputManager()->IsKeyReleased("mainmenu"))
		{
			ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");
			music->Stop();
			music2->Play(true);
		}

#pragma endregion

#pragma region Camera Handling

		// Update the shader's view matrix uniform
		glUseProgram(game->GetDefaultSpriteShader()->GetId());
		glm::mat4 view = camera.GetViewMatrix(game->GetSettings()->screenWidth, game->GetSettings()->screenHeight);
		glUniformMatrix4fv(glGetUniformLocation(game->GetDefaultSpriteShader()->GetId(), "view"), 1, GL_FALSE, glm::value_ptr(view));

#pragma endregion

#pragma region Movement

		vec2 oldMonsterPos = sprite->GetPosition();
		float x = oldMonsterPos.x, y = oldMonsterPos.y;
		if (game->GetInputManager()->IsKeyPressed("Move Right")) {
			x = (game->GetSettings()->screenWidth / 2.15) + 200;
			targetCameraPos = glm::vec2(22.0f, 0.0f);
		}
		if (game->GetInputManager()->IsKeyPressed("Move Left")) {
			x = (game->GetSettings()->screenWidth / 2.15) - 200;
			targetCameraPos = glm::vec2(-22.0f, 0.0f);

		}
		if (game->GetInputManager()->IsKeyPressed("Move Center")) {
			x = game->GetSettings()->screenWidth / 2.15;
			targetCameraPos = glm::vec2(0.0f, 0.0f);

		}

		camera.SmoothFollow(targetCameraPos, 0.005f, game->GetGameTime());

		sprite->Update(game->GetGameTime());
		sprite->PlayAnim("idle");

		sprite->SetPosition(x, y);

#pragma endregion

#pragma region sprite angle according to mouse pos

		int mouseX, mouseY;

		//Get the current mouse position on the screen using SDL
		SDL_GetMouseState(&mouseX, &mouseY);

		//Store the mouse position in a glm::vec2 for easier calculations
		glm::vec2 mousePos = { mouseX, mouseY };

		//Determine the center of the screen
		glm::vec2 screenCenter(game->GetSettings()->screenWidth / 2.0f, game->GetSettings()->screenHeight);

		//Calculate the direction vector from the screen center to the mouse position
		glm::vec2 mouseDirection = mousePos - screenCenter;

		//Avoid division by zero: if the mouse is exactly at the screen center, set a default direction
		if (glm::length(mouseDirection) == 0.f)
		{
			mouseDirection = { 1,0 }; // Default direction if mouse is at center
		}
		else
		{
			//Normalize the direction vector to get a unit vector pointing towards the mouse position
			mouseDirection = normalize(mouseDirection);
		}

		//Calculate the angle in degrees from the X-axis to the mouse direction vector
		float angle = atan2(mouseDirection.y, -mouseDirection.x) * (180 / M_PI) + 90;

		//Set the sprite’s rotation based on the calculated angle
		sprite->SetRotation(angle);



#pragma endregion

#pragma region Obstacle and Enemies Mapping

		//float duration;
		duration += game->GetGameTime();

		// Check if it's time to spawn an obstacle
		if (currentTimestampIndex < spawnTimestamps.size() &&
			duration / 1000 >= spawnTimestamps[currentTimestampIndex]) {
			std::cout << "Spawning obstacle at: " << duration << std::endl;
			GenerateObstaclePattern();  // Spawn the obstacle
			currentTimestampIndex++;    // Move to the next timestamp
		}


		//1 beat = 1 / 2.6 of a second
		bps = (duration / 1000) * 2.6f;

		// Obstacle pattern every phase of the song based on seconds
		if (duration / 1000 < 36)
		{
			if (floor(bps) > previousBps)
			{
				previousBps = floor(bps);  // Update the previous BPS value
				//SpawnBullets();

				if (previousBps % 2 == 0)
				{
					GenerateEnemyPattern();
				}

			}
		}

		if (duration / 1000 >= 36 && duration / 1000 < 59)
		{
			if (floor(bps) > previousBps) {
				previousBps = floor(bps);  // Update the previous BPS value
				//SpawnBullets();

				if (previousBps % 2 == 0)
				{
					GenerateEnemyPattern();
				}

				if (previousBps % 8 == 0)
				{
					GenerateEnemylv2Pattern();
				}

			}

		}

		if (duration / 1000 >= 59 && duration / 1000 < 84)
		{
			if (floor(bps) > previousBps) {
				previousBps = floor(bps);  // Update the previous BPS value
				//SpawnBullets();

				if (previousBps % 2 == 0)
				{
					GenerateEnemyPattern();
				}

				if (previousBps % 8 == 0)
				{
					GenerateEnemylv2Pattern();
				}

			}

		}

		if (duration / 1000 >= 107 && duration / 1000 < 132)
		{
			if (floor(bps) > previousBps) {
				previousBps = floor(bps);  // Update the previous BPS value
				//SpawnBullets();

				if (previousBps % 2 == 0)
				{
					GenerateEnemyPattern();
				}

				if (previousBps % 8 == 0)
				{
					GenerateEnemylv2Pattern();
				}


			}

		}

		if (duration / 1000 >= 156 && duration / 1000 < 167)
		{
			if (floor(bps) > previousBps) {
				previousBps = floor(bps);  // Update the previous BPS value
				//SpawnBullets();


				if (previousBps % 2 == 0)
				{
					GenerateEnemyPattern();
				}

				if (previousBps % 8 == 0)
				{
					GenerateEnemylv2Pattern();
				}


			}

		}

		if (duration / 1000 >= 167 && duration / 1000 < 192)
		{
			if (floor(bps) > previousBps) {
				previousBps = floor(bps);  // Update the previous BPS value
				//SpawnBullets();



				if (previousBps % 2 == 0)
				{
					GenerateEnemyPattern();
				}

				if (previousBps % 8 == 0)
				{
					GenerateEnemylv2Pattern();
				}


			}

		}


		if (duration / 1000 >= 215 && duration / 1000 < 240)
		{
			if (floor(bps) > previousBps) {
				previousBps = floor(bps);  // Update the previous BPS value
				//SpawnBullets();

				if (previousBps % 2 == 0)
				{
					GenerateEnemyPattern();
				}

				if (previousBps % 8 == 0)
				{
					GenerateEnemylv2Pattern();
				}


			}

		}

		if (duration / 1000 >= 240 && duration / 1000 < 254)
		{
			if (floor(bps) > previousBps) {
				previousBps = floor(bps);  // Update the previous BPS value
				//SpawnBullets();



				if (previousBps % 2 == 0)
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
					score -= 5;
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

				//obstacle->SetRotation(spinT);
			}
			else
			{
				// If obstacle is null, erase it and move to the next
				it = platforms.erase(it);
			}
		}

#pragma endregion

#pragma region Bullet Handling

		timeInterval += game->GetGameTime();

		if (game->GetInputManager()->IsKeyPressed("Attack")) {
			//sprite->PlayAnim("attack");
			SpawnBullets();
		}

		for (Bullet* b : inUseBullets) {
			// If bullet off screen then remove a bullet from in-use container, and insert into ready-to-use container
			if (b->GetPosition().x < -b->sprite->GetScaleWidth() || b->GetPosition().x > game->GetSettings()->screenWidth) {
				readyBullets.push_back(b);
				inUseBullets.erase(remove(inUseBullets.begin(), inUseBullets.end(), b), inUseBullets.end());
			}

			b->Update(game->GetGameTime());
		}

		// Bullet behaviour
		for (Bullet* b : inUseBullets)
		{
			if (game->GetSettings()->screenWidth <= b->GetPosition().x)
			{
				// Remove the bullet from in-use list and return to ready bullets
				readyBullets.push_back(b);
				inUseBullets.erase(remove(inUseBullets.begin(), inUseBullets.end(), b), inUseBullets.end());
				break;  // Exit the loop after handling the collision
			}

			if (game->GetSettings()->screenHeight <= b->GetPosition().y)
			{
				// Remove the bullet from in-use list and return to ready bullets
				readyBullets.push_back(b);
				inUseBullets.erase(remove(inUseBullets.begin(), inUseBullets.end(), b), inUseBullets.end());
				break;  // Exit the loop after handling the collision
			}

			for (auto it = enemies.begin(); it != enemies.end();)
			{
				Sprite* enemy = *it;

				if (b->GetBoundingBox()->CollideWith(enemy->GetBoundingBox()))
				{
					// Increase score when enemy is hit by bullet
					score += 5;
					//text->SetText("Score: " + std::to_string(score));

					// Erase the enemy after collision
					it = enemies.erase(it);

					// Remove the bullet from in-use list and return to ready bullets
					readyBullets.push_back(b);
					inUseBullets.erase(remove(inUseBullets.begin(), inUseBullets.end(), b), inUseBullets.end());
					break;  // Exit the loop after handling the collision
				}
				else
				{
					++it;  // Continue iterating if no collision
				}
			}

			//bullet behaviour for lv2 evilship
			for (auto it = enemies2.begin(); it != enemies2.end();)
			{
				Sprite* enemy = *it;

				if (b->GetBoundingBox()->CollideWith(enemy->GetBoundingBox()))
				{
					// Increase score when enemy is hit by bullet
					enemielv2health -= 10;
					//text->SetText("Score: " + std::to_string(score));

					//If bullet hits 3 time lv2 enemie is dead
					if (enemielv2health <= 0)
					{
						//if enemy die score added by 15
						score += 35;

						// Erase the enemy after collision
						it = enemies2.erase(it);

					}

					// Remove the bullet from in-use list and return to ready bullets
					readyBullets.push_back(b);
					inUseBullets.erase(remove(inUseBullets.begin(), inUseBullets.end(), b), inUseBullets.end());
					break;  // Exit the loop after handling the collision
				}
				else
				{
					++it;  // Continue iterating if no collision
				}
			}
		}

#pragma endregion

#pragma region Enemies Handling

		for (auto it = enemies.begin(); it != enemies.end();)
		{
			Sprite* enemy = *it;

			float enemyX = enemy->GetPosition().x;
			float enemyY = enemy->GetPosition().y;
			float y_2 = enemyY;


			// Check if the enemy should speed up or move toward the sprite
			if (y_2 < (game->GetSettings()->screenHeight / 2 + 200))
			{

				// Get sprite position
				float spriteX = sprite->GetPosition().x;
				float spriteY = sprite->GetPosition().y;

				// Calculate direction vector to the sprite
				float dirX = spriteX - enemyX;
				float dirY = spriteY - enemyY;

				// Normalize direction
				float length = sqrt(dirX * dirX + dirY * dirY);
				if (length != 0)
				{
					dirX /= length;
					dirY /= length;
				}

				// Move enemy toward sprite while decrementing y_2 for normal falling
				float speed = 1.0f; // Adjust this speed
				enemyX += dirX * speed * game->GetGameTime();
				enemyY += dirY * speed * game->GetGameTime();

				// Set new position and continue moving downward
				y_2 -= 1.0f * game->GetGameTime(); // Keep falling
				enemy->SetPosition(enemyX, y_2);

				// Set enemy rotation to face sprite
				float angle = atan2(dirY, dirX) * (180 / M_PI) + 90;
				enemy->SetRotation(angle);

			}
			else
			{
				// Regular downward movement before reaching the threshold
				y_2 -= 0.5f * game->GetGameTime();
				enemy->SetPosition(enemyX, y_2);
			}

			enemy->Update(game->GetGameTime());

			if (enemy->GetBoundingBox()->CollideWith(sprite->GetBoundingBox()))
			{
				it = enemies.erase(it); // Handle collision
				score -= 10;
			}
			else if (y_2 <= -300)
			{
				it = enemies.erase(it); // Remove enemy if off-screen
			}
			else
			{
				++it;
			}

		}

		//level2 enemy
		for (auto it = enemies2.begin(); it != enemies2.end();)
		{
			Sprite* enemy = *it;

			float enemyX = enemy->GetPosition().x;
			float enemyY = enemy->GetPosition().y;
			float y_2 = enemyY;


			// Check if the enemy should speed up or move toward the sprite
			if (y_2 < (game->GetSettings()->screenHeight / 2 + 200)) {

				// Get sprite position
				float spriteX = sprite->GetPosition().x;
				float spriteY = sprite->GetPosition().y;

				// Calculate direction vector to the sprite
				float dirX = spriteX - enemyX;
				float dirY = spriteY - enemyY;

				// Normalize direction
				float length = sqrt(dirX * dirX + dirY * dirY);
				if (length != 0) {
					dirX /= length;
					dirY /= length;
				}

				// Move enemy toward sprite while decrementing y_2 for normal falling
				float speed = 0.7f; // Adjust this speed
				enemyX += dirX * speed * game->GetGameTime();
				enemyY += dirY * speed * game->GetGameTime();

				// Set new position and continue moving downward
				y_2 -= 0.7f * game->GetGameTime(); // Keep falling
				enemy->SetPosition(enemyX, y_2);

				// Set enemy rotation to face sprite
				float angle = atan2(dirY, dirX) * (180 / M_PI) + 90;
				enemy->SetRotation(angle);

			}
			else
			{
				// Regular downward movement before reaching the threshold
				y_2 -= 0.3f * game->GetGameTime();
				enemy->SetPosition(enemyX, y_2);
			}

			enemy->Update(game->GetGameTime());

			if (enemy->GetBoundingBox()->CollideWith(sprite->GetBoundingBox())) {
				it = enemies2.erase(it); // Handle collision
				score -= 55;
			}
			else if (y_2 <= -300) {
				it = enemies2.erase(it); // Remove enemy if off-screen
			}
			else {
				++it;
			}

		}

#pragma endregion


		//sets parallax speed
		MoveLayer(backgrounds, 0.005f);
		MoveLayer(middlegrounds, 0.03f);
		MoveLayer(foregrounds, 0.3f);

	}
	else if (gstate == GameState::FINISH)
	{
		inUseBullets.clear();

		camera.position = glm::vec2(0, 0);

		finish_duration += game->GetGameTime();

		//finish music
		if (finish_music->IsPlaying() == false)
		{
			finish_music->Play(true);
			finish_music->IsPlaying() == true;
		}

		//increases parallax speed
		MoveLayer(backgrounds, 0.02f);
		MoveLayer(middlegrounds, 0.12f);
		MoveLayer(foregrounds, 1.2f);

		//switch to main menu
		if (game->GetInputManager()->IsKeyReleased("mainmenu")) {

			music->Stop();

			finish_music->Stop();

			finish_duration = 0;

			you->SetPosition((game->GetSettings()->screenWidth) - ((game->GetSettings()->screenWidth) * 2), (game->GetSettings()->screenHeight) / 2);

			survived->SetPosition((game->GetSettings()->screenWidth) - ((game->GetSettings()->screenWidth) * 2), (game->GetSettings()->screenHeight) / 3);

			score_finish->SetText(std::to_string(score))->SetPosition((game->GetSettings()->screenWidth) - ((game->GetSettings()->screenWidth) * 2), score_finish->GetPosition().y);

			ResetVariables();

			gstate = GameState::RUNNING;

			music2->Play(true);

			ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");
		}

		//spaceship goes upward
		int y_2 = sprite->GetPosition().y;
		y_2 += 4;

		//sets sprite y value and sets angle to face upward
		sprite->SetPosition(sprite->GetPosition().x, y_2)
			->SetRotation(2);

		//add a reset button
		if (game->GetInputManager()->IsKeyReleased("Reset")) {

			finish_duration = 0;

			you->SetPosition((game->GetSettings()->screenWidth) - ((game->GetSettings()->screenWidth) * 2), (game->GetSettings()->screenHeight) / 2);

			survived->SetPosition((game->GetSettings()->screenWidth) - ((game->GetSettings()->screenWidth) * 2), (game->GetSettings()->screenHeight) / 3);

			score_finish->SetText(std::to_string(score))->SetPosition((game->GetSettings()->screenWidth) - ((game->GetSettings()->screenWidth) * 2), score_finish->GetPosition().y);

			gstate = GameState::RESET;
		}

		//finish sprite goes from left to middle of the screen
		float x_2y = you->GetPosition().x;
		float x_2s = survived->GetPosition().x;

		//logic for "you" sprite goes from left to right
		if (x_2y < ((game->GetSettings()->screenWidth / 3) / 1.1f))
		{
			x_2y += 4.0f * (game->GetGameTime());
		}
		else
		{
			x_2y = (game->GetSettings()->screenWidth / 3) / 1.1f;
		}

		//logic for "survived" sprite goes from left to right and a slight delay
		if ((finish_duration / 1000) >= 1)
		{
			if (x_2s < ((game->GetSettings()->screenWidth / 3) / 1.1f))
			{
				x_2s += 4.0f * (game->GetGameTime());
			}
			else
			{
				x_2s = (game->GetSettings()->screenWidth / 3) / 1.1f;
			}
		}

		you->SetPosition(x_2y, game->GetSettings()->screenHeight / 2);
		survived->SetPosition(x_2s, game->GetSettings()->screenHeight / 3);

		you->Update(game->GetGameTime());
		survived->Update(game->GetGameTime());

		text1->SetText(" ");

		// Get current x position of the score text
		x_score = score_finish->GetPosition().x;

		// Logic for score text movement with delay, stops at center screen
		if ((finish_duration / 1000) >= 1)
		{
			// Set initial position
			score_finish->SetText(std::to_string(score))->SetPosition(1, score_finish->GetPosition().y);

			// Move the score text to the right if it hasn't reached the center
			if (x_score < (game->GetSettings()->screenWidth / 2.15f))
			{
				x_score += 5 * game->GetGameTime();  // Adjust the speed as needed
			}
			else
			{
				x_score = (game->GetSettings()->screenWidth / 2.15f); // Stop at center
			}

			// Update position
			score_finish->SetPosition(x_score, score_finish->GetPosition().y);

			// Debugging print
			std::cout << x_score << std::endl;
		}

		std::cout << "Game state is at finish" << std::endl;
	}
	else if (gstate == GameState::GAME_OVER)
	{
		std::cout << "Game state is at game over" << std::endl;

		camera.position = glm::vec2(0, 0);

		//gameover music
		if (gov_music->IsPlaying() == false)
		{
			gov_music->Play(true);
			gov_music->IsPlaying() == true;
		}

		//make sure score value doesnt pass below 0
		if (score <= 0)
		{
			score = 0;

			text1->SetText(std::to_string(score));
		}

		std::cout << std::to_string(score) << std::endl;

		//game over sprite slowly goes down to player screen
		float y_2 = gov->GetPosition().y;

		if (y_2 >= (((game->GetSettings()->screenHeight) / 4) * 1.5f))
		{
			y_2 -= 3.0f * (game->GetGameTime());
		}

		if (y_2 <= (((game->GetSettings()->screenHeight) / 4) * 1.5f))
		{
			y_2 = gov->GetPosition().y;
		}

		//set game over sprite to the screen view
		gov->SetPosition((((game->GetSettings()->screenWidth) / 3) * 1.19f), y_2);

		//updates the game over sprite
		gov->Update(game->GetGameTime());

		//plays the animation 
		gov->PlayAnim("idle");

		//stops the parallax speed
		MoveLayer(backgrounds, 0.0f);
		MoveLayer(middlegrounds, 0.0f);
		MoveLayer(foregrounds, 0.0f);

		//swtich to main menu if key is "pressed"
		if (game->GetInputManager()->IsKeyReleased("mainmenu"))
		{
			music->Stop();

			gov_music->Stop();

			ResetVariables();

			gov->SetPosition((((game->GetSettings()->screenWidth) / 3) * 1.19f), (game->GetSettings()->screenHeight) * 1.5f);

			gstate = GameState::RUNNING;

			ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");

			music2->Play(true);

		}

		//reset button
		if (game->GetInputManager()->IsKeyReleased("Reset")) {

			gov->SetPosition((((game->GetSettings()->screenWidth) / 3) * 1.19f), (game->GetSettings()->screenHeight) * 1.5f);
			gstate = GameState::RESET;
		}
	}
	else if (gstate == GameState::RESET)
	{
		std::cout << "Game resets" << std::endl;

		ResetVariables();

		finish_music->Stop();

		gov_music->Stop();

		gstate = GameState::RUNNING;
	}

}

void Engine::TutorialScreen::Draw()
{

#pragma region Camera Render

	glUseProgram(game->GetDefaultSpriteShader()->GetId());

	// Set the camera's view matrix
	glm::mat4 view = camera.GetViewMatrix(game->GetSettings()->screenWidth, game->GetSettings()->screenHeight);
	glUniformMatrix4fv(glGetUniformLocation(game->GetDefaultSpriteShader()->GetId(), "view"), 1, GL_FALSE, glm::value_ptr(view));

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
		//std::cout << "obstacle sprite is drawn" << std::endl;

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
		//std::cout << "enemy sprite is drawn" << std::endl;
	}

	for (Sprite* e : enemies2)
	{
		e->Draw();
		//std::cout << "enemy lv2 sprite is drawn" << std::endl;
	}

	DrawLayer(foregrounds);

	//Score
	text1->Draw();

	//Draw Rotation Point
	//sprite2->Draw();

	//doesnt work
	if (gstate == GameState::GAME_OVER)
	{
		//Draw game over sprite
		gov->Draw();
		//std::cout << "game over sprite is drawn" << std::endl;

		reset_txt->Draw();
		back_txt->Draw();
	}

	if (gstate == GameState::FINISH)
	{
		you->Draw();
		survived->Draw();
		score_finish->Draw();

		reset_txt->Draw();
		back_txt->Draw();
	}

}

void Engine::TutorialScreen::ResetVariables()
{
	//resets bullets
	inUseBullets.clear();

	//resets meteor
	platforms.clear();

	//resets enemies
	enemies.clear();
	enemies2.clear();
	//enemielv2health = 30;

	//resets other variables
	score = 500;
	duration = 0;
	previousBps = 0;
	bps = 0;

	spawnTimestamps;
	currentTimestampIndex = 0;

	//resets sprite position
	sprite->SetPosition(game->GetSettings()->screenWidth / 2.15f, (game->GetSettings()->screenHeight / 12) - 50.0f);

	camera.position = glm::vec2(0, 0);

}

#pragma region Obstacle Spawning

void Engine::TutorialScreen::SpawnObstacle(float xPosition)
{

	Texture* platformTexture = new Texture("meteor.png");
	vec2 start = vec2(xPosition, game->GetSettings()->screenHeight);  // Starting position (top of the screen)

	Sprite* platformSprite = new Sprite(platformTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	platformSprite->SetSize(100, 100)->SetPosition(start.x, start.y);
	platformSprite->SetBoundingBoxSize(platformSprite->GetScaleWidth() - (3.5 * platformSprite->GetScale()), platformSprite->GetScaleHeight() - 50);



	platforms.push_back(platformSprite);  // Add the new obstacle to the list
}

void Engine::TutorialScreen::GenerateObstaclePattern()
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
void Engine::TutorialScreen::SpawnEnemies(float xPosition) {
	Texture* enemyTexture = new Texture("evilShip.png");
	vec2 start = vec2(xPosition, game->GetSettings()->screenHeight);  // Starting position (top of screen)

	Sprite* enemySprite = new Sprite(enemyTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	enemySprite->SetSize(100, 100)->SetPosition(start.x, start.y)->SetFlipVertical(true);
	enemySprite->SetBoundingBoxSize(enemySprite->GetScaleWidth() - (3.5 * enemySprite->GetScale()), enemySprite->GetScaleHeight() - 50);

	enemies.push_back(enemySprite);  // Add enemy ship to the list
}

void Engine::TutorialScreen::GenerateEnemyPattern() {
	float randomX = rand() % (int)((game->GetSettings()->screenWidth));  // Random X position
	SpawnEnemies(randomX);  // Spawn an enemy at the random X position
}

//Mid EvilShip
void Engine::TutorialScreen::SpawnEnemieslv2(float xPosition) {
	Texture* enemyTexture2 = new Texture("evilShip2.png");
	vec2 start = vec2(xPosition, game->GetSettings()->screenHeight);  // Starting position (top of screen)

	Sprite* enemySprite2 = new Sprite(enemyTexture2, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	enemySprite2->SetSize(200, 200)->SetPosition(start.x, start.y)->SetFlipVertical(true);
	enemySprite2->SetBoundingBoxSize(enemySprite2->GetScaleWidth() - (3.5 * enemySprite2->GetScale()), enemySprite2->GetScaleHeight() - 50);

	enemies2.push_back(enemySprite2);  // Add enemy ship to the list

	enemielv2health = 50;
}

void Engine::TutorialScreen::GenerateEnemylv2Pattern() {

	float minX = game->GetSettings()->screenWidth / 3;
	float maxX = (game->GetSettings()->screenWidth / 3) * 2;
	float randomX = minX + (rand() % (int)(maxX - minX + 1));  // Random X between minX and maxX

	SpawnEnemieslv2(randomX);  // Spawn an enemy at the random X position
}

//Large Evil Ship
void Engine::TutorialScreen::SpawnEnemieslv3(float xPosition) {
	Texture* enemyTexture3 = new Texture("evilShip3.png");
	vec2 start = vec2(xPosition, game->GetSettings()->screenHeight);  // Starting position (top of screen)

	Sprite* enemySprite3 = new Sprite(enemyTexture3, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	enemySprite3->SetSize(300, 300)->SetPosition(start.x, start.y)->SetFlipVertical(true);
	enemySprite3->SetBoundingBoxSize(enemySprite3->GetScaleWidth() - (3.5 * enemySprite3->GetScale()), enemySprite3->GetScaleHeight() - 50);

	enemies3.push_back(enemySprite3);  // Add enemy ship to the list
}

void Engine::TutorialScreen::GenerateEnemylv3Pattern() {
	float randomX = rand() % (int)((game->GetSettings()->screenWidth));  // Random X position
	SpawnEnemieslv3(randomX);  // Spawn an enemy at the random X position
}



#pragma endregion

#pragma region Parallax Functions

void Engine::TutorialScreen::MoveLayer(vector<Sprite*>& bg, float speed)
{
	for (Sprite* s : bg) {
		if (s->GetPosition().y < -game->GetSettings()->screenHeight + offset) {
			s->SetPosition(0, game->GetSettings()->screenHeight + offset - 1);
		}
		s->SetPosition(s->GetPosition().x, s->GetPosition().y - speed * game->GetGameTime());
		s->Update(game->GetGameTime());
	}
}

void Engine::TutorialScreen::DrawLayer(vector<Sprite*>& bg)
{
	for (Sprite* s : bg) {
		s->Draw();
	}
}

void Engine::TutorialScreen::AddToLayer(vector<Sprite*>& bg, string name)
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

void Engine::TutorialScreen::SpawnBullets()
{
	if (timeInterval >= 150) {
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

		glm::vec2 screenCenter(game->GetSettings()->screenWidth / 2.0f, game->GetSettings()->screenHeight);

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
