#pragma once

// enum Command {
//     NONE,
//     GET,
//     PUT
// };

typedef struct Request {
    char *cmd;
    char *targetPath;
    char *version;
    int contentLength;
    int outfd;
    int infd;
} Request;

typedef struct Response { 
    int statusCode;
} Response;
