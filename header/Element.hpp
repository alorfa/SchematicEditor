#pragma once

#include <SFML/Graphics.hpp>
#include <list>

#include "VertexObject.hpp"
#include "Receiver.hpp"
#include "Sender.hpp"
#include "Signal.hpp"

namespace se
{
	namespace Drawings
	{
		extern VertexObject NOT, OR, NOT_OR, AND, NOT_AND, XOR, NOT_XOR;
	}

	enum ElemType
	{
		NOT = 0b1,
		OR = 0b10,
		NOT_OR = NOT | OR,
		AND = 0b100,
		NOT_AND = NOT | AND,
		XOR = 0b110,
		NOT_XOR = NOT | XOR
	};
	enum ElemDirection
	{
		Down, Left, Up, Right
	};
	class Element : public sf::Drawable
	{
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		VertexObject drawing;

		ElemType type;
		ElemDirection direction;
		sf::Vector2f pos;
		float size;

		Sender sender;
	public:
		Receiver left, right;
		Receiver down;

		static void init();

		Element(ElemType type, ElemDirection direction, sf::Vector2f position);

		void Step();

		inline sf::Vector2f getPosition() const {
			return pos;
		}
		inline ElemDirection getDirection() const {
			return direction;
		}
		inline ElemType getType() const {
			return type;
		}
		inline float getSize() const {
			return size;
		}

		void setType(ElemType type);
		void setDirection(ElemDirection direction);
		void setPosition(sf::Vector2f position);
		void setReceiver(Receiver* receiver);
		void setColor(sf::Color color);
	};
}