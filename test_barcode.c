#include "barcode.h"

//************************************************************************************
//load_unique_reads_to_trie function
//************************************************************************************
void load_unique_reads_to_trie(hattrie_t* trie_unique, char* reads_file_path, int* results) {
  hattrie_t* trie_repeat;
  int size = 80; //size of read, default value=80 and increases in case of need
  int  pos; //index to buffer
  int c; //reading character
  long long line_number=1;
  long long read_num =0;
  char *buffer = (char *)malloc(size);
  long long cur_line_num = 1;
  int has_N; //1 if line contains 'N', '0' otherwise 
  FILE* f;
  long long unique_read_num=0;
  int is_single_line=0;

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
         is_single_line = handle_line(buffer, trie_unique, trie_repeat, has_N);
         unique_read_num = unique_read_num + is_single_line;
        line_number+=1;
      } while(c != EOF);
      fclose(f);
  }
  else {
    fprintf(stderr, "Error: couldnt open file %s \n",reads_file_path);
  }
  free(buffer);
  hattrie_free(trie_repeat);
  results[0] = unique_read_num;
  results[1]= size;

}


//gets a fastq file and prints it
void load_fastq_to_trie_and_print(char* file_path, char* label){
 FILE* f;
  int size = 50; //size of read, default value=80 and increases in case of need
  int  pos; //index to buffer
  int c; //reading character
  long long line_number=1;
  long long read_num =0;
  long long old_repeat_num=0;
  value_t* m_key;
  int len;
  char *buffer = (char *)malloc(size);
  long long cur_line_num = 1;
  hattrie_t* trie_reads;
  printf("opening file to read \n");
  f = fopen(file_path, "r");
  printf("finished opening file \n");
  if(f){
      trie_reads = hattrie_create();
      do { // read all lines in file
        pos = 0;
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
        if (line_number%2==0) { //data is only in even lines
          read_num+=1;
          len = strlen(buffer);
          m_key = hattrie_tryget(trie_reads, buffer, len);
          if(m_key==NULL){ //if not in trie insert it with value 1
              m_key = hattrie_get(trie_reads, buffer, len);
              *m_key=1;
          }
          else{//if it's already inside, increase it's value by 1
            old_repeat_num = *m_key;
            *m_key = old_repeat_num+1;
          }
        }
        line_number+=1;
      } while(c != EOF);
      fclose(f);
  }
  else {
    fprintf(stderr, "Error: couldnt open file %s \n",file_path);
  }
  free(buffer);

  hattrie_iteration(trie_reads, "reprinted_trie", label, 1);
  hattrie_free(trie_reads);
}



void test_make_unique_and_repeat_files(char * read_file_path, char* label) {
    int read_size=0; //the size of the reads
    int table_factor; //arbitrary
    long long num_of_reads;
    long long bf_table_size = table_factor*num_of_reads;
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
    make_repeat_and_unique_tries(read_file_path, trie_unique, trie_repeat, NULL);
    hattrie_iteration(trie_unique, "unique", label, 0);
    hattrie_iteration(trie_repeat, "repeat", label, 1);
      printf("done making unique and repat tries, about to free unique\n");
      if (MEM_CHECK){
        sleep(20);
       system("date");
        system("smem");
        sleep(1);
        system("smem");
      }

    hattrie_free(trie_unique);
      printf("after freeing unique before freeing repeat\n");
      if (MEM_CHECK){
        sleep(30);
       system("date");
        system("smem");
        sleep(1);
        system("smem");
      }

    hattrie_free(trie_repeat);
      printf("after repeat\n");
      if (MEM_CHECK){
        sleep(30);
       system("date");
        system("smem");
        sleep(1);
        system("smem");
      }

}



