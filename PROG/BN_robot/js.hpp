#ifndef JS_H
#define JS_H

#include <iostream>
#include <vector>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <wiringPi.h>
#include <cmath>
#include <linux/joystick.h>

struct axis_state {
    short x, y;
};

enum class GamePadType {
    None = 0,
    Button = 1,
    Axis = 2
};

class Js {
        public:
                Js(bool active);
                Js(bool active, int lim_min, int lim_max);
                ~Js();

                std::vector<int> GetEvent();
        private:
                size_t get_axis_state(struct js_event *event, struct axis_state axes[3]);
                int read_event(int fd, struct js_event *event);

                int m_js;

                bool m_active = false;

                double m_lim1, m_lim2;
};

#endif //S_H
