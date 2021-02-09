/*
 *  Powderpunk Server - A Pretentious Project
 *
 *  Author: Eduardo "mrtheduts" Souza
 *          eduardomoreirafds@gmail.com
 *
 *  Date:   09 Feb 2021
 *
 *  (C) Copyright 2021 Eduardo "mrtheduts" Souza All Rights Reserved
 *
 */
#ifndef USER_COMMAND_H
#define USER_COMMAND_H

typedef struct {
  char* mod;
  char* cmd;

  unsigned int num_args;
  char** args;

  unsigned int num_targets;
  char** targets;
} UserCommand, *pUserCommand;

pUserCommand CreateUsrCmd(char* mod, char* cmd, unsigned int num_args,
                          char** args, unsigned int num_targets,
                          char** targets);

void DestroyUsrCmd(pUserCommand usr_cmd);

void PrintUsrCmd(pUserCommand usr_cmd);

#endif

