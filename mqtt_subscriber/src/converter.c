#include <stdio.h>
#include <string.h>

int dataTypeConvert(int typeNumber, char *typeChar)
{
  int res = 0;

  switch (typeNumber)
  {
  case 0:
    strcpy(typeChar, "String");
    break;
  case 1:
    strcpy(typeChar, "Number");
    break;
  default:
    res = -1;
  }

  return res;
}

int compareTypeConvert(int typeNumber, char *typeChar)
{
  int res = 0;

  switch (typeNumber)
  {
  case 0:
    strcpy(typeChar, "==");
    break;
  case 1:
    strcpy(typeChar, "<");
    break;
  case 2:
    strcpy(typeChar, ">");
    break;
  case 3:
    strcpy(typeChar, "!=");
    break;
  case 4:
    strcpy(typeChar, ">=");
    break;
  case 5:
    strcpy(typeChar, "<=");
    break;

  default:
    strcpy(typeChar, "Not found");
    res = -1;
  }

  return res;
}