void test_hash_unique_and_make_fp_fn(char * reads_file_path, char* genome_file_path, char* label) {
    int read_size=0; //the size of the reads
    int table_factor; //arbitrary
    long long bf_table_size;
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
    long long line_number=1;
    long long read_num =0;
    char *buffer = (char *)malloc(size);
    long long cur_line_num = 1;
    int has_N; //1 if line contains 'N', '0' otherwise 
    FILE* f;
    hattrie_t* trie_repeat;
    long long unique_read_num=0;
    int is_single_line=0;

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
          is_single_line = handle_line(buffer, trie_unique, trie_repeat, has_N);
          line_number+=1;
          unique_read_num = unique_read_num + is_single_line; 
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
   table_factor = TABLE_FACTOR; //arbitrary
  //  num_of_reads = line_number/2;
    bf_table_size = table_factor*unique_read_num;
    num_of_hash_func = (long long) ceil(table_factor*0.69314);
    printf("table size is %lld number of hash func is %d \n",bf_table_size,num_of_hash_func); 
//hashing uniqe reads trie using bloom filter
    bf_unique = bloom_filter_new(bf_table_size, string_hash, num_of_hash_func);
    hash_trie_into_bf(trie_unique, bf_unique);
//create trie for all of the sliding windows in the genome reference which are in the unique reads according to the bf_unique
    read_size = size-2;
    trie_genome_unique = hattrie_create();
    query_bf_with_genome(bf_unique, genome_f ,trie_genome_unique, read_size);
    fclose(genome_f);
    printf("printing genome mapped unique reads, true and FP \n");
    hattrie_iteration(trie_genome_unique, "genome_unique_all", label,0);
    trie_fp = hattrie_create();
    trie_fn = hattrie_create();
    trie_genome_true = hattrie_create();     //reads that really maps to the genome

    printf("start checking for false positive \n");
    check_fp(trie_unique,trie_genome_unique, trie_fp, trie_genome_true);
    hattrie_iteration(trie_fp, "fp_1", label,0);

    printf("printing genome mapped unique reads, ture true values only \n");
    hattrie_iteration(trie_genome_true, "genome_unique_true", label, 0);

    printf("start checking for false negative \n");
    check_fn(trie_unique,trie_genome_true, trie_fn);
    hattrie_iteration(trie_fn, "fn_unique", label,0);
    printf("printing bloom filter\n");
    print_bf(bf_unique, bf_table_size,num_of_hash_func, label, "bf_1");
    

    bloom_filter_free(bf_unique);
    hattrie_free(trie_unique);
    hattrie_free(trie_genome_unique);
    hattrie_free(trie_genome_true);
    hattrie_free(trie_fn);
    hattrie_free(trie_fp);

}

//this function get a read file path, genome file path, and a label
//encodes the file, which create a repeat_file, bloom-filter file , false negatives file, false positives file
void test_encode(char * read_file_path, char* genome_file_path, char* label) {
    int read_size=0; //the size of the reads
    int table_factor; //arbitrary
    long long num_of_reads;
    long long bf_table_size = table_factor*num_of_reads;
    int num_of_hash_func;
    int results[2];
    int size=0;
    long long read_num=0;
    FILE* genome_f;
    BloomFilter* bf_unique; //BF for the unique tries
  //  char* output_label=(char *)malloc(50); //label name for the output files
    hattrie_t* trie_unique; //hattrie that holds the unique reads
    hattrie_t* trie_repeat; //hattrie that holds the repetetive reads, and the one that has N inside of them.
    hattrie_t* trie_fp; //triw that holds false negatives set
    hattrie_t* trie_fn;//trie that holds false positives set

///////////////
//part 1
//////////////    
    trie_repeat = hattrie_create();
    trie_unique = hattrie_create();

    make_repeat_and_unique_tries(read_file_path, trie_unique, trie_repeat, results);
//    hattrie_iteration(trie_unique, "unique", label);
    hattrie_iteration(trie_repeat, "repeat", label, 1);

    hattrie_free(trie_repeat);
///////////////
//part 2
//////////////
    read_num = results[0];
    size = results[1];
    read_size = size-2;
    genome_f = fopen(genome_file_path, "r");
    table_factor = 10; //arbitrary
    bf_table_size = table_factor*read_num;
    num_of_hash_func = (long long) ceil(table_factor*0.69314);
    printf("table size is %lld number of hash func is %d \n",bf_table_size,num_of_hash_func);
   
  
    bf_unique = bloom_filter_new(bf_table_size, string_hash, num_of_hash_func);
    trie_fp = hattrie_create();
    trie_fn = hattrie_create();

    encode(trie_unique, genome_f, bf_unique ,read_size, label, bf_table_size,num_of_hash_func, NULL);
     
    fclose(genome_f);
    bloom_filter_free(bf_unique);
    hattrie_free(trie_unique);
    hattrie_free(trie_fn);
}

