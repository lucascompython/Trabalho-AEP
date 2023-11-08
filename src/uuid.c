#ifdef __unix__

#include <stdio.h>
#include <stdlib.h>
#include <uuid/uuid.h>

char *uuid_gen(void)
{
  uuid_t uuid;
  uuid_generate_random(uuid);
  char *uuid_str = malloc(37);
  if (uuid_str == NULL)
  {
    return NULL;
  }
  uuid_unparse(uuid, uuid_str);
  return uuid_str;
}

#elif _WIN32

// #include <rpcdce.h>
// #include <stdio.h>

// char *uuid_gen(void)
// {
//   UUID uuid;
//   UuidCreate(&uuid);
//   unsigned char *str;
//   UuidToStringA(&uuid, &str);
//   return str;
// } // TODO: Fix this, não funciona nao sei porque

#include <stdlib.h>

char *uuid_gen(void)
{
  char v[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  // 8 dash 4 dash 4 dash 4 dash 12
  static char buf[37] = {0};

  // gen random for all spaces because lazy
  for (int i = 0; i < 36; ++i)
  {
    buf[i] = v[rand() % 16];
  }

  // put dashes in place
  buf[8] = '-';
  buf[13] = '-';
  buf[18] = '-';
  buf[23] = '-';

  // needs end byte
  buf[36] = '\0';

  return buf;
}
#endif
