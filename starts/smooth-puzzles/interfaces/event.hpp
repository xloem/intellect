#include <stdint.h>
#include <stdlib.h>
#include <linux/input.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <iostream>
struct dev_input_event
{
	double time;
	double where;
	double value;

	operator bool() { return time != 0; }

	bool is(double location)
	{
		//std::cerr << "time=" << time << std::endl;
		//std::cerr << "location?" << location << "=" << where << std::endl;
		//std::cerr << "value=" << value << std::endl;
		return time != 0 && where == location;
	}

	bool is_press(double location)
	{
		return is(location) && value == 1;
	}

	bool is_release(double location)
	{
		return is(location) && value == 0;
	}
};

class dev_input_event_source
{
public:
	dev_input_event_source(char const * path)
	 // one place to identify these is /proc/bus/input/devices .  keyboard has EV=120013, maybe find event # from Handlers
	: event_descriptor(open(path, O_RDONLY))
	{ }
	double spacebar_location()
	{
		return event_location(INPUT_PROP_DIRECT, KEY_SPACE);
	}
	dev_input_event read()
	{
		dev_input_event result;
		struct input_event event; // time, type, code, value
		size_t size = ::read(event_descriptor, &event, sizeof(event));
		if (size < sizeof(event)) {
			result.time = 0;
		} else {
			result.time = event.time.tv_sec + event.time.tv_usec / 1000000;
			result.where = event_location(event.type, event.code);
			result.value = event.value;
		}
		return result;
	}
	~dev_input_event_source()
	{
		close(event_descriptor);
		event_descriptor = 0;
	}
private:
	double event_location(uint16_t type, uint16_t code)
	{
		uint32_t combined = (type << (sizeof(code)*8)) + code;
		srandom(combined);
		double offset = random() / double(RAND_MAX) / 128.;
		// we have the option of making things be different in the future,
		// just a little.
		return combined + offset;
	}
	int event_descriptor;
};
