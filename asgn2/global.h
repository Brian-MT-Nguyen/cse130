#pragma once

typedef struct Request {
    int infd;
    char *cmd;
    char *targetPath;
    char *version;
    int contentLength;
    char *msgBody;
    int remainingBytes;
} Request;
