#include "header.hpp"
#include <iostream>
#include <ctime>
#include <thread>
#include <windows.h>
#include "Wire.hpp"
#include "Element.hpp"
#include "WireSignal.hpp"
#include "Style.hpp"
#include "SchematicEditor.hpp"

using std::cin;
using std::cout;
using std::cerr;
using std::endl;

uint WIDTH, HEIGHT;
uint FPS;

bool left_is_pressed;
bool right_is_pressed;

constexpr float ZOOM = 1.1f;

int main(int argc, char** argv)
{
	sf::RenderWindow window;

	se::Style style;
	if (se::read_config(style))
	{
		cout << "config is readed!" << endl;
	}
	else
	{
		cout << "Error: config cannot be readed!" << endl;
		return -1;
	}

	window.create(sf::VideoMode(WIDTH, HEIGHT), "Schematic Editor", sf::Style::Default);
	{
		DISPLAY_DEVICE device;
		DEVMODE devmode;
		ZeroMemory(&device, sizeof device);
		ZeroMemory(&devmode, sizeof devmode);
		device.cb = sizeof(device);
		EnumDisplayDevices(NULL, (DWORD)0, &device, 0);
		devmode.dmSize = sizeof(DEVMODE);
		EnumDisplaySettings((LPSTR)device.DeviceName, ENUM_REGISTRY_SETTINGS, &devmode);
		FPS = devmode.dmDisplayFrequency;
	}
	window.setFramerateLimit(FPS);

	{
		double begin = clock();
		se::Element::init();
		se::Element::setPassiveColor(style.ElementColor);
		se::Element::setActiveColor(style.SignalColor);
		se::WireSignal::setColor(style.SignalColor);
		se::Signal::setSignalSpeed(4.f / FPS);
		se::Wire::setColor(style.WireColor);
		cout << (clock() - begin) / 1000. << " seconds" << endl;
	}

	PRINT(sizeof(sf::VertexArray));
	cout << endl;

	sf::Vector2u window_size;
	sf::Vector2i mouse_pos;

	se::SchematicEditor editor(&window, style);

	std::vector<se::Element*> elements;
	elements.push_back(new se::Element(se::NOT, se::Up, sf::Vector2f(0.f, -1.f)));
	elements.push_back(new se::Element(se::OR, se::Right, sf::Vector2f(1.5f, -1.f)));
	elements.push_back(new se::Element(se::AND, se::Down, sf::Vector2f(3.f, -1.f)));
	elements.push_back(new se::Element(se::XOR, se::Left, sf::Vector2f(4.5f, -1.f)));
	elements.push_back(new se::Element(se::NOT_OR, se::Up, sf::Vector2f(6.f, -1.f)));
	elements.push_back(new se::Element(se::NOT_AND, se::Up, sf::Vector2f(7.5f, -1.f)));
	elements.push_back(new se::Element(se::NOT_XOR, se::Up, sf::Vector2f(9.f, -1.f)));

	se::Wire test_wire(se::WireRotate::Up, sf::Vector2f(), 10.f);
	se::Wire second_wire(se::WireRotate::Right, sf::Vector2f(0.f, 10.f), 5.f);

	test_wire.setReceiver(&second_wire.receiver);

	constexpr size_t elem_index = 6;

	elements[elem_index]->setReceiver(&test_wire.receiver);

	//std::thread thread(rotate_wire, &test_wire);
	//thread.detach();

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				switch (event.key.code)
				{
				case sf::Keyboard::Escape:
				case sf::Keyboard::Enter:
					window.close();
					break;
				}
				break;
			case sf::Event::Resized:
			{
				sf::Vector2u new_window_size = window.getSize();
				sf::Vector2f new_camera_size(editor.getCamera().getSize());

				new_camera_size.x *= (float)new_window_size.x / (float)window_size.x;
				new_camera_size.y *= (float)new_window_size.y / (float)window_size.y;

				editor.setCameraSize(new_camera_size);
				break;
			}
			case sf::Event::MouseWheelScrolled:
				if (event.mouseWheelScroll.delta == 1)
					editor.scale(1.f / ZOOM);
				if (event.mouseWheelScroll.delta == -1)
					editor.scale(ZOOM);
				break;
			}
		}
		if (window.hasFocus())
		{
			left_is_pressed = false;
			right_is_pressed = false;
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
				left_is_pressed = true;
			if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
				right_is_pressed = true;
		}
		if (right_is_pressed)
		{
			sf::Vector2i new_mouse_pos = sf::Mouse::getPosition(window);
			sf::Vector2i removal;

			removal.x = new_mouse_pos.x - mouse_pos.x;
			removal.y = new_mouse_pos.y - mouse_pos.y;

			editor.moveCamera(removal);

			elements[elem_index]->right.ReceiveSignal();
		}
		if (left_is_pressed)
		{
			elements[elem_index]->left.ReceiveSignal();
		}

		mouse_pos = sf::Mouse::getPosition(window);

		window_size = window.getSize();

		editor.useCamera();

		window.clear(style.BackGround);

		editor.drawLinies();

		for (auto ptr : elements)
		{
			ptr->Step();
			window.draw(*ptr);
		}
		test_wire.Step();
		second_wire.Step();
		window.draw(test_wire);
		window.draw(second_wire);
		window.display();
	}

	return 0;
}