#ifndef mylib_h
#define mylib_h
///----------------------------------------------------------------------------|
/// "mylib.h"
///----------------------------------------------------------------------------:
#include "debug.h"

namespace myl  /// (mylib)
{

///-------------------------------|
/// Ёргономика.                   |
///-------------------------------:
    namespace win
    {
#include <windows.h>
        inline void set(WORD text = 7, WORD background = 0)
        {   SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),
                                    (background << 4) | text);
        }
        struct  Color
        {   Color(WORD text)
            {   set(text);
            }
            ~Color(         )
            {   set(    );
            }
        };
    };

#include <ctime>
#include <cstdlib>
    struct  Rand
    {   Rand()
        {   srand((unsigned)time(0));
        }

        int operator()(int range_min, int range_max)
        {   return rand() % (range_max - range_min) + range_min;
        }
    } rrand; /// rrand(0, 5) числа от 0 до 4 включительно!

    #include <conio.h>
    bool is_press_key(char key)
    {   if (_kbhit())
        {   if(_getch() == key){  return true;}
        }
        return false;
    }

} /// End namespace myl (mylib)

#include <random>
#include <iostream>
#include <memory>
#include <functional>

void f(int n1, int n2, int n3, const int& n4, int n5)
{   std::cout << n1 << ' ' << n2 << ' ' << n3 << ' ' << n4 << ' ' << n5 << '\n';
}

int g(int n1)
{   return n1;
}

struct Foo
{   void print_sum(int n1, int n2)
    {   std::cout << n1+n2 << '\n';
    }
    int data = 10;
};

inline void test_01()
{   using namespace std::placeholders;  // for _1, _2, _3...

    std::cout << "demonstrates argument reordering and pass-by-reference:\n";
    int n = 7;
    // (_1 and _2 are from std::placeholders, and represent future
    // arguments that will be passed to f1)
    auto f1 = std::bind(f, _2, 42, _1, std::cref(n), n);
    n = 10;
    f1(1, 2, 1001); // 1 is bound by _1, 2 is bound by _2, 1001 is unused
    // makes a call to f(2, 42, 1, n, 7)

    std::cout << "achieving the same effect using a lambda:\n";
    auto lambda = [ncref=std::cref(n), n=n](auto a, auto b, auto /*unused*/)
    {   f(b, 42, a, ncref, n);
    };
    lambda(1, 2, 1001); // same as a call to f1(1, 2, 1001)

    std::cout << "nested bind subexpressions share the placeholders:\n";
    auto f2 = std::bind(f, _3, std::bind(g, _3), _3, 4, 5);
    f2(10, 11, 12); // makes a call to f(12, g(12), 12, 4, 5);

    std::cout << "common use case: binding a RNG with a distribution:\n";
    std::default_random_engine e;
    std::uniform_int_distribution<> d(0, 10);
    auto rnd = std::bind(d, e); // a copy of e is stored in rnd
    for(int n=0; n<10; ++n)
        std::cout << rnd() << ' ';
    std::cout << '\n';

    std::cout << "bind to a pointer to member function:\n";
    Foo foo;
    auto f3 = std::bind(&Foo::print_sum, &foo, 95, _1);
    f3(5);

    std::cout << "bind to a pointer to data member:\n";
    auto f4 = std::bind(&Foo::data, _1);
    std::cout << f4(foo) << '\n';

    std::cout << "use smart pointers to call members of the referenced objects:\n";
    std::cout << f4(std::make_shared<Foo>(foo)) << ' '
              << f4(std::make_unique<Foo>(foo)) << '\n';
}

float test(int a, int b)
{   return 7.0f;
}

struct TestClass1
{   float test1()
    {   return 5.0f;
    }
};

struct TestClass2
{   float  test2(int a, int b)
    {   return 3.0f;
    }
};

inline void test_02()
{   TestClass1 obj1;
    TestClass2 obj2;

    std::function<float()>
        c1 = std::bind(&TestClass1::test1, &obj1);
    c1();


    std::function<float(int, int)>
        c2 = std::bind(&TestClass2::test2, &obj2, std::placeholders::_1,
                                                  std::placeholders::_2);
    c2(1, 2);

    std::function<float()>
        c3 = std::bind(TestClass2::test2, &obj2, 123, 456);
    c3();
}

///----------------------------------------------------------------------------|
/// Паттерн "Делегат без паяльника".
///----------------------------------------------------------------------------:
#include <iostream>
#include <vector>
#include <functional>

struct Buttn
{   std::function<void()> callback;
    /// ...
};

struct  Gui
{	    Gui() : m(2)
        {
        }

    void press_all()
    {   m[0].callback();
        m[1].callback();
    }

    std::vector<Buttn> m;
};

struct  Game
{	    Game()
        {
        /// gui.m[0].callback = std::bind(&Game::test_01, this);
        /// gui.m[1].callback = std::bind(&Game::test_02, this, 123);

            ///-------------------------------------------|
            /// С лямбдой даже симпотнее.                 |
            ///-------------------------------------------:
            gui.m[0].callback = [this]() { test_01(   ); };
            gui.m[1].callback = [this]() { test_02(123); };
        }

    void test_01(     ){   std::cout << "test_01()\n";}
    void test_02(int a){   std::cout << "test_02()\n";}

    Gui gui;
};

void foo0(     ){   std::cout << "foo()\n";}
void foo1(int a){   std::cout << "foo()\n";}

inline void main_00()
{
    Game game;
         game.gui.press_all();

         game.gui.m[0].callback = foo0;
         game.gui.m[1].callback = std::bind(&::foo1, 10);

         game.gui.press_all();
}

#endif // mylib_h
