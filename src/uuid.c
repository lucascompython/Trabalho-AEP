#ifdef __unix__

#include <stdio.h>
#include <stdlib.h>
#include <uuid/uuid.h>

char *uuid_gen(void) {
  uuid_t uuid;
  uuid_generate_random(uuid);
  char *uuid_str = malloc(37);
  uuid_unparse(uuid, uuid_str);
  return uuid_str;
}

#elif _WIN32

#include <rpcdce.h>

char *uuid_gen(void) {
  UUID uuid;
  UuidCreate(&uuid);
  unsigned char *str;
  UuidToStringA(&uuid, &str);
  return str;
}

#endif
