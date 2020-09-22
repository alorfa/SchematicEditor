#pragma once

#include <list>
#include "Wire.hpp"

namespace se
{
	class FullWire : public sf::Drawable
	{
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
		std::list<se::Wire*> list;
	};
}