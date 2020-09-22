#include <iostream>
#include "header.hpp"
#include "json/json.h"
#include "File.hpp"
#include "Style.hpp"

/*
#define READ_COLOR(name, field)					\
if (value[#name][0].isUInt())					\
	Style::field.r = value[#name][0].asUInt();	\
else return false;								\
if (value[#name][1].isUInt())					\
	Style::field.g = value[#name][1].asUInt();	\
else return false;								\
if (value[#name][2].isUInt())					\
	Style::field.b = value[#name][2].asUInt();	\
else return false/**/

namespace se
{
	bool read_color(const Json::Value& value, const std::string&& field_name, sf::Color& color)
	{
		if (value[field_name][0].isUInt())
			color.r = value[field_name][0].asUInt();
		else return false;
		if (value[field_name][1].isUInt())
			color.g = value[field_name][1].asUInt();
		else return false;
		if (value[field_name][2].isUInt())
			color.b = value[field_name][2].asUInt();
		else return false;

		return true;
	}
	bool parse_style(const Json::Value& value, se::Style& style)
	{
		bool result = true;
		result &= read_color(value, "background", style.BackGround);
		result &= read_color(value, "back_lines", style.BackLines);
		result &= read_color(value, "wires", style.WireColor);
		result &= read_color(value, "element", style.ElementColor);
		result &= read_color(value, "signal", style.SignalColor);

		//return true;
		return result;
	}
	bool read_config(se::Style& style)
	{
		al::File file;
		if (file.open("config.json", "r"))
		{
			std::string config_data;
			file.read(config_data);

			Json::CharReaderBuilder reader_builder;
			Json::CharReader* reader = reader_builder.newCharReader();
			Json::Value value;
			std::string errors;
			reader->parse(config_data.data(), config_data.data() + config_data.size(), &value, &errors);
			delete reader;

			/*
			"window": {
				"height": number,
				"width": number
			} */
			Json::Value window_value = value["window"];
			if (window_value["height"].isUInt())
				HEIGHT = window_value["height"].asUInt();
			else return false;

			if (window_value["width"].isUInt())
				WIDTH = window_value["width"].asUInt();
			else return false;

			// "style": "name of style"
			Json::Value style_value = value["style"];
			if (style_value.isString())
			{
				al::File style_file;
				std::string style_data;

				style_file.open(style_value.asString() + ".json", "r");
				style_file.read(style_data);

				reader = reader_builder.newCharReader();
				reader->parse(style_data.data(), style_data.data() + style_data.size(), &style_value, &errors);
				delete reader;
				return parse_style(style_value, style);
			}
			else if (style_value.isObject())
			{
				return parse_style(style_value, style);
			}
			else
			{
				return false;
			}

			return true;
		}
		else
		{
			return false;
		}
	}
}