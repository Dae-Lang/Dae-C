#ifndef STRING_H
#define STRING_H

#include <stdlib.h>

#include "dac/bool.h"

typedef char* String;

size_t String_Length(const String);

Bool String_StartsWith(const String, const String, size_t);

size_t String_IndexOf(const String, char, size_t);

String String_Substring(const String, size_t, size_t);

Bool String_Equals(const String, const String);

void String_Concat(String, String);

int String_ToInt(String);

#endif