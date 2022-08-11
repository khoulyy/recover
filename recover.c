#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // TODO: Program accepts only one command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover IMAGE\n");
        return 1;
    }

    // TODO: Open memory card
    FILE *file = fopen(argv[1], "r");

    if (!file)
    {
        return 1;
    }

    BYTE *buffer = malloc(sizeof(BYTE) * 512);

    int filecounter = 0;
    char filename[8];
    FILE *img = NULL;

    while (fread(buffer, sizeof(BYTE), 512, file) == 512)
    {
        // TODO: Look for beginning of a JPEG
        if (*buffer == 0xff && *(buffer + 1) == 0xd8 && *(buffer + 2) == 0xff && (*(buffer + 3) & 0xf0) == 0xe0)
        {
            // TODO: Open a new JPEG file
            if (filecounter == 0)
            {
                sprintf(filename, "%03i.jpg", filecounter);
                img = fopen(filename, "w");
                fwrite(buffer, sizeof(BYTE), 512, img);
                filecounter++;
            }
            else if (filecounter > 0)
            {
                fclose(img);
                sprintf(filename, "%03i.jpg", filecounter);
                img = fopen(filename, "w");
                fwrite(buffer, sizeof(BYTE), 512, img);
                filecounter++;
            }
        }
        else if (filecounter > 0)
        {
            fwrite(buffer, sizeof(BYTE), 512, img);
        }
    }

    // close files
    fclose(file);
    fclose(img);

    // free memory
    free(buffer);
}