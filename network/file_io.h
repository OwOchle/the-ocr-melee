#pragma once
#include "network.h"

#define FILE_FORMAT_VERSION 1 // The file format version

typedef enum NETWORK_ERRNO
{
    INVALID_VERSION = -1,
    READFILE_ERROR = 1,
    MALFORMED_FILE = 2,
    WRITEFILE_ERROR = 3,
    NO_ERROR = 0
} NETWORK_ERRNO;

/**
 * Reads a network from file `filename`
 * @param network A pointer of network pointer. NULL if an error occurs
 * @param filename A filename to read network from
 * @return An error code. 0 if no error, network is not NULL
 */
NETWORK_ERRNO network_read(Network **network, const char *filename);

/**
 * Writes a network to file `filename`
 * @param network The network to write.
 * @param filename A filename to write network to
 * @return An error code. 0 if no error
 */
NETWORK_ERRNO network_write(Network *network, char *filename);

/**
 * File format description
 * | version (8 bit) |
 * entry nodes count (16 bit) |
 * layer count (8 bit) denoted l afterwards |
 * l × nodes in layer Ll (16 bit each) denoted Lln |
 * node L0N0 bias (32 bit) |
 * weight of L(l-1)N(x) to LlN(x) (32 bit) |
 */