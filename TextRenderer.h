#pragma once
#include<iostream>
#include<map>

#include<glad/glad.h>
#include<ft2build.h>
#include FT_FREETYPE_H
#include<glm/glm.hpp>

#include "ResourceManager.h"

struct Character {
	unsigned int TextureID;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

class TextRenderer
{
public:

	std::map<char, Character> Characters;	// Map to hold characters for easy access
	FT_Library ft;

	int init(const char* vertex_shader, const char* fragment_shader);
	int importCharacters(const char* file, FT_UInt pixel_width, FT_UInt pixel_height, unsigned int num_char);
	void renderText(std::string text, float x_pos, float y_pos, float scale, glm::vec3 color);

	TextRenderer();
	~TextRenderer();

private:
	GLuint VBO;
	GLuint VAO;
	void textureGen(unsigned int& texture, FT_Face& face);
};