#include "VertexObject.hpp"
#include "header.hpp"

namespace se
{
	void VertexObject::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		for (size_t i = 0; i < container.size(); i++)
			target.draw(*container[i], states);
	}

	VertexObject::VertexObject() {}
	VertexObject::VertexObject(const VertexObject& other)
	{
		this->operator=(other);
	}
	VertexObject::~VertexObject()
	{
		for (size_t i = 0; i < container.size(); i++)
			delete container[i];
	}

	void VertexObject::add(sf::VertexArray* new_array)
	{
		container.push_back(new_array);
	}
	VertexObject& VertexObject::operator=(const VertexObject& object)
	{

		for (size_t i = 0; i < container.size(); i++)
			delete container[i];

		container.clear();
		for (size_t i = 0; i < object.container.size(); i++)
		{
			container.push_back(new sf::VertexArray(object.operator[](i)));
		}
		return *this;
	}
	sf::VertexArray& VertexObject::operator[](size_t index) const
	{
		return *(container[index]);
	}
}