void test_decode(int read_size,char* genome_file_path, char* directory, char* label) {
  char bf_path[1024]="";
  char repeat_file_path[1024]="";
  char fn_file_path[1024]="";
  char fp_file_path[1024]="";
  make_path(repeat_file_path,directory, label, "repeat"); 
  make_path(fn_file_path,directory, label, "fn_unique");
  make_path(fp_file_path,directory, label, "fp_1");
  make_path(bf_path,directory, label, "bf_1");
//  decode(bf_path, repeat_file_path, genome_file_path, fn_file_path, fp_file_path, read_size, label);
  printf("done test_decode \n");
}

///////////////////////////////////////////
//test_decode_partial -for debug
///////////////////////////////////////////
///gets a bloom filter with the test data,
//prints bllom filter
//////////////////////////////
void test_decode_partial(char* bf_path) {
  BloomFilter* bf;
  hattrie_t* trie_repeat;
  hattrie_t* trie_orig;
  long long table_size=0;
  int num_of_hash=0;
  int bf_results[2];


  trie_repeat = hattrie_create();
  trie_orig = hattrie_create();
  load_bf(bf_path, &bf, bf_results);
  table_size = bf_results[0];
  num_of_hash = bf_results[1];
  print_bf(bf, table_size,num_of_hash, "test_bf", "bf_1");
  bloom_filter_free(bf);
  load_file_to_trie("../data/test.fasta", trie_orig);
  load_file_to_trie("test_1_repeat.txt", trie_repeat);
  load_file_to_trie("test_3_fn_unique.txt", trie_repeat);
  hattrie_iteration(trie_orig, "orig_load", "test_bf",1);
  hattrie_iteration(trie_repeat, "repeat_load", "test_bf",1);

  hattrie_free(trie_orig);
  hattrie_free(trie_repeat);
}



////////////////////
//test_encode_decode
///////////////////
//this function get a read file path, genome file path, and a label
//encodes the file, which create a repeat_file, bloom-filter file , false negatives file, false positives file like in encoding test
// and then decodes it which results in decoded file
void test_encode_decode(char * read_file_path, char* genome_file_path, char* label, int with_zip, int with_cascade) {
  int read_size=100; //the size of the reads //TODO, make a read size funtion
  encode_file(read_file_path, genome_file_path, label, with_zip, with_cascade);
  decode_file(genome_file_path, label, with_zip, with_cascade);
}


//////////////////
//test test_load_file_to_trie_and_print
/////////////////////////
void test_load_file_to_trie_and_print(char* file_path, char* file_name){
load_fastq_to_trie_and_print(file_path, file_name);
printf("done test_load_to_trie_and_print test \n");
}



