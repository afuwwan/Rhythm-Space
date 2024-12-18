#include "Bullet.h"
#include "BoundingBox.h"

Engine::Bullet::Bullet(Sprite* sprite)
{
	this->sprite = sprite;
}

Engine::Bullet::~Bullet()
{
}

void Engine::Bullet::SetPosition(float x, float y)
{
	sprite->SetPosition(x, y);
}

vec2 Engine::Bullet::GetPosition()
{
	return sprite->GetPosition();
}

Engine::Sprite* Engine::Bullet::SetRotation(float degree)
{
	return sprite->SetRotation(degree);
}

Engine::BoundingBox* Engine::Bullet::GetBoundingBox()
{
	return sprite->GetBoundingBox();
}

void Engine::Bullet::Update(float deltaTime)
{
	
	float x = GetPosition().x;
	x += xVelocity * deltaTime;
	SetPosition(x, GetPosition().y);
	sprite->Update(deltaTime);

	//bullet move up top
	float y = GetPosition().y;
	y += yVelocity * deltaTime;
	SetPosition(GetPosition().x, y);
	sprite->Update(deltaTime);
}

void Engine::Bullet::Draw()
{
	sprite->Draw();
}
