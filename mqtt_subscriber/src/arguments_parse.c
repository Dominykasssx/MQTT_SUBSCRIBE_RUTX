#include <argp.h>
#include "arguments_parse.h"
#include <stdlib.h>

void arguments_init(struct arguments *arguments)
{
    arguments->use_tls = "";
    arguments->brokerIp = "";
    arguments->brokerPort = "";
    arguments->username = "";
    arguments->password = "";
}

error_t parse_opt (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = state->input;

  switch (key){
    case 't':
      arguments->use_tls = atoi(arg);
      break;
    case 'b':
      arguments->brokerIp = arg;
      break;
	  case 'p':
      arguments->brokerPort = atoi(arg);
      break;
    case 'u':
      arguments->username = arg;
      break;
    case 's':
      arguments->password = arg;
      break;
    case 'c':
      arguments->certificate = arg;
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num > 5)
        argp_usage (state);
      break;
    default:
        return ARGP_ERR_UNKNOWN;
  }
  return 0;
}
