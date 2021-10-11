#include "stdio.h"
#include "string.h"

int unpack();

struct OFFSETS
{
  unsigned int start;
  unsigned int len;
};

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    printf("Error. Use %s [FILE] or drag and drop files.", argv[0]);
    getchar();
  }
  else if (argc > 2)
  {
    for (int i = 1; i < argc; i++)
    {
      unpack(argv[i]);
    }
  }
  else
  {
    unpack(argv[1]);
  }

  return 0;
}

int unpack(char file_path[])
{
  FILE *fsrc;
  FILE *fdst;

  fsrc = fopen(file_path, "r");
  strcat(file_path, ".txt");
  fdst = fopen(file_path, "w");

  unsigned int lines;
  fread(&lines, sizeof(unsigned int), 1, fsrc);

  struct OFFSETS offsets[lines];

  fread(&offsets, sizeof(struct OFFSETS) * lines, 1, fsrc);

  for (int i = 0; i < lines; i++)
  {
    wchar_t text[offsets[i].len];

    fseek(fsrc, offsets[i].start, 0);
    fread(&text, sizeof(wchar_t) * offsets[i].len, 1, fsrc);

    fprintf(fdst, "%s\n", text);
  }

  fclose(fsrc);
  fclose(fdst);
}