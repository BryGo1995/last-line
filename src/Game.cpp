#include"Game.h"

SpriteRenderer* Renderer;

void Game::init() {

	// Default game values
	float shipVelocity = 90.0f;

	// Seed the random number generator
	srand(time(0));

	// Load the shaders
	ResourceManager::LoadShader("resources/shaders/2Dshader.vert", "resources/shaders/2Dshader.frag", nullptr, "sprite");
	// Configure the shader uniforms
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
	
	// Set the shader image and projection uniforms
	ResourceManager::GetShader("sprite").Use();
	ResourceManager::GetShader("sprite").SetInteger("image", 0);
	ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

	// Create the Sprite Renderer
	Shader myShader;
	myShader = ResourceManager::GetShader("sprite");
	Renderer = new SpriteRenderer(myShader);

	// Load textures
	ResourceManager::LoadTexture("resources/sprites/earth.png", true, "earth");
	ResourceManager::LoadTexture("resources/sprites/ship2.png", true, "ship");
	ResourceManager::LoadTexture("resources/sprites/bullet.png", true, "bullet");
	ResourceManager::LoadTexture("resources/sprites/asteroid.png", true, "asteroid");
	ResourceManager::LoadTexture("resources/sprites/game_over.png", true, "game_over");
	Texture2D earth = ResourceManager::GetTexture("earth");
	Texture2D ship = ResourceManager::GetTexture("ship");
	Texture2D bullet = ResourceManager::GetTexture("bullet");
	Texture2D asteroid = ResourceManager::GetTexture("asteroid");
	Texture2D gameover = ResourceManager::GetTexture("game_over");

	// Create the default game objects (earth and ship)
	this->earth = new GameObject(glm::vec2(525.0f, 525.0f), glm::vec2(250.0f, 250.0f), earth);
	this->spaceship = new Spaceship(-90.0f, 200.0f, glm::vec2(625.0f, 425.0f), glm::vec2(50.f, 50.f), ship, glm::vec3(1.0f), glm::vec2(0.0f, 0.0f), shipVelocity);
	this->gameOver = new GameObject(glm::vec2(200.0f, 200.0f), glm::vec2(900.0f, 900.0f), gameover);

	// Initialize the asteroids initial positions outside of the screen
	unsigned int numAsteroids = 3;
	this->initAsteroids(numAsteroids, asteroid);

	// Initialize the text renderer objects for the score and boost meter
	this->scoreDisplay.init("resources/shaders/glyphShader.vert", "resources/shaders/glyphShader.frag", "score");
	this->BoostDisplay.init("resources/shaders/glyphShader.vert", "resources/shaders/glyphShader.frag", "boost");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	this->scoreDisplay.importCharacters("resources/fonts/Antonio-Regular.ttf", 0, 256, 128);
	this->BoostDisplay.importCharacters("resources/fonts/Antonio-Regular.ttf", 0, 256, 128);

	// Set the text renderer uniforms to have the same orthographic projection
	glm::mat4 t_projection = glm::ortho(0.0f, static_cast<float>(Width), 0.0f, static_cast<float>(Height));
	ResourceManager::GetShader("score").Use();
	ResourceManager::GetShader("score").SetMatrix4("projection", t_projection, true);
	ResourceManager::GetShader("boost").Use();
	ResourceManager::GetShader("boost").SetMatrix4("projection", t_projection, true);

	// Initialize the boost and ammo meter
	this->boostMeter = 100.0f;
	this->ammoMeter = 100.0f;
}

