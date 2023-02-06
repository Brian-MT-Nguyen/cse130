#pragma once
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <errno.h>

int parseRequest(Request *req, char *buffer, ssize_t bytesRead);
