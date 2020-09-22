#include "WireSignal.hpp"
#include "Wire.hpp"

namespace se
{
	void WireSignal::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		sf::Vector2f pos[4];
		switch (wire->getRotate())
		{
		case WireRotate::Right:
			pos[0] = sf::Vector2f(begin, -(WIRE_THICKNESS / 2.f));
			pos[1] = sf::Vector2f(begin, pos[0].y + WIRE_THICKNESS);
			pos[2] = sf::Vector2f(end, pos[1].y);
			pos[3] = sf::Vector2f(end, pos[0].y);
			break;
		case WireRotate::Down:
			pos[0] = sf::Vector2f(-(WIRE_THICKNESS / 2.f), -begin);
			pos[1] = sf::Vector2f(pos[0].x + WIRE_THICKNESS, -begin);
			pos[2] = sf::Vector2f(pos[1].x, -end);
			pos[3] = sf::Vector2f(pos[0].x, -end);
			break;
		case WireRotate::Left:
			pos[0] = sf::Vector2f(-begin, -(WIRE_THICKNESS / 2.f));
			pos[1] = sf::Vector2f(-begin, pos[0].y + WIRE_THICKNESS);
			pos[2] = sf::Vector2f(-end, pos[1].y);
			pos[3] = sf::Vector2f(-end, pos[0].y);
			break;
		case WireRotate::Up:
			pos[0] = sf::Vector2f(-(WIRE_THICKNESS / 2.f), begin);
			pos[1] = sf::Vector2f(pos[0].x + WIRE_THICKNESS, begin);
			pos[2] = sf::Vector2f(pos[1].x, end);
			pos[3] = sf::Vector2f(pos[0].x, end);
			break;
		}
		for (size_t i = 0; i < 4; i++)
		{
			pos[i] = se::to_SFML_coordinates(pos[i] + wire->getStartingPos());
			rectangle[i].position = pos[i];
		}

		target.draw(rectangle, states);
	}
	void WireSignal::setColor(sf::Color color)
	{
		for (size_t i = 0; i < 4; i++)
			rectangle[i].color = color;
	}
	WireSignal::WireSignal(Wire* wire)
		: wire(wire) {}

	sf::VertexArray WireSignal::rectangle = sf::VertexArray(sf::PrimitiveType::Quads, 4);
}