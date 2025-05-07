#ifndef TEXT_H
#define TEXT_H

#define WHITE   glm::vec4(255, 255, 255, 255)
#define RED     glm::vec4(255, 0.0f, 0.0f, 255)
#define GREEN   glm::vec4(0.0f, 255, 0.0f, 255)
#define BLUE    glm::vec4(0.0f, 0.0f, 255, 255)
#define CYAN    glm::vec4(0.0f, 255, 255, 255)
#define MAGENTA glm::vec4(255, 0.0f, 255, 255)
#define YELLOW  glm::vec4(255, 255, 0.0f, 255)
#define ORANGE  glm::vec4(255, 127.5f, 0.0f, 255)
#define PURPLE  glm::vec4(127.5f, 0.0f, 127.5f, 255)
#define BLACK   glm::vec4(0.0f, 0.0f, 0.0f, 255)

#include <string>

#include <glm/glm.hpp>


class Text
{
public:
    Text(char* fontPath = (char*)"assets/fonts/tahoma.ttf", int size = 32) 
    { 
        _font = fontPath; 
        _size = size; 
        centered = true; 
        moveWithEntity = true;
    }
    ~Text() {};

    char* GetFontName() { return _font; }
    int GetSize() { return _size; }

    glm::vec3 position = glm::vec3();

    glm::vec2 pivot = glm::vec2(0.0f, 0.0f);

    glm::vec4 color = WHITE;

    std::string text;

    bool moveWithEntity;
    bool centered;

private:
    char* _font;
    int _size;

};

#endif