#include "js.hpp"

Js::Js() 
	: Js(0,10) {}

Js::Js(int lim_min, int lim_max) {
	m_lim_min = lim_min;
	m_lim_max = lim_max;
	const char *device = "/dev/input/js0";
    const int js = open(device, O_RDONLY);

    if (js == -1) {
        perror("Could not open joystick");
    }
}

Js::~Js() {
    close(js);
}

std::vector<int> Js::GetEvent() {
	struct js_event event;
    read_event(js, &event);
    std::vector<int> res;
    switch (event.type){
        case JS_EVENT_BUTTON:{
        	res.push_back(GamePadType::Button);
        	res.push_back(event.number);
        	res.push_back(event.value);
            break;
        }
        case JS_EVENT_AXIS:{
            size_t axis;
		    struct axis_state axes[3] = {{0 , 0}};
		    int x = 0, y = 0, z = 0;
		    axis = get_axis_state(&event, axes);
		    if(axis == 0) {
		        x = static_cast<int>(axes[axis].x) / 3.2768;
		        y =static_cast<int>(axes[axis].y) / 3.2768;
		    }
		    else if(axis == 1) {
		        z = static_cast<int>((axes[axis].x)+32767)/6553600.;
		    } else{
		        z = -static_cast<int>((axes[axis].y)+32767) / 6553600.;

		    }
		    res.push_bak(GamePadType::Axis);
		    res.push_bak(x);
		    res.push_bak(y);
		    res.push_bak(z);
            break;
        }

        default:{
            std::cout << "unknown event js0" << std::endl;
            break;
        }
    }
    fflush(stdout);
    return res;
}

size_t Js::get_axis_state(struct js_event *event, struct axis_state axes[3]) {
    const size_t axis = event->number / 2;

    if (axis < 3) {
        if (event->number % 2 == 0){axes[axis].x = event->value;}
        else{axes[axis].y = event->value;}
    }
    return axis;
}

int Js::read_event(int fd, struct js_event *event) {
    return (static_cast<size_t>(read(fd , event, sizeof(*event))) == sizeof(*event)) ? 0 : -1;
}