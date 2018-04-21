#ifndef BUILTIN_H
#define BUILTIN_H

#include "parsing.h"
#include "defs.h"

extern char prompt[PRMTLEN];

int cd(struct execcmd *parsed);

int exit_shell(struct execcmd *parsed);

int pwd(struct execcmd *parsed);

#endif // BUILTIN_H
