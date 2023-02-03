#pragma once
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int handleRequest(Request *req, char *buffer);
