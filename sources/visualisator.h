#ifndef visualisator_h
#define visualisator_h
///----------------------------------------------------------------------------|
/// "visualisator.h"
///----------------------------------------------------------------------------:
#include "field_mouse.h"

struct  Visualisator
{       Visualisator(sf::RenderWindow& w, Field& f) :
            window     (w      ),
            field      (f      ),
            field_mouse(field  ),
            hor   ({400+5,   5}),
            ver   ({    5, 400}),
            fish  ({hor.getSize().x/field.sz.x - 20,
                    ver.getSize().y/field.sz.y - 20}),
            click ({5,5})

        {   font .loadFromFile  ("snd/consola.ttf");

            text.setFont                (font);
            text.setCharacterSize         (50);
            text.setStyle  (sf::Text::Regular);
            text.setFillColor(sf::Color::Blue);
            text.setString           (L"123Я");

            fish .setFillColor(sf::Color::Yellow);
            click.setFillColor(sf::Color::Green );

            field.init_callback(button_game.get());

            field.button_game = &button_game;
        }

    sf::Vector2i mouse_pos;
    void draw()
    {

        while (window.isOpen())
        {   elapsed = clock.restart();

            sf::Event event;
            while (window.pollEvent(event))
            {
                if(field.exit_game) window.close();

                if (event.type == sf::Event::Closed)
                    window.close();

                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                {   window.close();
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {   field.mixedn();
                }
                /// Experimental ...
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
                {   field.m[1][1]->start(100, UP);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
                {   field.m[1][1]->start(100, LEFT);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
                {   field.m[1][1]->start(100, DOWN);
                }
                else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
                {   field.m[1][1]->start(100, RIGHT);
                }

                if(!field.is_ai)
                {   mouse_pos = sf::Mouse::getPosition(window);
                    button_game.cursor_over(mouse_pos);
                    field_mouse.scan_over  (mouse_pos);
                }

                if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && !field.is_ai)
                {   button_game.cursor_click();

                    ///-----------------|
                    /// Сканируем фишки.|
                    ///-----------------:
                    if(!field.anim->done)
                        field_mouse.scan_press(mouse_pos);
                }
            }

            window.clear();

            draw_grid();
            draw_fish();

            button_game.draw();

                        click.setPosition(mouse_pos.x, mouse_pos.y);
            window.draw(click);

            window.display();
        }
    }

private:
    sf::RenderWindow& window;
    sf::Font            font;
    sf::Text            text;
    Field&             field;
    Field_mouse  field_mouse;
    Button_game  button_game;
    sf::Clock          clock;
    sf::Time         elapsed;

    ///---------------------------|
    /// Нарисовать сетку.         |
    ///---------------------------:
    sf::RectangleShape hor, ver, fish, click;
    void draw_grid()
    {
        const sf::Vector2i SZ(field.sz.x + 1, field.sz.y + 1);

        auto h = hor;
        auto v = ver;

        const sf::Vector2f offset_x(400/field.sz.x, 0);
        const sf::Vector2f offset_y(0, 400/field.sz.y);

        for(int y = 0; y < SZ.y; ++y)
        {   window.draw(h);
                        h.move(offset_y);
        }

        for(int x = 0; x < SZ.x; ++x)
        {   window.draw(v);
                        v.move(offset_x);
        }
    }

    ///---------------------------|
    /// Нарисовать фигуры.        |
    ///---------------------------:
    void draw_fish()
    {   auto f = fish;
             f.move({6,6});

        auto t = sf::Text(text);
             t.move({6+5,6+5});

        const sf::Vector2f offset_x(400/field.sz.x,              0);
        const sf::Vector2f offset_y (-400         , 400/field.sz.y);

        field.anim->process();

        static sf::Time tai;
                        tai += elapsed;
        if(tai > sf::milliseconds(500))
        {   field.ai();
            tai = sf::Time::Zero;
        }

        for    (auto& r : field.m)
        {   for(auto& c :       r)
            {
                std::string s = std::to_string(c->n);
                t.setString(s);

                if(c->mouse_over) field_mouse.set_style_over    (f);
                else              field_mouse.set_style_standart(f);

                if(c->n != 16)
                {
                    f.setPosition(c->position);
                    t.setPosition(c->position);

                    if(c->go != 0.f)
                    {   auto a = c->moved();
                        auto ff = f;
                        auto tt = t;

                        ff.move(a.x, a.y);
                        tt.move(a.x, a.y);

                        window.draw(ff);
                        window.draw(tt);
                    }
                    else
                    {   window.draw(f);
                        window.draw(t);
                    }
                }
                //f.move(offset_x);
                //t.move(offset_x);
            }   //f.move(offset_y);
                //t.move(offset_y);
        }
        //window.draw(text);
    }
};

inline void testclass_Visualisator()
{
    Field field;

    sf::RenderWindow window(sf::VideoMode(cfg_default.window.x,
                                          cfg_default.window.y),
                                          "testclass_Visualisator");
    IButton::window = &window;

    Visualisator visualisator(window, field);
                 visualisator.draw();
}

#endif // visualisator_h
