#include "Element.hpp"
#include "header.hpp"
#include <cmath>

#ifdef DEBUG
#include <iostream>
#endif //DEBUG

constexpr float PI = 3.14159265358979f;

std::ostream& operator<<(std::ostream& stream, sf::Vector2f point)
{
	stream << '[' << point.x << ", " << point.y << ']';

	return stream;
}

namespace
{
	sf::Vector2f get_circle_point(float radian, sf::Vector2f size, sf::Vector2f starting_point = sf::Vector2f())
	{
		starting_point.x += std::cos(radian) * size.x;
		starting_point.y += std::sin(radian) * size.y;
		return starting_point;
	}
	inline float to_radian(float angle)
	{
		return angle * PI / 180.f;
	}

	constexpr int POLYGONS_IN_CIRCLE = 128;
	sf::VertexArray* create_circle(size_t count_of_polygons,
		sf::Vector2f external, sf::Vector2f internal, sf::Vector2f starting_point = sf::Vector2f())
	{
		external = se::to_SFML_coordinates(external);
		internal = se::to_SFML_coordinates(internal);
		starting_point = se::to_SFML_coordinates(starting_point);

		const float ANGLE = 360.f / count_of_polygons;
		auto circle = new sf::VertexArray(sf::PrimitiveType::Quads, count_of_polygons * 4);
		float angle = 90.f;

		for (size_t i = 0; i < circle->getVertexCount(); i += 4)
		{
			(*circle)[i].position = get_circle_point(to_radian(angle), external, starting_point);
			(*circle)[i + 1].position = get_circle_point(to_radian(angle), internal, starting_point);
			angle += ANGLE;
			(*circle)[i + 2].position = get_circle_point(to_radian(angle), internal, starting_point);
			(*circle)[i + 3].position = get_circle_point(to_radian(angle), external, starting_point);
		}
		return circle;
	}
	float get_angle(sf::Vector2f point1, sf::Vector2f point2)
	{
		float x, y, hypotenuse;
		float sinx, cosx;

		x = point2.x - point1.x;
		y = point2.y - point1.y;
		hypotenuse = sqrt(x * x + y * y);

		sinx = y / hypotenuse;
		cosx = x / hypotenuse;

		return acosf(cosx) / PI * 180.f;
	}
	sf::VertexArray* create_triangle(sf::Vector2f left_point, float right_x, float top_y, float thickness)
	{
		constexpr int TRIANGLES_IN_EDGE = 64;
		constexpr float ANGLE = 360.f / TRIANGLES_IN_EDGE / 2.f;

		sf::Vector2f left(left_point);
		sf::Vector2f right(right_x, left_point.y);
		sf::Vector2f top((right_x + left_point.x) / 2, top_y);

		float angle;
		const sf::Vector2f size(thickness, thickness);

		sf::Vector2f base_left, base_right;
		sf::Vector2f left_bottom, left_top;
		sf::Vector2f right_bottom, right_top;

		base_left = sf::Vector2f(left.x, left.y - thickness);
		base_right = sf::Vector2f(right.x, right.y - thickness);
		angle = get_angle(left, top);
		left_bottom = get_circle_point(to_radian(angle + 90.f), size, left);
		left_top = get_circle_point(to_radian(angle + 90.f), size, top);
		angle = get_angle(right, top);
		right_top = get_circle_point(to_radian(angle - 90.f), size, top);
		right_bottom = get_circle_point(to_radian(angle - 90.f), size, right);

		auto* triangle = new sf::VertexArray(sf::PrimitiveType::Triangles, TRIANGLES_IN_EDGE * 3 * 3 + 6 * 3);

		sf::Vector2f last_point, point;

		angle = 270.f;		
		for (size_t i = 0; i < TRIANGLES_IN_EDGE * 3; i += 3)
		{
			last_point = get_circle_point(to_radian(angle), size, left);
			angle -= ANGLE;
			point = get_circle_point(to_radian(angle), size, left);

			(*triangle)[i].position = last_point;
			(*triangle)[i + 1].position = left;
			(*triangle)[i + 2].position = point;
		}
		angle = 270.f;
		for (size_t i = TRIANGLES_IN_EDGE * 3; i < TRIANGLES_IN_EDGE * 3 * 2; i += 3)
		{
			last_point = get_circle_point(to_radian(angle), size, right);
			angle += ANGLE;
			point = get_circle_point(to_radian(angle), size, right);

			(*triangle)[i].position = last_point;
			(*triangle)[i + 1].position = right;
			(*triangle)[i + 2].position = point;
		}
		angle = 90.f + 90.f;
		for (size_t i = TRIANGLES_IN_EDGE * 3 * 2; i < TRIANGLES_IN_EDGE * 3 * 3; i += 3)
		{
			last_point = get_circle_point(to_radian(angle), size, top);
			angle -= ANGLE;
			point = get_circle_point(to_radian(angle), size, top);

			(*triangle)[i].position = last_point;
			(*triangle)[i + 1].position = top;
			(*triangle)[i + 2].position = point;
		}

		for (size_t index = TRIANGLES_IN_EDGE * 9; index < TRIANGLES_IN_EDGE * 9 + 18; index += 3)
		{
			size_t i = index - TRIANGLES_IN_EDGE * 9;
			if (i == 0 || i == 3)
			{
				if (i == 0)
					(*triangle)[index].position = left;
				else
					(*triangle)[index].position = base_right;
				(*triangle)[index + 1].position = base_left;
				(*triangle)[index + 2].position = right;
			}
			else if (i == 6 || i == 9)
			{
				if (i == 6)
					(*triangle)[index].position = left;
				else
					(*triangle)[index].position = left_top;
				(*triangle)[index + 1].position = left_bottom;
				(*triangle)[index + 2].position = top;
			}
			else
			{

				if (i == 12)
					(*triangle)[index].position = top;
				else
					(*triangle)[index].position = right_bottom;
				(*triangle)[index + 1].position = right_top;
				(*triangle)[index + 2].position = right;
			}
		}
		for (size_t i = 0; i < triangle->getVertexCount(); i++)
			(*triangle)[i].position = se::to_SFML_coordinates((*triangle)[i].position);

		return triangle;
	}
	constexpr int POLYGONS_IN_SECTOR = POLYGONS_IN_CIRCLE / 2;
	sf::VertexArray* create_AND_element(size_t polygons_in_sector, sf::Vector2f center, float thickness,
		float base_length, float lines_length, float sector_length)
	{
		const float ANGLE = 360.f / polygons_in_sector / 2;

		auto* elem = new sf::VertexArray(sf::PrimitiveType::Quads, polygons_in_sector * 4 + 3 * 4);

		const sf::Vector2f internal(base_length / 2.f, sector_length);
		const sf::Vector2f external(internal.x + thickness, internal.y + thickness);

		float angle = 180.f;
		for (size_t i = 0; i < polygons_in_sector * 4; i += 4)
		{
			(*elem)[i].position = get_circle_point(to_radian(angle), internal, center);
			(*elem)[i + 1].position = get_circle_point(to_radian(angle), external, center);
			angle -= ANGLE;
			(*elem)[i + 2].position = get_circle_point(to_radian(angle), external, center);
			(*elem)[i + 3].position = get_circle_point(to_radian(angle), internal, center);
		}
		sf::Vector2f left_bottom, left_top, right_bottom, right_top;
		left_top.x = center.x - base_length / 2.f;
		left_top.y = center.y;
		left_bottom.x = left_top.x;
		left_bottom.y = left_top.y - lines_length;
		right_top.x = left_top.x + base_length;
		right_top.y = left_top.y;
		right_bottom.x = right_top.x;
		right_bottom.y = right_top.y - lines_length;

		std::cout << std::endl;
		PRINT(left_top);
		PRINT(left_bottom);
		PRINT(right_bottom);
		PRINT(right_top);
		std::cout << std::endl;

		for (size_t index = polygons_in_sector * 4; index < elem->getVertexCount(); index += 4)
		{
			size_t i = index - polygons_in_sector * 4;
			if (i == 0)
			{
				(*elem)[index].position = left_top;
				(*elem)[index + 1].position = sf::Vector2f(left_top.x - thickness, left_top.y);
				(*elem)[index + 2].position = sf::Vector2f(left_bottom.x - thickness, left_bottom.y - thickness);
				(*elem)[index + 3].position = left_bottom;
			}
			else if (i == 4)
			{
				(*elem)[index].position = right_bottom;
				(*elem)[index + 1].position = sf::Vector2f(right_bottom.x + thickness, right_bottom.y - thickness);
				(*elem)[index + 2].position = sf::Vector2f(left_bottom.x - thickness, left_bottom.y - thickness);
				(*elem)[index + 3].position = left_bottom;
			}
			else
			{
				(*elem)[index].position = right_bottom;
				(*elem)[index + 1].position = sf::Vector2f(right_bottom.x + thickness, right_bottom.y - thickness);
				(*elem)[index + 2].position = sf::Vector2f(right_top.x + thickness, right_top.y);
				(*elem)[index + 3].position = right_top;
			}
		}

		for (size_t i = 0; i < elem->getVertexCount(); i++)
			(*elem)[i].position = se::to_SFML_coordinates((*elem)[i].position);

		return elem;
	}
	sf::VertexArray* create_sector_for_OR(size_t polygons_in_sector,
		sf::Vector2f size, float thickness, sf::Vector2f starting_point)
	{
		const float ANGLE = 180.f / polygons_in_sector;
		float angle = 0.f;

		size.x /= 2.f;
		size.y /= 2.f;
		sf::Vector2f external(size.x + thickness, size.y + thickness);

		auto* sector = new sf::VertexArray(sf::PrimitiveType::Quads, polygons_in_sector * 4);

		for (size_t i = 0; i < polygons_in_sector * 4; i += 4)
		{
			(*sector)[i].position = get_circle_point(to_radian(angle), size, starting_point);
			(*sector)[i + 1].position = get_circle_point(to_radian(angle), external, starting_point);
			angle += ANGLE;
			(*sector)[i + 2].position = get_circle_point(to_radian(angle), external, starting_point);
			(*sector)[i + 3].position = get_circle_point(to_radian(angle), size, starting_point);
		}

		for (size_t i = 0; i < sector->getVertexCount(); i++)
			(*sector)[i].position = se::to_SFML_coordinates((*sector)[i].position);

		return sector;
	}
	sf::VertexArray* create_OR_base(size_t polygons_in_sector,
		sf::Vector2f size, float thickness, sf::Vector2f starting_point)
	{
		auto* elem = new sf::VertexArray(sf::PrimitiveType::Quads, polygons_in_sector * 8);

		sf::Vector2f center = starting_point;
		center.y -= size.y / 2.f;
		center.x -= size.x / 2.f;


		float rotate;

		float angle = 0.f;
		while (true)
		{
			if (get_circle_point(to_radian(angle), size, center).x < starting_point.x)
			{
				rotate = 360.f / angle / 6.2223f;
				size.y *= 1.f / std::sin(to_radian(angle));
				break;
			}
			else
				angle += 1.f;
		}
		sf::Vector2f external(size.x + thickness, size.y + thickness);

		angle = 0.f;
		for (size_t i = 0; i < polygons_in_sector * 4; i += 4)
		{
			(*elem)[i].position = get_circle_point(to_radian(angle), size, center);
			(*elem)[i + 1].position = get_circle_point(to_radian(angle), external, center);
			angle += rotate;
			(*elem)[i + 2].position = get_circle_point(to_radian(angle), external, center);
			(*elem)[i + 3].position = get_circle_point(to_radian(angle), size, center);
		}
		center.x += size.x;
		for (size_t i = 0; i < polygons_in_sector * 4; i++)
		{
			sf::Vector2f back_point(starting_point.x - (*elem)[i].position.x, (*elem)[i].position.y);
			(*elem)[i + polygons_in_sector * 4].position = back_point;
		}

		for (size_t i = 0; i < elem->getVertexCount(); i++)
			(*elem)[i].position = se::to_SFML_coordinates((*elem)[i].position);

		return elem;
	}
}

