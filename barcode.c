#include "barcode.h"

#include <stdlib.h>
//#include <Python.h>
//
//double get_f_val_tmp(numreads, genome_len, read_len)
//{
//   // Set PYTHONPATH TO working directory
//   setenv("PYTHONPATH",".",1);
//
//   PyObject *pName, *pModule, *pDict, *pFunc, *pValue, *presult;
//
//
//   // Initialize the Python Interpreter
//   Py_Initialize();
//
//
//   // Build the name object
//   pName = PyString_FromString((char*)"calc_f"); //the name of the .py file
//
//   // Load the module object
//   pModule = PyImport_Import(pName);
//
//
//   // pDict is a borrowed reference 
//   pDict = PyModule_GetDict(pModule);
//
//
//   // pFunc is also a borrowed reference 
//   pFunc = PyDict_GetItemString(pDict, (char*)"get_f");
//
//   if (PyCallable_Check(pFunc))
//   {
//       pValue=Py_BuildValue("(z)",(char*)"something");
//       PyErr_Print();
//       printf("Let's give this a shot!\n");
//       presult=PyObject_CallObject(pFunc,pValue);
//       PyErr_Print();
//   } else 
//   {
//       PyErr_Print();
//   }
//   printf("Result is %d\n",PyInt_AsLong(presult));
//   Py_DECREF(pValue);
//
//   // Clean up
//   Py_DECREF(pModule);
//   Py_DECREF(pName);
//
//   // Finish the Python Interpreter
//   Py_Finalize();
//
//
//    return(pValue);
//}


int make_scalce_file(char* file_name, char* new_file_name, int read_len){
  char command[2048]="";
  char forth_line[2048]="";
  char third_forth_lines[2048]="+\n";
  char first_line[2048]="@1\n";
  char one_char[15]="1";
  long long line_num=1;
  FILE* f_new;
  char c;
  FILE* f;
  int i;
  int pos;
  char *buffer = (char *)malloc(read_len+2);
  printf("opening %s and creating new fastq format file %s \n", file_name, new_file_name);
  f = fopen(file_name, "r");
  f_new = fopen(new_file_name, "w");

  for(i=0; i<read_len; i++){
    strcat(forth_line, one_char);
  }
  printf("done for");
  strcat(forth_line,"\n");
  strcat(third_forth_lines,forth_line);
  if(f){
      do { // read all lines in file
        pos=0;
       // fprintf(f_new,"%s",first_line);
        do{
          c = fgetc(f);
           // fprintf(f_new,"%c",c);
           buffer[pos++] = c;
        }while(c != EOF && c != '\n');
        if(c!=EOF){
          buffer[pos]=0;
          fprintf(f_new,"%s",first_line);
          fprintf(f_new,"%s",buffer);
          fprintf(f_new,"%s",third_forth_lines);
       }
      }while(c != EOF);
   }
  fclose(f);
  fclose(f_new);
  free(buffer);
  printf("done\n");
}



int rm_scalce_files(char* prefix_name){
  char file_name[1024]="";

  strcat(file_name, prefix_name);
  strcat(file_name, "_1.scalcen");
  unlink(file_name);
  memset(&file_name[0], 0, sizeof(file_name));
  strcat(file_name, prefix_name);
  strcat(file_name, ".txt");
  unlink(file_name);

  memset(&file_name[0], 0, sizeof(file_name));
  strcat(file_name, prefix_name);
  strcat(file_name, "_fastq.txt");
  unlink(file_name);
}
///////////////////
//scalce_compress
//////////////////////
//this function get the prefix of the files, and the reads length,
//make fake fastq file and then compress this fake fastq with scalce
int scalce_compress_files(char* prefix_name, int read_len, int number_of_cascades)
{
  char file_name[1024]="";
  char new_file_name[1024]="";
  char comp_file_name[1024]="";
  char str[15];
  char command[1024]="";
  char buffer[2048]="";


  strcat(file_name, prefix_name);
  strcat(file_name, "_repeat.txt");
  strcat(new_file_name, prefix_name);
  strcat(new_file_name, "_repeat_fastq.txt");
  printf("doing scale for %s \n", file_name);
  make_scalce_file(file_name, new_file_name, read_len);
  strcat(comp_file_name, prefix_name);
  strcat(comp_file_name, "_repeat");
  strcat(command, "scalce-bin ");
  strcat(command, new_file_name);
  strcat(command, " -A -T 1 -n library -o ");
  strcat(command, comp_file_name);
  printf("executing the next command %s \n", command);
  snprintf(buffer, sizeof(buffer), "%s", command);
  system(buffer);
  rm_scalce_files(comp_file_name);

  memset(&new_file_name[0], 0, sizeof(new_file_name));
  memset(&file_name[0], 0, sizeof(file_name));
  memset(&comp_file_name[0], 0, sizeof(comp_file_name));
  memset(&buffer[0], 0, sizeof(buffer));
  memset(&command[0], 0, sizeof(command));
  strcat(file_name, prefix_name);
  strcat(file_name, "_fn_unique.txt");
  strcat(new_file_name, prefix_name);
  strcat(new_file_name, "_fn_unique_fastq.txt");
  make_scalce_file(file_name, new_file_name,read_len);
  strcat(comp_file_name, prefix_name);
  strcat(comp_file_name, "_fn_unique");
  strcat(command, "scalce-bin ");
  strcat(command, new_file_name);
  strcat(command, " -A -T 1 -n library -o ");
  strcat(command, comp_file_name);
  printf("executing the next command %s \n", command);
  snprintf(buffer, sizeof(buffer), "%s", command);
  system(buffer);
  rm_scalce_files(comp_file_name);



  memset(&file_name[0], 0, sizeof(file_name));
  memset(&new_file_name[0], 0, sizeof(new_file_name));
  memset(&comp_file_name[0], 0, sizeof(comp_file_name));
  memset(&buffer[0], 0, sizeof(buffer));
  memset(&command[0], 0, sizeof(command));
  strcat(file_name, prefix_name);
  strcat(file_name, "_fp_");
  sprintf(str, "%d", number_of_cascades);
  strcat(file_name, str);
  strcat(file_name,".txt");
  strcat(new_file_name, prefix_name);
  strcat(new_file_name, "_fp_");
  strcat(new_file_name, str);
  strcat(new_file_name,"_fastq.txt");
  make_scalce_file(file_name, new_file_name, read_len);
  strcat(comp_file_name, prefix_name);
  strcat(comp_file_name, "_fp_");
  strcat(comp_file_name, str);
  strcat(command, "scalce-bin ");
  strcat(command, new_file_name);
  strcat(command, " -A -T 1 -n library -o ");
  strcat(command, comp_file_name);
  printf("executing the next command %s \n", command);
  snprintf(buffer, sizeof(buffer), "%s", command);
  system(buffer);
  rm_scalce_files(comp_file_name);

}

int take_reads_from_fastq_file(char* decomp_file_name, char* new_file_name, int read_len){
  char command[2048]="";
  long long line_num=1;
  FILE* f_new;
  char c;
  FILE* f;
  int pos;
  long long line_number=1;
  char *buffer = (char *)malloc(read_len+2);
  printf("opening fastq %s and creating new reads file %s \n", decomp_file_name, new_file_name);
  f = fopen(decomp_file_name, "r");
  f_new = fopen(new_file_name, "w");

  if(f){
      do { // read all lines in file
        pos=0;
       // fprintf(f_new,"%s",first_line);
        do{
          c = fgetc(f);
           // fprintf(f_new,"%c",c);
           buffer[pos++] = c;
        }while(c != EOF && c != '\n');
        if(((line_number+2)%4)==0){
          buffer[pos]=0;
          fprintf(f_new,"%s",buffer);
       }
       line_number+=1;
      }while(c != EOF);
   }
  fclose(f);
  fclose(f_new);
  free(buffer);
  printf("done\n");
}



int scalce_decompress_file(char* prefix_name, int read_len){
  char file_name[1024]=""; //compressed ifle name
  char decomp_file_name[1024]=""; //final name
  char new_file_name[1024]=""; //final name
  char str[15];
  char command[1024]="";
  char buffer[2048]="";
  char scalen_file_name[1024]="";

  strcat(file_name, prefix_name);
  strcat(new_file_name, prefix_name);
  strcat(new_file_name,".txt");
  strcat(scalen_file_name,prefix_name);
  strcat(scalen_file_name, "_1.scalcern");
  strcat(file_name, "_1.scalcer");
  strcat(command,"scalce-bin ");
  strcat(command,file_name);
  strcat(command, " -n library -d -o ");
  strcat(command, prefix_name);
  printf("executing the next command %s \n", command);
  snprintf(buffer, sizeof(buffer), "%s", command);
  system(buffer);

  strcat(decomp_file_name, prefix_name);
  strcat(decomp_file_name, "_1.fastq");
  take_reads_from_fastq_file(decomp_file_name, new_file_name, read_len);
  unlink(decomp_file_name);
}

