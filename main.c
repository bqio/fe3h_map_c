#include "stdio.h"
#include "stdint.h"
#include "string.h"

void unpack(char file_path[]);
uint8_t validate_file(FILE *fp);

struct OFFSETS
{
  uint32_t start;
  uint32_t len;
};

uint8_t validate_file(FILE *fp)
{
  uint32_t lines_count;
  uint32_t first_offset;

  fseek(fp, 0, 0);
  fread(&lines_count, 4, 1, fp);
  fseek(fp, (8 * lines_count) + 4, 0);
  fread(&first_offset, 4, 1, fp);

  if (((8 * lines_count) + 4) == first_offset)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

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

void unpack(char file_path[])
{
  FILE *fsrc;

  fsrc = fopen(file_path, "r");

  if (!validate_file(fsrc))
  {
    fclose(fsrc);
    printf("Incorrect file %s", file_path);
    return;
  }

  FILE *fdst;
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