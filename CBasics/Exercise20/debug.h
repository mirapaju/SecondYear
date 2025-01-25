#ifndef EXERCISE20_DEBUG_H
#define EXERCISE20_DEBUG_H

int read_range(char *prompt, int min_value, int max_value);
void set_debug_level(int debug_level);
int dprintf(int debug_level, const char *fmt, ...);

#endif //EXERCISE20_DEBUG_H