int scalce_decompress_files(char* prefix_name, int read_len, int number_of_cascades){
  char fn_file_name[1024]=""; //compressed ifle name
  char fp_file_name[1024]=""; //compressed ifle name
  char repeat_file_name[1024]=""; //compressed ifle name
  char str[15];
  strcat(fn_file_name, prefix_name);
  strcat(fn_file_name, "_fn_unique");
  strcat(repeat_file_name, prefix_name);
  strcat(repeat_file_name, "_repeat");
  strcat(fp_file_name, prefix_name);
  strcat(fp_file_name, "_fp_");
  sprintf(str, "%d", number_of_cascades);
  strcat(fp_file_name, str);

  scalce_decompress_file(fn_file_name, read_len);
  scalce_decompress_file(repeat_file_name, read_len);
  scalce_decompress_file(fp_file_name, read_len);
}

                                                   
//////////////////
//get_f_val
///////////////////////////////////////
//executuing python script to get f value which prints into  a file, and then opening the file and reading it
///////////////////////////////////////
double get_f_val(long long numreads, long long genome_len, int read_len)
{
   char command[1024]="";
   char read_len_c[1024]="";
   char numreads_c[1024]="";
   char genome_len_c[1024]="";
   char str[128]="";
   char buffer[2048]="";
   char f_path[1028]="./f_val.txt";
   char cCurrentPath[2048];
   double f_value;
   FILE *f_file;
   
   strcat(command, PYTHON);
   strcat(command, " calc_f.py ");
   sprintf(read_len_c, "%d", read_len);
   sprintf(numreads_c, "%lld", numreads);
   sprintf(genome_len_c, "%lld", genome_len);
   strcat(command, numreads_c);
   strcat(command, " ");
   strcat(command, genome_len_c);
   strcat(command, " ");
   strcat(command, read_len_c);

  printf("executing the next command %s \n", command);
  snprintf(buffer, sizeof(buffer), "%s", command);
  system(buffer);

  f_file = fopen(f_path, "r");
  fscanf(f_file, "%lf", &f_value);
  fclose(f_file);
  printf("f values is %lf\n", f_value);

  return(f_value);

}

//***************************
//get_bf_params_
//***************************
//get the optimal table size and number of hash func
void get_bf_params(long long * bf_params, long long numreads, long long genome_len, int read_len){
  double f_val;
  int stop=0;
  int num_of_hush;
  long long table_size;
  f_val=get_f_val(numreads, genome_len, read_len);
  printf("f values is %lf\n", f_val);
  num_of_hush=1; 
  while (stop==0){
    table_size=ceil(-(num_of_hush*numreads)/(log(-(pow(f_val,(1.0/num_of_hush))-1)))); //table_size=m
    printf("for number of hush %d we get table size of %lld\n", num_of_hush, table_size);
    if ((sizeof(char)*((table_size + 7)/8000)<8000000)|| (num_of_hush>64)){
      stop=1;
      bf_params[0]=num_of_hush;
      bf_params[1]=table_size;
      printf("final number of hush func is %lld, table_size is %lld\n", bf_params[0], bf_params[1]);
    }
    else{
     num_of_hush=num_of_hush+1;
    }
  }
}




//************************************************************************************
// copy_file_to_mem function
//************************************************************************************
char* copy_file_to_mem(char* file_path){
  FILE *f = fopen(file_path, "rb");
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET);
  char *memfile = malloc(fsize + 1);
  fread(memfile, fsize, 1, f);
  fclose(f);
  memfile[fsize] = 0;
  return(memfile);
}

//************************************************************************************
//zip_encode_files function will encode repeat, fn,fp,bf files of an encoded file
//if encode_od_decode==0, it's on encode mode so it will zip files and the will remove them
//if its ==1, it's decode mode, so it will only remove the extracted files
//************************************************************************************
int zip_or_remove_encoded_files(char* archive_prefix_name, int number_of_cascades, int encode_or_decode)
{
  char archive_name[1024]=""; //name of the file you want to name your data 
  char file_name[1024]=""; 
  char buffer[2048]="";
  char zip_command[2048]="";
  char rm_command[2048]="";
  char files_list[2048]=""; 
  char str[15]; 
  int i;
  
//  strcat(file_name, archive_prefix_name);
//  strcat(file_name, "_repeat.txt ");
//  strcat(files_list, file_name);
//
//  memset(&file_name[0], 0, sizeof(file_name));
//  strcat(file_name, archive_prefix_name);
//  strcat(file_name, "_fn_unique.txt " );
//  strcat(files_list, file_name);
//
//  memset(&file_name[0], 0, sizeof(file_name));
  strcat(file_name, archive_prefix_name);
  strcat(file_name, "_params.txt " );
  strcat(files_list, file_name);
//
//  memset(&file_name[0], 0, sizeof(file_name));
//  strcat(file_name, archive_prefix_name);
//  strcat(file_name, "_fp_");
//  sprintf(str, "%d", number_of_cascades);
//  strcat(file_name, str);
//  strcat(file_name,".txt ");
//  strcat(files_list, file_name);




  for(i=1; i<(number_of_cascades+1); i++){
    sprintf(str, "%d", i);
    memset(&file_name[0], 0, sizeof(file_name));
    strcat(file_name, archive_prefix_name);
    strcat(file_name, "_bf_");
    strcat(file_name, str);
    strcat(file_name,".txt ");
    strcat(files_list, file_name);
  }
  strcat(archive_name, archive_prefix_name);
  strcat(archive_name,".7z ");
  strcat(zip_command,"7z a ");
  strcat(zip_command, archive_name);
  strcat(zip_command, files_list);
  if(encode_or_decode==0){
    printf("executing zip command %s \n", zip_command);
    fprintf(stderr, "executing zip command %s \n", zip_command);
    snprintf(buffer, sizeof(buffer), "%s", zip_command);
    system(buffer);
  }
  
  memset(&buffer[0], 0, sizeof(buffer));
  strcat(rm_command,"rm ");
  strcat(rm_command, files_list);
  printf("executing rm command %s \n", rm_command);
  fprintf(stderr, "executing zip command %s \n", rm_command);
  snprintf(buffer, sizeof(buffer), "%s", rm_command);
  system(buffer);

}

//************************************************************************************
//unzip_encode_files function will unzip repeat, fn,fp,bf files of an encoded file
//************************************************************************************
int unzip_encoded_files(char* archive_prefix_name)
{
  char archive_name[1024]=""; //name of the file you want to name your data 
  char file_name[1024]="";
  char buffer[1024]="";
  char unzip_command[1024]="";

  strcat(archive_name, archive_prefix_name);
  strcat(archive_name,".7z ");
  strcat(unzip_command,"7z e ");
  strcat(unzip_command, archive_name);
  printf("executing unzip command %s \n", unzip_command);
  fprintf(stderr, "executing zip command %s \n", unzip_command);
  snprintf(buffer, sizeof(buffer), "%s", unzip_command);
  system(buffer);


}









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
int handle_line(char *line, hattrie_t* trie_unique, hattrie_t* trie_repeat, int has_N) {
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
        return 1;
      }
      else{//if it's in uqniqu, take it out from unique and push to repeat with value 2
        if(hattrie_del(trie_unique, line, len)!=0){
          fprintf(stderr, "error deleting node\n");  
          exit(1);
        }
        m_key = hattrie_get(trie_repeat, line, len);
        *m_key=2;
        return -1;

      }
    }
    else{//if it's in repeat 
      old_repeat_num = *m_key;
      m_key = hattrie_get(trie_repeat, line, len);
      *m_key = old_repeat_num+1;
    }
    return 0;
}

//************************************************************************************
//hattrie_iteration function
//************************************************************************************
//this function take a hattrie, and iterate on all of its keys,
//and write it to a file located in  user given output path (cwd if none) and with a label
//the label is the name for the trie
//with_duplicates==1, then it will print each key as the number of its value (with duplicates)
//otherwise only once
//************************************************************************************
int hattrie_iteration(hattrie_t* T, char* m_label, char* output_label, int with_duplicates)
{   
    printf("iterating through keys for %s trie \n", m_label);
    fprintf(stderr, "iterating through keys for %s trie \n", m_label);

    hattrie_iter_t* i = hattrie_iter_begin(T, false);

    long long count = 0;
    value_t* value;
    value_t  v;
    int repeat_number;
    long long len;
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
        if (with_duplicates==1){
          for(repeat_number = 1; repeat_number <= *value; repeat_number++){       
            fprintf(fp, "%s", key); 
          }
        }  
        else {
          fprintf(fp, "%s", key);
        }
        hattrie_iter_next(i);
    }
    hattrie_iter_free(i);
    fclose(fp);
    printf("done iterating.\n");
    fprintf(stderr, "done iterating.\n");

    return(1);
}



