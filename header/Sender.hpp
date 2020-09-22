#pragma once

#include "Receiver.hpp"

namespace se
{
	class Sender
	{
	public:
		bool gives_signal;
		Receiver* receiver;

		inline void GiveSignal()
		{
			if (gives_signal && receiver)
				receiver->ReceiveSignal();
		}
	};
}