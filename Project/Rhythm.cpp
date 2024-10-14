#include "Rhythm.h"

#pragma region TO DO
/*

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
	  - Has 3 Notes
	  - Avoid enenmies according to the beat (you can like make it semi according to beat)
	  - Map notes with rhythm plus based on the time mapped (Done)
	  - Add a bps counter (doesnt really need to but Done)

	- Gameplay
	  - Move Left and Right and Center (Done)

	  - When 3 button pressed 
		1. Plays a jumping or zooming in animation
		2. make the sprite hides its bounding box

	  - Player can shoot bullets 

	- Obstacles (when shot at nothing happens, when hit you ded)
	  - Try implementing obejct pooling 
	  - 3 objects spawns according to beat
	  - if not just do it manually but not recommend

	- Enemies (when you kill it you get 25 score if not then -10) (create a class for this)
	  - Randomly Spawn and automatically aims to the main sprite
	   or same like obstacle 

	   so its spawns accordingly to the beat replacing some obstacles


	- Finish State 
	  - when the song ends

///////////////////////////////////////

*/

#pragma endregion


Engine::Rhythm::Rhythm(Setting* setting) :Engine::Game(setting)
{
	setting->windowTitle = "Rhythm Space";
}

Engine::Rhythm::~Rhythm()
{
	delete texture_N1;
	//delete sprite;
}

//void Engine::Rhythm::setupCamera()
//{
//	// LookAt camera (position, target/direction, up)
//	viewPos = glm::vec3(0, 0, 0);
//	glm::mat4 view = glm::lookAt(viewPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
//	shader->setMat4("view", view);
//
//}


//GLFWwindow* wind = 0;
//glm::ivec2 getRelMousePosition()
//{
//	double x = 0, y = 0;
//	glfwGetCursorPos(wind, &x, &y);
//	return { x, y };
//}

void Engine::Rhythm::Init()
{

#pragma region Sprite and Background

	// Init Notes Sprite 
	texture_N1 = new Texture("ship.png");
	sprite = new Sprite(texture_N1, defaultSpriteShader, defaultQuad);

	texture_bg = new Texture("Space.png");
	bg = (new Sprite(texture_bg, defaultSpriteShader, defaultQuad))->SetSize((float)setting->screenWidth, (float)setting->screenHeight);

	sprite->SetScale(0.30)
		 ->SetPosition(setting->screenWidth / 2.15, (setting->screenHeight / 12) - 50);


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

	inputManager->AddInputMapping("Move Right", SDLK_d)
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

	//Texture* platformTexture = new Texture("cathy.png");
	//vec2 start = vec2(2000, 0);
	//for (int i = 0; i < 1; i++) {
	//	Sprite* platformSprite = new Sprite(platformTexture, game->GetDefaultSpriteShader(), game->GetDefaultQuad());
	//	platformSprite->SetSize(150, 200)->SetPosition(start.x + i * 1000, start.y);
	//	platformSprite->SetBoundingBoxSize(platformSprite->GetScaleWidth() - (3.5 * platformSprite->GetScale()), platformSprite->GetScaleHeight() - 10);
	//	platforms.push_back(platformSprite);
	//}

#pragma endregion

#pragma region Bullet Init

	 //Add bullets
	bulletTexture = new Texture("bullet.png");
	int bulletNum = 100000;
	for (int i = 0; i < bulletNum; i++) {
		Sprite* bs = (new Sprite(bulletTexture, defaultSpriteShader, defaultQuad))->SetNumXFrames(1)->SetNumYFrames(1)->SetScale(1);
		readyBullets.push_back(new Bullet(bs));
	}

#pragma endregion


}

