#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>
#include "Style.hpp"

typedef unsigned int uint;

extern uint WIDTH, HEIGHT;
extern uint FPS;

const int PIXELS_BETWEEN_LINES = 40;
const float CELL_SIZE = (float)PIXELS_BETWEEN_LINES;
const int OUTLINE_SIZE = 4;

namespace se
{
	bool read_config(se::Style& style);
	template <typename T>
	sf::Vector2<T> to_SE_coordinates(sf::Vector2<T> pos)
	{
		pos.x /= PIXELS_BETWEEN_LINES;
		pos.y /= -PIXELS_BETWEEN_LINES;
		return pos;
	}
	template <typename T>
	sf::Vector2f to_SE_coordinates(T x, T y)
	{
		sf::Vector2<T> result;
		result.x = x / PIXELS_BETWEEN_LINES;
		result.y = y / -PIXELS_BETWEEN_LINES;
		return result;
	}
	template <typename T>
	sf::Vector2<T> to_SFML_coordinates(sf::Vector2<T> pos)
	{
		pos.x *= PIXELS_BETWEEN_LINES;
		pos.y *= -PIXELS_BETWEEN_LINES;
		return pos;
	}
	template <typename T>
	sf::Vector2<T> to_SFML_coordinates(T x, T y)
	{
		sf::Vector2<T> result;
		result.x = x * PIXELS_BETWEEN_LINES;
		result.y = y * -PIXELS_BETWEEN_LINES;
		return result;
	}
}

extern float SIGNAL_PER_SECOND;

#define DEBUG

#ifdef DEBUG
#include <iostream>
#define PRINT(x) std::cerr << #x " = " << (x) << std::endl
#define PRINTR(x) std::cerr << "\r" #x " = " << (x) << "        "
#else
#define PRINT(x)
#define PRINTR(x)
#endif //DEBUG