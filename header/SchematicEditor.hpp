#pragma once

#include <SFML/Graphics.hpp>
#include "Style.hpp"
#include "header.hpp"
#include <list>
#include "Element.hpp"
#include "Wire.hpp"

namespace se
{
	class SchematicEditor
	{
		sf::RenderWindow* window;
		std::list<se::Element> elements;
		std::list<se::Wire> wires;

		se::Style style;

		sf::RectangleShape horizontal_line;
		sf::RectangleShape vertical_line;

		sf::Vector2f camera_starting_point;
		sf::Vector2f camera_final_point;

		sf::View camera;

		float zoom = 1.f;
	public:
		SchematicEditor(sf::RenderWindow* window, se::Style& style);

		const sf::View& getCamera() const;
		void setWindow(sf::RenderWindow* window);
		void setStyle(se::Style& style);
		void setCamera(sf::View& camera);

		void scale(float scale);
		void moveCamera(sf::Vector2i removal);
		void setCameraSize(sf::Vector2f size);

		void updateElements();
		void updateWires();

		void useCamera();
		void clearWindow();
		void drawLinies();
		void drawElements();
		void drawWires();
	};
}