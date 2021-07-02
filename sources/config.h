#ifndef config_h
#define config_h
///----------------------------------------------------------------------------|
/// "config.h"
///----------------------------------------------------------------------------:
#include <string>
#include <fstream>

#include "debug.h"

struct  Config
{       Config()
        {   calculate_cash();
        }

    sf::Vector2i window                {800, 600};
    float speed_fish                       = 0.5f; // CFG->speed_fish
    sf::Vector2f sizefield_row_x_column{  4,   4};
    sf::Vector2f sizecell              {100, 100};
    sf::Vector2f sizefish              {100 - 20, 100 - 20};

    ///--------------------------|
    /// Кешируемые.              |
    ///--------------------------:
    sf::Vector2f sizefield;

    void calculate_cash()
    {   sizefield = sf::Vector2f(sizecell.x * sizefield_row_x_column.x,
                                 sizecell.y * sizefield_row_x_column.y);
    }

    void from_file()
    {
        std::ifstream file("config.txt");
        if(!file.is_open()) return;

        std::string s; std::getline(file, s, '\f');

        if(s.find("SET_DEFAULT+") != std::string::npos) return;

        window = get(s, "window", window);
    }

private:
    template<typename T>
    T        get(std::string& s, const char* nameval, T val)
    {   auto b = s.find(nameval);
             b = s.find ('=', b);
             b = s.find_first_not_of("= ", b);
        auto e = s.find (' ', b);
        return std::stod(s.substr(b, e - b));
    }

    std::string& get(std::string& s, const char* nameval, std::string& val)
    {   auto b = s.find(nameval);
             b = s.find ('=', b);
             b = s.find_first_not_of("= ", b);
        auto e = s.find (' ', b);
        return  val = s.substr(b, e - b);
    }

    sf::Vector2i& get(std::string& s, const char* nameval, sf::Vector2i& val)
    {   auto b = s.find(nameval);
             b = s.find ('=', b);
             b = s.find_first_not_of("= ", b) ;
        auto e = s.find_first_of    (", ", b) ;

        val.x = std::stod(s.substr(b,  e - b));

             b = s.find_first_not_of(" ,", e) ;
             e = s.find_first_of  (", \n", b) ;

        val.y = std::stod(s.substr(b,  e - b));

        return  val;
    }
};

       const Config  cfg_default;
extern const Config* CFG;

inline void testclass_Config()
{   Config cfg;
           cfg.from_file();

    l(cfg.window.x)
    l(cfg.window.y)
}

#endif // config_h
