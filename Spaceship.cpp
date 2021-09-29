#include"Spaceship.h"

void Spaceship::fireBullet(SpriteRenderer& renderer) {

	// Calculate the position of the guntip (it has its own circle)
	this->GunTip.x = (this->Radius + 25.0f) * cos(glm::radians(this->Angle)) + 650;
	this->GunTip.y = (this->Radius + 25.0f) * sin(glm::radians(this->Angle)) + 650;
	
	glm::vec2 bulletSize = glm::vec2(3.0f, 10.0f);
	glm::vec2 bulletPos = glm::vec2(this->GunTip.x - bulletSize.x / 2, this->GunTip.y - bulletSize.y / 2);
	//glm::vec2 guntip;
	//guntip.x = (this->Position.x + (this->Size.x) / 2) - (bulletSize.x / 2);
	//guntip.y = this->Position.y - bulletSize.y - 2.0f;

	// Load the texture
	Texture2D bullet = ResourceManager::GetTexture("bullet");

	// Calculate the bullets velocity
	glm::vec2 velocity;
	velocity.x = cos(glm::radians(this->Angle));
	velocity.y = sin(glm::radians(this->Angle));

	velocity = 200.0f * glm::normalize(velocity);

	GameObject newBullet(bulletPos, bulletSize, bullet, glm::vec3(1.0f), velocity);
	newBullet.Rotation = 90.0f + this->Angle;
	this->Bullets.push_back(newBullet);
}

void Spaceship::updateBullets(float dt, SpriteRenderer& renderer) {

	unsigned int i = 0;
	// Remove the bullets out of the screen
	if (Bullets.empty()) {
		// Use swap trick to clear the vector capacity
		std::vector<GameObject> emptyVec;
		emptyVec.swap(Bullets);
		Bullets = emptyVec;
	}
	else {
		// Delete bullets if they leave the screen
		for (auto it = Bullets.begin(); it != Bullets.end(); ++i) {

			if (it->Position.y > 1300 || it->Position.y < 0) {
				it = Bullets.erase(it);
			}
			else if (it->Position.x > 1300 || it->Position.x < 0) {
				it = Bullets.erase(it);
			}
			else
				++it;
		}
	}
	
	// Update the bullet position based on its velocity vector
 	for (auto it = Bullets.begin(); it != Bullets.end(); ++it) {

		// Add velocity vector to position
		it->Position += it->Velocity * dt;

		it->Draw(renderer);
	}
}

void Spaceship::resetBullets() {

	// Use the vector clear function
	this->Bullets.clear();
}

//Spaceship::~Spaceship() {
//
//	for (auto it = Bullets.begin(); it != Bullets.end(); ++it) {
//
//		delete (*it);
//	}
//}