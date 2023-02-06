#pragma once
#include "asgn2_helper_funcs.h"
#include "global.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>

int handleRequest(Request *req);
