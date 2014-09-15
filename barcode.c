#include "barcode.h"
//************************************************************************************
//check_legal_char function
//************************************************************************************
//checks that reads are only A/C/T/G/N charcters (capital or non-capital letters)
//************************************************************************************
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

//************************************************************************************
//handle_line function
//************************************************************************************
//this function handles each read she receive the next way:
//if already in repeat, it will reinsert, if not check if conatains N(has_N=1) in it and puse it to "repeat" if so.
//else, if it's in unique read  take out the read from the uniqe and insert both to repeat trie,
// if not, insert it to uniqe read. 
//************************************************************************************
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

//************************************************************************************
//hattrie_iteration function
//************************************************************************************
//this function take a hattrie, and iterate on all of its keys,
//and write it to a file located in  user given output path (cwd if none) and with a label
//the label is the name for the trie
//************************************************************************************
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

//************************************************************************************
//make_repeat_and_unique_tries function
//************************************************************************************
//gets a file path conataining reads and two empty tries, to fill them with:
//1.unique trie
//2.repeat trie
//************************************************************************************


void make_repeat_and_unique_tries(char* reads_file_path, hattrie_t* trie_unique, hattrie_t* trie_repeat) {
  FILE* f;
  int size = 80; //size of read, default value=80 and increases in case of need
  int  pos; //index to buffer
  int c; //reading character
  int line_number=1;
  int read_num =0;
  char *buffer = (char *)malloc(size);
  int cur_line_num = 1;
  int has_N; //1 if line contains 'N', '0' otherwise
  printf("opening file to read \n"); 
  f = fopen(reads_file_path, "r");
  printf("finished opening file \n");
  if(f){
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
          if(c != EOF) buffer[pos++] = toupper((char)c);
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
//        if(line_number%50000==0){
//          printf("%d \n", line_number); //tmp remove!
//        }
      } while(c != EOF);
      fclose(f);
  }
  else {
    fprintf(stderr, "Error: couldnt open file %s \n",reads_file_path);
  }  
  free(buffer); 
}


//************************************************************************************
//set_complentary_strand
//************************************************************************************
void set_complementary_strand(char * orig_strand, char *comp_strand) {
  int index=0;
  while (orig_strand[index]!='\0'){
    if (orig_strand[index]=='A') {
      comp_strand[index]='T';
    }
    if (orig_strand[index]=='T') {
      comp_strand[index]='A';
    }
    if (orig_strand[index]=='C') {
      comp_strand[index]='G';    
    }
    if (orig_strand[index]=='G') {
      comp_strand[index]='C';
    }
    if (orig_strand[index]=='\n') {
      comp_strand[index]='\n';
    }


   index++;
  }
  comp_strand[index] = '\0';
//  printf("orig_strnad = %s, comp_strand =%s",orig_strand, comp_strand);
}



//************************************************************************************
//hash_trie_into_bf function
//************************************************************************************
void hash_trie_into_bf(hattrie_t* T, BloomFilter* bf) {
    size_t count = 0;
    value_t* value;
    value_t  v;
    int repeat_number;
    size_t len;
    char* key;
//    BloomFilterValue pkey_to_bf=NULL;
    hattrie_iter_t* i = hattrie_iter_begin(T, false); 
    while (!hattrie_iter_finished(i)) {
        ++count;
        key = hattrie_iter_key(i, &len);
//        printf("hashing string %s \n", key);
        bloom_filter_insert(bf, key); //insert key into bloom filter
        hattrie_iter_next(i);
    }
//    free(pkey_to_bf);
    hattrie_iter_free(i);
}


//************************************************************************************
//check_if_trie_in_bf function
//************************************************************************************
void check_if_trie_in_bf(hattrie_t* T, BloomFilter* bf) {
    size_t count = 0;
    value_t* value;
    value_t  v;
    int repeat_number;
    size_t len;
    char* key;
    int in_bf;
    
//    BloomFilterValue pkey_to_bf=NULL;
    hattrie_iter_t* i = hattrie_iter_begin(T, false);
    while (!hattrie_iter_finished(i)) {
        ++count;
        key = hattrie_iter_key(i, &len);
        in_bf = bloom_filter_query(bf, key); //checks if key in bloom filter
        printf("%d \n", in_bf);
        if (in_bf!=0) {
          printf("%s",key);
        }
        hattrie_iter_next(i);
    }
//    free(pkey_to_bf);
    hattrie_iter_free(i);
}

