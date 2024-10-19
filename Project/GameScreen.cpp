#include "GameScreen.h"

#pragma region TO DO
/*
//////// Gameplay Explanation /////////

   - sprite can move to left center and right
   - movement limited with 3 section
   - sprite rotates according mousepos
   - sprite shoot bullets
   - enemies follow player pos
   - enemie dead in one shot with bullets
   - obstacle spawns randomly on any of the 3 section
   - obstacle spawns following (beat-ish) making player
	 has to semi avoid obstacle according to beat

///////////// Screens ////////////////

	- Main Menu
	  - Every Button Has SFX
	  - Use FNF Intro Music for BGM
	  - Buttons :
		- Play
		- Controls
		- Exit

	- Controls Screen
	  - Displays Controls Text

	- In Game Screen

///////////////////////////////////////

//////////// Game Play ////////////////

	- Create a class to handle the spaceship and move the functions to there

	- In Game State
	  - Has 3 Section for sprite to move (Done) (9 - 10 Oct)

	  - Avoid enenmies according to the beat (you can like make it semi according to beat)

	  - Map notes with rhythm plus based on the time mapped (Done) (9 - 12 Oct)
	  - Add a bps counter (doesnt really need to but Done) (9 - 12 Oct)
	  - Add Song Bacgkround (9 - 12 Oct)
	  - Add MousePos follow sprite angle (9 - 12 Oct)

	- Gameplay
	  - Move Left and Right and Center (Done) (9 - 12 Oct)

	  - When Space button pressed
		1. Plays a jumping or zooming in animation
		2. make the sprite hides its bounding box

	  - Sprite angle follows mouse pos (Done) (13 - 14 Oct)

	  - Player can shoot bullets (Add Bounding Box to kill enemies)

	- Obstacles (when shot at nothing happens, when hit you ded)
	  - Try implementing obejct pooling
	  - 3 objects spawns according to beat
	  - if not just do it manually but not recommend

	- Enemies (when you kill it you get 25 score if not then -10) (create a class for this)
	  - Randomly Spawn and automatically aims to the main sprite
	   or same like obstacle
	   so its spawns accordingly to the beat replacing some obstacles

	- Bullets
	  - Add Bullets spawning (Done) (9 - 12 Oct)
	  - Bullet follows Sprite Angle and mouse pos (13 - 14 Oct)
	  - Add Bullets BoundingBox

	- Finish State
	  - when the song ends

///////////////////////////////////////

	- Sprite
	 - movement on 3 section (Done)
	 - move angle follow mousepos (Done)
	 - add bullets
		- add bounding box (Done 15 Oct)
		- follow sprite angle (Done)
		- bullet sprite also rotates when ship sprite rotates (19 Oct)
	 - add bounding box (Done 15 Oct)
	 - death and alive state

	- Enemies
	 - add bounding box
	 - follow player position
	 - death and alive state

	- Obstacle
	 - spawns on 3 section where sprite moves
	 - spawns according to the beat (beat-ish)

	- Gameplay
	 - add a feature Divide into 2 screen (17 Oct)
	 - Main Menu
	  - Customize Buttons and Titles
		- SFX and Sprite
	 - In game
	  - Sprite Enemies Obstacle
	 - Quit
	 - Parallax Background (Done)
	 - Add Music (Done)
	 - Add Sfx
	 - Add Beat Counter (Done)

*/

#pragma endregion

Engine::GameScreen::GameScreen()
{
	delete texture_N1;
	//delete sprite;
}

