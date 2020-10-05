#ifndef JS_H
#define JS_H

#include <iostream>
#include <vector>
#include <termios.h>
#include <unistd.h>
#include <joystick.h>
#include <fcntl.h>

struct axis_state {
    short x, y;
};

enum class GamePadType {
    Button = 0,
    Axis = 1
};

class Js {
        public:
                Js();
                Js(int lim_min, int lim_max);
                ~Js();

                std::vector<int> GetEvent();
        private:
                size_t get_axis_state(struct js_event *event, struct axis_state axes[3]);
                int read_event(int fd, struct js_event *event);

                int m_lim_min, m_lim_max;
};

#endif //S_H