//************************************************************************************
//query_bf_with_genome function
//************************************************************************************
//this function gets the bloom filter with the unique reads, the gnome reference file,
//and the read size, and put all the sliding windows size of read_size in which the
//bloom filter says that they are in the bloom filter, into trie_genome_unique
//meaning trie_genome uniuqe holds all the unique reads that can map into the genome reference
//as well as bloom filter's false positive sliding windows
//************************************************************************************
void query_bf_with_genome(BloomFilter* bf_unique, FILE* genome_file ,hattrie_t* trie_genome_uniqe, int read_size) {
  int in_bf; //1 if cur_window in genome bloom-filter, 0 otherwisei
  int in_bf_comp; //1 if comp_window in genome bloom-filter, 0 otherwise
  int cur_window_index=0; //current index in the window
  int line_number =1; // count number of line in file
  char* cur_window; //assuming read size is no larger then 600 chars
  char* complementary_window; //complementary strand to cur_window
  char* prev_window;
  char* prev_comp_window;
  int c; //holds current character
  long  int count_in_bf=0;
  long int count_comp_in_bf=0;
  long int count_windows=0;
  value_t* m_key;
  printf("start querying unique BF through genome \n");
  cur_window =  (char *)malloc(read_size+2); //+1 to '\n' , and +1 to'\0'
  complementary_window = (char *)malloc(read_size+2);
  prev_window =  (char *)malloc(read_size);
  prev_comp_window = (char *)malloc(read_size);
  do { 
    c = fgetc(genome_file);
    if (c =='>' ) {
      do {
        c = (char) fgetc(genome_file);
      } while (c != '\n'); 
      line_number++;
      cur_window_index=0;
      continue;
    }
    check_legal_char(c, line_number);
    if (c=='N') {
      cur_window_index=0;
      continue;
    }
    if (c=='\n') { 
      line_number++;
      continue;
    }



    if ((line_number+cur_window_index)%1000000==0) {
      printf("cur line number is %d line_number\n", line_number);
    }



    if (cur_window_index == read_size-1) { // case we start full window or moving to next charcter in sliding window
      // assigning cur_window = prev_window + c
      count_windows++;
      strncpy(cur_window, prev_window, read_size-1); //will copy prev window (99 charcters) to beginning of cur_Window
      strncpy(complementary_window+1, prev_comp_window, read_size-1); //will copy prev window (99 charcters) of cur_Window, leaving first character free
      cur_window[read_size-1] = toupper(c);
      if (toupper(c)=='C') complementary_window[0]='G';
      if (toupper(c)=='G') complementary_window[0]='C';
      if (toupper(c)=='T') complementary_window[0]='A';
      if (toupper(c)=='A') complementary_window[0]='T';
      cur_window[read_size] = '\n';
      cur_window[read_size+1] = '\0';
      complementary_window[read_size] = '\n';
      complementary_window[read_size+1] = '\0';

      in_bf = bloom_filter_query(bf_unique, cur_window);
      if (in_bf!=0) {
//        printf("in bf xxxx \n");
        count_in_bf++;
        m_key = hattrie_get(trie_genome_uniqe, cur_window, strlen(cur_window));
        *m_key=1;
      }
      in_bf_comp = bloom_filter_query(bf_unique, complementary_window);
      if (in_bf_comp!=0) {
//      printf("in bf xxxx \n");
        count_comp_in_bf++;
        m_key = hattrie_get(trie_genome_uniqe, complementary_window, strlen(complementary_window));
        *m_key=1;
      }

//      printf("regular window is %s , comp is %s",cur_window, complementary_window);
      strncpy(prev_window, cur_window+1, read_size-1); //will copy from second character 'read_size-1' characters into prev_window ,e.g for 100 will copy  2-100
      strncpy(prev_comp_window, complementary_window, read_size-1); //will copy the first 'read_size-1' (99) characters to prev window
    } 
    else {
      cur_window[cur_window_index]=toupper(c);
      if (toupper(c)=='C') complementary_window[read_size-cur_window_index-1]='G';
      if (toupper(c)=='G') complementary_window[read_size-cur_window_index-1]='C';
      if (toupper(c)=='T') complementary_window[read_size-cur_window_index-1]='A';
      if (toupper(c)=='A') complementary_window[read_size-cur_window_index-1]='T';
 
      if (cur_window_index==(read_size-2)) { //if it's the last character before window is the size of a read
        strncpy(prev_window, cur_window, read_size-1); //will copy from first character 'read_size -1' characters into prev_window,e.g for 100 will copy  1-99
        strncpy(prev_comp_window, complementary_window+1, read_size-1);// will copy from
      }
      cur_window_index++;
    }
  } while(c != EOF);
  free(cur_window);
  free(prev_window);
  free(complementary_window);
  free(prev_comp_window);
  printf("there where total of %ld windows, in which %ld of original strands and %ld of complementary strands gave true for being in the BF \n",count_windows, count_in_bf, count_comp_in_bf);
}
             
