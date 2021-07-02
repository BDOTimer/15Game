#ifndef field_h
#define field_h
///----------------------------------------------------------------------------|
/// "field.h"
///----------------------------------------------------------------------------:
#include "mylib.h"
#include "config.h"
#include "gui.h"

template<class T> using m1_t = std::vector<T>;
template<class T> using m2_t = std::vector<std::vector<T>>;

enum eDIR
{   UP   ,
    DOWN ,
    LEFT ,
    RIGHT,
    NONE
};

//struct Vec2 { float x, y;};
//struct Vec2i{ int   x, y;};

typedef sf::Vector2f Vec2 ;
typedef sf::Vector2i Vec2i;


struct  myImage
{   myImage()
    {
    }
    void draw()
    {
    }
};

///----------------------------------------------------------------------------|
/// Fish
///----------------------------------------------------------------------------:
struct  Fish
{   Fish(Animat*& a) : anim(a)
    {
    }

    int n = -1;

    void draw()
    {   if(n == 16) std::cout << '*';
        else        std::cout << n;
    }

    ///----------------------|
    /// Экран.               |
    ///----------------------:
    sf::Vector2f   position;
    Vec2               vec2;
    bool mouse_over = false;

    ///-------------------------| Experiment
    /// Анимация смещения.      |
    ///-------------------------:
    Animat*& anim;
    float      go;
    eDIR       dr;

    bool init_animat()
    {   anim->init(go, 100.f, CFG->speed_fish);

        go = 0.f;
        anim->init(go, 100.f, CFG->speed_fish);

        switch(dr)
        {
            case UP   :
            case LEFT :
            {
                anim->callb_calc  = [p = anim](){       *(p->val) -= p->speed;};
                anim->callb_is    = [p = anim](){return *(p->val) < -p->DD   ;};
                break;
            }
            case DOWN :
            case RIGHT:
            {
                anim->callb_calc  = [p = anim](){       *(p->val) += p->speed;};
                anim->callb_is    = [p = anim](){return *(p->val) >  p->DD   ;};
                break;
            }
            default   :;
        }
        return false;
    }

    void start(float Dist, eDIR drr)
    {   if(anim->done)     return;

        dr = drr;

        init_animat();
        anim->start();
        go     =    0;
    }

    const Vec2& moved()
    {   anim->process();

        switch(dr)
        {
            case LEFT:
            case RIGHT:
                vec2.x =  go;
                vec2.y =   0;
                break;

            case UP:
            case DOWN:
                vec2.x =   0;
                vec2.y =  go;
                break;

            default:;
        }

        return vec2;
    }

    void finish()
    {   go     =    0;
    }
};

///----------------------------------------------------------------------------|
/// Fild
///----------------------------------------------------------------------------:
struct  Field
{   Field() :
        anim   (new Animat),
        szfield(400,   400),
        sz     (  4,     4),
        szfish (  szfield.x/sz.x,   szfield.y/sz.y),
        m      (sz.y, m1_t<Fish*>(sz.x))

    {   for    (auto& r : m)
        {   for(auto& c : r)
            {   c = new Fish(anim);
            }
        }
        init();
    }

    Animat* anim = nullptr;
    ///-------------|
    /// Размер поля.|
    ///-------------:
    sf::Vector2f  szfield;
    sf::Vector2i  sz     ;
    sf::Vector2f  szfish ;

    void init()
    {   //szfish = sf::Vector2f{400/4, 400/4};

        const float NS = 6.f;

        sf::Vector2f pos(NS, NS);

        int cnt = 0;
        for    (auto& r : m)
        {   for(auto& c : r)
            {   c->n = ++cnt;
                c->position = pos;
                pos.x += szfish.x;
            }
            pos.y += szfish.y;
            pos.x  = NS;
        }

        goal   = sf::Vector2i(3,3);
        prev.x = -1;
    }

    ///-----------------------------|
    /// Пустая ячейка с номером 16. |
    ///-----------------------------:
    sf::Vector2i goal;

