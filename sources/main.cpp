///----------------------------------------------------------------------------|
/// Игра пятнашки.
///     -mwindows
///----------------------------------------------------------------------------:
#define SFML_STATIC
#include <iostream>
#include <vector>
#include <memory>
#include <SFML/Graphics.hpp>

#include "config.h"
const Config* CFG    = &cfg_default;

#include "gui.h"

#include "res\resurces.h"
void myload_from_memory(sf::Image& image)
{   image.loadFromMemory(res01, sizeof res01);
}

///----------------------------------------------------------------------------|
/// Run
///----------------------------------------------------------------------------:
struct  Run
{       Run() : window(sf::VideoMode(840, 480), "Test 2021")

        {   myload_from_memory(heroimage);
            heroimage.createMaskFromColor(sf::Color(0, 0, 0));

            herotexture.loadFromImage(heroimage);
            herosprite.setTexture  (herotexture);
            herosprite.setPosition (50, 25);
        }
       ~Run()
        {
        }

    sf::RenderWindow window;
    sf::Image     heroimage;
    sf::Texture herotexture;
    sf::Sprite   herosprite;

    void loop()
    {
        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear();
            window.draw(herosprite);//выводим спрайт на экран
            window.display();
        }
    }
};

///-----------------|
/// Тесты.          |
///-----------------:
#include "field.h"
#include "visualisator.h"

void tests()
{   setlocale(0, "");
    std::wcout << L"СТАРТ ТЕСТ...\n\n";
    {
      //testclass_Config      ();
      //testclass_Field       ();
      //testclass_Button      ();
      //testclass_Animat(); return;

        testclass_Visualisator();
    }
    std::wcout << L"\nФИНИШ ТЕСТ.\n";
    std::cin.get();
}

///----------------------------------------------------------------------------|
/// Start.
///----------------------------------------------------------------------------:
int main()
{
    tests(); return 0;

    //std::unique_ptr<Run> run(new Run);
    Run*   p = new Run;
           p->loop();
}
