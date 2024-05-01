#ifndef SPEAKER_H
#define SPEAKER_H

#include <stdlib.h>

const size_t MAX_PHRASE_LENGTH = 1<<12;

#ifndef SILENT
#define say(...) _say(__VA_ARGS__)
#else
#define say(...) (0)
#endif

/**
 * @brief Say one string of text.
 * 
 * @param format format string, same as for printf
 * @param __va_args__ arguments, same as for printf
 */
void _say(const char* format, ...) __attribute__((format (printf, 1, 2)));

void speaker_set_mute(bool new_mute);

bool speaker_get_mute();

#endif