void test_cascade_encode(char* label){
    hattrie_t* trie_unique;
    long long number_of_fp_reads=52012448;
    char* unique_path="hg19.c0.1.beast.newv_memory_4_decoded_file.txt";
    printf("start cascading with number of intial false positive reads are %lld\n", number_of_fp_reads);
    trie_unique=hattrie_create();
    load_file_to_trie(unique_path ,trie_unique);
    printf("done load file to tore \n");
    cascade_fp(trie_unique, label, &number_of_fp_reads);
}
//////////
//just for debugremove once done
//////////////////////
void test_stuff(char* label){
  hattrie_t* trie_fp;
  hattrie_t* trie_cp;

    long long count = 0;
    long long total_count =0;
    value_t* value;
    value_t  v;
    long long repeat_number;
    long long len;
    char* check_key; //current read from trie_to_check to see if in trie_true
    char* m_key; // result of key when checking if trie_to_check is in trie_true
    char* push_key;
    char* to_check_true_key;
    hattrie_iter_t* i;
    char fp_file_path[1024]="";
    char directory[1024]=".";
    long long array_size = 0;
    long long table_size=3011655520;
    long long bf_table_size=3011655520;
    int num_of_hash_func=7;
    char* bf_array;
    BloomFilter* bf_unique;
    hattrie_t* trie_unique;
  system("free");
  printf("loading unique trie \n");
     trie_unique=hattrie_create();
//  printf("this is the trie %d\n", trie_unique);
     load_file_to_trie("/vol/scratch/yaronein/hg19.c10.fasta", trie_unique);
  printf("done loading unique trie \n");
//hattrie_iteration(trie_unique, "stuff", "stuff", 1);

    if (MEM_CHECK){
      sleep(20);
    system("date");
      system("smem");
      system("free");
      sleep(1);
      system("smem");
     }
  printf("freeing unique trie \n");
  hattrie_free(trie_unique);
  printf("after freeing unique trie \n");
    if (MEM_CHECK){
      sleep(30);
    system("date");
      system("smem");
      system("free");
      sleep(2);
      system("smem");
     }
 // printf("this is the trie %d\n", trie_unique);
  //printf("iterating\n");
//  hattrie_iteration(trie_unique, "stuff", "stuff", 1);
//  printf("done iterating\n");



//         hash_trie_into_bf(trie_unique, bf);

//bf_table_size=10000;
//table_size=10000;
//    array_size = (table_size + 7) / 8;
//    bf_unique = bloom_filter_new(bf_table_size, string_hash, num_of_hash_func);
//
//  printf("before mallocing bf array\n");
//    system("date");
//  system("smem");
////  bf_array = (char *)malloc(sizeof(char)*array_size);
//  printf("after mallocing bf array\n");
//  system("smem");
//  printf("doing print bf\n");
//  print_bf(bf_unique, bf_table_size,num_of_hash_func, label, "unique_bf");
//  printf("done doing print bf\n");
//    system("date");
//  system("smem");
//
//
//  make_path(fp_file_path,directory, label, "fp_unique");
//  trie_fp = hattrie_create();
//  trie_cp = hattrie_create();
// 
//
//  printf("start test\n");
//  system("smem");
//
//  printf("loading fp file to trie \n");
//  load_file_to_trie(fp_file_path, trie_fp);
//  printf("done lading fp\n");
//  system("date");  
//  system("smem");
//  printf("start iterating fp an copy to fp\n");
//
//  i= hattrie_iter_begin(trie_fp, false);
//while (!hattrie_iter_finished(i)) {
//        total_count++;
//        check_key = hattrie_iter_key(i, &len);
//        m_key = hattrie_tryget(trie_cp, check_key, len);
//        if(m_key==NULL){ //if not in trie_true - means false positive, then insert to FP trie, and also remove it from 
//          count++;
//          push_key = hattrie_get(trie_cp, check_key, len);
//          *push_key=1;
//        }
//        if(check_key==push_key){
//           printf("check_key=push_key\n");
//        }
//        if(&check_key==&push_key){
//           printf("ponit-check_key=pointpush_key\n");
//         }
//
//
//        hattrie_iter_next(i);
//        hattrie_del(trie_fp, check_key,len);
//    }
//  printf("done iterating\n");
//    system("date");
//  system("smem");
//    printf("we had %lld false_positives out of %lld reads \n", count, total_count);
//    hattrie_iter_free(i);
//    count=0;
//  system("sleep(20)");
//  system("smem");
//  system("sleep(1)");
//  system("smem");
//
//   printf("freein trie_cp\n");
//     hattrie_free(trie_cp);
//  system("sleep(20)");
//   system("smem");
//  system("sleep(1)");
//  system("smem");
//
//  printf("freeing trie fp and done\n");
//  hattrie_free(trie_fp);
//  system("sleep(20)");
//  system("smem");
//  system("sleep(1)");
//  system("smem");


}







