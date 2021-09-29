#include<iostream>
#include<vector>

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>

#include"Shader.h"
#include"Game.h"

// Function declarations
void processInput(GLFWwindow* window, Game* game, float dt);

int main(void) {

	const unsigned int SCR_HEIGHT = 1300;
	const unsigned int SCR_WIDTH = 1300;

	// Delta Time Variables
	// Globals
	float deltaTime = 0.0f;
	float oldTime = 0.0f;
	float newTime = 0.0f;

	// Generate the counter for the boost meter delay
	unsigned int boostCount = 0;

	// Generate the circle vertex positions
	const unsigned int numPoints = 50;
	const unsigned int vertPos = 3;
	const float scale = 0.25f;
	GLfloat vertices[numPoints * vertPos];

	// Create the circle vertices
	for (unsigned int i = 0; i < numPoints; i++) {

		vertices[3 * i] = scale * cos(glm::radians((float)(360 * i / numPoints)));
		vertices[3 * i + 1] = scale * sin(glm::radians((float)(360 * i / numPoints)));
		vertices[3 * i + 2] = 0.0f;
	}

	// Initialize and setup glfw versions and profile
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create the window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Last Line", NULL, NULL);
	if (!window) {

		std::cout << "GLFW window failed to create!" << std::endl;
		glfwTerminate();
		return -1;
	}

	// Add the window to the current executing thread
	glfwMakeContextCurrent(window);

	// Initialize glad
	gladLoadGL();

	// Enable blending for transaparent backgrounds
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Set the viewport
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// Create the new game object and initialize
	Game* myGame = new Game(SCR_WIDTH, SCR_HEIGHT);
	myGame->init();

	while (!glfwWindowShouldClose(window)) {

		// Calculate delta time for use with processing functions
		newTime = glfwGetTime();
		deltaTime = newTime - oldTime;
		oldTime = newTime;

		// Process keyboard input
		processInput(window, myGame, deltaTime);

		// Set the background color
		glClearColor(0.03f, 0.07f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Update and render the game
		myGame->update(deltaTime);
		myGame->render();

		// Display the rendered back buffer
 		glfwSwapBuffers(window);

		// Poll window events
		glfwPollEvents();
	}

	// Termindate glfw
	glfwTerminate();

	return 0;
}

void processInput(GLFWwindow* window, Game* game, float dt) {

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		game->Keys[GLFW_KEY_W] = true;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		game->Keys[GLFW_KEY_S] = true;
	}
	else {
		game->Keys[GLFW_KEY_S] = false;
	}
		
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		game->Keys[GLFW_KEY_A] = true;
	}
	else {
		game->Keys[GLFW_KEY_A] = false;
	}

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		game->Keys[GLFW_KEY_D] = true;
	}
	else {
		game->Keys[GLFW_KEY_D] = false;
	}

	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		game->Keys[GLFW_KEY_ENTER] = true;
	}
	else {
		game->Keys[GLFW_KEY_ENTER] == false;
	}

	// Fire button
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		game->Keys[GLFW_KEY_SPACE] = true;
	}
	else {
		game->Keys[GLFW_KEY_SPACE] = false;
	}

	game->processInput(dt);
}