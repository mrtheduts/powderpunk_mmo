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
#include "user_command.h"

#include <stdio.h>
#include <stdlib.h>

pUserCommand CreateUsrCmd(char* mod, char* cmd, unsigned int num_args,
                          char** args, unsigned int num_targets,
                          char** targets) {
  pUserCommand new_usr_cmd = (pUserCommand)malloc(sizeof(UserCommand));

  new_usr_cmd->mod = mod;
  new_usr_cmd->cmd = cmd;

  new_usr_cmd->num_args = num_args;
  new_usr_cmd->args = args;

  new_usr_cmd->num_targets = num_targets;
  new_usr_cmd->targets = targets;

  return new_usr_cmd;
}

void DestroyUsrCmd(pUserCommand usr_cmd) {
  unsigned int i;

  for (i = 0; i < usr_cmd->num_args; ++i) free(usr_cmd->args[i]);
  free(usr_cmd->args);

  for (i = 0; i < usr_cmd->num_targets; ++i) free(usr_cmd->targets[i]);
  free(usr_cmd->targets);
}

void PrintUsrCmd(pUserCommand usr_cmd) {
  unsigned int i;
  printf("pUserCommand:\n");
  printf("\tMod: %s\n", usr_cmd->mod);
  printf("\tCmd: %s\n", usr_cmd->cmd);

  printf("\tArgs[%d]:", usr_cmd->num_args);
  for (i = 0; i < usr_cmd->num_args; ++i) {
    printf(" \"%s\"", usr_cmd->args[i]);
  }
  printf("\n");

  printf("\tTargets[%d]:", usr_cmd->num_targets);
  for (i = 0; i < usr_cmd->num_targets; ++i) {
    printf(" \"%s\"", usr_cmd->targets[i]);
  }
  printf("\n");
}
