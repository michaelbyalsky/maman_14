#ifndef MAMAN_14_TABLES_H
#define MAMAN_14_TABLES_H

#include "globals.h"

Macro* createMacro(const char* name, const char* value);
void insertMacro(Macro** head, Macro* newMacro);
Macro* findMacro(Macro* head, const char* name);

#endif
