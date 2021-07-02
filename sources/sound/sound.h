#ifndef sound_h
#define sound_h
///----------------------------------------------------------------------------|
/// "sound.h"
///
///----------------------------------------------------------------------------:
#include <memory>

/// NSound::ESOUND::E_CLICK
namespace NSound {
enum ESOUND
{
    E_CLICK = 0, /// "e_click.ogg"
    E_MOVED    , /// "e_moved.ogg"
    e_dropai   , /// "e_dropai.ogg"
    e_music    , /// "e_music.ogg"

    /// ...
    E_NONE
};

struct PSound;
struct  Sound
{       Sound();

    std::unique_ptr<PSound> psound;

    void play(ESOUND n);
    void stop(ESOUND n);
};

}; // namespace NSound

extern NSound::Sound sound;


#endif // sound_h
