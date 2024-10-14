#ifndef BULLET_H
#define BULLET_H

#include "Sprite.h"

namespace Engine {
	class Bullet
	{
	public:
		Bullet(Sprite* sprite);
		~Bullet();
		float xVelocity = 0;
		float yVelocity = 0;
		Sprite* sprite;
		void SetPosition(float x, float y);
		//void SetRotation(float degree);
		vec2 GetPosition();
		//vec2 GetRotation();
		void Update(float deltaTime);
		void Draw();
	};
}
#endif

