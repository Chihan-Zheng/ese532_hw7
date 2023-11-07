#include "Utilities.h"

void Exit_with_error(const char *s)
{
    printf("%s\n", s);
    exit(EXIT_FAILURE);
}

void Load_data(unsigned char *Data)
{
    unsigned int Size = FRAMES * FRAME_SIZE;

    FILE *File = fopen("../data/Input.bin", "rb");
    if (File == NULL)
        Exit_with_error("fopen for Load_data failed");

    if (fread(Data, 1, Size, File) != Size)
        Exit_with_error("fread for Load_data failed");

    if (fclose(File) != 0)
        Exit_with_error("fclose for Load_data failed");
}

void Store_data(const char *Filename, unsigned char *Data, unsigned int Size)
{
    FILE *File = fopen(Filename, "wb");
    if (File == NULL)
        Exit_with_error("fopen for Store_data failed");

    if (fwrite(Data, 1, Size, File) != Size)
        Exit_with_error("fwrite for Store_data failed");

    if (fclose(File) != 0)
        Exit_with_error("fclose for Store_data failed");
}

void Check_data(unsigned char *Data, unsigned int Size)
{
    int error_code = 0;
    unsigned char *Data_golden = (unsigned char *)malloc(MAX_OUTPUT_SIZE);
    FILE *File = fopen("../data/Golden.bin", "rb");
    if (File == NULL)
        Exit_with_error("fopen for Check_data failed");

    if (Size == 0)
        Exit_with_error("fclose for Check_data failed, Size==0");

    if (fread(Data_golden, 1, Size, File) != Size)
        Exit_with_error("fread for Check_data failed, Different Size");

    if (fclose(File) != 0)
        Exit_with_error("fclose for Check_data failed");

    for (unsigned int i = 0; i < Size; i++)
    {
        if (Data_golden[i] != Data[i])
        {
            free(Data_golden);
            error_code = i + 1;
        }
    }

    free(Data_golden);

    if (error_code != 0)
    {
        printf("You got errors in data %d\n", error_code);
        Exit_with_error("Input.bin and Golden.bin doesn't match");
    }
    else
    {
        printf("Application completed successfully.\n");
    }
}