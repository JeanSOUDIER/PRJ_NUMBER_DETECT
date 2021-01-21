#ifndef JS_H
#define JS_H

#include <wiringPi.h>

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/joystick.h>

#include <cmath>
#include <iostream>
#include <string>
#include <vector>

/**

                     JS class

    DUHAMEL Erwan (erwanduhamel@outlook.com)
    SOUDIER Jean  (jean.soudier@insa-strasbourg.fr)


    Provides a class for Joystick arm piloting.

    Features :

    • Pilote the arm with a gamepad Xbox360 (never used because to hard to stabilised)

    Functions :

    • JS                           | activates the joystick with the limits of the specified in arguments
    • GetEvent()                   | returns the shifting in a vector of <type,datas> with <TypeAxis,Xaxis,Yaxis,LR/RT> or <TypeButton,Number,Value,0>
    • get_axis_state()             | returns the axis with is current state
    • read_event()                 | returns the current event of the gamepad

*/

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
