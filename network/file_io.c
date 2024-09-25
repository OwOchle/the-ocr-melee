#include "file_io.h"
#include "network.h"
#include "arrays.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

NETWORK_ERRNO network_read(Network **network, const char *filename)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        *network = NULL;
        return READFILE_ERROR;
    }

    size_t justRead;
    char buf[4];

    if (fread(buf, sizeof(char), 2, file) < 2)
    {
        fclose(file);
        *network = NULL;
        return MALFORMED_FILE;
    }

    if (buf[0] != FILE_FORMAT_VERSION)
    {
        fclose(file);
        *network = NULL;
        return INVALID_VERSION;
    }

    char layerCount = buf[1];

    int16_t nodesPerLayers[layerCount];

    for (char l = 0; l < layerCount; l++)
    {
        if (!(justRead = fread(buf, sizeof(int16_t), 1, file)))
        {
            fclose(file);
            *network = NULL;
            return MALFORMED_FILE;
        }

        memcpy(nodesPerLayers + l, buf, 2);
    }

    fclose(file);
    return NO_ERROR;
}

NETWORK_ERRNO network_write(Network *network, char *filename)
{
    FILE *file = fopen(filename, "w");

    if (file == NULL)
    {
        return WRITEFILE_ERROR;
    }

    char ver = FILE_FORMAT_VERSION;
    fwrite(&ver, sizeof(ver), 1, file);

    fwrite(&network->entryCount, sizeof(uint16_t), 1, file);

    fwrite(&network->layerCount, sizeof(char), 1, file);

    for (char i = 0; i < network->layerCount; i++)
    {
        fwrite(&network->layers[i]->nodeCount, sizeof(uint16_t), 1, file);
    }

    for (char i = 0; i < network->layerCount; i++)
    {
        uint16_t prevNodeCount;

        if (i == 0) {
            prevNodeCount = network->entryCount;
        } else {
            prevNodeCount = network->layers[i-1]->nodeCount;
        }

        for (uint16_t n = 0; n < network->layers[i]->nodeCount; n++)
        {
            fwrite(&network->layers[i]->bias[n], sizeof(float), 1, file);

            for (uint16_t w = 0; w < prevNodeCount; w++) {
                float weight = array_get_as_matrix(network->layers[i]->weights, network->layers[i]->nodeCount, w, n);

                fwrite(&weight, sizeof(float), 1, file);
            }
        }
    }

    fclose(file);

    return NO_ERROR;
}