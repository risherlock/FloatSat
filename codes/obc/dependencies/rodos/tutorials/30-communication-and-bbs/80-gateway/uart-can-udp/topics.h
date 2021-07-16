
#pragma once

struct UserCmd {
    int32_t cmdLen;
    char cmdLine[500];
};

extern Topic<UserCmd> command;


