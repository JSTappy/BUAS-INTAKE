#ifndef TEXT_H
#define TEXT_H

#include <string>

#include <glm/glm.hpp>


class Text
{
public:
    Text(char* fontPath = (char*)"assets/fonts/tahoma.ttf", int size = 64){ _font = fontPath; _size = size;}
    ~Text() {};

    char* GetFontName() { return _font; }
    int GetSize() { return _size; }

    glm::vec2 pivot = glm::vec2(0.0f, 0.0f);

    glm::vec4 color = glm::vec4(255, 255, 255, 255);
    std::string text;

private:
    char* _font;
    int _size;

};

#endif