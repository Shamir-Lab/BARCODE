#include "barcode.h"
//************************************************************************************
//main function
//************************************************************************************
void load_unique_reads_to_trie(hattrie_t* trie_unique, char* reads_file_path, int* results) {
  hattrie_t* trie_repeat;
  int size = 80; //size of read, default value=80 and increases in case of need
  int  pos; //index to buffer
  int c; //reading character
  int line_number=1;
  int read_num =0;
  char *buffer = (char *)malloc(size);
  int cur_line_num = 1;
  int has_N; //1 if line contains 'N', '0' otherwise 
  FILE* f;
  f = fopen(reads_file_path, "r");
  trie_repeat = hattrie_create();
  if(f){
      do { // read all lines in file
        pos = 0;
        has_N = 0;
        do{ // read one line
          c = fgetc(f);
          if(c != EOF) buffer[pos++] = toupper((char)c);
          if(pos >= size) { // increase buffer length - leave room for 0
            size +=1;
            buffer = (char*)realloc(buffer, size);
          }
        }while(c != EOF && c != '\n');
        buffer[pos] = 0;
        // line is now in buffer
          read_num+=1;
          handle_line(buffer, trie_unique, trie_repeat, has_N);
        line_number+=1;
      } while(c != EOF);
      fclose(f);
  }
  else {
    fprintf(stderr, "Error: couldnt open file %s \n",reads_file_path);
  }
  free(buffer);
  hattrie_free(trie_repeat);
  results[1] = read_num;
  results[2]= size;
       hattrie_iteration(trie_unique, "unique", "test_2");

}




void test_make_unique_and_repeat_files(char * read_file_path, char* label) {
    int read_size=0; //the size of the reads
    int table_factor; //arbitrary
    int num_of_reads;
    int bf_table_size = table_factor*num_of_reads;
    int num_of_hash_func;

    BloomFilter* bf_unique; //BF for the unique tries
  //  char* output_label=(char *)malloc(50); //label name for the output files
    hattrie_t* trie_unique; //hattrie that holds the unique reads
    hattrie_t* trie_repeat; //hattrie that holds the repetetive reads, and the one that has N inside of them.
    hattrie_t* trie_genome_unique; //put 'accepts' (everything that uniqe BF says yes that it's in genome) into a trie
    hattrie_t* trie_fp; //triw that holds false negatives set
    hattrie_t* trie_fn;//trie that holds false positives set
//    f = fopen(argv[1], "r");
    trie_repeat = hattrie_create();
    trie_unique = hattrie_create();
    make_repeat_and_unique_tries(read_file_path, trie_unique, trie_repeat);
    hattrie_iteration(trie_unique, "unique", label);
    hattrie_iteration(trie_repeat, "repeat", label);
    hattrie_free(trie_unique);
    hattrie_free(trie_repeat);

}



void test_hash_unique_and_make_fp_fn(char * reads_file_path, char* genome_file_path, char* label) {
    int read_size=0; //the size of the reads
    int table_factor; //arbitrary
    int bf_table_size;
    int num_of_hash_func;
//    int results[2];
    FILE* genome_f;
    BloomFilter* bf_unique; //BF for the unique tries
  //  char* output_label=(char *)malloc(50); //label name for the output files
    hattrie_t* trie_unique; //hattrie that holds the unique reads
    hattrie_t* trie_genome_unique; //put 'accepts' (everything that uniqe BF says yes that it's in genome) into a trie
    hattrie_t* trie_fp; //triw that holds false negatives set
    hattrie_t* trie_fn;//trie that holds false positives set
    hattrie_t* trie_genome_true; //holds reads that really mapped into the genome reference



    int size = 80; //size of read, default value=80 and increases in case of need
    int  pos; //index to buffer
    int c; //reading character
    int line_number=1;
    int read_num =0;
    char *buffer = (char *)malloc(size);
    int cur_line_num = 1;
    int has_N; //1 if line contains 'N', '0' otherwise 
    FILE* f;
    hattrie_t* trie_repeat;
        ////
    //REMOVE FROM ORIGINAL PIPELINE!!! XXXXXXXX
    /////
    trie_unique = hattrie_create();
    printf("start load uniqe\n");
//    load_unique_reads_to_trie(trie_unique, read_file_path, results);
    f = fopen(reads_file_path, "r");
    trie_repeat = hattrie_create();
    if(f){
      do { // read all lines in file
        pos = 0;
        has_N = 0;
        do{ // read one line
          c = fgetc(f);
          if(c != EOF) buffer[pos++] = toupper((char)c);
          if(pos >= size) { // increase buffer length - leave room for 0
            size +=1;
            buffer = (char*)realloc(buffer, size);
          }
        }while(c != EOF && c != '\n');
        buffer[pos] = 0;
        // line is now in buffer
          read_num+=1;
          handle_line(buffer, trie_unique, trie_repeat, has_N);
        line_number+=1;
      } while(c != EOF);
      fclose(f);
    }
    else {
      fprintf(stderr, "Error: couldnt open file %s \n",reads_file_path);
    }
    free(buffer);





//    hattrie_iteration(trie_unique, "unique", label);
    printf("done load uniqe\n");
//    hattrie_iteration(trie_unique, "unique", label);
   

    genome_f = fopen(genome_file_path, "r");   
    table_factor = 10; //arbitrary
  //  num_of_reads = line_number/2;
    bf_table_size = table_factor*read_num;
    num_of_hash_func = (int) ceil(table_factor*0.69314);
//hashing uniqe reads trie using bloom filter
    bf_unique = bloom_filter_new(bf_table_size, string_hash, num_of_hash_func);
    hash_trie_into_bf(trie_unique, bf_unique);
//create trie for all of the sliding windows in the genome reference which are in the unique reads according to the bf_unique
    read_size = size-2;
    trie_genome_unique = hattrie_create();
    query_bf_with_genome(bf_unique, genome_f ,trie_genome_unique, read_size);
    fclose(genome_f);
    printf("printing genome mapped unique reads, true and FP \n");
    hattrie_iteration(trie_genome_unique, "genome_unique_all", label);
    trie_fp = hattrie_create();
    trie_fn = hattrie_create();
    trie_genome_true = hattrie_create();     //reads that really maps to the genome

    printf("start checking for false positive \n");
    check_fp(trie_unique,trie_genome_unique, trie_fp, trie_genome_true);
    hattrie_iteration(trie_fp, "fp_unique", label);

    printf("printing genome mapped unique reads, ture true values only \n");
    hattrie_iteration(trie_genome_true, "genome_unique_true", label);

    printf("start checking for false negative \n");
    check_fn(trie_unique,trie_genome_true, trie_fn);
    hattrie_iteration(trie_fn, "fn_unique", label);
    printf("printing bloom filter\n");
    print_bf(bf_unique, bf_table_size, label, "unique_bf");
    

    bloom_filter_free(bf_unique);
    hattrie_free(trie_unique);
    hattrie_free(trie_genome_unique);
    hattrie_free(trie_genome_true);
    hattrie_free(trie_fn);
    hattrie_free(trie_fp);

}
//arg1 = number of test
//arg2 = label
//arg3 = reads file
//arg4 = genome reference file
int main(int argc, char *argv[]) {
  char* test_number=NULL;
  test_number = argv[1];
  if (strcmp(test_number, "1") ==0) {
    printf("doing test 1 \n");
    test_make_unique_and_repeat_files(argv[3], argv[2]);
  }

  if (strcmp(test_number, "2") ==0) {
    printf("doing test 2 \n");
    test_hash_unique_and_make_fp_fn(argv[3], argv[4], argv[2]);
  }
  return 0;
}