void Game::processInput(float dt) {

	if (this->State == GAME_ACTIVE) {

		// *** Boost ***
		if (this->Keys[GLFW_KEY_S]) {
			// Increase speed and decrement boost meter if > 0
			if (this->boostMeter > 50.0f) {
				this->spaceship->AngVelocity = 150.0f;
				this->boostMeter -= 1.0f;
			}
			else {

				this->boostMeter = 0.0f;
			}
		}
		else {
			this->spaceship->AngVelocity = 90.0f;
		}

		// *** Move Counter Clockwise ***
		if (this->Keys[GLFW_KEY_A]) {
			// Perform translation to the spaceship
			this->spaceship->Angle -= this->spaceship->AngVelocity * dt;
			this->spaceship->Position.x = (float)cos(glm::radians(this->spaceship->Angle)) * this->spaceship->Radius + 625;
			this->spaceship->Position.y = (float)sin(glm::radians(this->spaceship->Angle)) * this->spaceship->Radius + 625;

			// Rotate the spaceship as well (rotate at the speed that the ship is moving)
			this->spaceship->Rotation = 90.0f + this->spaceship->Angle;
		}

		// *** Move Clockwise ***
		if (this->Keys[GLFW_KEY_D]) {
			// Rotate clockwise
			// Perform translation to the spaceship
			this->spaceship->Angle += this->spaceship->AngVelocity * dt;
			this->spaceship->Position.x = (float)cos(glm::radians(this->spaceship->Angle)) * this->spaceship->Radius + 625;
			this->spaceship->Position.y = (float)sin(glm::radians(this->spaceship->Angle)) * this->spaceship->Radius + 625;

			// Rotate the spaceship as well (rotate at the speed that the ship is moving)
			this->spaceship->Rotation = 90.0f + this->spaceship->Angle;
		}

		// *** Fire Gun ***
		if (this->Keys[GLFW_KEY_SPACE]) {

			this->spaceship->fireBullet(*Renderer);
		}

		if (this->Keys[GLFW_KEY_ENTER]) {

			// Clear the enter key input since it will affect game reset
			this->Keys[GLFW_KEY_ENTER] = false;
		}
	}

	if (this->State == GAME_OVER) {

		// Reset the game if the player presses enter
		if (this->Keys[GLFW_KEY_ENTER]) {
			this->State = GAME_ACTIVE;
			this->reset();
		}
	}
}

void Game::update(float dt) {

	if (this->State == GAME_ACTIVE) {
		
		// Update the bullet state
		this->spaceship->updateBullets(dt, *Renderer);

		bool wasCollision = false;
		// Update the asteroid state and check the collision state
		for (auto it = this->Asteroids.begin(); it != this->Asteroids.end();) {


			it->updateLocation(dt);
			
			// Check for collisions with bullets
			for (auto it2 = this->spaceship->Bullets.begin(); it2 != this->spaceship->Bullets.end();) {

				if (it->boxCircleCollision((*it2))) {

              					it->isDestroyed = true;
					it2 = this->spaceship->Bullets.erase(it2);
				}
				else {
					++it2;
				}
			}

			// Check collisions with earth and spaceship (cause game over)
			if (it->circleCircleCollision(earth) || it->boxCircleCollision(*spaceship)) {

				it->isDestroyed = true;
				this->State = GAME_OVER;
			}

			// If the destroyed flag has been set, remove the asteroid
			if (it->isDestroyed) {
				
				it = Asteroids.erase(it);
				wasCollision = true;
				
				// Increment the score
				this->score += 1;
			}
			else {
				++it;
			}
		}

		// Check collision flag (if there was a destroyed asteroid from bullets, generate more asteroids)
		if (wasCollision) {
			this->addAsteroids(ResourceManager::GetTexture("asteroid"));
		}

		// Update the score display
		this->updateScore();

		// Update the boost meter to increase slowly with a cap of 100
		if (this->boostMeter > 100.0f) {
			this->boostMeter = 100.0f;
		}
		else {
			this->boostMeter += 1.0f * dt;
		}

		this->updateBoost();
	}
}

void Game::render() {
	
	if (this->State == GAME_ACTIVE) {
		this->earth->Draw(*Renderer);
		this->spaceship->Draw(*Renderer);

		std::vector<Asteroid> it;
		for (auto it = this->Asteroids.begin(); it != this->Asteroids.end(); ++it) {

			it->Draw(*Renderer);
		}
	}

	if (this->State == GAME_OVER) {

		// Draw the Game Over Screen
		this->gameOver->Draw(*Renderer);
	}
}

