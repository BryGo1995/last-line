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

bool Asteroid::boxCircleCollision(GameObject& object) {

	// Calculate the asteroid center
	glm::vec2 center;
	center.x = this->Position.x + (this->Size.x / 2);
	center.y = this->Position.y + (this->Size.y / 2);

	// Find the closest edge to the asteroid
	glm::vec2 edge;
	if (center.x < object.Position.x) {
		edge.x = object.Position.x;
	}
	else {
		edge.x = object.Position.x + object.Size.x;
	}

	if (center.y < object.Position.y) {
		edge.y = object.Position.y;
	}
	else {
		edge.y = object.Position.y + object.Size.y;
	}

	// Take the difference between the two vectors
	glm::vec2 diff = edge - center;
	
	// Check if the difference is smaller than the radius
	return glm::length(diff) < this->hitboxRadius;
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