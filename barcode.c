#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "hat-trie.h"

//this function handles each read she receive the next way:
//if already in repeat, it will reinsert, if not check if conatains N(has_N=1) in it and puse it to "repeat" if so.
//else, if it's in unique read  take out the read from the uniqe and insert both to repeat trie,
// if not, insert it to uniqe read. 
void handle_line(char *line, hattrie_t* trie_unique, hattrie_t* trie_repeat, int has_N) {
    value_t* m_key;
    int len;
    int old_repeat_num; //holds value of number of repeats of a key before enetring another same key
    len = strlen(line);
    //if already in repeat, it will reinsert, if not check if conatains N in it and puse it to "repeat" if so.
    //else, if it's in unique read  take out the read from the uniqe and insert both to repeat trie,
    // if not, insert it to uniqe read. 
    m_key = hattrie_tryget(trie_repeat, line, len);
    if(m_key==NULL){ //if not in repeat
      if(has_N==1){ //if it's new key with N in it
        m_key = hattrie_get(trie_repeat, line, len);
        *m_key=1;
      }
      else if(hattrie_tryget(trie_unique, line, len)==NULL){// if it's new unique key without N
        m_key = hattrie_get(trie_unique, line, len);
        *m_key=1;
      }
      else{//if it's in uqniqu, take it out from unique and push to repeat with value 2
        if(hattrie_del(trie_unique, line, len)!=0){
          fprintf(stderr, "error deleting node\n");  
          exit(1);
        }
        m_key = hattrie_get(trie_repeat, line, len);
        *m_key=2;
      }
    }
    else{//if it's in repeat 
      old_repeat_num = *m_key;
      m_key = hattrie_get(trie_repeat, line, len);
      *m_key = old_repeat_num+1;
    }
}

//this function take a hattrie, and iterate on all of its keys.
//the label is the name for the trie
void hattrie_iteration(hattrie_t* T, char* m_label, char* output_label)
{   
    printf("iterating through keys for %s trie \n", m_label);

    hattrie_iter_t* i = hattrie_iter_begin(T, false);

    size_t count = 0;
    value_t* value;
    value_t  v;
    int repeat_number;
    size_t len;
    const char* key;
    FILE *fp;
    char output_path[1024]="";
    strcat(output_path, "./");
    if (output_label) {
      strcat(output_path, output_label);
      strcat(output_path, "_");
    }  
    strcat(output_path, m_label);
    strcat(output_path, ".txt");
    fp=fopen(output_path, "w");

    while (!hattrie_iter_finished(i)) {
        ++count;

        key = hattrie_iter_key(i, &len);
        value   = hattrie_iter_val(i);
        for(repeat_number = 1; repeat_number <= *value; repeat_number++){       
          fprintf(fp, "%s", key); 
        }
        hattrie_iter_next(i);
    }
    hattrie_iter_free(i);
    
    fclose(fp);
    printf("done.\n");
}




//checks that reads are only A/C/T/G/N charcters (capital or non-capital letters)
int check_legal_char(char c , int line_number) {
  char up_c;
  up_c = toupper(c); //could be both capital and non-capital letters
  if (up_c=='A' || up_c=='T' || up_c=='C' || up_c=='G'||  up_c=='N' || up_c=='\n' || up_c==EOF)
  {
    return(1);
  }
  else{
    fprintf(stderr, "Error: illegal charcter %c in line %d \n",c,line_number);
    exit(1);
  }
}
 
int main(int argc, char *argv[]) {
    int size = 80; //size of read, default value=80 and increases in case of need
    int  pos; //index to buffer
    int c; //reading character
    int line_number=1;
    int read_num =0;
    char *buffer = (char *)malloc(size);
    int cur_line_num = 1;
    int has_N; //1 if line contains 'N', '0' otherwise 
    FILE *f = fopen(argv[1], "r");
  //  char* output_label=(char *)malloc(50); //label name for the output files
    hattrie_t* trie_unique; //hattrie that holds the unique reads
    hattrie_t* trie_repeat; //hattrie that holds the repetetive reads, and the one that has N inside of them.
    if(f) {
      trie_repeat = hattrie_create();
      trie_unique = hattrie_create();
    //  output_label = argv[2];
    //  printf("%s\n", output_label);
      do { // read all lines in file
        pos = 0;
        has_N = 0;
        do{ // read one line
          c = fgetc(f);
          if(line_number%2==0){ //check valid data only for read lines   
            check_legal_char(c, line_number);
            if(has_N==0){
              has_N = c=='N' ? 1 : 0;
            }            
          }
          if(c != EOF) buffer[pos++] = (char)c;
          if(pos >= size) { // increase buffer length - leave room for 0
            size +=1;
            buffer = (char*)realloc(buffer, size);
          }
        }while(c != EOF && c != '\n');
        buffer[pos] = 0;
        // line is now in buffer
        if (line_number%2==0)
        { //data is only in even lines
          read_num+=1;
          handle_line(buffer, trie_unique, trie_repeat, has_N);            
        } 
        has_N = 0;
        line_number+=1;  
      } while(c != EOF); 
      fclose(f);           
    }
    hattrie_iteration(trie_unique, "unique", argv[2]);
    hattrie_iteration(trie_repeat, "repeat", argv[2]);
    free(buffer);
    hattrie_free(trie_unique);
    hattrie_free(trie_repeat);
    return 0;
}
