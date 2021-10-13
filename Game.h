#pragma once

#include"ResourceManager.h"
#include"SpriteRenderer.h"
#include"GameObject.h"
#include"Spaceship.h"
#include"Asteroid.h"
#include"TextRenderer.h"

#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/random.hpp>
#include<vector>
#include<iostream>
#include<string>


enum GameState {

	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN,
	GAME_OVER
};

class Game {

public:
	GameState State;
	bool Keys[1024];
	unsigned int Width, Height;

	// Game Objects
	GameObject* earth;
	Spaceship* spaceship;
	std::vector<Asteroid> Asteroids;
	GameObject* gameOver;

	unsigned int score;
	TextRenderer scoreDisplay;

	TextRenderer BoostDisplay;

	float boostMeter;
	float ammoMeter;

	// Initialize initial game state
	void init();

	// Game loop
	void processInput(float dt);
	void update(float dt);
	void render();

	// Constructor and Destructor
	Game(unsigned int width, unsigned int height);
	~Game();

private:
	void initAsteroids(unsigned int numAsteroids, Texture2D asteroid);

	void addAsteroids(Texture2D texture);

	void updateScore();

	void updateBoost();

	void reset();
};