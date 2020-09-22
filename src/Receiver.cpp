#include "Receiver.hpp"

namespace se
{
	Receiver::Receiver()
		: got_signal(false) {}

	Receiver::~Receiver() {}

	bool Receiver::Step(float limit)
	{
		bool erase_first_signal = false;
		bool new_signal = true;
		bool signal_arrived = false;

		for (auto it = list.begin(); it != list.end(); ++it)
		{
			(*it)->Step(got_signal, limit);
			if ((*it)->getBegin() == 0.f)
				new_signal = false;
			if (!got_signal)
				new_signal = false;
			if ((*it)->getBegin() >= limit)
				erase_first_signal = true;
			if ((*it)->getEnd() >= limit)
				signal_arrived = true;
		}
		if (list.empty() && !got_signal)
			new_signal = false;

		if (new_signal)
			list.push_back(std::move(std::unique_ptr<Signal>(new Signal())));
		if (erase_first_signal)
			list.pop_front();

		got_signal = false;

		return signal_arrived;
	}
	void Receiver::Reset()
	{
		for (auto it = list.begin(); it != list.end(); ++it)
			(*it)->Reset();
	}
}