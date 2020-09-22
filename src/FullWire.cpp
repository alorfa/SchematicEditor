#include "FullWire.hpp"

namespace se
{
	void FullWire::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (auto it = list.begin(); it != list.end(); it++)
		{
			target.draw(**it, states);
		}
	}
}