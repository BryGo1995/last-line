#include"Asteroid.h"

void Asteroid::updateLocation(float dt) {

	// Add the velocity to the postion
	this->Position += this->Velocity * dt;
}

bool Asteroid::checkCollision(GameObject* object) {

	// Collision on x axis
	bool collisionX = this->Position.x + this->Size.x >= object->Position.x &&
		object->Position.x + object->Size.x >= this->Position.x;

	// Collision on y axis
	bool collisionY = this->Position.y + this->Size.y >= object->Position.y && 
		object->Position.y + object->Size.y >= this->Position.y;

	return collisionX && collisionY;
}

bool Asteroid::checkBulletCollision(GameObject& bullet) {

	// Collision on x axis
	bool collisionX = this->Position.x + this->Size.x >= bullet.Position.x &&
		bullet.Position.x + bullet.Size.x >= this->Position.x;

	// Collision on y axis
	bool collisionY = this->Position.y + this->Size.y >= bullet.Position.y &&
		bullet.Position.y + bullet.Size.y >= this->Position.y;

	return collisionX && collisionY;
}