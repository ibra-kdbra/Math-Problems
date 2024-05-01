#define SPEAKER_CPP
#include "speaker.h"

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#include "util/dbg/debug.h"

static bool speaker_mute = false;

void _say(const char* format, ...) {
    if (speaker_mute) return;

    va_list args;
    va_start(args, format);

                                /*   v- length of the TTS module name */
    char request[MAX_PHRASE_LENGTH + 9] = "espeak \"";

    vsprintf(request + strlen(request), format, args);
    request[strlen(request)] = '"';
    request[sizeof(request) / sizeof(*request) - 1] = '\0';

    log_printf(STATUS_REPORTS, "status", "Sending voicing request [%s].\n", request);

    if (system(request) != 0) {
        log_printf(WARNINGS, "warning", "Failed to access espeak module, the speaker was muted.\n");
        speaker_set_mute(true);
    }

    va_end(args);
}

void speaker_set_mute(bool new_mute) {
    speaker_mute = new_mute;
    log_printf(STATUS_REPORTS, "status", "Speaker mute was set to %d.\n", new_mute);
}

bool speaker_get_mute() {
    return speaker_mute;
}