    void show()
    {   for    (auto& r : m)
        {   for(auto& c : r)
            {   c->draw();
                std::cout << "\t";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    ///-----------------------------|
    /// Где последний был раз ход.  |
    /// (для блокировки повтора)    |
    ///-----------------------------:
    sf::Vector2i prev;

    ///-----------------------------|
    /// Перемешать.                 |
    ///-----------------------------:
    void mixedn(const int mode = 50)
    {   /// while(m[3][3]->n != 1 || m[0][0]->n != 16)
        for(int i = mode; i--;)
        {   mixed();
        }
    }

    void mixed()
    {   for(bool repeate = true; repeate;)
        {   m1_t<eDIR> r;
            {   if(goal.x != 0) r.push_back(LEFT );
                if(goal.x != 3) r.push_back(RIGHT);
                if(goal.y != 0) r.push_back(UP   );
                if(goal.y != 3) r.push_back(DOWN );
            }

            sf::Vector2i fish = goal;

            switch(r[ myl::rrand(0, r.size()) ])
            {   case    UP:
                    fish.y--;
                    break;
                case  DOWN:
                    fish.y++;
                    break;
                case  LEFT:
                    fish.x--;
                    break;
                case RIGHT:
                    fish.x++;
                    break;
                default   :
                    ;
            }

            if(prev != fish)
            {   std::swap(m[fish.y][fish.x]->n, m[goal.y][goal.x]->n);
                prev    = goal ;
                save_step(goal); ///<---|1
                goal    = fish ;
                repeate = false;
            }

            MYASSERT(m[goal.y][goal.x]->n == 16)
        }
    }

    void reset()
    {   init();
    }

    ///----------------------|
    /// callback_mixed.      |
    ///----------------------:
    void callback_mixed_full()
    {   mixedn();
    }
    void callback_mixed_step()
    {   mixed();
    }
    void callback_mixed_reset()
    {   reset();
    }

    template<typename T>
    void init_callback(T& m)
    {   MYASSERT(m.size() == 3)
        m[0]->callback = [p = this]() { p->callback_mixed_full (   ); };
        m[1]->callback = [p = this]() { p->callback_mixed_step (   ); };
        m[2]->callback = [p = this]() { p->callback_mixed_reset(   ); };
        m[3]->callback = [p = this]() { p->callback_ai         (   ); };
        m[4]->callback = [p = this]() { p->callback_exit_game  (   ); };
    }

    Vec2i saved_moving_fish;
    eDIR where_do_step(Vec2i posi)
    {   eDIR d;
             if(posi.x + 1 == goal.x && posi.y     == goal.y) d = RIGHT;
        else if(posi.x - 1 == goal.x && posi.y     == goal.y) d = LEFT ;
        else if(posi.x     == goal.x && posi.y + 1 == goal.y) d = DOWN ;
        else if(posi.x     == goal.x && posi.y - 1 == goal.y) d = UP   ;
        else                                                  d = NONE ;

        if(d != NONE)
        {   saved_moving_fish  = posi;
            anim->callb_finish = [this](){finish_moved();};
            sound.play(NSound::E_MOVED);
            save_step(goal); ///<---|2
        }

        return d;
    }

    void finish_moved()
    {   std::swap(m[saved_moving_fish.y][saved_moving_fish.x]->n,
                  m[             goal.y][             goal.x]->n);

                  m[saved_moving_fish.y][saved_moving_fish.x]->go = 0.f;
                  goal = saved_moving_fish;
    }

    ///------------------------------------|
    /// Память ходов.                      |
    ///------------------------------------:
    std::vector<sf::Vector2i> memory;
    void save_step(sf::Vector2i& step)
    {   memory.push_back(step);
    }

    sf::Vector2i get_step()
    {   sf::Vector2i a = memory.back    ();
                         memory.pop_back();
        return a;
    }

    Button_game* button_game;
    bool is_ai = false;
    void callback_ai()
    {   is_ai = true;
        sound.play(NSound::e_music);
        button_game->disable_bat( );
    }

    void ai()
    {   if(!is_ai) return;
        if(memory.empty())
        {   is_ai = false;
            sound.stop(NSound::e_music);
            button_game->enable_bat(  );
            return;
        }

        sound.play(NSound::e_dropai);
        sf::Vector2i a = get_step();
        std::swap(m[goal.y][goal.x]->n, m[a.y][a.x]->n);
        goal = a;
    }

    bool exit_game = false;
    void callback_exit_game()
    {    exit_game = true;
         sound.play(NSound::e_music);
         sf::sleep (sf::milliseconds (3000));
    }

    m2_t<Fish*> m;
};

inline void testclass_Field()
{   Field field;
    field.show();

    while(true)
    {   std::cin.get();

        field.mixedn();
        field.show  ();
    }
}

#endif // field_h
