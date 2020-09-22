#pragma once

#include <SFML/Graphics.hpp>
#include "header.hpp"
#include <list>
#include "WireSignal.hpp"
#include "Sender.hpp"
#include "Receiver.hpp"

const float WIRE_THICKNESS = 0.20f;
const float SHIFT = (1.f - WIRE_THICKNESS) / 2.f;

namespace se
{
	class WireSignal;

	enum class WireRotate { Up, Right, Down, Left };

	class Wire : public sf::Drawable
	{
		static sf::VertexArray rectangle;

		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		std::list<WireSignal> list;

		WireRotate rotate;
		sf::Vector2f starting_pos;
		float offset;

		Sender sender;
	public:
		Receiver receiver;

		static void setColor(sf::Color color);

		Wire(se::WireRotate rotate, sf::Vector2f starting_pos, float offset);

		void setReceiver(Receiver* receiver);
		void ReceiveSignal();
		void Step();

		inline WireRotate getRotate() const {
			return rotate;
		}
		inline sf::Vector2f getStartingPos() const {
			return starting_pos;
		}
		inline float getOffset() const {
			return offset;
		}

		inline void setRotate(WireRotate rotate) {
			this->rotate = rotate;
		}
		inline void setStartingPos(sf::Vector2f starting_pos) {
			this->starting_pos = starting_pos;
		}
		inline void setOffset(float offset) {
			this->offset = offset;
		}
	};
}