//arg1 = number of test
//arg2 = label
//arg3 = reads file
//arg4 = genome reference file
//nohup /usr/bin/time -v ./test_barcode 1 test_1 ../data/test.fasta ../data/hg19_samp.fa
int main(int argc, char *argv[]) {
  char file_path[1024]="";
  char file_path2[1024]="";
  char* test_number=NULL;
  long long num_of_reads=0;
  test_number = argv[1];
  if (strcmp(test_number, "1") ==0) {
    system("date");
    system("smem");
    printf("doing test 1 \n");
    test_make_unique_and_repeat_files(argv[3], argv[2]);
    printf("done test 1");
      if (MEM_CHECK){
        sleep(30);
       system("date");
        system("smem");
        sleep(1);
        system("smem");
      }

  }

  if (strcmp(test_number, "2") ==0) {
    printf("doing test 2 \n");
    test_hash_unique_and_make_fp_fn(argv[3], argv[4], argv[2]);
    printf("done test 2");

  }
/////////////
//test 3- encoing
/////////////////
// ./test_barcode 3 test_3 ../data/test.fasta ../data/hg19_samp.fa
  if (strcmp(test_number, "3") ==0) {
    printf("doing test 3 \n");
    test_encode(argv[3], argv[4], argv[2]);
    printf("done test 3");

  }

  if (strcmp(test_number, "4") ==0) {
    printf("doing test 4 \n");
    test_decode_partial(argv[2]);
    printf("done test 4");

  }
/////////////
//test 5- deencoing
/////////////////
//arg1 test number
//arg2 = label
//arg3 = direory of files 
//arg4 = referenc genome
//arg5 = number of reads
// ./test_barcode 5 test_3 . ../data/hg19_samp.fa 100
  if (strcmp(test_number, "5") ==0) {
    printf("doing test 5 of decoding \n");
    if (sscanf (argv[5], "%lld", &num_of_reads)!=1) { printf ("error - argument 5 not an integer"); }
    test_decode(num_of_reads,argv[4], argv[3], argv[2]);
    printf("done test 5");

  }

//arg1 test number
//arg2 = path offile to reprint
//are3 = file name


  if (strcmp(test_number, "6") ==0) {
    printf("doing test 6 of loading file to trie and reprint it \n");
    test_load_file_to_trie_and_print(argv[2], argv[3]);
    printf("done test 6");

  }

//arg1 = number of test
//arg2 = label
//arg3 = reads file
//arg4 = genome reference file

  if (strcmp(test_number, "7") ==0) {
    printf("doing test 7 of encoding file and decoding it \n");
    test_encode_decode(argv[3], argv[4], argv[2], 0, 0);
    printf("done test 7");
  }


//arg1 = number of test
//arg2 = label
//arg3 = reads file
// ./test_barcode 8 test_8 ../data/test.fasta
//zip test- will make repeat and unique files from fasta file, and zip them
  if (strcmp(test_number, "8") ==0) {
    printf("doing test 8 \n");
    test_make_unique_and_repeat_files(argv[3], argv[2]);
    strcat(file_path, argv[2]);
    strcat(file_path, "_repeat.txt");
    printf("doing zip \n");
    zip(file_path, argv[2],file_path);
    memset(&file_path[0], 0, sizeof(file_path));
    strcat(file_path, argv[2]);
    strcat(file_path, "_unique.txt");
    printf("doing zip unique \n");
    zip(file_path, argv[2],file_path);
    printf("done test 8");

  }
//arg1 = number of test
//arg2 = label of archvie
//unzip test

  if (strcmp(test_number, "9") ==0) {
    printf("doing test 9 \n");
    strcat(file_path, argv[2]);
    strcat(file_path, "_repeat.txt");
    printf("doing unzip repeat \n");
    unzip(argv[2],file_path);
    memset(&file_path[0], 0, sizeof(file_path));
    strcat(file_path, argv[2]);
    strcat(file_path, "_unique.txt");
    printf("doing zip unique \n");
    unzip( argv[2],file_path);
    printf("done test 9");

  }

///////////////////////////////////
//test 10- encoding +zipping files
//arg1 = number of test
//arg2 = label
//arg3 = reads file
//arg4 = genome reference file
// ./test_barcode 10 test_10  ../data/test.fasta  ../data/hg19_samp.fa
///////////////////////////////////
  if (strcmp(test_number, "10") ==0) {
    printf("doing test 10 of encoding and zipping files \n");
    test_encode(argv[3], argv[4], argv[2]);
    zip_encoded_files(argv[2]);
    printf("done test 10");

  }

/////////////
//test 11-unizip archive + dencoding
/////////////////
//arg1 test number
//arg2 = label (of archive )
//arg3 = direory of files 
//arg4 = referenc genome
//arg5 = length of reads
// ./test_barcode 11 test_10 . ../data/hg19_samp.fa 100
  if (strcmp(test_number, "11") ==0) {
    printf("doing test 5 of unzip+ decoding \n");
    if (sscanf (argv[5], "%lld", &num_of_reads)!=1) { printf ("error - argument 5 not an integer"); }
    printf("unzippin \n");
    unzip_encoded_files(argv[2]);
    printf("decoding \n");
    test_decode(num_of_reads,argv[4], argv[3], argv[2]);
    printf("done test 5");

  }


//arg1 = number of test
//arg2 = label
//arg3 = reads file
//arg4 = genome reference file
/// ./test_barcode 12 teting_with_zip ../data/test.fasta  ../data/hg19_samp.fa
  if (strcmp(test_number, "12") ==0) {
    printf("doing test 12 of encoding file and decoding it and zipping \n");
    test_encode_decode(argv[3], argv[4], argv[2], 1, 0);
    printf("done test 12");
  }

/////
//debug test
//arg1 = number of test
//arg2 = label
////////////////////////
  if (strcmp(test_number, "13") ==0) {
    printf("doing test 13 of checking memory \n");
    test_stuff( argv[2]);
    printf("done test 13");
    if (MEM_CHECK){
      sleep(30);
    system("date");
      system("smem");
      sleep(2);
      system("smem");
     }

  }


//arg1 = number of test
//arg2 = label
//arg3 = reads file
//arg4 = genome reference file
/// ./test_barcode 14 testing_cascade_without_zip ../data/fasta_input/hg19.100000.fasta  ../data/hg19_samp.fa
  if (strcmp(test_number, "14") ==0) {
    printf("doing test 14 of cascade encoding file and decoding it without ziipng \n");
    test_encode_decode(argv[3], argv[4], argv[2], 0, 1);
    printf("done test 14 of encode decode with cascade and no zip");
  }

//arg1 = number of test
//arg2 = label
/// ./test_barcode 15 testing_cascade_encode_small ../data/test.fasta  ../data/hg19_samp.fa
  if (strcmp(test_number, "15") ==0) {
    printf("doing test 15 of cascade encoding file \n");
    test_cascade_encode(argv[2]);
    printf("done test 15 of cascade encode");
  }


  return 0;
}
