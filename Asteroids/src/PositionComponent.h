#pragma once
#include "Component.h"
class PositionComponent :
    public Component {
public:
    PositionComponent(int t_x, int t_y);

    void update(int d_t);
    int x(void) const;
    int y(void) const;

    void set_x(int t_x);
    void set_y(int t_y);

    int x_velocity(void) const;
    int y_velocity(void) const;

    void set_x_velocity(int t_x_vel);
    void set_y_velocity(int t_y_vel);

private:
    int m_x = 0;
    int m_y = 0;
    int m_x_velocity = 0;
    int m_y_velocity = 0;
};