void Engine::GameScreen::Init()
{
	// Spawn setting
	maxSpawnTime = 300;
	numObjectPerSpawn = 1;
	numObjectsInPool = 50;

	// Load a texture
	Texture* texture = new Texture("turtles.png");
	for (int i = 0; i < numObjectsInPool; i++) {
		Turtle* o = new Turtle(CreateSprite(texture));
		objects.push_back(o);
	}

	// Add input mappings
	game->GetInputManager()->AddInputMapping("mainmenu", SDLK_ESCAPE);

	////////////

#pragma region Sprite and Background

	// Init Notes Sprite 
	texture_N1 = new Texture("ship.png");
	sprite = new Sprite(texture_N1, game->GetDefaultSpriteShader(), game->GetDefaultQuad());

	texture_bg = new Texture("Space.png");
	bg = (new Sprite(texture_bg, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetSize((float)game->GetSettings()->screenWidth, (float)game->GetSettings()->screenHeight);

	sprite->SetScale(0.30)
		->SetPosition(game->GetSettings()->screenWidth / 2.15, (game->GetSettings()->screenHeight / 12) - 50)
		->SetBoundingBoxSize(sprite->GetScaleHeight(), sprite->GetScaleWidth());


#pragma endregion

#pragma region Camera init

	//cam_follow = new Game(defaultProjection);

#pragma endregion

#pragma region Music and Sound init

	music = (new Music("Shirobon-On-The-Run.ogg"))->SetVolume(45)->Play(false);

	//metronome = (new Sound("beep.ogg"))->SetVolume(70)->Play(true);

	//hit = (new Sound(""))->SetVolume(70);

#pragma endregion

#pragma region Input Init

	game->GetInputManager()->AddInputMapping("Move Right", SDLK_d)
		->AddInputMapping("Move Left", SDLK_a)
		->AddInputMapping("Move Center", SDLK_s)
		->AddInputMapping("Attack", SDL_BUTTON_LEFT)
		->AddInputMapping("Avoid", SDLK_SPACE)
		->AddInputMapping("Quit", SDLK_ESCAPE)
		->AddInputMapping("Quit", SDL_CONTROLLER_BUTTON_Y);

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

	Texture* platformTexture = new Texture("cathy.png");
	vec2 start = vec2(game->GetSettings()->screenWidth / 2.15, game->GetSettings()->screenHeight);
	for (int i = 0; i < 1; i++) {
		Sprite* platformSprite = new Sprite(platformTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
		platformSprite->SetSize(100, 100)->SetPosition(start.x, start.y);
		platformSprite->SetBoundingBoxSize(platformSprite->GetScaleWidth() - (3.5 * platformSprite->GetScale()), platformSprite->GetScaleHeight() - 10);
		platforms.push_back(platformSprite);
	}

#pragma endregion

#pragma region Bullet Init

	//Add bullets
	bulletTexture = new Texture("projectiles.png");
	int bulletNum = 100000;
	for (int i = 0; i < bulletNum; i++) {
		Sprite* bs = (new Sprite(bulletTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))->SetNumXFrames(3)
																										->SetNumYFrames(2)
																										->SetScale(0.07)
																										->SetBoundingBoxSize((0.7 * 1) / 1, (0.7 * 1) / 1)
																										->AddAnimation("idle", 1,1)
																										->PlayAnim("idle");
		readyBullets.push_back(new Bullet(bs));
	}

#pragma endregion

}

void Engine::GameScreen::Update()
{
	// Back to main menu
	if (game->GetInputManager()->IsKeyReleased("mainmenu")) {
		ScreenManager::GetInstance(game)->SetCurrentScreen("mainmenu");
	}

	// Set background
	game->SetBackgroundColor(235, 229, 52);

	// Time to spawn objects
	if (spawnDuration >= maxSpawnTime) {
		SpawnObjects();
		spawnDuration = 0;
	}
	// Update all objects
	for (Turtle* o : objects) {
		o->Update((game->GetGameTime()));
	}
	// Count spawn duration
	spawnDuration += (game->GetGameTime());

	//////////////

#pragma region Camera Handling

	//glm::mat4 view = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	//glm::mat4 projection = glm::perspective(45.0f, (GLfloat)this->screenWidth / (GLfloat)this->screenHeight, 0.1f, 100.0f);
	//glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(setting->screenWidth), 0.0f, static_cast<GLfloat>(setting->screenHeight), -1.0f, 1.0f);

	//vec2 notePosition = note1->GetPosition();

	// Define camera view matrix to follow note1's position
	//glm::vec3 cameraPosition(notePosition.x, notePosition.y, 0.0f); // Camera positioned behind the scene
	//view = glm::lookAt(cameraPosition, glm::vec3(notePosition.x, notePosition.y, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Update the shader with the view matrix to apply the camera
	//shader->setMat4(view, "view");

	//std::cout << "Camera Position: " << cameraPosition.x << ", " << cameraPosition.y << ", " << cameraPosition.z << std::endl;

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

#pragma region sprite angle according to mouse pos

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

	float angle = atan2(mouseDirection.y, -mouseDirection.x) * (180 / M_PI) + 90;

	sprite->SetRotation(angle);

#pragma endregion

#pragma region Obstacle handling

	//float duration;
	duration += game->GetGameTime();

	//1 beat = 1 / 3 of a second
	bps = (duration / 1000) * 2.6; //song end in around beat 437?

	// Add function where every time the value of bps is added by 1 the sound metronome is played 

	//metronome->Stop(); //true will loop the sound based on game time which we dont want

	//if (bps % 0) {
	//	SpawnBullets();  // Play the sound once
	//}

	int previousBps = 0;

	if (floor(bps) > previousBps) {
		previousBps = floor(bps);  // Update the previous BPS value
		SpawnBullets();  // Spawn Obstacle

	}

	std::cout << "Beats : " << bps << std::endl;

	// when sprite hit an obstacle the sate switch to game over
	for (Sprite* s : platforms) {
		if (s->GetBoundingBox()->CollideWith(sprite->GetBoundingBox())) {
			//gstate = GameState::GAME_OVER;
			//return;
			NULL;
		}
	}

	for (Sprite* s : platforms)
	{

		float x_ = s->GetPosition().x;
		float y_ = s->GetPosition().y;
		float velocity = 0.5f;

		y_ -= velocity * game->GetGameTime();
		s->SetFlipHorizontal(false)
			->SetPosition(x_, y_)->Update(game->GetGameTime());

		std::cout << "The Value of x_ is : " << std::endl;
		std::cout << to_string(y_) << std::endl;

		if (y_ <= -200)
		{
			s->SetPosition(game->GetSettings()->screenWidth / 2.15, game->GetSettings()->screenHeight);
		}

	}

#pragma endregion

#pragma region Bullet Handling

	timeInterval += game->GetGameTime();

	if (game->GetInputManager()->IsKeyPressed("Attack")) {
		sprite->PlayAnim("attack");
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

	for (Bullet* b : inUseBullets) {
		for (Sprite* s : platforms) {
			if (b->GetBoundingBox()->CollideWith(s->GetBoundingBox())) {
				// Reset obstacle position
				s->SetPosition(game->GetSettings()->screenWidth / 2.15, game->GetSettings()->screenHeight);

				// Remove the bullet from in-use list and return to ready bullets
				readyBullets.push_back(b);
				inUseBullets.erase(remove(inUseBullets.begin(), inUseBullets.end(), b), inUseBullets.end());
				break;
			}
		}
	}

#pragma endregion

	MoveLayer(backgrounds, 0.005f);
	MoveLayer(middlegrounds, 0.03f);
	MoveLayer(foregrounds, 0.3f);

}

void Engine::GameScreen::Draw()
{
	// Render all objects
	//for (Turtle* o : objects) {
	//	o->Draw();
	//}

	/////////////

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
	for (Sprite* s : platforms) {
		s->Draw();
	}

	//Bullets
	for (Bullet* b : inUseBullets) {
		b->Draw();
	}
	
	//Draw Note Sprite
	sprite->Draw();


	//for (Turtle* o : objects) {
	//	o->Draw();
	//}

	DrawLayer(foregrounds);


}

Engine::Sprite* Engine::GameScreen::CreateSprite(Texture* texture)
{
	return (new Sprite(texture, game->GetDefaultSpriteShader(), game->GetDefaultQuad()))
		->SetNumXFrames(14)
		->SetNumYFrames(4)
		->AddAnimation("hit", 2, 4)
		->AddAnimation("spikes", 5, 12)
		->AddAnimation("idle-1", 14, 27)
		->AddAnimation("idle-2", 28, 41)
		->AddAnimation("spikes-out", 42, 49)
		->PlayAnim("spikes")->SetScale(1.5)
		->SetAnimationDuration(100);
}

void Engine::GameScreen::SpawnObjects()
{
	//Find Die object to reuse for spawning
	int spawnCount = 0;
	for (Turtle* o : objects) {
		if (spawnCount == numObjectPerSpawn) {
			break;
		}
		if (o->IsDie()) {
			// Set state to spawn

			// Random spawn position
			int min = 0;
			int max = (int)(game->GetSettings()->screenWidth - o->GetWidth());
			float x = (float)(rand() % (max - min + 1) + min);
			float y = (game->GetSettings()->screenHeight - 200) + o->GetHeight();
			o->SetSpawn()->SetPosition(x, y);
			spawnCount++;
		}
	}
}

#pragma region Enemies (EvilShip) Handling

#pragma endregion

#pragma region Parallax Functions

void Engine::GameScreen::MoveLayer(vector<Sprite*>& bg, float speed)
{
	for (Sprite* s : bg) {
		if (s->GetPosition().y < -game->GetSettings()->screenHeight + offset) {
			s->SetPosition(0, game->GetSettings()->screenHeight + offset - 1);
		}
		s->SetPosition(s->GetPosition().x, s->GetPosition().y - speed * game->GetGameTime());
		s->Update(game->GetGameTime());
	}
}

void Engine::GameScreen::DrawLayer(vector<Sprite*>& bg)
{
	for (Sprite* s : bg) {
		s->Draw();
	}
}

void Engine::GameScreen::AddToLayer(vector<Sprite*>& bg, string name)
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

void Engine::GameScreen::SpawnBullets()
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


