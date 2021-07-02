///----------------------------------------------------------------------------|
/// "sound.cpp"
///----------------------------------------------------------------------------:
#include "sound.h"

#define SFML_STATIC
#include <SFML/Audio.hpp>

#include <chrono>
#include <thread>
#include <atomic>

namespace NSound {

std::atomic<bool> done{true};

void pause(int ms)
{   const int d = 50;
    for  (int i =  0; done && (i < ms); i += d)
    {   std::this_thread::sleep_for(std::chrono::milliseconds {d});
    }
}

struct  PSound
{       PSound() : sound(E_NONE)
    {
        mysound.push_back(sf::SoundBuffer());
        mysound[E_CLICK].loadFromFile ("snd/e_click.ogg");
        sound  [E_CLICK].setBuffer (mysound[E_CLICK]);

        mysound.push_back(sf::SoundBuffer());
        mysound[E_MOVED].loadFromFile ("snd/e_moved.ogg");
        sound  [E_MOVED].setBuffer (mysound[E_MOVED]);

        mysound.push_back(sf::SoundBuffer());
        mysound[e_dropai].loadFromFile ("snd/e_dropai.ogg");
        sound  [e_dropai].setBuffer (mysound[e_dropai]);

        mysound.push_back(sf::SoundBuffer());
        mysound[e_music].loadFromFile ("snd/e_music.ogg");
        sound  [e_music].setBuffer (mysound[e_music]);
    }

    void set_sample(ESOUND E)
    {   sound[E].setBuffer(mysound[E]);
    }

    std::vector<sf::SoundBuffer> mysound;
    std::vector<sf::Sound>         sound;

    void test()
    {   sound[0].resetBuffer        ();
        sound[0].setBuffer(mysound[0]);
        sound[0].play               ();
    }
};

Sound::Sound() : psound(new PSound){}

void Sound::play(ESOUND E)
{
    psound->set_sample(E);

    ///-----------------|
    /// Убирает щелчки. |
    ///-----------------:
    if(sf::SoundSource::Playing != psound->sound[E].getStatus())
    {   //psound->sound.resetBuffer ();
        //psound->sound.setBuffer(mysound[0]);
        psound->sound[E].play();
    }
    else psound->sound[E].play();
}

void Sound::stop(ESOUND E)
{   //psound->sound.setVolume (0);
    psound->sound[E].stop();
}

}; // namespace NSound

NSound::Sound sound; /// sound.play(NSound::E_MOVED);
