#include "SchematicEditor.hpp"

namespace se
{
	SchematicEditor::SchematicEditor(sf::RenderWindow* window, se::Style& style)
	{
		camera = window->getView();

		setWindow(window);
		setStyle(style);
	}
	const sf::View& SchematicEditor::getCamera() const
	{
		return camera;
	}
	void SchematicEditor::setWindow(sf::RenderWindow* window)
	{
		this->window = window;
	}
	void SchematicEditor::setStyle(se::Style& style)
	{
		this->style = style;

		horizontal_line.setFillColor(style.BackLines);
		vertical_line.setFillColor(style.BackLines);
	}
	void SchematicEditor::setCamera(sf::View& camera)
	{
		this->camera = camera;
	}
	void SchematicEditor::scale(float scale)
	{
		sf::Vector2f new_size = camera.getSize();
		new_size *= scale;
		zoom *= scale;
		camera.setSize(new_size);
	}
	void SchematicEditor::moveCamera(sf::Vector2i removal)
	{
		sf::Vector2f new_pos = camera.getCenter();
		new_pos.x -= (float)removal.x * zoom;
		new_pos.y -= (float)removal.y * zoom;
		camera.setCenter(new_pos);
	}
	void SchematicEditor::setCameraSize(sf::Vector2f size)
	{
		camera.setSize(size);
	}
	void SchematicEditor::updateElements()
	{
		for (auto it = elements.begin(); it != elements.end(); ++it)
			it->Step();
	}
	void SchematicEditor::updateWires()
	{
		for (auto it = wires.begin(); it != wires.end(); ++it)
			it->Step();
	}
	void SchematicEditor::useCamera()
	{
		window->setView(camera);
	}
	void SchematicEditor::clearWindow()
	{
		window->clear(style.BackGround);
	}
	void SchematicEditor::drawLinies()
	{
		camera_starting_point.x = camera.getCenter().x - camera.getSize().x / 2;
		camera_starting_point.y = camera.getCenter().y - camera.getSize().y / 2;
		camera_final_point.x = camera_starting_point.x + camera.getSize().x;
		camera_final_point.y = camera_starting_point.y + camera.getSize().y;

		int starting_y;
		int starting_x;
		int offset;
		starting_y = (int)camera_starting_point.y;
		offset = starting_y % PIXELS_BETWEEN_LINES;
		starting_y -= offset;
		starting_x = (int)camera_starting_point.x;
		offset = starting_x % PIXELS_BETWEEN_LINES;
		starting_x -= offset;
		for (int y = starting_y - PIXELS_BETWEEN_LINES / 2; y < camera_final_point.y; y += PIXELS_BETWEEN_LINES)
		{
			horizontal_line.setPosition(
				camera_starting_point.x, (float)y);
			horizontal_line.setSize(sf::Vector2f(10000.f * zoom, 2.f * zoom));
			window->draw(horizontal_line);
		}
		for (int x = starting_x - PIXELS_BETWEEN_LINES / 2; x < camera_final_point.x; x += PIXELS_BETWEEN_LINES)
		{
			vertical_line.setPosition(
				(float)x, camera_starting_point.y);
			vertical_line.setSize(sf::Vector2f(2.f * zoom, 10000.f * zoom));
			window->draw(vertical_line);
		}
	}
	void SchematicEditor::drawElements()
	{
		for (auto it = elements.begin(); it != elements.end(); ++it)
			window->draw(*it);
	}
	void SchematicEditor::drawWires()
	{
		for (auto it = wires.begin(); it != wires.end(); ++it)
			window->draw(*it);
	}
}