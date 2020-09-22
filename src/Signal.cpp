#include "Signal.hpp"

namespace se
{
	void Signal::setSignalSpeed(float cell_per_frame)
	{
		SIGNAL_PER_SECOND = cell_per_frame;
	}

	Signal::Signal()
	{
		begin = 0.f;
		end = SIGNAL_PER_SECOND;
	}
	void Signal::Step(bool has_signal, float limit)
	{
		if (has_signal)
		{
			if (begin != 0.f)
				begin += SIGNAL_PER_SECOND;
		}
		else
		{
			begin += SIGNAL_PER_SECOND;
		}
		end += SIGNAL_PER_SECOND;
		if (end > limit)
			end = limit;
	}
	void Signal::Reset()
	{
		Signal();
	}
}

std::ostream& operator<<(std::ostream& stream, se::Signal& signal)
{
	stream << "[" << signal.getBegin() << ", " << signal.getEnd() << ']';

	return stream;
}