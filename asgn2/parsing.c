#include "parsing.h"
#define REQEX "^([a-zA-Z]{1,8}) /([a-zA-Z0-9.-]{1,63}) (HTTP/[0-9]\\.[0-9])\r\n"
#define HEADEX "([a-zA-Z0-9.-]{1,128}): ([ -~]{1,128})\r\n"

int parseRequest(Request *req, char *buffer) {
    //puts(buffer);
    regex_t re;
    regmatch_t matches[4];
    int rc;
    rc = regcomp(&re, REQEX, REG_EXTENDED);
    rc = regexec(&re, buffer, 4, matches, 0);
    if(rc == 0) {
        req->cmd = buffer;
        req->targetPath = buffer + matches[2].rm_so;
        req->version = buffer + matches[3].rm_so;

        buffer[matches[1].rm_eo] = '\0';
        req->targetPath[matches[2].rm_eo - matches[2].rm_so] = '\0';
        req->version[matches[3].rm_eo - matches[3].rm_so] = '\0';
        
        buffer = buffer + matches[3].rm_eo + 2;
    } else {
        return(1);
    }
    req->contentLength = -1;
    rc = regcomp(&re, HEADEX, REG_EXTENDED);
    rc = regexec(&re, buffer, 3, matches, 0);
    while(rc == 0) { 
        buffer[matches[1].rm_eo] = '\0';
        buffer[matches[2].rm_eo] = '\0';
        if(strncmp(buffer, "Content-Length", 14) == 0) {
            req->contentLength = atoi(buffer + matches[2].rm_so);
        }
        buffer = buffer + matches[2].rm_eo + 2;
        rc = regexec(&re, buffer, 3, matches, 0);
    }

    if(buffer[0] == '\r' && buffer[1] == '\n') {
        req->msgBody = buffer + 2;
    }
    return(0);
}
