#include "file_io.h"
#include "network.h"
#include "stdio.h"
#include "stdlib.h"

NETWORK_ERRNO network_read(Network **network, const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        *network = NULL;
        return READFILE_ERROR;
    }

    int justRead;
    char buf[4];

    if (fread(buf, sizeof(char), 2, file) < 2) {
        fclose(file);
        *network = NULL;
        return MALFORMED_FILE;
    }

    if (buf[0] != FILE_FORMAT_VERSION){
        fclose(file);
        *network = NULL;
        return INVALID_VERSION;
    }

    char layerCount = buf[1];
    
    int16_t nodesPerLayers[layerCount];

    

    fclose(file);
    return NO_ERROR;
}