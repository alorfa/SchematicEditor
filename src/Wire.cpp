#include "Wire.hpp"

float SIGNAL_PER_SECOND = 0.f;

namespace se
{
	void Wire::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		sf::Vector2f pos[4];

		switch (rotate)
		{
		case WireRotate::Right:
			pos[0].y = -(WIRE_THICKNESS / 2.f);
			pos[1].y = pos[0].y + WIRE_THICKNESS;
			pos[2] = sf::Vector2f(offset, pos[1].y);
			pos[3] = sf::Vector2f(offset, pos[0].y);
			break;
		case WireRotate::Down:
			pos[0].x = -(WIRE_THICKNESS / 2.f);
			pos[1].x = pos[0].x + WIRE_THICKNESS;
			pos[2] = sf::Vector2f(pos[1].x, -offset);
			pos[3] = sf::Vector2f(pos[0].x, -offset);
			break;
		case WireRotate::Left:
			pos[0].y = -(WIRE_THICKNESS / 2.f);
			pos[1].y = pos[0].y + WIRE_THICKNESS;
			pos[2] = sf::Vector2f(-offset, pos[1].y);
			pos[3] = sf::Vector2f(-offset, pos[0].y);
			break;
		case WireRotate::Up:
			pos[0].x = -(WIRE_THICKNESS / 2.f);
			pos[1].x = pos[0].x + WIRE_THICKNESS;
			pos[2] = sf::Vector2f(pos[1].x, offset);
			pos[3] = sf::Vector2f(pos[0].x, offset);
			break;
		}

		for (size_t i = 0; i < 4; i++)
		{
			pos[i] = to_SFML_coordinates(pos[i] + starting_pos);
			rectangle[i].position = pos[i];
		}

		target.draw(rectangle, states);

		for (auto it = list.begin(); it != list.end(); ++it)
			target.draw(*it);
	}

	void Wire::setColor(sf::Color color)
	{
		for (size_t i = 0; i < 4; i++)
			rectangle[i].color = color;
	}
	Wire::Wire(se::WireRotate rotate, sf::Vector2f starting_pos, float offset)
		: rotate(rotate), starting_pos(starting_pos), offset(offset)
	{
		receiver.got_signal = false;

		sender.gives_signal = false;
		sender.receiver = nullptr;
	}

	void Wire::setReceiver(Receiver* receiver)
	{
		sender.receiver = receiver;
	}
	void Wire::ReceiveSignal()
	{
		receiver.ReceiveSignal();
	}
	void Wire::Step()
	{
		sender.gives_signal = false;

		bool erase_first_signal = false;
		bool new_signal = true;
		
		for (auto it = list.begin(); it != list.end(); ++it)
		{
			it->Step(receiver.got_signal, offset);
			if (it->getBegin() == 0.f)
				new_signal = false;
			if (!receiver.got_signal)
				new_signal = false;
			if (it->getBegin() >= offset)
				erase_first_signal = true;
			if (it->getEnd() >= offset)
				sender.gives_signal = true;
		}
		if (list.empty() && !receiver.got_signal)
			new_signal = false;

		if (new_signal)
			list.push_back(WireSignal(this));
		if (erase_first_signal)
			list.pop_front();

		receiver.got_signal = false;

		sender.GiveSignal();
	}

	sf::VertexArray Wire::rectangle = sf::VertexArray(sf::PrimitiveType::Quads, 4);
}