//************************************************************************************
//hattrie_iteration function
//************************************************************************************
//this function take a hattrie, and iterate on all of its keys,
//and write it to a file located in  user given output path (cwd if none) and with a label
//the label is the name for the trie
//************************************************************************************
int hattrie_iteration_2(hattrie_t* T, FILE* fp, int with_duplicates)
{
    printf("doing hatrie_iteration_2\n");
    fprintf(stderr, "doing hatrie_iteration_2\n");

    hattrie_iter_t* i = hattrie_iter_begin(T, false);

    long long count = 0;
    value_t* value;
    value_t  v;
    int repeat_number;
    long long len;
    const char* key;

    while (!hattrie_iter_finished(i)) {
        ++count;

        key = hattrie_iter_key(i, &len);
        value   = hattrie_iter_val(i);
        if (with_duplicates==1){
          for(repeat_number = 1; repeat_number <= *value; repeat_number++){
            fprintf(fp, "%s", key);
          }
       } 
       else {
          fprintf(fp, "%s", key);
       }

        hattrie_iter_next(i);
    }
    hattrie_iter_free(i);
    printf("done iterating. iterated of %lld strings\n",count);
    fprintf(stderr, "done iterating. iterated of %lld strings\n",count);

    return(1);
}




void copy_file(FILE* source, FILE* target){
   char ch;
   while( ( ch = fgetc(source) ) != EOF )
      fputc(ch, target);
   printf("File copied successfully.\n");
   fprintf(stderr, "File copied successfully.\n");

}
//************************************************************************************
//make_repeat_and_unique_tries function
//************************************************************************************
//gets a file path conataining reads and two empty tries, to fill them with:
//1.unique trie
//2.repeat trie
//************************************************************************************


int make_repeat_and_unique_tries(char* reads_file_path, hattrie_t* trie_unique, hattrie_t* trie_repeat, long long* results) {
  FILE* f;
  int size = 80; //size of read, default value=80 and increases in case of need
  int  pos; //index to buffer
  int c; //reading character
  long long line_number=1;
  long long read_num =0;
  char *buffer = (char *)malloc(size);
  long long cur_line_num = 1;
  int has_N; //1 if line contains 'N', '0' otherwise
  long long unique_read_num=0;
  int is_single_line=0;

  printf("opening file to read \n");
  fprintf(stderr, "opening file to read \n");
 
  f = fopen(reads_file_path, "r");
  printf("finished opening file \n");
  fprintf(stderr, "finished opening file \n");
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
          is_single_line = handle_line(buffer, trie_unique, trie_repeat, has_N);
          unique_read_num = unique_read_num + is_single_line;
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
  
  if (results!=NULL) {
    results[0] = unique_read_num;
    results[1]= size;
   }
}


//************************************************************************************
//set_complentary_strand
//************************************************************************************
//this funtino gets a read orig_strand, and a pointer to an empty string comp_strand, 
//and set com_strand to the complementry strand of orif_strand
//////////////////////////////////////
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
//this function gets a poiner to a trie with reads, and a pointer to a bf,
//and fill the bf with the trie reads
////////////////////////////////
int hash_trie_into_bf(hattrie_t* T, BloomFilter* bf) {
    long long count = 0;
    value_t* value;
    value_t  v;
    int repeat_number;
    long long len;
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
    return(1);
}


