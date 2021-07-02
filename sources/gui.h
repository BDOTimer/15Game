#ifndef gui_h
#define gui_h
///----------------------------------------------------------------------------|
/// "gui.h"
///----------------------------------------------------------------------------:
#include "mylib.h"
#include "config.h"
#include "sound/sound.h"
#include <functional>

#include "animat.h"

enum eSTATUS_BATTON
{   E_OVER ,
    E_CLICK,
    E_FREE
};

struct  Config_style
{       Config_style()
        {   font.loadFromFile("snd/consola.ttf");
        }

    sf::Color f              = sf::Color::White;
    sf::Color b              = {120,150,180};
    sf::Color border         = {150,180,210};
    sf::Color border_ms_over = {210,180,210};
    sf::Color b_ms_over      = {120,180,240};

    sf::Font  font;
};

///----------------------------------------------------------------------------|
/// Интефейс батона.
///----------------------------------------------------------------------------:
struct IButton
{   IButton(std::wstring s, sf::Rect<int> r) :
        str  (s),
        rect (r),
        shape({float(r.width), float(r.height)})
    {   init ( );
    }
    virtual ~IButton() {}

    virtual void draw() = 0;

    bool is_cursor(sf::Vector2i cur)
    {   /// ...
        return false;
    }

    void init()
    {   shape.setPosition        (rect.left, rect.top);
        shape.setOutlineColor    (style.border       );
        shape.setOutlineThickness(3                  );
        shape.setFillColor       (style.b            );

        bind_style(text);

        //test = Config_style::test;
    }

    std::wstring                str;
    sf::Text                   text;
    sf::Rect<int>              rect;
    sf::RectangleShape        shape;
    eSTATUS_BATTON  status = E_FREE;

    std::function<void()> callback;

    void bind_style(sf::Text& txt)
    {   txt.setFont         (style.font         );
        txt.setCharacterSize(20                 );
        txt.setStyle        (sf::Text::Regular  );
        txt.setFillColor    (style.f            );
        txt.setString       (str.c_str()        );

        sf::Vector2f pos(      rect.left,         rect.top    );
        sf::Vector2f sz (float(rect.width), float(rect.height));

        sf::FloatRect b = txt.getLocalBounds();

        float px = (rect.width  - b.width)/2;
        float py = (rect.height - b.height)/2;

        /// l(b.top)
        /// l(b.left)
        /// l(b.width)
        /// l(b.height)

        txt.setPosition(rect.left + px, rect.top + py - b.height / 2);
    }

    static sf::RenderWindow* window;
    static Config_style       style;
};
Config_style      IButton::style;
sf::RenderWindow* IButton::window = nullptr;

///----------------------------------------------------------------------------|
/// Стандартный батон.
///----------------------------------------------------------------------------:
struct  Button : public IButton
{       Button(std::wstring s, sf::Rect<int> r) : IButton(s, r)
        {   str = s;
        }
       ~Button(){}

    void draw()
    {   window->draw(shape);
        window->draw(text );
    }
};

///----------------------------------------------------------------------------|
/// Юзерская группа батонов.
///----------------------------------------------------------------------------:
struct  Button_game
{       Button_game() : posx(420), posy(10), posstep(40)
        {   bats.emplace_back(new Button
                    (L"Cмешать!",             {posx, posy, 250, 30}));

            posy += posstep;

            bats.emplace_back(new Button
                    (L"Сдвинуть рандомно...", {posx, posy, 250, 30}));

            posy += posstep;

            bats.emplace_back(new Button
                    (L"Сброс.", {posx, posy, 250, 30}));

            posy += posstep;

            bats.emplace_back(new Button
                    (L"Собрать.", {posx, posy, 250, 30}));

            posy += posstep*2;

            bats.emplace_back(new Button
                    (L"ВЫХОД.", {posx, posy, 250, 30}));
        }
       ~Button_game()
        {   for(auto p : bats) delete p;
        }

    int posx, posy, posstep;

    std::vector<Button*> bats;

    auto& get(){ return bats; }

    void draw()
    {   for(auto p : bats) p->draw();
    }

    Button* pbat_over = nullptr;
    void cursor_over(const sf::Vector2i& cur)
    {   sf::IntRect c(cur.x, cur.y, 2, 2);
        auto  t = pbat_over;
        pbat_over = nullptr;
        for(auto p : bats)
        {   if(p->rect.intersects(c))
            {    p->shape.setOutlineColor(IButton::style.border_ms_over);
                 p->shape.setFillColor   (IButton::style.b_ms_over);
                 pbat_over = p;
            }
            else
            {   p->shape.setOutlineColor(IButton::style.border);
                p->shape.setFillColor   (IButton::style.b     );
            }
        }

        if(nullptr == pbat_over && nullptr != t)
        {   t->shape.setOutlineColor(IButton::style.border);
            t->shape.setFillColor   (IButton::style.b);
        }
    }

    void cursor_click()
    {   if(nullptr != pbat_over)
        {   pbat_over->callback();
            sound.play(NSound::ESOUND::E_CLICK);
        }
    }

    void disable_bat()
    {   for(auto p : bats)
        {   p->shape.setOutlineColor(sf::Color(100, 100 , 100));
            p->shape.setFillColor   (sf::Color( 90,  90 ,  50));
            p->text.setFillColor    (sf::Color( 90, 150 ,  50));
        }
    }

    void enable_bat()
    {   for(auto p : bats)
        {   p->shape.setOutlineColor(IButton::style.border);
            p->shape.setFillColor   (IButton::style.b     );
            p->text.setFillColor    (IButton::style.f     );
        }
    }
};

///--------------------------|
/// Тест.                    |
///--------------------------:
inline void testclass_Button()
{
    sf::RenderWindow window(sf::VideoMode(cfg_default.window.x,
                                          cfg_default.window.y),
                                          "testclass_Visualisator");
    IButton::window = &window;

    Button_game  button_game;
    sf::Vector2i  mouse_pos;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close ();
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {   window.close();
            }

            mouse_pos = sf::Mouse::getPosition(window);
        }

        button_game.cursor_over(mouse_pos);

        window.clear    ();
        button_game.draw();
        window.display  ();
    }
}

#endif // gui_h
