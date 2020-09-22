#pragma once

#include <SFML/Graphics.hpp>
#include "header.hpp"
#include "Signal.hpp"

namespace se
{
	class Wire;

	class WireSignal : public Signal, public sf::Drawable
	{
		static sf::VertexArray rectangle;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		Wire* const wire;
	public:
		static void setColor(sf::Color color);

		WireSignal(Wire* wire);
	};
}