//************************************************************************************
//check_fp function
//************************************************************************************
//put all keys that are in trie_to_check but not in trie_true(false positive case)
//into trie_push.
//also it's going to put all the true reads(ones that are in trie_true) from the trie_to_ order to stay only with true reads and put it in triw_to_check_true
///in our case put all the windows from the reference genome that bloome filter said are in the unique read but are not (due to bloom filter false positive)
//************************************************************************************
void check_fp(hattrie_t* trie_true, hattrie_t* trie_to_check, hattrie_t* trie_push,  hattrie_t* trie_to_check_true) {
    int count = 0;
    int total_count =0;
    value_t* value;
    value_t  v;
    int repeat_number;
    size_t len;
    char* check_key; //current read from trie_to_check to see if in trie_true
    char* m_key; // result of key when checking if trie_to_check is in trie_true
    char* push_key;
    char* to_check_true_key;
    hattrie_iter_t* i = hattrie_iter_begin(trie_to_check, false);
//    trie_real_unique = hattrie_create();  //TMP- REOMVE!!!!
    while (!hattrie_iter_finished(i)) {
        total_count++;
        check_key = hattrie_iter_key(i, &len);
        m_key = hattrie_tryget(trie_true, check_key, len);
        if(m_key==NULL){ //if not in trie_true - means false positive, then insert to FP trie, and also remove it from 
          count++;
          push_key = hattrie_get(trie_push, check_key, len);
          *push_key=1;
        }
        else { //means it is true
          if (trie_to_check_true!= NULL){
            to_check_true_key = hattrie_get(trie_to_check_true, check_key, len);
            *to_check_true_key = 1;
          }
        }

       
        hattrie_iter_next(i);
    }
    printf("we had %d false_positives out of %d reads \n", count, total_count);
    hattrie_iter_free(i);
//    hattrie_iteration(trie_real_unique, "true_unique_fn", "testing"); //TMP- REOMVE!!!!
//    hattrie_free(trie_real_unique);//TMP- REOMVE!!!!
}

//************************************************************************************
//check_fn function
//************************************************************************************
//put all keys that are in trie_true but not in trie_true_reference (false nrgative case)
//into trie_push
//trie_true_reference should contain all the TRUE(!!) windows in the reference genome which are actually mapped to the trie_reads
//in our case put all the reads that are unique but are not mapped into the genome reference (due to mutations)
//************************************************************************************
void check_fn(hattrie_t* trie_reads, hattrie_t* trie_true_reference, hattrie_t* trie_push) {
  check_fp(trie_true_reference, trie_reads, trie_push, NULL); 
}

char *byte_to_binary(int x)
{
    static char b[9];
    b[0] = '\0';

    int z;
    for (z = 128; z > 0; z >>= 1)
    {
        strcat(b, ((x & z) == z) ? "1" : "0");
    }

    return b;
}



//************************************************************************************
//print_bf function
//************************************************************************************
//gets bf and it's table size,and two labels and prints its content to a file
//************************************************************************************
void print_bf(BloomFilter* bf, int table_size, char* label_1, char* label_2) {
  unsigned char* bf_array; //array to load bf into in order to print it
  int array_size = 0;
  FILE *fp;
  char output_path[1024]="";
  int index=0;
  char* cur_byte;
  strcat(output_path, "./");
  if (label_1) {
    strcat(output_path, label_1);
    strcat(output_path, "_");
  }
  strcat(output_path, label_2);
  strcat(output_path, ".txt");
  fp=fopen(output_path, "w"); 
  array_size = (table_size + 7) / 8;
  bf_array = (char *)malloc(sizeof(char)*array_size);
  bloom_filter_read(bf, bf_array);
  while (index<(array_size-1)){
    cur_byte = byte_to_binary(bf_array[index]);
    fprintf(fp, "%s\n",cur_byte);
    index++;
  }
  free(bf_array);
  fclose(fp);
  printf("done.\n");
}
