#include "TextRenderer.h"

TextRenderer::TextRenderer()
{
}

TextRenderer::~TextRenderer()
{
}

int TextRenderer::init(const char* vertex_shader, const char* fragment_shader, const char* name) {

	// Set the name to be used in the shader
	this->ShaderName = name;

	// Load the font using freetype
	if (FT_Init_FreeType(&this->ft)) {
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return -1;
	}

	// Load the shader program
	ResourceManager::LoadShader(vertex_shader, fragment_shader, nullptr, this->ShaderName);

	// Initialize the VBO and VAO
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);
	glBindVertexArray(this->VAO);
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);	// Initialize an empty buffer (6 vertices of vec4)
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return 0;
}

int TextRenderer::importCharacters(const char* file, FT_UInt pixel_width, FT_UInt pixel_height, unsigned int num_char) {

	// Create a new face
	FT_Face face;
	if (FT_New_Face(this->ft, file, 0, &face)) {
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return -1;
	}

	// Define the pixel font size
	FT_Set_Pixel_Sizes(face, pixel_width, pixel_height);

	for (unsigned char c = 0; c < num_char; c++) {
		// Load the character glyph
		if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
			std::cout << "ERROR::FREETYPE: Failed to load glyph." << std::endl;
			return -1;
		}
		// Generate the texture
		unsigned int texture;
		this->textureGen(texture, face);

		// Create a new character from Character struct
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};

		// Add the character to the Characters map
		this->Characters.insert(std::pair<char, Character>(c, character));
	}

	FT_Done_Face(face);
	FT_Done_FreeType(this->ft);
}


void TextRenderer::renderText(std::string text, float x_pos, float y_pos, float scale, glm::vec3 color) {
	// Activate the shader program
	ResourceManager::GetShader(this->ShaderName).Use();
	ResourceManager::GetShader(this->ShaderName).SetVector3f("textColor", color, true);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(this->VAO);

	// Iterate through the input string
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		// Retrieve the character from the Characters vector
		Character charIn = this->Characters[*c];

		// Position and size the character based on the bearings
		float x = x_pos + charIn.Bearing.x * scale;
		float y = y_pos - (charIn.Size.y - charIn.Bearing.y) * scale;

		float w = charIn.Size.x * scale;
		float h = charIn.Size.y * scale;

		// Update the VBO based on the coordinates generated above
		float vertices[6][4] = {
			{ x,     y + h,   0.0f, 0.0f },
			{ x,     y,       0.0f, 1.0f },
			{ x + w, y,       1.0f, 1.0f },

			{ x,     y + h,   0.0f, 0.0f },
			{ x + w, y,       1.0f, 1.0f },
			{ x + w, y + h,   1.0f, 0.0f }
		};
		// Render the glpyh texture over the newly set quad
		glBindTexture(GL_TEXTURE_2D, charIn.TextureID);
		// Update the content of the empty VBO
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		// Unbind and call the gldraw function
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Advance the cursor for the next glyph
		x_pos += (charIn.Advance >> 6) * scale;
	}
	// Unbind the VAO and texture
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void TextRenderer::textureGen(unsigned int& texture, FT_Face& face) {

	// Generate the texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RED,
		face->glyph->bitmap.width,
		face->glyph->bitmap.rows,
		0,
		GL_RED,
		GL_UNSIGNED_BYTE,
		face->glyph->bitmap.buffer
	);

	// Set the texture options
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}