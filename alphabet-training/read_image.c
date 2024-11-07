#include <stdlib.h>
#include <stdio.h>

#include "read_image.h"
#include "utils/array.h"

uchar buf_avg(uchar buf[3])
{
    int sum = buf[0] + buf[1] + buf[2];
    return sum/3;
}

uchar *__load_image(FILE *file)
{
    if (fseek(file, 0x36, SEEK_CUR))
    {
        return NULL;
    }
    
    uchar *out = calloc(IMAGE_SIZE * IMAGE_SIZE, sizeof(uchar));
    
    if (out == NULL)
    {
        return NULL;
    }
    
    uchar buf[3];
    
    for (int y = IMAGE_SIZE - 1; y >= 0; y--)
    {
        for (int x = 0; x < IMAGE_SIZE; x++)
        {
            if (!fread(buf, sizeof(uchar), 3, file))
            {
                free(out);
                return NULL;
            }
            
            out[(y * IMAGE_SIZE) + x] = buf_avg(buf);
        }
        
        fseek(file, 1, SEEK_CUR);
    }
    
    return out;
}

uchar *load_image(char *path)
{
    FILE *file = fopen(path, "r");
    
    if (file == NULL)
    {
        return NULL;
    }
    
    uchar *res = __load_image(file);
    
    fclose(file);
    
    return res;
}