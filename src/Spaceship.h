#pragma once

#include<vector>

#include"GameObject.h"
#include"ResourceManager.h"

// Spaceship powerup state
enum ShipPowerup {
	INFINITE_BOOST,
	INFINITE_AMMO,
	ANGULAR_GUN

};

enum ShipUpgrades {
	INCOMING_INDICATOR,
	DOUBLE_BARREL

};



class Spaceship : public GameObject {

public:

	float Angle;
	float Radius;
	float AngVelocity;
	glm::vec2 GunTip;

	Texture2D BulletTexture;
	std::vector<GameObject> Bullets;
	
	unsigned int BoostMeter;

	Spaceship(float angle, float radius, glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity, float angvelocity)
		: GameObject(pos, size, sprite, color, velocity), Angle{ angle }, Radius{ radius }, AngVelocity{ angvelocity } {};

	// Fire a new bullet
	void fireBullet(SpriteRenderer& renderer);

	// Update the bullet locations
	void updateBullets(float dt, SpriteRenderer& renderer);

	// Reset the bullets
	void resetBullets();

private:

};