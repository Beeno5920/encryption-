#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int shift(char c);
char encrypt(char c, int key);
void caesar(int key);
void vigenere(int key, char* s);
void caesar_file(FILE* inF, FILE* outF, int key);
void vigenere_file(FILE* inF, FILE* outF, int key, char* s);
void assign_out_file_name(char* outF_name, char* str);

int main(int argc, char* argv[])
{
  int digit = 0, alpha = 0;

  if(argc < 2){
    printf("Usage: %s key (text file(s))\n", argv[0]);
    return 1;
  }

  for(int i = 0, n = strlen(argv[1]); i < n; i++){
    if(isdigit(argv[1][i]))
      digit++;
    else if(isalpha(argv[1][i]))
      alpha++;
  }

  if(alpha > 0 && digit > 0){
    printf("Please input either number or a word as the key!\n");
    return 1;
  }

  if(argc == 2){
    if(digit > 0 && alpha == 0){
      int key = atoi(argv[1]);
      caesar(key);
    }else if(digit == 0 && alpha > 0){
      int key = shift(argv[1][0]);
      vigenere(key, argv[1]);
    }
  }else{
    for(int i = 2; i < argc; i++){
      FILE* inF = fopen(argv[i], "r");
      if(inF == NULL){
        printf("Fail to open file: %s\n", argv[i]);
        continue;
      }
      char* outF_name = malloc((strlen(argv[i]) + 10) * sizeof(*outF_name));
      assign_out_file_name(outF_name, argv[i]);
      FILE* outF = fopen(outF_name, "w");

      if(digit > 0 && alpha == 0){
        int key = atoi(argv[1]);
        caesar_file(inF, outF, key);
      }else if(digit == 0 && alpha > 0){
        int key = shift(argv[1][0]);
        vigenere_file(inF, outF, key, argv[1]);
      }

      if(fclose(outF) != 0){
        printf("Fail to close the file: %s\n", outF_name);
        return 1;
      }
      free(outF_name);

      if(fclose(inF) != 0){
        printf("Fail to close the file: %s\n", argv[i]);
      }
    }
  }

  return 0;
}

int shift(char c){
    if(isupper(c))
        return c - 65;
    return c - 97;
}

char encrypt(char c, int key){
  if(isupper(c))
    return (c + key - 65) % 26 + 65;
  if(islower(c))
    return (c + key - 97) % 26 + 97;
  return c;
}

void caesar(int key){
    printf("plaintext: ");
    char input[256];
    scanf("%s", input);
    printf("ciphertext: ");
    for(int i = 0, n = strlen(input); i < n; i++){
      printf("%c", encrypt(input[i], key));
    }
    printf("\n");
}

void vigenere(int key, char* s){
    printf("plaintext: ");
    char input[256];
    scanf("%s", input);
    printf("ciphertext: ");
    int k = 0;
    for(int i = 0, n = strlen(input); i < n; i++){
        if(isspace(input[i])){
            printf(" ");
            continue;
        }
        printf("%c", encrypt(input[i], key));
        if(encrypt(input[i], key) == input[i])
          continue;

        if(k == strlen(s) - 1)
            k = -1;
        k++;
        key = shift(s[k]);
    }
    printf("\n");
}

void caesar_file(FILE* inF, FILE* outF, int key){
  char* line = NULL;
  size_t sz = 0;
  while(getline(&line, &sz, inF) >= 0){
    char* cursor = line;
    while(*cursor != '\0'){
      char c = *cursor;
      c = encrypt(c, key);
      *cursor = c;
      cursor++;
    }
    fprintf(outF, "%s", line);
  }
  free(line);
}

void vigenere_file(FILE* inF, FILE* outF, int key, char* s){
  char* line = NULL;
  size_t sz = 0;
  int k = 0;
  while(getline(&line, &sz, inF) >= 0){
    char* cursor = line;
    while(*cursor != '\0'){
      char c = *cursor;
      c = encrypt(c, key);

      if(c == *cursor){
        *cursor = c;
        continue;
      }

      if(k == strlen(s) - 1)
        k = -1;
      k++;
      key = shift(s[k]);
      *cursor = c;
      cursor++;
    }
    fprintf(outF, "%s", line);
  }
  free(line);
}

void assign_out_file_name(char* outF_name, char* str){
  int i = strlen(str);
  int len = i;
  int index = 0;

  for(; i > 0; i--){
    if(str[i] == '.')
      index = i;
    if(str[i] == '/'){
      i++;
      break;
    }
  }
  outF_name = realloc(outF_name, sizeof(*outF_name) * (len - i + 10));
  memcpy(outF_name, &str[i], (index - i));
  strcat(outF_name, "_enc.txt");

  return;
}