namespace se
{
	namespace Drawings
	{
		VertexObject NOT, OR, NOT_OR, AND, NOT_AND, XOR, NOT_XOR;
	}
	void Element::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(drawing);
	}

	void Element::init()
	{
		sf::Vector2f size(10.f, 10.f);

		{
			sf::VertexArray* triangle, * circle;

			sf::Vector2f external(0.15f, 0.15f);
			sf::Vector2f internal(0.15f - 0.05f, 0.15f - 0.05f);
			sf::Vector2f starting_pos(0.f, 0.47f);
			circle = create_circle(POLYGONS_IN_CIRCLE, external, internal, starting_pos);
			sf::Vector2f left(-0.45f, -0.45f);
			float right = 0.45f;
			float top = 0.3f;
			triangle = create_triangle(left, right, top, 0.05f);

			Drawings::NOT.add(circle);
			Drawings::NOT.add(triangle);
		}
		{
			sf::VertexArray* element;

			float base_length = 0.90f;
			float lines_length = 0.4f;
			float sector_length = 0.4f;
			float thickness = 0.05f;
			sf::Vector2f center(0.f, -0.05f);
			element = create_AND_element(POLYGONS_IN_SECTOR, center, thickness, base_length, lines_length, sector_length);

			Drawings::AND.add(element);

			sf::VertexArray* circle;
			sf::Vector2f external(0.15f, 0.15f);
			sf::Vector2f internal(0.15f - 0.05f, 0.15f - 0.05f);
			sf::Vector2f starting_pos(0.f, 0.52f);
			circle = create_circle(POLYGONS_IN_CIRCLE, external, internal, starting_pos);
			element = new sf::VertexArray(*element);

			Drawings::NOT_AND.add(element);
			Drawings::NOT_AND.add(circle);
		}
		{
			sf::VertexArray *element, *sector1, *sector2, *circle;
			sf::Vector2f size(0.9f, 0.4f);
			sf::Vector2f starting_point(0.f, -0.675f);
			element = create_OR_base(POLYGONS_IN_SECTOR, sf::Vector2f(0.9f, 1), 0.05f, sf::Vector2f(0.f, -0.175f));
			sf::Vector2f external(0.15f, 0.15f);
			sf::Vector2f internal(0.15f - 0.05f, 0.15f - 0.05f);
			sf::Vector2f starting_pos(0.f, 0.49f);
			circle = create_circle(POLYGONS_IN_CIRCLE, external, internal, starting_pos);

			sector1 = create_sector_for_OR(POLYGONS_IN_SECTOR, size, 0.05f, starting_point);
			starting_point.y -= 0.2f;
			sector2 = create_sector_for_OR(POLYGONS_IN_SECTOR, size, 0.05f, starting_point);

			Drawings::OR.add(element);
			Drawings::OR.add(sector1);

			element = new sf::VertexArray(*element);
			sector1 = new sf::VertexArray(*sector1);

			Drawings::NOT_OR.add(element);
			Drawings::NOT_OR.add(sector1);
			Drawings::NOT_OR.add(circle);

			element = new sf::VertexArray(*element);
			sector1 = new sf::VertexArray(*sector1);

			Drawings::XOR.add(element);
			Drawings::XOR.add(sector1);
			Drawings::XOR.add(sector2);

			element = new sf::VertexArray(*element);
			sector1 = new sf::VertexArray(*sector1);
			sector2 = new sf::VertexArray(*sector2);
			circle = new sf::VertexArray(*circle);

			Drawings::NOT_XOR.add(element);
			Drawings::NOT_XOR.add(sector1);
			Drawings::NOT_XOR.add(sector2);
			Drawings::NOT_XOR.add(circle);
		}
	}
	Element::Element(ElemType type, ElemDirection direction, sf::Vector2f position)
	{
		setType(type);
		setDirection(direction);
		setPosition(position);
	}

	void Element::Step()
	{
		bool erase_first_signal = false;
		bool new_signal = true;

		bool down_arriver = down.Step(size);
		bool left_arriver = left.Step(size);
		bool right_arriver = right.Step(size);

		//PRINTR(left.list.size());

		sender.gives_signal = false;

		switch (type)
		{
		case ElemType::NOT:
			if (down_arriver)
				sender.gives_signal = true;
			break;
		case ElemType::AND:
		case ElemType::NOT_AND:
			if (left_arriver && right_arriver)
				sender.gives_signal = true;
			break;
		case ElemType::OR:
		case ElemType::NOT_OR:
			if (left_arriver || right_arriver)
				sender.gives_signal = true;
			break;
		case ElemType::XOR:
		case ElemType::NOT_XOR:
			if (left_arriver || right_arriver)
				sender.gives_signal = true;
			if (left_arriver && right_arriver)
				sender.gives_signal = false;
			break;
		}
		if (type & ElemType::NOT)
			sender.gives_signal = !sender.gives_signal;

		sender.GiveSignal();
	}

	void Element::setType(ElemType type)
	{
		this->type = type;

		switch (type)
		{
		case ElemType::NOT:
			drawing = Drawings::NOT;
			size = 1.15f;
			break;
		case ElemType::OR:
			drawing = Drawings::OR;
			size = 1.f;
			break;
		case ElemType::NOT_OR:
			drawing = Drawings::NOT_OR;
			size = 1.f;
			break;
		case ElemType::AND:
			drawing = Drawings::AND;
			size = 0.8f;
			break;
		case ElemType::NOT_AND:
			drawing = Drawings::NOT_AND;
			size = 1.f;
			break;
		case ElemType::XOR:
			drawing = Drawings::XOR;
			size = 1.f;
			break;
		case ElemType::NOT_XOR:
			drawing = Drawings::NOT_XOR;
			size = 1.f;
			break;
		}
	}
	void Element::setDirection(ElemDirection direction)
	{
		this->direction = direction;
	}
	void Element::setPosition(sf::Vector2f position)
	{
		sf::Vector2f offset(position.x - pos.x, position.y - pos.y);
		pos = position;

		drawing.move(offset);
	}
	void Element::setReceiver(Receiver* receiver)
	{
		sender.receiver = receiver;
	}
	void Element::setColor(sf::Color color)
	{
		for (size_t array_index = 0; array_index < drawing.size(); array_index++)
		{
			sf::VertexArray& array = drawing[array_index];
			for (size_t i = 0; i < array.getVertexCount(); i++)
				array[i].color = color;
		}
	}
}