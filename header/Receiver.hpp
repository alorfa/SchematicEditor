#pragma once

#include "Signal.hpp"
#include <list>
#include <memory>

namespace se
{
	class Receiver
	{
	public:
		std::list<std::unique_ptr<Signal>> list;
	public:
		bool got_signal;

		Receiver();
		~Receiver();

		//returns true if at least one signal reached the end
		bool Step(float limit);
		void Reset();

		inline void ReceiveSignal()
		{
			got_signal = true;
		}
	};
}