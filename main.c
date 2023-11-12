#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ARG_STRING_LENGTH 2

#define LINE_AMOUNT 256
#define LINE_LENGTH 80

#define ARGS_AMOUNT 7
#define ADD_ARG 0
#define REMOVE_ARG 1
#define CHECK_ARG 2
#define UNCHECK_ARG 3
#define PRINT_ARG 4
#define TOUCH_ARG 5
#define IS_CHECKED_ARG 6

typedef struct arg_s {
  int id;
  char string[ARG_STRING_LENGTH];
} arg_t;

arg_t arg_defs[ARGS_AMOUNT] = {
  {0, "-a"}, //add checkbox
  {1, "-r"}, //remove checkbox
  {2, "-c"}, //check checkbox
  {3, "-u"}, //uncheck checkbox
  {4, "-p"}, //print todo list
  {5, "-t"}, //create todo list
  {6, "-q"}, //check if a checkbox is checked
};

char lines[LINE_AMOUNT][LINE_LENGTH];

void get_file_lines(char * path){
  FILE * file;
  int current_line = 0;
  
  if ((file = fopen(path, "r")) == NULL){
    fprintf(stderr, "todo: error: unable to load %s\n", path);
    exit(-1);
  }

  while (fgets(lines[current_line], LINE_LENGTH, file) != NULL){
    current_line++;
  }
  fclose(file);
}

void add_checkbox(char * path, char * requirement){
  FILE * file;
  char * text = malloc(LINE_LENGTH);
  strcpy(text, "[ ] ");
  strcat(text, requirement);
  strcat(text, "\n");
  
  if ((file = fopen(path, "a+")) == NULL){
    fprintf(stderr, "todo: error: unable to load %s\n", path);
    exit(-1);
  }

  fputs(text, file);

  fclose(file);
  free(text);
}

void remove_checkbox(char * path, char * requirement){
  FILE * file;
  char * text = malloc(LINE_LENGTH);
  int current_line = 0;
  strcpy(text, requirement);
  strcat(text, "\n");

  while (1){
    if (strstr(lines[current_line], requirement) != NULL){
      break;
    }
    current_line++;
    if (lines[current_line] == NULL){
      fprintf(stderr, "todo: error: could not find specified text\n");
      free(text);
      exit(-1);
    }
  }

  free(text);

  strcpy(lines[current_line], "");
  for (int line = current_line ; line < LINE_AMOUNT ; line++){
    if (lines[line+1] == NULL || line + 1 >= LINE_AMOUNT){
      break;
    }
    strcpy(lines[line], lines[line+1]);
  }

  if ((file = fopen(path, "w")) == NULL){
    fprintf(stderr, "todo: error: unable to load %s\n", path);
    exit(-1);
  }

  for (int line = 0 ; line < LINE_AMOUNT ; line++){
    if (lines[line] == NULL){
      break;
    }
    fputs(lines[line], file);
  }

  fclose(file);
}

void check_checkbox(char * path, char * requirement){
  FILE * file;
  char * text = malloc(LINE_LENGTH);
  int current_line = 0;
  strcpy(text, "[X] ");
  strcat(text, requirement);
  strcat(text, "\n");
  
  while (1){
    if (strstr(lines[current_line], requirement) != NULL){
      break;
    }
    current_line++;
    if (lines[current_line] == NULL){
      fprintf(stderr, "todo: error: could not find specified text\n");
      free(text);
      exit(-1);
    }
  }
  
  strcpy(lines[current_line], text);

  free(text);
  
  if ((file = fopen(path, "w")) == NULL){
    fprintf(stderr, "todo: error: unable to load path %s\n", path);
    exit(-1);
  }

  for (int line = 0; line < LINE_AMOUNT ; line++){
    if (lines[line] == NULL){
      break;
    }
    fputs(lines[line], file);
  }

  fclose(file);
}

void uncheck_checkbox(char * path, char * requirement){
  FILE * file;
  char * text = malloc(LINE_LENGTH);
  int current_line = 0;
  strcpy(text, "[ ] ");
  strcat(text, requirement);
  strcat(text, "\n");
  
  while (1){
    if (strstr(lines[current_line], requirement) != NULL){
      break;
    }
    current_line++;
    if (lines[current_line] == NULL){
      fprintf(stderr, "todo: error: could not find specified text\n");
      free(text);
      exit(-1);
    }
  }
  
  strcpy(lines[current_line], text);

  free(text);
  
  if ((file = fopen(path, "w")) == NULL){
    fprintf(stderr, "todo: error: unable to load path %s\n", path);
    exit(-1);
  }

  for (int line = 0; line < LINE_AMOUNT ; line++){
    if (lines[line] == NULL){
      break;
    }
    fputs(lines[line], file);
  }

  fclose(file);
}

void print_checklist(){
  for (int line = 0 ; line < LINE_AMOUNT ; line++){
    if (lines[line] == NULL){
      break;
    }
    printf("%s", lines[line]);
  }
}

void create_checklist(char * path){
  FILE * file;

  if (access(path, F_OK) == 0){
    fprintf(stderr, "todo: error: %s already exists\n", path);
    exit(-1);
  }
  
  file = fopen(path, "w");
  printf("todo: created %s\n", path);
  
  fclose(file);
}

int is_checked(char * requirement){
  char * text = malloc(LINE_LENGTH);
  strcpy(text, "[X] ");
  strcat(text, requirement);
  strcat(text, "\n");
  
  for (int line = 0 ; line < LINE_AMOUNT ; line++){
    if (lines[line] == NULL){
      fprintf(stderr, "todo: error: could not find specified text\n");
      free(text);
      return 1;
    }
    if (strcmp(text, lines[line]) == 0){
      printf("todo: %s is checked\n", requirement);
      free(text);
      return 0;
    }
  }

  printf("todo: %s is unchecked\n", requirement);
  free(text);
  return 1;
}

int main(int argc, char ** argv){
  if (argc == 1){
    fprintf(stderr, "todo: error: no arguments\n");
    return -1;
  }

  if (argc == 2){
    fprintf(stderr, "todo: error: nothing to do\n");
    return -1;
  }

  int arg_id;
  for (arg_id = 0 ; arg_id < ARGS_AMOUNT ; arg_id++){
    if (strcmp(arg_defs[arg_id].string, argv[2]) == 0){
      break;
    }
  }
  
  if (arg_id == ARGS_AMOUNT){
    fprintf(stderr, "todo: error: unknown argument\n");
    return -1;
  }

  if (arg_id != TOUCH_ARG){
    get_file_lines(argv[1]);
  }

  switch(arg_id){
    case ADD_ARG:
      add_checkbox(argv[1], argv[3]);
      break;
    case REMOVE_ARG:
      remove_checkbox(argv[1], argv[3]);
      break;
    case CHECK_ARG:
      check_checkbox(argv[1], argv[3]);
      break;
    case UNCHECK_ARG:
      uncheck_checkbox(argv[1], argv[3]);
      break;
    case PRINT_ARG:
      print_checklist();
      break;
    case TOUCH_ARG:
      create_checklist(argv[1]);
      break;
    case IS_CHECKED_ARG:
      return is_checked(argv[3]);
  };

  return 0;
}
