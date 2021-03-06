#include "barcode.h"

int VERBOSE_MODE; //gloabal variable instead of deifne to avoid linker issues
int KEEP_FILES=0; //global variable to keep unnecessary files. 1 to keep, 0 to remove

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
  char ref_genome[2048]="";
  char prefix[2048]="";
  char input_file[2048]="";
  
  if (argv[2]!=NULL){
    if (strcmp(argv[2], "-v")==0){
      printf("verbose mode\n");
      VERBOSE_MODE=1;
      if (argv[3]!=NULL){
        sprintf(prefix,"%s", argv[3]);
      }
      if (argv[4]!=NULL){
        sprintf(ref_genome,"%s", argv[4]);
     }
      if (argv[5]!=NULL){
        sprintf(input_file,"%s", argv[5]);
      }
    }
    else {
      printf("no verbose mode\n");
      VERBOSE_MODE=0;
      if (argv[2]!=NULL){
        sprintf(prefix, "%s", argv[2]);
      }
      if (argv[3]!=NULL){
        sprintf(ref_genome, "%s", argv[3]);
      }
      if (argv[4]!=NULL){
        sprintf(input_file,"%s", argv[4]);
      }
    }
  }
/////////////
//Help
////////////
  if ((test_number==NULL) || (strcmp(test_number, "-h") ==0)){
    printf("\nProgram: barcode\n"
           "Contact: royerozov@tau.ac.il\n\n"
           "Usage:   barcode  <command> [options]\n\n"
           "Encode command: barcode encode [-v] <prefix> <reference genome> <in.file>\n"
           "Decode command: barcode decode [-v] <prefix> <reference genome>\n\n"
           "Options: -v\tverbose mode, minimal screen output\n\n");
           return(0);
  }

  else if ((strcmp(test_number, "encode") ==0) && (argv[2]==NULL)){
    printf("\nUsage: barcode encode [-v] <prefix> <reference genome> <in.file>\n\n"
           "Options: -v\tverbose mode, minimal screen output\n\n");
           return(0);
     }
 
  else if ((strcmp(test_number, "decode") ==0) && (argv[2]==NULL)){
    printf("\nUsage: barcode decode [-v] <prefix> <reference genome>\n\n"
           "Options: -v\tverbose mode, minimal screen output\n\n");
           return(0);
  }



/////////////
//test 1- encoding
/////////////////
// ./test_barcode encode test_encode ../data/test.fasta ../data/hg19_samp.fa
  else if (strcmp(test_number, "encode") ==0) {
    printf("doing encode \n");
    strcat(file_path, argv[2]);
    strcat(file_path, "_stderr.txt");
    stderr = fopen (file_path, "w");
    test_encode(input_file, ref_genome, prefix, 1, 1);
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
  else if (strcmp(test_number, "decode") ==0) {
    printf("doing test 5 of decoding \n");
    strcat(file_path, argv[2]);
    strcat(file_path, "_stderr.txt");
    stderr = fopen (file_path, "w");
    test_decode(ref_genome, prefix, 1, 1);
    printf("done decode");
    fclose(stderr);
  }


//doing encode+decode with zip and with cascade
/// ./test_barcode 15 testing_cascade_with_zip ../data/fasta_input/hg19.100000.fasta  ../data/hg19_samp.fa
  else if (strcmp(test_number, "15") ==0) {
    printf("doing test 15 of cascade encoding file and decoding it with ziipng \n");
    test_encode_decode(input_file, ref_genome, prefix, 1, 1);
    printf("done test 14 of encode decode with cascade with zip");
  }
  
  else{
    printf("Error!\n"
           "Usage:   barcode  <command> [options]\n"
           "Encode command: barcode encode [-v] <prefix> <in.file> <reference genome>\n"
           "Decode command: barcode decode [-v] <prefix> <reference genome>\n\n"
           "Options: -v\tverbose mode, minimal screen output\n\n");

  }
  return 0;
}
