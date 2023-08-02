#pragma once
#include <chrono>
#include <thread>
#include <cstdint>

// class for measure time passed for some caculations and procedures of functions.
struct Timer_t {

	using Clk       = std::chrono::steady_clock;
	using Duration  = std::chrono::duration<uint64_t, std::ratio<1, 1000000000>>;
	using TimePoint = std::chrono::time_point<Clk, Duration>;

	explicit Timer_t() { m_start = Clk::now(); } // se instancia y se aprovecha se marcar inicio

	void start() { m_start =  Clk::now(); }

	uint64_t timePassed() const
	{
		Duration passed { Clk::now() - m_start };
		return passed.count();
	}

	uint64_t waitForUntil_ns(const uint64_t& timeMustPass) const
	{
		if (timeMustPass > timePassed()){
			Duration toWait { timeMustPass - timePassed() };
			std::this_thread::sleep_for(toWait);
			return toWait.count();
		}
		return 0;
	}

	template<typename CALLABLE>
	auto measureTimeToProcc(CALLABLE proccess)
	{
	    Timer_t timer {};
	    proccess();
	    return static_cast<double>(timer.timePassed())/1000000; // ms
	}

private:
	TimePoint m_start;
};