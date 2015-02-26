#include "barcode.h"


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

//this function get a read file path, genome file path, and a label
//encodes the file, which create a repeat_file, bloom-filter file , false negatives file, false positives file
void test_encode(char * read_file_path, char* genome_file_path, char* label, int with_zip, int with_cascade) {
  encode_file(read_file_path, genome_file_path, label, with_zip, with_cascade);
}

////////////////////
//testdecode
///////////////////
//this function get a read file path, genome file path, and a label
// and then decodes it which results in decoded file
void test_decode(char* genome_file_path, char* label, int with_zip, int with_cascade) {
  decode_file(genome_file_path, label, with_zip, with_cascade);
}


////////////////////
//test_encode_decode
///////////////////
//this function get a read file path, genome file path, and a label
//encodes the file, which create a repeat_file, bloom-filter file , false negatives file, false positives file like in encoding test
// and then decodes it which results in decoded file
void test_encode_decode(char * read_file_path, char* genome_file_path, char* label, int with_zip, int with_cascade) {
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

//////////
//just for debugremove once done
//////////////////////
void test_stuff(char * read_file_path, char* genome_file_path, char* label){
    int read_size=0; //the size of the reads
    int table_factor; //arbitrary
    long long num_of_reads;
    long long bf_table_size = table_factor*num_of_reads;
    unsigned int num_of_hash_func;
    long long results[2];
    int size=0;
    int number_of_cascades=4; //TODO change
    long long read_num=0;
    long long number_of_fp_reads; // number of false positive redads
    FILE* genome_f;
    BloomFilter* bf_unique; //BF for
    hattrie_t* trie_unique; //hattrie that holds the unique reads
    hattrie_t* trie_repeat; //hattrie that holds the repetetive reads, and the one that has N inside of them.
 


    sleep(20);
    system("date");
    system("smem");
    sleep(1);
    system("smem");
    trie_repeat = hattrie_create();
    trie_unique = hattrie_create();
    printf("make unique and repeat tries\n");

    if (make_repeat_and_unique_tries(read_file_path, trie_unique, trie_repeat, results)){
      printf("done making unique and repat tries\n");
        sleep(20);
       system("date");
        system("smem");
        sleep(1);
        system("smem");
    }
   printf("freein repaet trie\n");
         hattrie_free(trie_repeat);

        sleep(20);
       system("date");
        system("smem");
        sleep(1);
        system("smem");
   printf("freein unique trie\n");
         hattrie_free(trie_unique);

        sleep(20);
       system("date");
        system("smem");
        sleep(1);
        system("smem");

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


/////////////
//Help
////////////
  if ((test_number==NULL) || (strcmp(test_number, "-h") ==0)){
    printf("--help\n To compress file, it needs three arguments,arbitrary  name-prefix, the file to compress, and the reference genome. The command should look like:\n ./barcode encode CompressedFilePrefix FileToCompress ReferenceGenomeFile\n This should results with new compreesd file CompressedFilePrefix.7z\n To uncompress file it needs two arguments, the compressed file prefix (without .7z), and the reference genome file. The command should like:\n ./barcode decode CompressedFilePrefix ReferenceGenomeFile\n");
  return(0);
  }




/////////////
//test 1- encoding
/////////////////
// ./test_barcode encode test_encode ../data/test.fasta ../data/hg19_samp.fa
  if (strcmp(test_number, "encode") ==0) {
    printf("doing encode \n");
    strcat(file_path, argv[2]);
    strcat(file_path, "_stderr.txt");
    stderr = fopen (file_path, "w");
    test_encode(argv[3], argv[4], argv[2], 1, 1);
    printf("done encode");
    fclose(stderr);
  }




/////////////
//test 5- deencoing
/////////////////
//arg1 test name
//arg2 = achive_label
//arg3 = referenc genome
// ./test_barcode 5 test_3 . ../data/hg19_samp.fa 100
  if (strcmp(test_number, "decode") ==0) {
    printf("doing test 5 of decoding \n");
    strcat(file_path, argv[2]);
    strcat(file_path, "_stderr.txt");
    stderr = fopen (file_path, "w");
    test_decode(argv[3], argv[2], 1, 1);
    printf("done decode");
    fclose(stderr);
  }

//arg1 test number
//arg2 = path offile to reprint
//are3 = file name


  if (strcmp(test_number, "6") ==0) {
    printf("doing test 6 of loading file to trie and reprint it \n");
    test_load_file_to_trie_and_print(argv[2], argv[3]);
    printf("done test 6");

  }

//doing encode+decode with zip and with cascade
/// ./test_barcode 15 testing_cascade_with_zip ../data/fasta_input/hg19.100000.fasta  ../data/hg19_samp.fa
  if (strcmp(test_number, "15") ==0) {
    printf("doing test 15 of cascade encoding file and decoding it with ziipng \n");
    test_encode_decode(argv[3], argv[4], argv[2], 1, 1);
    printf("done test 14 of encode decode with cascade with zip");
  }



//this for testing: doing encode and decode without zip and without cascade
//arg1 = number of test
//arg2 = label
//arg3 = reads file
//arg4 = genome reference file
  if (strcmp(test_number, "7") ==0) {
    strcat(file_path, argv[2]);
    strcat(file_path, "_stderr.txt");
    stderr = fopen (file_path, "w");
    printf("doing test 7 of encoding file and decoding it \n");
    test_encode_decode(argv[3], argv[4], argv[2], 0, 0);
    printf("done test 7");
    fclose(stderr);
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
        printf("doing encode \n");
    strcat(file_path, argv[2]);
    test_stuff(argv[3], argv[4], argv[2]);
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

  return 0;
}
