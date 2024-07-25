#include "src/ctap/audio_api.h"

static void start_the_audio(void)
{
    return;
}

const aud_api_t aud_api = {.start_the_audio = start_the_audio};
