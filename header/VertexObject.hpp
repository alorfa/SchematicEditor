#pragma once

#include <vector>
#include <SFML/Graphics.hpp>

namespace se
{
	class VertexObject : public sf::Drawable
	{
		std::vector<sf::VertexArray*> container;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	public:
		VertexObject();
		VertexObject(const VertexObject& other);
		~VertexObject();

		void add(sf::VertexArray* new_array);
		void move(sf::Vector2f offset);
		VertexObject& operator=(const VertexObject& object);
		sf::VertexArray& operator[](size_t index) const;

		inline size_t size() {
			return container.size();
		}
	};
}