#pragma once

#include"GameObject.h"
#include"Spaceship.h"

class Asteroid : public GameObject {

public:

	bool isDestroyed;
	float hitboxRadius;

	Asteroid(bool destroyed, glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
		: GameObject(pos, size, sprite, color, velocity), isDestroyed(destroyed) {};

	void updateLocation(float dt);
	bool boxBoxCollision(GameObject* object);
	bool boxCircleCollision(GameObject& object);
	bool circleCircleCollision(GameObject* object);

private:
	
};