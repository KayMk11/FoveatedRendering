#pragma once

class KeyboardListener
{
public:
    virtual void process_keyboard(float delta_time, std::vector<unsigned int> &pressed_keys) = 0;
};

class MouseListener
{
public:
    bool first_mouse = true;
    float last_x;
    float last_y;

    virtual void process_mouse(float new_x, float new_y) = 0;
};

class ScrollListener
{
public:
    virtual void process_scroll(float offset) = 0;
};