//************************************************************************************
//check_if_trie_in_bf function
//************************************************************************************
//this function is for debug, it gets a trie and a bf,
//if a read in the trie has an accept in the bf, it will print it
////////////////////////////////////////////
void check_if_trie_in_bf(hattrie_t* T, BloomFilter* bf) {
    long long count = 0;
    value_t* value;
    value_t  v;
    int repeat_number;
    long long len;
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
//check_accept_read_is_fp function
//***********************************************************************************
//trie true: holds inside the references reads to check agains(uniuqe reads)
//read- is there read we want to check if it's FP or true accept. the read lenght is len
//if FP we write it to fp_file, else we keep it in trie_true_accept.
int check_accept_read(char* read, hattrie_t* trie_true,hattrie_t* trie_true_accept, FILE* fp_file, long long len) {
   char* m_key; // result of key when checking if trie_to_check is in trie_true
   char* to_check_true_key;
   m_key = hattrie_tryget(trie_true, read, len);
   if(m_key==NULL){ //if not in trie_true - means false positive, then insert to FP trie, and also remove it from 
      fprintf(fp_file, "%s", read);
      return(1);     
   }
   else { //means it is true
     if (trie_true_accept!= NULL){
       to_check_true_key = hattrie_get(trie_true_accept, read, len);
       *to_check_true_key = 1;
       return(0);
     }
   }
}

//************************************************************************************
//cascading_fp_encode function
//************************************************************************************
//this function gets ,refernce reads trie, FP reference trie, a number of where this reference FP is in the cascade, an empty BF,
// and a file_path where to write the new FP against the reference FP
// first it hashes the FP into the BF
//then it goes through the the reference reads, and check if it has an accpet in the BF,
// if it has an accept it will check in the reference FP if it's there, and if it's not it
//will print it to the file (so we'lll have in the new FP in the cascade)

int cascading_fp_encode(hattrie_t* ref_reads_trie, hattrie_t* ref_fp_trie, char* ref_fp_path, BloomFilter* bf_ref_fp, hattrie_t* new_fp_trie, char* new_fp_file_path,long long* num_of_new_fp_reads, int iteration) {
  FILE *fp_file;
//  long long num_of_inserted_reads=0;
  long long total_count=0;
  long long count_in_bf=0;
  int in_bf_comp=0;
  long long len=0;
  char* check_key;
  char* m_key;
  
  hattrie_iter_t* i = hattrie_iter_begin(ref_reads_trie, false);
  printf("start cascading_fp_encode\n");
  fprintf(stderr, "start cascading_fp_encode\n");
  printf("hashing FP number %d to BF number %d\n", iteration-1, iteration);
  fprintf(stderr, "hashing FP number %d to BF number %d\n", iteration-1, iteration);
  if (ref_fp_trie==NULL){  //in the first case where FP file is big, we will load it to BF startight from file
    load_file_to_bf(ref_fp_path, bf_ref_fp);
  printf("done hashing FP file to BF \n");
  fprintf(stderr, "done hashing FP file to BF \n");
  }
  else{ //if not in first iteration, we will load to trie since we gonna use it after anyways
    hash_trie_into_bf(ref_fp_trie, bf_ref_fp); //first hash FP1 to BF2
    printf("done hashing FP trie to BF \n");
    fprintf(stderr, "done hashing FP trie to BF  \n");
  }
  if(new_fp_trie==NULL){
    fp_file=fopen(new_fp_file_path, "w");
  }
  *num_of_new_fp_reads=0;
  while (!hattrie_iter_finished(i)) { //check for each read in the unique reads, if BF2 has accpet, put it in FP2 file
    total_count++;
//    printf("%d\n",total_count);
    check_key = hattrie_iter_key(i, &len);
    in_bf_comp = bloom_filter_query(bf_ref_fp, check_key);
    if (in_bf_comp!=0) { //if the read is accpet- meaning it FP while it's realy a true postive, then it's FP to the FP, we should print it to FP2
      if(new_fp_trie==NULL){ //in this case we will write read straight to trie
        fprintf(fp_file, "%s", check_key);
        *num_of_new_fp_reads=*num_of_new_fp_reads+1;
      }
      else{ //we will load read to trie
         m_key = hattrie_get(new_fp_trie, check_key, strlen(check_key));
         *m_key=1;
         *num_of_new_fp_reads=*num_of_new_fp_reads+1;
      }
    }
        hattrie_iter_next(i);
    }

  hattrie_iter_free(i);
  if(new_fp_trie==NULL){
    fclose(fp_file);
  }
  else{
    if (MEM_CHECK){
     printf("done new making FP %d \n", iteration);
     fprintf(stderr,"done makin new FP %d \n", iteration);
     sleep(20);
     system("date");
     system("smem");
     sleep(1);
     system("smem");
    }
  }


//  else { FOR DEBUG
//    hattrie_iteration(new_fp_trie,"cascade_cehck", new_fp_file_path, 1);
//  }
  printf("we checked for %lld reads in which, we had total number of %lld which written into new  FP file\n",total_count, *num_of_new_fp_reads);
  fprintf(stderr, "we checked for %lld reads in which, we had total number of %lld which written into new  FP file\n",total_count, *num_of_new_fp_reads);
  printf("done creating FP number %d\n", iteration);
  fprintf(stderr, "done creating FP number %d\n", iteration);
  printf("done cascading_fp_encode iteation\n");
  return(1);
}

//************************************************************************************
//cascade_fp function
//************************************************************************************
cascade_fp(hattrie_t* unique_reads, char* label, long long* number_of_fp_reads, int read_size, long long genome_len){
//open uniqu
  BloomFilter* bf;
  long long bf_table_size;
  int num_of_hash_func;
  char bf_label[1024]="";
  char fp_label[1024]="";
  char new_fp_label[1024]="";
  char new_fp_file_path[1024]="";
  char fp_ref_file_path[1024]="";
  int number_of_cascade=4;
  int i;
  char str[15];
  long long num_of_new_fp_reads;
  long long  bf_params[2];
  hattrie_t* fp_ref_trie;
  hattrie_t* cur_fp_trie;
  hattrie_t* new_fp_trie;

  printf("start cascading_fp_encode with %lld initial FP reads\n", *number_of_fp_reads);
  fprintf(stderr, "start cascading_fp_encode with %lld initial FP reads\n", *number_of_fp_reads);

  strcat(new_fp_file_path, "./");
  strcat(new_fp_file_path, label);
  strcat(new_fp_file_path, "_fp_2.txt");

  strcat(fp_ref_file_path, "./");
  strcat(fp_ref_file_path, label);
  strcat(fp_ref_file_path, "_fp_1.txt");

  strcat(bf_label, "bf_2");
//  bf_table_size = TABLE_FACTOR*(*number_of_fp_reads);
//  num_of_hash_func = (long long) ceil(TABLE_FACTOR*HASH_FUNC_FACTOR);
  get_bf_params(bf_params, *number_of_fp_reads, genome_len,read_size);
  num_of_hash_func=bf_params[0];
  bf_table_size=bf_params[1];
  bf = bloom_filter_new(bf_table_size, string_hash, num_of_hash_func);
  printf("created empty BF2 with  %lld table size,  and %d number of hash func\n", bf_table_size,num_of_hash_func);
  fprintf(stderr, "created empty BF2 with  %lld table size,  and %d number of hash func\n", bf_table_size,num_of_hash_func);

 //first iteration
  new_fp_trie=hattrie_create();
  cascading_fp_encode(unique_reads, NULL, fp_ref_file_path, bf, new_fp_trie, new_fp_file_path, &num_of_new_fp_reads, 2);
  if (MEM_CHECK){
     printf("before freeing unique reads \n");
     sleep(30);
    system("date");
     system("smem");
     sleep(1);
     system("smem");
  }


  printf("freeing unique reads\n");
  fprintf(stderr, "freeing unique reads\n");
  hattrie_free(unique_reads);
  if (MEM_CHECK){
     printf("after freeing unique reads \n");
     sleep(30);
    system("date");
     system("smem");
     sleep(1);
     system("smem");
  }

  print_bf(bf, bf_table_size,num_of_hash_func, label, bf_label);
  printf("freeing bf2 \n");
  fprintf(stderr, "freeing bf2 \n");
  bloom_filter_free(bf);
  if (MEM_CHECK){
     printf("after freeing bf2 \n");
     sleep(30);
    system("date");
     system("smem");
     sleep(1);
     system("smem");
  }

  fp_ref_trie=hattrie_create();
  printf("loadin fp1 to trie from file\n");
  fprintf(stderr, "loadin fp1 to trie from file\n");
  load_file_to_trie(fp_ref_file_path, fp_ref_trie); //for the big FP1 in the first cascade, we will load it to trie from FP since we didn't load it before to the hash from trie but directly from a file
  unlink(fp_ref_file_path); //removing FP1 file
  if (MEM_CHECK){
     printf("after loading fp1 \n");
     sleep(30);
    system("date");
     system("smem");
     sleep(1);
     system("smem");
  }

////bf1 belong to reads, bf2 belongs to pf1 and the new fp will be fp_2
  for(i=3; i<(number_of_cascade+1); i++){ //if numbef of cascade=4, it will make bf3 that encodes fp2, and make fp3(which is FP relative to fp2), and bf4 which encode fp3 and make fp4 (which is FP relative to fp3)
     cur_fp_trie=new_fp_trie; //load previous "new fp" to be the current fp trie (what's going to be encoded
     if(i==number_of_cascade){ //if it's last iteration the print FP straight to file
       new_fp_trie=NULL;
     }
     else{ //otherwise load it to trie
       printf("creating fp %d\n",i);
       fprintf(stderr, "creating fp %d\n",i);
       new_fp_trie=hattrie_create(); //new trie to get the new FP
     }
     get_bf_params(bf_params, num_of_new_fp_reads, genome_len,read_size);
     num_of_hash_func=bf_params[0];
     bf_table_size=bf_params[1];  
//     bf_table_size = (long long) TABLE_FACTOR*num_of_new_fp_reads; //setting new bf based on previos number of fp reads
//     num_of_hash_func = (long long) ceil(TABLE_FACTOR*HASH_FUNC_FACTOR);
     bf = bloom_filter_new(bf_table_size, string_hash, num_of_hash_func);
     printf("created empty BF number %d with  %lld table size and %d number of hash func\n",i, bf_table_size,num_of_hash_func);
     fprintf(stderr, "created empty BF number %d with  %lld table size and %d number of hash func\n",i, bf_table_size,num_of_hash_func);
     memset(&new_fp_file_path[0], 0, sizeof(new_fp_file_path));
     strcat(new_fp_file_path, "./");
     strcat(new_fp_file_path, label);
     strcat(new_fp_file_path, "_fp_");
     sprintf(str, "%d", i);
     strcat(new_fp_file_path, str);
     strcat(new_fp_file_path, ".txt");
     num_of_new_fp_reads=0; //reset number of new fp reads
     cascading_fp_encode(fp_ref_trie, cur_fp_trie, NULL, bf, new_fp_trie, new_fp_file_path, &num_of_new_fp_reads, i); //use the previois fp ref_file as the current refference.i (e.g fp_ref_trie=fp1,cur_fp_trie=fp_2,bf=bf2==> fp_new=fp3=fp1 not it bf_2
  //after that, fp_ref_trie should be free and set to cur_fp_trie, and cur_fp_trie shold be new_fp_file_path
     printf("freeing FP number %d \n", i-2);
     fprintf(stderr, "freeing FP number %d \n", i-2);
     free(fp_ref_trie);
     if (MEM_CHECK){
       if (fp_ref_trie){
        printf("after freeing FP number %d \n", i-2);
        fprintf(stderr, "after freeing FP number %d \n", i-2);
        sleep(20);
       system("date");
        system("smem");
        sleep(1);
        system("smem");
       }
     }
     fp_ref_trie=cur_fp_trie; //now fp_ref_trie (the reads which are going to be encoded to BF) are going to be the previous "new FP" which was created
     memset(&bf_label[0], 0, sizeof(bf_label));
     strcat(bf_label, "bf_");
     strcat(bf_label, str);
     printf("printing BF number %d \n", i);
     fprintf(stderr, "printing BF number %d \n", i);
     print_bf(bf, bf_table_size,num_of_hash_func, label, bf_label);
     printf("freeing BF number %d \n", i);
     fprintf(stderr, "freeing BF number %d \n", i);
     bloom_filter_free(bf);
     if (MEM_CHECK){
       if (bf){
        printf("after freeing BF number %d \n", i);
        fprintf(stderr, "after freeing BF number %d \n", i);
        sleep(20);
       system("date");
        system("smem");
        sleep(1);
        system("smem");
       }
     }
  }
  printf("after freeing FP number %d \n", i-1);
  fprintf(stderr, "after freeing FP number %d \n", i-1);
  free(cur_fp_trie);
  if (MEM_CHECK){
    if (cur_fp_trie){
     printf("after freeing FP number %d \n", i-1);
     fprintf(stderr,"after freeing FP number %d \n", i-1);
     sleep(20);
    system("date");
     system("smem");
     sleep(1);
     system("smem");
    }
  }
}



//************************************************************************************
//cascading_fp_encode function
//************************************************************************************
//this function gets ,refernce reads trie, FP reference trie, a number of where this reference FP is in the cascade, an empty BF,
// and a file_path where to write the new FP against the reference FP
// first it hashes the FP into the BF
//then it goes through the the reference reads, and check if it has an accpet in the BF,
// if it has an accept it will check in the reference FP if it's there, and if it's not it
//will print it to the file (so we'lll have in the new FP in the cascade)

int cascading_fp_encode_low_mem(hattrie_t* ref_reads_trie, char* ref_fp_path, BloomFilter* bf_ref_fp, char* new_fp_file_path,long long* num_of_new_fp_reads, int iteration) {
  FILE *fp_file;
//  long long num_of_inserted_reads=0;
  long long total_count=0;
  long long count_in_bf=0;
  int in_bf_comp=0;
  long long len=0;
  char* check_key;
  char* m_key;
  hattrie_iter_t* i = hattrie_iter_begin(ref_reads_trie, false);
  printf("start cascading_fp_encode\n");
  fprintf(stderr, "start cascading_fp_encode\n");
  printf("hashing FP number %d to BF number %d\n", iteration -1, iteration);
  fprintf(stderr, "hashing FP number %d to BF number %d\n", iteration -1, iteration);
  load_file_to_bf(ref_fp_path, bf_ref_fp); //first hash FP1 to BF2
  printf("donehashing FP \n");
  fprintf(stderr, "donehashing FP \n");
  fp_file=fopen(new_fp_file_path, "w");
  *num_of_new_fp_reads=0;
  hattrie_iteration(ref_reads_trie,"check_post", "check_post",1);
  while (!hattrie_iter_finished(i)) { //check for each read in the unique reads, if BF2 has accpet, put it in FP2 file
    total_count++;
//    printf("%d\n",total_count);
    check_key = hattrie_iter_key(i, &len); 
    in_bf_comp = bloom_filter_query(bf_ref_fp, check_key);
    if (in_bf_comp!=0) { //if the read is accpet- meaning it FP while it's realy a true postive, then it's FP to the FP, we should print it to FP2
        fprintf(fp_file, "%s", check_key);
        *num_of_new_fp_reads=*num_of_new_fp_reads+1;
      }
        hattrie_iter_next(i);
    }
  
  hattrie_iter_free(i);
  fclose(fp_file);
  printf("we checked for %lld reads in which, we had total number of %lld which written into new  FP file\n",total_count, *num_of_new_fp_reads);
  fprintf(stderr, "we checked for %lld reads in which, we had total number of %lld which written into new  FP file\n",total_count, *num_of_new_fp_reads);
  printf("done printing FP number %d\n", iteration);
  fprintf(stderr, "done printing FP number %d\n", iteration);
  printf("done cascading_fp_encode\n");
  fprintf(stderr, "done cascading_fp_encode\n");
  return(1);
}


//************************************************************************************
//query_bf_with_genome_2 function
//************************************************************************************
//this function gets the bloom filter with the unique reads, the gnome reference file,
//and the read size, and put all the sliding windows size of read_size in which the
//bloom filter says that they are in the bloom filter, into trie_genome_unique
//meaning trie_genome uniuqe holds all the unique reads that can map into the genome reference
//as well as bloom filter's false positive sliding windows
//************************************************************************************
int query_bf_with_genome_2(BloomFilter* bf_unique, FILE* genome_file ,hattrie_t* trie_unique, int read_size, hattrie_t* trie_true_accept,char* m_label, char* output_label, long long* number_of_fp_reads) {
  int in_bf; //1 if cur_window in genome bloom-filter, 0 otherwisei
  int in_bf_comp; //1 if comp_window in genome bloom-filter, 0 otherwise
  int cur_window_index=0; //current index in the window
  long long line_number =1; // count number of line in file
  char* cur_window; //assuming read size is no larger then 600 chars
  char* complementary_window; //complementary strand to cur_window
  char* prev_window;
  char* prev_comp_window;
  int c; //holds current character
  long  long count_in_bf=0;
  long long count_comp_in_bf=0;
  long long count_windows=0;
  value_t* m_key;
  FILE *fp_file;
  char output_path[1024]="";
  printf("start querying unique BF through genome \n");
  fprintf(stderr, "start querying unique BF through genome \n");

  *number_of_fp_reads=0;

//opening False positive file to write
  strcat(output_path, "./");
  if (output_label) {
    strcat(output_path, output_label);
    strcat(output_path, "_");
  }
  strcat(output_path, m_label);
  strcat(output_path, ".txt");
  fp_file=fopen(output_path, "w");

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
        count_in_bf++;
        *number_of_fp_reads = *number_of_fp_reads+check_accept_read(cur_window, trie_unique, trie_true_accept, fp_file, strlen(cur_window));
      }
      in_bf_comp = bloom_filter_query(bf_unique, complementary_window);
      if (in_bf_comp!=0) {
        count_comp_in_bf++;
        *number_of_fp_reads = *number_of_fp_reads+check_accept_read(complementary_window, trie_unique, trie_true_accept, fp_file, strlen(complementary_window));
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
  fclose(fp_file);
  printf("there where total of %lld windows, in which %lld of original strands and %lld of complementary strands gave true for being in the BF \n",count_windows, count_in_bf, count_comp_in_bf);
  fprintf(stderr, "there where total of %lld windows, in which %lld of original strands and %lld of complementary strands gave true for being in the BF \n",count_windows, count_in_bf, count_comp_in_bf);
  printf("done querying unique BF through genome \n");
  fprintf(stderr, "done querying unique BF through genome \n");
  return(1);
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
    printf("we had %lld false_positives out of %lld reads \n", count, total_count);
    fprintf(stderr, "we had %lld false_positives out of %lld reads \n", count, total_count);
    hattrie_iter_free(i);
//    hattrie_iteration(trie_real_unique, "true_unique_fn", "testing"); //TMP- REOMVE!!!!
//    hattrie_free(trie_real_unique);//TMP- REOMVE!!!!
}


int check_fp_fn(hattrie_t* trie_true, hattrie_t* trie_to_check, hattrie_t* trie_to_check_true, char* m_label, char* output_label) {
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
    hattrie_iter_t* i = hattrie_iter_begin(trie_to_check, false);
    FILE *fp;
    char output_path[1024]="";
    printf("start check_fp_fn\n");
    fprintf(stderr, "start check_fp_fn\n");
    strcat(output_path, "./");
    if (output_label) {
      strcat(output_path, output_label);
      strcat(output_path, "_");
    }
    strcat(output_path, m_label);
    strcat(output_path, ".txt");
    fp=fopen(output_path, "w");


//    trie_real_unique = hattrie_create();  //TMP- REOMVE!!!!
    while (!hattrie_iter_finished(i)) {
        total_count++;
        check_key = hattrie_iter_key(i, &len);
        m_key = hattrie_tryget(trie_true, check_key, len);
        if(m_key==NULL){ //if not in trie_true - means false positive, then insert to FP trie, and also remove it from 
           fprintf(fp, "%s", check_key);
        }
        else { //means it is true 
           //check if it's in uqine- if not then print it to FN
          if (trie_to_check_true!= NULL){
            to_check_true_key = hattrie_get(trie_to_check_true, check_key, len);
            *to_check_true_key = 1;
          }
        }


        hattrie_iter_next(i);
    }
//    printf("we had %lld false_positives out of %lld reads \n", count, total_count);
    hattrie_iter_free(i);
    fclose(fp);
    printf("done check_fp\n");
    fprintf(stderr, "done check_fp\n");
    return(1);
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
//print_bf_binary function
//************************************************************************************
//gets bf and it's table size,and two labels and prints its content to a file,
//when first row is table size, second line is number of hash functions,
//third and then each row contain a byte in binary form
//************************************************************************************
int print_bf_binary(BloomFilter* bf, long long table_size, int num_of_hash_func, char* m_label, char* output_label) {
  unsigned char* bf_array; //array to load bf into in order to print it
  long long array_size = 0;
  FILE *fp;
  char output_path[1024]="";
  long long index=0;
  char* cur_byte;
  strcat(output_path, "./");
  if (output_label) {
    strcat(output_path, output_label);
    strcat(output_path, "_");
  }
  strcat(output_path, m_label);
  strcat(output_path, ".txt");
  fp=fopen(output_path, "w"); 
  fprintf(fp, "%lld\n",table_size);
  fprintf(fp, "%d\n",num_of_hash_func);
  array_size = (table_size + 7) / 8;
  bf_array = (char *)malloc(sizeof(char)*array_size);
  bloom_filter_read(bf, bf_array);
  while (index<(array_size)){
    cur_byte = byte_to_binary(bf_array[index]);
    fprintf(fp, "%s\n",cur_byte);
    index++;
  }
  free(bf_array);
  fclose(fp);
  printf("done.\n");
  return(1);
}


//////////////////////////
//print_bf
//////////////////////////////////////////
////gets bf and it's table size,and two labels and prints its content to a file,
//when first row is table size, second line is number of hash functions,
//and then each row contain a byte with the content of the bloom filter
////////////////////////////////
int print_bf(BloomFilter* bf, long long table_size, int num_of_hash_func, char* label_1, char* label_2) {
  unsigned char* bf_array; //array to load bf into in order to print it
  long long array_size = 0;
  FILE *fp;
  char output_path[1024]="";
  long long index=0;
  char* cur_byte;
  printf("start print_bf\n");
  fprintf(stderr, "start print_bf\n");
  strcat(output_path, "./");
  if (label_1) {
    strcat(output_path, label_1);
    strcat(output_path, "_");
  }
  strcat(output_path, label_2);
  strcat(output_path, ".txt");
  fp=fopen(output_path, "w");
  fprintf(fp, "%lld\n",table_size);
  fprintf(fp, "%d\n",num_of_hash_func);
  array_size = (table_size + 7) / 8;
  if (MEM_CHECK){
    printf("before mallocing bf array\n");
    sleep(20);
    system("date");
    system("smem");
    sleep(1);
    system("date");
    system("smem");
  }
  bf_array = (char *)malloc(sizeof(char)*array_size);
  if (MEM_CHECK){
    if (bf_array) {
      printf("after mallocing bf array\n");
      sleep(20);
     system("date");
      system("smem");
      sleep(1); 
    system("date");
      system("smem"); 
    }
  }
  printf("doing bloomfilter read\n");
  fprintf(stderr, "doing bloomfilter read\n");
  bloom_filter_read(bf, bf_array);
  while (index<(array_size)){
    fprintf(fp, "%c",bf_array[index]);
    index++;
  }
  sleep(3);
  printf("freein bf_array\n");
  fprintf(stderr, "freein bf_array\n");
  system("date");
  system("smem");
  free(bf_array);
  if (MEM_CHECK){
    if (bf_array){
     sleep(20);
    system("date");
     system("smem");
     sleep(1);
     system("smem");
    }
  }
  fclose(fp);
  printf("done print_bf.\n");
  fprintf(stderr, "done print_bf.\n");
  return(1);
} 





//************************************************************************************
//encode
//************************************************************************************
//gets trie_unique with uniqe reads, genome referemce, and pointer to empty bloofilter,FN-trie,FP-trie, label for ouput, and parameters for bloom filter
//and will load the bloom filter, the false negative trie and false positive trie
// and also wll result in printing bloom-filter, false negatives, false positives into files
//************************************************************************************int encode(hattrie_t* trie_unique, FILE* genome, BloomFilter* bf, int read_size, char* label, long long bf_table_size,int num_of_hash_func, long long* number_of_fp_reads;) {
int encode(hattrie_t* trie_unique, FILE* genome, BloomFilter* bf, int read_size, char* label, long long bf_table_size,int num_of_hash_func, long long* number_of_fp_reads) {
    hattrie_t* trie_genome_unique; //put 'accepts' (everything that uniqe BF says yes that it's in genome) into a trie
    hattrie_t* trie_genome_true; //holds reads that really mapped into the genome reference
    printf("start encode function\n");
    fprintf(stderr, "start encode function\n");
    system("date"); 
    system("smem");
    printf("hasing trie into bf \n");
    fprintf(stderr, "hasing trie into bf \n");
    hash_trie_into_bf(trie_unique, bf);
    
   // trie_genome_unique = hattrie_create(); //contain all the accepts windows
   // query_bf_with_genome(bf, genome,trie_genome_unique, read_size);
   trie_genome_unique = hattrie_create(); //will contain all the accepts windows with no FP
   trie_genome_true = hattrie_create();     //reads that really maps to the genome (true accepts)
   if (query_bf_with_genome_2(bf, genome, trie_unique, read_size,trie_genome_true,"fp_1", label, number_of_fp_reads)) {
    if (MEM_CHECK){ 
     sleep(20);
    system("date");
     system("smem");
     sleep(1);
     system("smem");
    }
   }
   
    printf("start checking for false negative \n");
    fprintf(stderr, "start checking for false negative \n");
    if (check_fp_fn(trie_genome_true,trie_unique, NULL,"fn_unique", label)){
      printf("done checking for false negative\n");
      if (MEM_CHECK){
        sleep(20);
        system("date");
        system("smem");
        sleep(1);
        system("smem");   
      }
    }

    printf("freeing genome_true \n");
    fprintf(stderr, "freeing genome_true \n");
    hattrie_free(trie_genome_true);
    if (MEM_CHECK){
      if (trie_genome_true){
        sleep(20);
    system("date");
        system("smem");
        sleep(1);
        system("smem");
      }
    }
  //  hattrie_iteration(trie_fn, "fn_unique", label);
    printf("printing bloom filter\n");
    fprintf(stderr, "printing bloom filter\n");
    print_bf(bf, bf_table_size,num_of_hash_func, label, "bf_1");
    printf("done printing bloom filter\n");
    fprintf(stderr, "done printing bloom filter\n");
    printf("done encode function\n");
    fprintf(stderr, "done encode function\n");
}


//************************************************************************************
//load_bf function
//************************************************************************************
//gets bf file name path a pointer to a bloom filter pointer, and a integer array size 2
//it loads the bloom filter file into the bloom filter pointer,
//and put into the array the bloom filter table-size and the number of hash func
//************************************************************************************
int load_bf(char* bf_path, BloomFilter** bf, long long* bf_results) {
  FILE* bf_file;
  long long table_size=0;
  int num_of_hash=0;
  char* bf_array;
  int c;
  long long ind=0;  
  long long array_size=0;
  bf_file = fopen(bf_path, "r");
  fscanf(bf_file, "%lld %d\n", &table_size, &num_of_hash);

  printf("table size is %lld, num of hash %d \n", table_size, num_of_hash);
  fprintf(stderr, "table size is %lld, num of hash %d \n", table_size, num_of_hash); 
  *bf = bloom_filter_new(table_size, string_hash, num_of_hash);
  array_size = (table_size + 7) / 8;

  bf_array = (char*)malloc(sizeof(char)*(array_size));
  while (ind<array_size){
    c = fgetc(bf_file);
//    printf("%c",c);
    bf_array[ind]=c;
    ind++;
  }
  bloom_filter_load(*bf, bf_array);    
  bf_results[0] = table_size;
  bf_results[1] = num_of_hash;
  fclose(bf_file);
  free(bf_array);
  printf("done load bf \n");
  fprintf(stderr, "done load bf \n");
  return(1);
}




//************************************************************************************
//load_file_to_trie function
//************************************************************************************
//gets file_path and a trie pointer, and load the reads in the file into the trie
//the value of the each read in the trie is the number of times a read appers
//************************************************************************************
int load_file_to_trie(char* reads_file_path, hattrie_t* reads_trie) {
  int size = 50; //size of read, default value=80 and increases in case of need
  long long  pos; //index to buffer
  int c; //reading character
  long long line_number=1;
  long long read_num =0;
  char *buffer = (char *)malloc(size);
  long long cur_line_num = 1;
  FILE* f;
  value_t* m_key;
  int len;
  char copy_buffer[200]="";
  long long old_repeat_num; //holds value of number of repeats of a key before enetring another same key
  printf("memory before load\n");
  fprintf(stderr, "memory before load\n");
  system("date");
  system("smem");
  f = fopen(reads_file_path, "r");
  if(f){
      do { // read all lines in file
        pos = 0;
        do{ // read one line
          c = fgetc(f);
          if(c != EOF) buffer[pos++] = toupper((char)c);
          if(pos >= size) { // increase buffer length - leave room for 0
            size +=1;
            buffer = (char*)realloc(buffer, size);
 //           strcpy(copy_buffer,buffer);
          }
        }while(c != EOF && c != '\n');
        buffer[pos] = 0;
        // line is now in buffer
        read_num+=1;
        // handle line 
        len = strlen(buffer);
        m_key = hattrie_tryget(reads_trie, buffer, len);
        if(m_key==NULL){ //if not in trie insert it with value 1
            m_key = hattrie_get(reads_trie, buffer, len);
            *m_key=1;         
        }
        else{//if it's already inside, increase it's value by 1
          old_repeat_num = *m_key;
          *m_key = old_repeat_num+1;
        }
      } while(c != EOF);
      fclose(f);
  }
  else {
    fprintf(stderr, "Error: couldnt open file %s \n",reads_file_path);
  }
  free(buffer);
  printf("done load file to trie: loaded %lld reads \n", read_num);
  fprintf(stderr, "done load file to trie: loaded %lld reads \n", read_num);
  return(1);
}

//************************************************************************************
//load_file_to_bf function
//************************************************************************************
//gets file_path and a trie pointer, and load the reads in the file into the trie
//the value of the each read in the trie is the number of times a read appers
//************************************************************************************
int load_file_to_bf(char* reads_file_path, BloomFilter* bf) {
  int size = 20; //size of read, default value=20 and increases in case of need
  long long  pos; //index to buffer
  int c; //reading character
  long long line_number=1;
  long long read_num =0;
  char *buffer = (char *)malloc(size);
  long long cur_line_num = 1;
  FILE* f;
  value_t* m_key;
  int len;
  char copy_buffer[200]="";
  long long old_repeat_num; //holds value of number of repeats of a key before enetring another same key
  printf("memory before load\n");
  fprintf(stderr, "memory before load\n");
  system("date");
  system("smem");
  f = fopen(reads_file_path, "r");
  if(f){
      do { // read all lines in file
        pos = 0;
        do{ // read one line
          c = fgetc(f);
          if(c != EOF) buffer[pos++] = toupper((char)c);
          if(pos >= size) { // increase buffer length - leave room for 0
            size +=1;
            buffer = (char*)realloc(buffer, size);
 //           strcpy(copy_buffer,buffer);
          }
        }while(c != EOF && c != '\n');
        buffer[pos] = 0;
        // line is now in buffer
        if (c != EOF){
          read_num+=1;
          // handle line
          bloom_filter_insert(bf, buffer); //insert read into bloom filter
        }
      } while(c != EOF);
      fclose(f);
  }
  else {
    fprintf(stderr, "Error: couldnt open file %s \n",reads_file_path);
  }
  free(buffer);
  printf("done load file to trie: loaded %lld reads \n", read_num);
  fprintf(stderr, "done load file to trie: loaded %lld reads \n", read_num);
  return(1);
}
    



int cascade_bloom_filter_query(BloomFilter**bf,int number_of_cascades, hattrie_t* trie_fp, char* window){
  int iteration;
  int in_bf;
  char* m_key;
 
  in_bf = bloom_filter_query(bf[0], window); 
  if (in_bf==1){ //if it's in the first bf (BF1), then if after it's ending in NOT being in an not even BF(3,5..) then it's FP,
                 // if it's ending not being in an even BF it' true
    for (iteration=1; iteration<number_of_cascades; iteration++){
      in_bf = bloom_filter_query(bf[iteration], window);
      if (in_bf==0){ 
        if(((iteration+1)%2)==0){
          return(1);
         }
        else{
          return(0);
        }
       } //else continue until it's not in one of the bf, if it's in all bf check in fp
    }//for
    //if it got to this step it means all bf's where 1
    m_key = hattrie_tryget(trie_fp, window, strlen(window));
    if((number_of_cascades%2)==0){ //if it's even and exist in FP, then it's true return 1
      if (m_key!=NULL) {
        return(1);
      }
      else{
        return(0);
      }
    }
    else{ //if it non-even number of cascades
      if (m_key!=NULL) { //not-even and exit in fp return 0
        return(0);
      }
      else{
        return(1);
      }
    }
  }
  else{ //if it's not in first bf then return 0
    return(0);
  }
}

///e/////////////////////////////
//decode_unique_reads_from_genome
/////////////////////////////////
//this function receives a path to the genome file reference, a pointer to fp trie,a pointer to an empty decoded_readds trie,
// a pointer to a bloom filter, read size, and decode the unique reads and loads it into the decoded_reads trie, based on the FP and the BF.
//if the BF accepts a read from the shifting window in the genome, and it doesn't exist in FP it will load it do decoded-reads
/////////////////////////////////
int decode_unique_reads_from_genome(char* genome_file_path, hattrie_t* trie_fp, hattrie_t* trie_decoded_reads, BloomFilter** bf, int read_size, int number_of_cascades){
  FILE* genome_file;
  int in_bf; //1 if cur_window in genome bloom-filter, 0 otherwisei
  int in_bf_comp; //1 if comp_window in genome bloom-filter, 0 otherwise
  long long cur_window_index=0; //current index in the window
  long long line_number =1; // count number of line in file
  char* cur_window; //assuming read size is no larger then 600 chars
  char* complementary_window; //complementary strand to cur_window
  char* prev_window;
  char* prev_comp_window;
  int c; //holds current character
  long  long count_in_bf=0;
  long long count_comp_in_bf=0;
  long long count_windows=0;
  value_t* m_key;
  
  printf("starting decode_unique_reads_from_genome- will load the unique decoded reads into newo uniqu etrie\n");
  fprintf(stderr, "starting decode_unique_reads_from_genome- will load the unique decoded reads into newo uniqu etrie\n");
  system("date");
  system("smem");
  genome_file = fopen(genome_file_path, "r");
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
      in_bf = cascade_bloom_filter_query(bf,number_of_cascades, trie_fp,cur_window);
      if (in_bf!=0) { //if it says read in bloom filter check first that it's not already in (it's been before or it was in repeats)
        count_in_bf++;
        m_key = hattrie_tryget(trie_decoded_reads, cur_window, strlen(cur_window));
        if(m_key==NULL) { //it it's not there, check to see if it's in false positives
    //      m_key = hattrie_tryget(trie_fp, cur_window, strlen(cur_window));
  //        if(m_key==NULL) { //if it's not in false positve, then it's a true read and need to be inserted
            m_key = hattrie_get(trie_decoded_reads, cur_window, strlen(cur_window));
            *m_key=1;
    //       }
         }
       }
      //check for reverse complementary stranad
      in_bf_comp = cascade_bloom_filter_query(bf,number_of_cascades, trie_fp, complementary_window);
      if (in_bf_comp!=0) { //if it says read in bloom filter check first that it's not already in (it's been before or it was in repeats)
        count_comp_in_bf++;
        m_key = hattrie_tryget(trie_decoded_reads, complementary_window, strlen(complementary_window));
        if(m_key==NULL) { //it it's not there, check to see if it's in false positives
      //    m_key = hattrie_tryget(trie_fp, complementary_window, strlen(complementary_window));
        //  if(m_key==NULL) { //if it's not in false positve, then it's a true read and need to be inserted
            m_key = hattrie_get(trie_decoded_reads, complementary_window, strlen(complementary_window));
            *m_key=1;
          // }
         }
       }
      
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
  printf("there where total of %lld windows, in which %lld of original strands and %lld of complementary strands gave true for being in the BF \n",count_windows, count_in_bf, count_comp_in_bf);
  fprintf(stderr, "there where total of %lld windows, in which %lld of original strands and %lld of complementary strands gave true for being in the BF \n",count_windows, count_in_bf, count_comp_in_bf); 
  printf("done decode_unique_reads_from_genome\n");
  fprintf(stderr, "done decode_unique_reads_from_genome\n");
  return(1);
}     
        
//////////////////////////////////////////////////
//decode
///////////decode////////////////////////////////////decode//this function does the full decoding, it gets paths to a bloom filter with the genome reference accepts, repeated reads trie , genome referece file, FN trie, FP trie as well as read size and output label, and prints out the decoded reads into a file
int decode(char* repeat_file_path, char* genome_file_path, char* fn_file_path, char* fp_file_path, int read_size, char* label, int cascade_number){
  BloomFilter* bf_dec[MAX_CASCADES];
  hattrie_t* trie_decoded_reads;
  hattrie_t*  trie_fp;
  long long bf_results[2];
  FILE* repeat_file;
  FILE* decoded_file;
  FILE* fn_file;
  int iteration;
  char output_path[1024]="";
  char bf_path[1024]="";
  char str[15];
  printf("start decoding \n");
  fprintf(stderr, "start decoding \n");
  printf("copying repeat file to decoded file\n");
  fprintf(stderr, "copying repeat file to decoded file\n"); 
// first we will copy all the repeat reads to file, and after we will add unique reads to it
  strcat(output_path, "./");
  if (label) {
    strcat(output_path, label);
    strcat(output_path, "_");
  }
  strcat(output_path, "decoded_file");
  strcat(output_path, ".txt");
  decoded_file=fopen(output_path, "w");
  repeat_file = fopen(repeat_file_path,"r");
  copy_file(repeat_file, decoded_file);
  fclose(repeat_file);
  fn_file = fopen(fn_file_path,"r");
  copy_file(fn_file, decoded_file);
  fclose(fn_file);
  trie_decoded_reads = hattrie_create();
  trie_fp = hattrie_create();
 //loading  bf as number of cascade
  for(iteration=0; iteration<cascade_number; iteration++){
     memset(&bf_path[0], 0, sizeof(bf_path));
     strcat(bf_path, label);
     strcat(bf_path, "_bf_");
     sprintf(str, "%d", iteration+1);
     strcat(bf_path, str);
     strcat(bf_path, ".txt");
    printf("loading bf %s \n", bf_path);
    fprintf(stderr, "loading bf %s \n", bf_path);
    if (load_bf(bf_path, &bf_dec[iteration], bf_results)) { //load_bf_file to bf, (bf_Results irrelevant here)
      if (MEM_CHECK){
        sleep(20);
      system("date");
        system("smem");
        sleep(1);
        system("smem");
      }
    }
  //FOR DEBUG: print_bf(bf_dec[iteration],bf_results[0], bf_results[1], "_loaded_bf_cascade_", str);
  }
  //load repeats and false negatives into decoded trie
//  printf("loading repeat file to trie\n");
//  load_file_to_trie(repeat_file_path, trie_decoded_reads);
//  printf("loading fn file to trie \n");
//  load_file_to_trie(fn_file_path, trie_decoded_reads);
  printf("loading fp file to trie \n");
  fprintf(stderr, "loading fp file to trie \n");
  if (load_file_to_trie(fp_file_path, trie_fp)){
    if (MEM_CHECK){
      sleep(20);
    system("date");
      system("smem");
      sleep(1);
      system("smem");  
     }
   }
  if (decode_unique_reads_from_genome(genome_file_path, trie_fp, trie_decoded_reads, bf_dec, read_size, cascade_number)){
     if (MEM_CHECK){
      sleep(20);
    system("date");
      system("smem");
      sleep(1);
      system("smem");
     }
  }

  //adding unique reads to file with already repeat reads
  hattrie_iteration_2(trie_decoded_reads, decoded_file, 0);
  fclose(decoded_file);
//  hattrie_iteration(trie_decoded_reads, "decoded_file", label);
  for(iteration=0; iteration<cascade_number; iteration++){
    printf("freeing bf_dec number %d \n", iteration);
    fprintf(stderr, "freeing bf_dec number %d \n", iteration);
    bloom_filter_free(bf_dec[iteration]);
    if (MEM_CHECK){
      if (bf_dec){
       sleep(20);
      system("date");
       system("smem");
       sleep(1);
       system("smem");
      }
    }
  }
  printf("freeing decoded reads \n");
  fprintf(stderr, "freeing decoded reads \n");
  hattrie_free(trie_decoded_reads);
 if (MEM_CHECK){
     if (trie_decoded_reads){ 
       sleep(20);
    system("date");
       system("smem");
       sleep(1);
       system("smem");
      }
  }
  printf("freeing trie fp and done decoding \n");
  fprintf(stderr, "freeing trie fp and done decoding \n");
  hattrie_free(trie_fp);
  if (MEM_CHECK){    
    if (trie_fp){
      sleep(20);
    system("date");
      system("smem");
      sleep(1);
      system("smem");
    }
  }
  printf("done decoce \n");
  fprintf(stderr, "done decoce \n");
  return(1);
}



///////////////////////////
//make_path function
////////////////////////////
void make_path(char* path, char* directory, char* label_1, char* label_2){
    strcat(path, directory);
    strcat(path, "/");
    strcat(path, label_1);
    strcat(path, "_");
    strcat(path, label_2);
    strcat(path, ".txt");
    printf("final path is %s \n",path);
}


//////////////////////////////////////////
///
//////////////////////////////////////////
void print_param_file(char*  label, int read_size, int number_of_cascades){
  char file_path[1024]="";
  FILE* f;
  printf("making param file\n");
  fprintf(stderr, "making param file\n");
  make_path(file_path, "./", label, "params");
  f = fopen(file_path, "w");
  fprintf(f, "%d\n",read_size);
  fprintf(f, "%d\n",number_of_cascades);
  fclose(f);
}




////////////////////////////////////////
//function encode file
///////////////////////////////////////
void encode_file(char * read_file_path, char* genome_file_path, char* label, int with_zip, int with_cascade){
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
    long long genome_len;
    BloomFilter* bf_unique; //BF for the unique tries
  //  char* output_label=(char *)malloc(50); //label name for the output files
    hattrie_t* trie_unique; //hattrie that holds the unique reads
    hattrie_t* trie_repeat; //hattrie that holds the repetetive reads, and the one that has N inside of them.
    long long bf_params[2];
    char str[15];


///////////////
//part 1
//////////////    
    printf("starting encode file \n");
    fprintf(stderr, "starting encode file \n");
    system("date");
    system("smem");
    trie_repeat = hattrie_create();
    trie_unique = hattrie_create();
    printf("make unique and repeat tries\n");
    fprintf(stderr, "make unique and repeat tries\n");
    if (make_repeat_and_unique_tries(read_file_path, trie_unique, trie_repeat, results)){
      printf("done making unique and repat tries\n");
      fprintf(stderr, "done making unique and repat tries\n");
      if (MEM_CHECK){
        sleep(20);
       system("date");
        system("smem");
        sleep(1);
        system("smem");
      }
    }
    //hattrie_iteration(trie_unique, "unique", label);
    hattrie_iteration(trie_repeat, "repeat", label, 1);
    printf("before free repeat\n");
    fprintf(stderr, "before free repeat\n");
    system("date");
    system("smem");
    hattrie_free(trie_repeat);
    if (MEM_CHECK){
      if (trie_repeat){
        printf("after freeing trie repeat\n");
        sleep(20);
    system("date");
        system("smem");
        sleep(1);
        system("smem");
      }
    }

///////////////
//part 2
//////////////
    read_num = results[0];
    size = results[1];
    read_size = size-2;
    genome_f = fopen(genome_file_path, "rb");
    fseek(genome_f, 0, SEEK_END);
    genome_len = 2*ftell(genome_f);
    printf("length of genome times 2 is %lld\n", genome_len);
    fclose(genome_f);
    genome_f = fopen(genome_file_path, "r");
//    table_factor = TABLE_FACTOR; //arbitrary
//    bf_table_size = (long long)table_factor*read_num;
//    num_of_hash_func = (unsigned int) ceil(table_factor*HASH_FUNC_FACTOR);
    get_bf_params(bf_params, read_num, genome_len,read_size);
    num_of_hash_func=bf_params[0];
    bf_table_size=bf_params[1];

    printf("read num is %lld table size is %lld  number of hash func is %u \n",read_num, bf_table_size, num_of_hash_func);
    fprintf(stderr, "read num is %lld table size is %lld number of hash func is %u \n",read_num, bf_table_size, num_of_hash_func);
    printf("creating bf\n");
    fprintf(stderr, "creating bf\n");
    bf_unique = bloom_filter_new(bf_table_size, string_hash, num_of_hash_func);
    if (MEM_CHECK){
      if(bf_unique){
      printf("memory after creating bf\n");
      sleep(20);
    system("date");
      system("smem");
      sleep(1);
      system("smem");
      }
    }
    printf("start encoding\n");
    fprintf(stderr, "start encoding\n");
    if (encode(trie_unique, genome_f, bf_unique ,read_size, label, bf_table_size,num_of_hash_func, &number_of_fp_reads)) {
      printf("done encoding\n");
    system("date");
      system("smem");
    }

    fclose(genome_f);
    printf("memory before freeing bf\n");
    fprintf(stderr, "memory before freeing bf\n");
    system("date");
    system("smem");
    bloom_filter_free(bf_unique);
    if (MEM_CHECK){
      if(bf_unique){
      printf("memory after freeing bf_unique and before freein trie uniuqe\n");
      sleep(20);
      system("date");
      system("smem");
      sleep(1);
      system("smem");
      }
    }
//start cascading
   if (with_cascade==1) {
    printf("start cascading with number of intial false positive reads are %lld\n", number_of_fp_reads);
    fprintf(stderr, "start cascading with number of intial false positive reads are %lld\n", number_of_fp_reads);
     cascade_fp(trie_unique, label, &number_of_fp_reads, read_size, genome_len);
   }
  else { //if it's in cascade trie unique it being free in cascade_fp, else we will free it here
   hattrie_free(trie_unique);
   number_of_cascades=1;
   if (MEM_CHECK){
     if (trie_unique){
       printf("memory after freein trie_unique\n");
       sleep(20);
       system("date");
       system("smem");
       sleep(1);
       system("smem");
     }
   }
  }
  print_param_file(label, read_size, number_of_cascades);
   //zippin
  if (with_zip==1){
   printf("zipping files\n");
   fprintf(stderr, "zipping files\n");
   zip_or_remove_encoded_files(label, number_of_cascades, 0);
   printf("done zipping files\n");
   fprintf(stderr, "done zipping files\n");
   printf("compressing fn fp and repeat files with scalce\n");
   scalce_compress_files(label, read_size, number_of_cascades);
  }
}

void decode_file(char* genome_file_path, char* label, int with_zip, int with_cascade) {
    int read_size=0; //the size of the reads //TODO, make a read size funtion
    int number_of_cascades=0; //TODO change
    FILE* genome_f;
    char bf_path[1024]="";
    char repeat_file_path[1024]="";
    char fn_file_path[1024]="";
    char fp_file_path[1024]="";
    char param_file_path[1024]="";
    char directory[1024]=".";
    FILE* pf_file;
    char str[15];

//unzippin
    if (with_zip==1){
      printf("unzipping files\n");
      unzip_encoded_files(label);
      printf("done unzipping files\n");
    }

    make_path(repeat_file_path,directory, label, "repeat");
    make_path(fn_file_path,directory, label, "fn_unique");
    make_path(param_file_path, directory, label, "params");
    //get read size and number of cascades
    pf_file = fopen(param_file_path, "r");
    fscanf(pf_file, "%d %d\n", &read_size, &number_of_cascades);
    fclose(pf_file);

    if (with_zip==1){
      printf("uncompressing repeat, fn and fp files with scalce\n");
      scalce_decompress_files(label, read_size, number_of_cascades);
      printf("done uncompressing files files\n");
    }
 
 
    strcat(fp_file_path, "./");
    strcat(fp_file_path, label);
    strcat(fp_file_path, "_fp_");
    sprintf(str, "%d", number_of_cascades);
    strcat(fp_file_path, str);
    strcat(fp_file_path,".txt");
    printf("fp path is %s\n", fp_file_path);
    fprintf(stderr, "fp path is %s\n", fp_file_path);


    pf_file = fopen(param_file_path, "r");
    fscanf(pf_file, "%d %d\n", &read_size, &number_of_cascades);
    fclose(pf_file);
    printf("read size is %d, number of cascades is %d \n", read_size, number_of_cascades);
    fprintf(stderr, "read size is %d, number of cascades is %d \n", read_size, number_of_cascades);
    printf("start decoding\n");
    fprintf(stderr, "start decoding\n");
   if (with_cascade==0) {
      number_of_cascades=1;
  }

    if (decode(repeat_file_path, genome_file_path, fn_file_path, fp_file_path, read_size, label, number_of_cascades)) {
      if (MEM_CHECK){
        printf("done decode\n");
        
        sleep(20);
    system("date");
        system("smem");
        sleep(1);
        system("smem");
      }
    }
   if (!MEM_CHECK){ //if in DEBUG mode, we will KEEP the files
     zip_or_remove_encoded_files(label, number_of_cascades, 1);
   }

}