Game::Game(unsigned int width, unsigned int height) {

	Width = width;
	Height = height;
	State = GAME_ACTIVE;

}

Game::~Game() {

	delete spaceship;
	delete earth;

	delete Renderer;
}

void Game::initAsteroids(unsigned int numAsteroids, Texture2D asteroid) {

	// Clear the asteroids
	this->Asteroids.clear();

	// Asteroid spawning zone around perimeter of screen
	glm::vec2 randLocation = glm::vec2(0.0f, 0.0f);
	glm::vec2 randSize = glm::vec2(100.0f, 100.0f);

	glm::vec2 velocity = glm::vec2(0.0f, 0.0f);

	unsigned int i = 0;
	for (i = 0; i < numAsteroids; i++) {

		// Randomize location for asteroid to spawn
		randLocation = glm::circularRand(1000.0f);
		randLocation += glm::vec2(650.0f, 650.0f);

		// Based on this location, take the difference vector from the center and that is now the velocity vector
		velocity = glm::vec2(650.0f, 650.0f) - randLocation;
		velocity = 50.0f * glm::normalize(velocity);

		Asteroid newAsteroid(false, randLocation, randSize, asteroid, glm::vec3(1.0f), velocity);
		newAsteroid.Rotation = 0.0f;
		newAsteroid.hitboxRadius = randSize.y / 2;
		this->Asteroids.push_back(newAsteroid);
	}
}

void Game::addAsteroids(Texture2D texture) { // TODO: Combine these two functions to simplify the init function (possibly remove)

	// Random number of asteroids (depending on game difficulty...eventually)
	unsigned int numAsteroids = rand() % 1 + 1;

	// Set random values for speed, size, and location
	unsigned int randSpeed = rand() % 200 + 50;
	float randSize = (float)(rand() % 50 + 50);
	// TODO: Add random rotation
	glm::vec2 randLocation = glm::vec2(0.0f, 0.0f);

	glm::vec2 velocity = glm::vec2(0.0f, 0.0f);

	unsigned int i = 0;
	for (i = 0; i < numAsteroids; i++) {

		// Randomize location for asteroid to spawn
		randLocation = glm::circularRand(1000.0f);
		randLocation += glm::vec2(650.0f, 650.0f);

		// Based on this location, take the difference vector from the center and that is now the velocity vector (scale the speed)
		velocity = glm::vec2(650.0f, 650.0f) - randLocation;
		velocity = (float)randSpeed * glm::normalize(velocity);

		// Create a new asteroid object
		Asteroid newAsteroid(false, randLocation, glm::vec2(randSize, randSize), texture, glm::vec3(1.0f), velocity);
		newAsteroid.hitboxRadius = randSize / 2;
		this->Asteroids.push_back(newAsteroid);
	}
}

void Game::updateScore() {

	// Convert the integer value of the score to a string
	std::string score = std::to_string(this->score);
	
	// Update the score display
	this->scoreDisplay.renderText(score, 625.0f, 1150.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
}

void Game::updateBoost() {

	// Convert the boost value to a string
	std::string boost = std::to_string(this->boostMeter);

	// Update the boost display
	this->BoostDisplay.renderText(boost, 625.0f, 10.0f, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
}

void Game::reset() {

	// Reset the ship position and rotation
	this->spaceship->Position = glm::vec2(625.0f, 425.0f);
	this->spaceship->Angle = -90.0f;
	this->spaceship->Rotation = 0.0f;
	this->boostMeter = 100.0f;

	// Reset the bullets
	this->spaceship->resetBullets();

	// Reset the asteroids (default 3 initial asteroids)
	this->initAsteroids(3, ResourceManager::GetTexture("asteroid"));

	// Reset the score
	this->score = 0;
}
