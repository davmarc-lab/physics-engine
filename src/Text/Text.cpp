#include "Text.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <string>

Text::Text(string text, const int fontSize) {
    this->text = text;
    this->fontSize = fontSize;

    // initialize freetype
    auto error = FT_Init_FreeType(&library);

    if (error) {
        cout << "Failed to load FreeType library." << endl;
        exit(-1);
    }

    error = FT_New_Face(library, "./fonts/arial.ttf", 0, &fontFace);
    if (error == FT_Err_Unknown_File_Format) {
        cout << "Error, this font type is not supported or could not be read."
             << endl;
        exit(-1);

    } else if (error) {
        cout << "Failed to load font fontFace for freetype." << endl;
        exit(-1);
    }
    this->generateText();
}

void Text::createVertexArray() {
    glGenVertexArrays(1, &this->buffers.vao);
    glBindVertexArray(this->buffers.vao);

    glGenBuffers(1, &this->buffers.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->buffers.vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Text::generateText() {
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    FT_Set_Pixel_Sizes(fontFace, 0, this->fontSize);
    // load first 128 characters of ASCII set
    for (unsigned char c = 0; c < 128; c++) {
        // Load character glyph
        if (FT_Load_Char(fontFace, c, FT_LOAD_RENDER)) {
            cout << "ERROR::FREETYTPE: Failed to load Glyph." << endl;
            continue;
        }
        // generate buffers.texture
        glGenTextures(1, &this->buffers.texture);
        glBindTexture(GL_TEXTURE_2D, this->buffers.texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, fontFace->glyph->bitmap.width,
                     fontFace->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE,
                     fontFace->glyph->bitmap.buffer);
        // set this->buffers.texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // now store character for later use
        Character character = {
            this->buffers.texture,
            ivec2(fontFace->glyph->bitmap.width, fontFace->glyph->bitmap.rows),
            ivec2(fontFace->glyph->bitmap_left, fontFace->glyph->bitmap_top),
            static_cast<unsigned int>(fontFace->glyph->advance.x)};
        characters.insert(pair<char, Character>(c, character));
    }

    std::string::const_iterator c;
    for (c = this->text.begin(); c != this->text.end(); c++) {
        Character ch = characters[*c];

        float width = ch.Size.x;
        float height = ch.Size.y;

        this->totalWidth += int(ch.Advance / 64);
        this->totalHeight =
            this->totalHeight > height ? this->totalHeight : height;
    }

    // destroy FreeType once we're finished
    FT_Done_Face(fontFace);
    FT_Done_FreeType(library);
}

void Text::renderText(Shader shader) {
    shader.use();

    float x = this->position.x;
    float y = this->position.y;

    auto color = vec3(this->getColorValues());
    glUniform3fv(glGetUniformLocation(shader.getId(), "textColor"), 1,
                 value_ptr(color));
    glUniformMatrix4fv(glGetUniformLocation(shader.getId(), "projection"), 1,
                       GL_FALSE, value_ptr(this->projection));

    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(this->buffers.vao);

    std::string::const_iterator c;
    for (c = this->text.begin(); c != this->text.end(); c++) {
        Character ch = characters[*c];

        float xpos = x + ch.Bearing.x * this->fontScale;
        float ypos = y - (ch.Size.y - ch.Bearing.y) * this->fontScale;

        float width = ch.Size.x * this->fontScale;
        float height = ch.Size.y * this->fontScale;

        float vertices[6][4] = {{xpos, ypos + height, 0.0f, 0.0f},
                                {xpos, ypos, 0.0f, 1.0f},
                                {xpos + width, ypos, 1.0f, 1.0f},

                                {xpos, ypos + height, 0.0f, 0.0f},
                                {xpos + width, ypos, 1.0f, 1.0f},
                                {xpos + width, ypos + height, 1.0f, 0.0f}};

        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        glBindBuffer(GL_ARRAY_BUFFER, this->buffers.vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        x += (ch.Advance >> 6) * this->fontScale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Text::clear() {
    characters.clear();
    glDeleteTextures(1, &this->buffers.texture);
    glDeleteVertexArrays(1, &this->buffers.vao);
    glDeleteFramebuffers(1, &this->buffers.vbo);
}