void Engine::Rhythm::Update()
{

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
	
	//float duration;
	duration += GetGameTime();

	//1 beat = 1 / 3 of a second
	bps = (duration / 1000) *  2.7; //song end in around beat 437?

	// Add function where every time the value of bps is added by 1 the sound metronome is played 

	//metronome->Stop(); //true will loop the sound based on game time which we dont want

	if (bps % 4) {
		//metronome->Play(false);  // Play the sound once
	}

	std::cout << "Beats : " << bps << std::endl;

#pragma region Movement

	vec2 oldMonsterPos = sprite->GetPosition();
	float x = oldMonsterPos.x, y = oldMonsterPos.y;
	if (inputManager->IsKeyPressed("Move Right")) {
		x = (setting->screenWidth / 2.15) + 200;
		//note1->PlayAnim("walk")->SetFlipHorizontal(false)->SetRotation(0);
	}
	if (inputManager->IsKeyPressed("Move Left")) {
		x = (setting->screenWidth / 2.15) - 200;
		//note1->PlayAnim("walk")->SetFlipHorizontal(true)->SetRotation(0);
	}
	if (inputManager->IsKeyPressed("Move Center")) {
		x = setting->screenWidth / 2.15;
		//note1->PlayAnim("walk")->SetFlipHorizontal(true)->SetRotation(0);
	}
	if (inputManager->IsKeyPressed("Avoid")) {
		x = setting->screenWidth / 2.15;
		// Remove Bounding Box
		// Plays Jumping Animation
	}

	sprite->Update(GetGameTime());
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

	glm::vec2 screenCenter(setting->screenWidth / 2.0f, setting->screenHeight / 2.0f);

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

	// when sprite hit an obstacle the sate switch to game over
	//for (Sprite* s : platforms) {
	//	if (s->GetBoundingBox()->CollideWith(sprite->GetBoundingBox())) {
	//		gstate = GameState::GAME_OVER;
	//		return;
	//	}
	//}

	//for (Sprite* s : platforms)
	//{

	//	float x_ = s->GetPosition().x;
	//	float y_ = 198;
	//	float velocity = 0.7f;

	//	x_ -= velocity * game->GetGameTime();
	//	s->SetFlipHorizontal(true)
	//		->SetPosition(x_, y_)->Update(game->GetGameTime());

	//	std::cout << "The Value of x_ is : " << std::endl;
	//	std::cout << to_string(x) << std::endl;

	//	if (x_ <= -200)
	//	{
	//		s->SetPosition(2000, 198);
	//	}

	//}

#pragma endregion

#pragma region Bullet Handling

	timeInterval += GetGameTime();

	if (inputManager->IsKeyPressed("Attack")) {
		sprite->PlayAnim("attack");
		SpawnBullets();
	}

	for (Bullet* b : inUseBullets) {
		// If bullet off screen then remove a bullet from in-use container, and insert into ready-to-use container
		if (b->GetPosition().x < -b->sprite->GetScaleWidth() || b->GetPosition().x > setting->screenWidth) {
			readyBullets.push_back(b);
			inUseBullets.erase(remove(inUseBullets.begin(), inUseBullets.end(), b), inUseBullets.end());
		}

		b->Update(GetGameTime());
	}

#pragma endregion


	MoveLayer(backgrounds, 0.005f);
	MoveLayer(middlegrounds, 0.03f);
	MoveLayer(foregrounds, 0.3f);
}

void Engine::Rhythm::Render()
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

	// Draw Background Sprite
	bg->Draw();


	// Draw parallax background
	DrawLayer(backgrounds);
	DrawLayer(middlegrounds);
	DrawLayer(foregrounds);

	//Obstacle
	//for (Sprite* s : platforms) {
	//		s->Draw();
	//}

	// Draw Note Sprite
	sprite->Draw();

	//Bullets
	for (Bullet* b : inUseBullets) {
		b->Draw();
	}


}

#pragma region Enemies (EvilShip) Handling

#pragma endregion


#pragma region Parallax Functions

void Engine::Rhythm::MoveLayer(vector<Sprite*>& bg, float speed)
{
	for (Sprite* s : bg) {
		if (s->GetPosition().y < -setting->screenHeight + offset) {
			s->SetPosition(0, setting->screenHeight + offset - 1);
		}
		s->SetPosition(s->GetPosition().x, s->GetPosition().y - speed * GetGameTime());
		s->Update(GetGameTime());
	}
}

void Engine::Rhythm::DrawLayer(vector<Sprite*>& bg)
{
	for (Sprite* s : bg) {
		s->Draw();
	}
}

void Engine::Rhythm::AddToLayer(vector<Sprite*>& bg, string name)
{
	Texture* texture = new Texture(name);

	Sprite* s = new Sprite(texture, defaultSpriteShader, defaultQuad);
	s->SetSize(setting->screenWidth, setting->screenHeight + offset);
	bg.push_back(s);

	Sprite* s2 = new Sprite(texture, defaultSpriteShader, defaultQuad);
	s2->SetSize(setting->screenWidth, setting->screenHeight + offset)->SetPosition(0, setting->screenHeight + offset - 1);
	bg.push_back(s2);
}

#pragma endregion

#pragma region Bullet handling

void Engine::Rhythm::SpawnBullets()
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
		float bulletStartX = sprite->GetPosition().x + (spriteWidth / 2.0f);
		float bulletStartY = sprite->GetPosition().y + (spriteHeight / 2.0f);

		// Set the bullet's initial position to the (0.5, 1) texture coordinate
		b->SetPosition(bulletStartX, bulletStartY);
		
		// Get the angle of the spaceship (sprite rotation)
		float angle = glm::radians(sprite->GetRotation() + 90);

		// Calculate x and y velocity based on the angle
		float speed = 0.75f;  // Adjust this as needed
		b->xVelocity = cos(angle) * speed;
		b->yVelocity = sin(angle) * speed;
		
		inUseBullets.push_back(b);
		timeInterval = 0;

	}

}

#pragma endregion



