#include <stdio.h>
#include <string.h>

#include <kernel/keyboard.h>
#include <kernel/kshell.h>

/* simple commands */
static void echo(int argc, char *argv[])
{
  for (int i = 1; i < argc; i++)
  {
    if (i > 1) printf(" ");
    printf("%s", argv[i]);
  }
  printf("\n");
}

static void yes(int argc, char *argv[])
{
  while (1)
  {
    if (argc < 2)
    {
      printf("y\n");
    }
    else
    {
      for (int i = 1; i < argc; i++)
      {
        if (i > 1) printf(" ");
        printf("%s", argv[i]);
      }
      printf("\n");
    }
  }
}

static void kshell_getline(char *buf)
{
  char c;
  while (1)
  {
    c = (char)keyboard_getchar();
    if (c == 0) continue;
    if (c == '\n') break;
    putchar(c);
    *buf++ = c;
  }
  putchar('\n');
  *buf++ = '\n';
  *buf = '\0';
}

static void eval(int argc, char *argv[])
{
  if (strcmp(argv[0], "echo") == 0)
  {
    echo(argc, argv);
    return;
  }
  else if (strcmp(argv[0], "yes") == 0)
  {
    yes(argc, argv);
    return;
  }

  printf("unknown command: only supported commands are echo and yes\n");
}

static void parse_and_eval(char *s)
{
  char *argv[BUFSIZ];

  while (*s != '\0')
  {
    int argc = 0;

    while (*s != '\0')
    {
      while (*s == '\n' || *s == '\t' || *s == ' ') ++s;
      if (*s != ';' && *s != '&' && *s != '\0') argv[argc++] = s;
      while (strchr("&;\n\t ", *s) == NULL) ++s;
      if (*s) *s++ = '\0';
    }
    argv[argc] = NULL;
    eval(argc, argv);
  }
}

void kshell_run(void)
{
  while (1)
  {
    printf("bs> ");
    char buf[BUFSIZ];
    kshell_getline(buf);
    parse_and_eval(buf);
  }
}
