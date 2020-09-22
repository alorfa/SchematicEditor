#pragma once

#include "header.hpp"

namespace se
{
	class Signal
	{
	protected:
		float begin;
		float end;
	public:
		static void setSignalSpeed(float cell_per_frame);

		Signal();

		void Step(bool has_signal, float limit);
		void Reset();

		inline float getBegin() {
			return begin;
		}
		inline float getEnd() {
			return end;
		}
	};
}

std::ostream& operator<<(std::ostream& stream, se::Signal& signal);