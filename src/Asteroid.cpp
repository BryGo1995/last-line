#include"Asteroid.h"

void Asteroid::updateLocation(float dt) {

	// Add the velocity to the postion
	this->Position += this->Velocity * dt;
}

bool Asteroid::boxBoxCollision(GameObject* object) {

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

bool Asteroid::circleCircleCollision(GameObject* object) {

	// Calculate the radius of both circular hitboxes
	float asteroidRadius = this->Size.x / 2;
	float objectRadius = object->Size.x / 2;

	// Calculate the center of the asteroid and objects
	glm::vec2 asteroidCenter, objectCenter;
	asteroidCenter = glm::vec2(this->Position.x + this->Size.x / 2,
		this->Position.y + this->Size.y / 2);
	objectCenter = glm::vec2(object->Position.x + object->Size.x / 2,
		object->Position.y + object->Size.y / 2);

	// Calculate the distance between the two centers
	float distance = glm::length(asteroidCenter - objectCenter);

	// Compare the distance to the sum of the two radii
	return distance <= (asteroidRadius + objectRadius);
}