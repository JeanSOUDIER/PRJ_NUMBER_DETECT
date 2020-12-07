#include "js.hpp"

Js::Js(bool active) 
	: Js(active,0,10) {} //default init

Js::Js(bool active, int lim_min, int lim_max) {
	m_lim1 = 32768/(lim_max-lim_min); //adjust the outputs
	m_lim2 = 65536/(lim_max-lim_min);
    m_active = active;
    if(m_active) {
		const std::string device = "/dev/input/js0"; //select the current joystick
        m_js = open(&device[0], O_RDONLY); //open it

        if (m_js == -1) {
            perror("Could not open joystick"); //throw error
            m_active = false;
        }
    }
}

Js::~Js() {
    if(m_active) {close(m_js);}
}

std::vector<int> Js::GetEvent() {
    std::vector<int> res = {static_cast<int>(GamePadType::None), 0, 0, 0};
    if(m_active) {
    	struct js_event event;
    	struct axis_state axes[3] = {0, 0, 0};
    	size_t axis;
    	while(res[0] == static_cast<int>(GamePadType::None)) { //while no event
	    	fflush(stdout);
	    	read_event(m_js, &event);
	        switch (event.type) {
	            case JS_EVENT_BUTTON:{
	            	res[0] = static_cast<int>(GamePadType::Button); //send button event
	            	res[1] = static_cast<int>(event.number);
	            	res[2] = static_cast<int>(event.value);
	                break;
	            }
	            case JS_EVENT_AXIS:{
	                size_t axis;
	    		    struct axis_state axes[3] = {{0 , 0}};
	    		    int x = 0, y = 0, z = 0, zz = 0;
	    		    axis = get_axis_state(&event, axes);
	                if(axis == 0) {
	                    x = (int)(axes[axis].x);
	                    if(std::abs(x) < 1000) {x = 0;}
	                    x = -x/m_lim1;
	                    y = (int)(axes[axis].y);
	                    if(std::abs(y) < 1000) {y = 0;}
	                    y = -y/m_lim1;
	                } else {
	                	if(static_cast<int>(axes[axis].x) != -32768 && static_cast<int>(axes[axis].y) != -32768) {
							if(axis == 1) {
		                        z = (int)(axes[axis].x)+32767;
		                    } else {
		                        zz = (int)(axes[axis].y)+32767;
		                    }
		                    z = (z-zz)/m_lim2;
		                } else {
		                	z = 0;
		                }
	                }
	                std::cout << static_cast<int>(axes[axis].x) << " " << static_cast<int>(axes[axis].y) << std::endl;
	    		    res[0] = static_cast<int>(GamePadType::Axis); //send axis event
	    		    res[1] = static_cast<int>(x);
	    		    res[2] = static_cast<int>(y);
	    		    res[3] = static_cast<int>(z);
	                break;
	            }

	        	default:{
	                std::cout << "unknown event js0 " << static_cast<int>(event.type) << std::endl;
	                break;
	            }
	        }
	    }
    } else {
    	std::cout << "not active" << std::endl;
    }
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
    ssize_t bytes;

    bytes = read(fd, event, sizeof(*event));

    if (bytes == sizeof(*event))
        return 0;

    /* Error, could not read full event. */
    return -1;
}
