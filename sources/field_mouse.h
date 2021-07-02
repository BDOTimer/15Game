#ifndef field_mouse_h
#define field_mouse_h
///----------------------------------------------------------------------------|
/// "field_mouse.h"
///----------------------------------------------------------------------------:
#include "field.h"

struct  Field_mouse
{       Field_mouse(Field& f) : field(f)
        {
        }

    Field& field;

    bool scan_press(const sf::Vector2i& mouse_pos)
    {
        if(!over) return false;
        sf::FloatRect cur(mouse_pos.x, mouse_pos.y, 2, 2);

        for     (size_t r = 0; r < field.m   .size(); ++r)
        {   for (size_t c = 0; c < field.m[0].size(); ++c)
            {
                sf::FloatRect fish(field.m[r][c]->position, CFG->sizefish);
                if(cur.intersects(fish))
                {
                    eDIR dirr  = field.where_do_step({(int)c, (int)r});
                    if(  dirr != NONE )
                    {   field.m[r][c]->start(100, dirr);
                    }
                    return true;
                }
            }
        }
        return false;
    }

    bool over = false;
    bool scan_over(const sf::Vector2i& mouse_pos)
    {   sf::FloatRect cur(mouse_pos.x, mouse_pos.y, 2, 2);
        over = false;

        for     (size_t r = 0; r < field.m   .size(); ++r)
        {   for (size_t c = 0; c < field.m[0].size(); ++c)
            {
                sf::FloatRect fish(field.m[r][c]->position, CFG->sizefish);
                if(cur.intersects(fish))
                {
                    field.m[r][c]->mouse_over = true;

                    if(overxy.y != r || overxy.x != c)
                    {   field.m[overxy.y][overxy.x]->mouse_over = false;

                        overxy.x = c;
                        overxy.y = r;
                    }

                    over = true;
                    return true;
                }
            }
        }

        field.m[overxy.y][overxy.x]->mouse_over = false;
        over = false;
        return false;
    }

    Vec2 overxy{0,0};

    void set_style_over(sf::RectangleShape& f)
    {   f.setFillColor(sf::Color{200, 220, 40});
    }

    void set_style_standart(sf::RectangleShape& f)
    {   f.setFillColor(sf::Color::Yellow);
    }
};

#endif // field_mouse_h
