//
// Hashes input line by line using Fowler–Noll–Vo's Hash (FNV-1a).
//

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

static uint32_t fnv1a(const void *buffer, size_t length) {
  const uint8_t *start = (const uint8_t *)buffer;
  const uint8_t *end = start + length;

  uint32_t hash = 2166136261ul;

  for (const uint8_t *I = start; I != end; ++I)
    hash = (hash ^ *I) * 16777619ul;

  return hash;
}

static const char *trim(const char *string) {
  const char *begin = string, *end = string + strlen(string);

  // Trim leading whitespace.
  while (isspace(*begin))
    ++begin;

  // Trim trailing whitespace.
  while (isspace(*end))
    --end;

  const size_t length = end - begin - 1;
  return strncpy((char *)malloc(length + 1), begin, length);
}

int main(int argc, const char *argv[]) {
  // Maximum length of line.
  static const unsigned LIMIT = 255;

  // Line buffer.
  char line[LIMIT + 1] = { 0, };

  // Calculate and output hash for each line.
  while (fgets(line, LIMIT + 1, stdin)) {
    const char *trimmed = trim(line);
    const uint32_t hash = fnv1a((const void *)trimmed, strlen(trimmed));
    fprintf(stdout, "%#0.8x # \"%s\"\n", hash, trimmed);
  }

  return EXIT_SUCCESS;
}
