#include "barcode.h"
#include "miniz.c"

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
//zip function gets a file path, a file name, archive name ,
// and adds the file into the archive with the name "file_name", if archive doesn't
//exist it will create it
//************************************************************************************
int zip(char* file_path, char* archive_prefix_name, char* file_name)
{
  mz_bool status;
  char archive_filename[1024]=""; //name of the file you want to name your data 
  char* data; //your data t4hat you want to compress
  const char *s_pComment = "This is a comment";
  strcat(archive_filename,archive_prefix_name);
  strcat(archive_filename, ".zip");
  data=copy_file_to_mem(file_path);
  printf("done memcopy\n");
    // Add a new file to the archive. Note this is an IN-PLACE operation, so if it fails your archive is probably hosed (its central directory may not be complete) but it should be recoverable using zip -F or -FF. So use caution with this guy.
    // A more robust way to add a file to an archive would be to read it into memory, perform the operation, then write a new archive out to a temp file and then delete/rename the files.
    // Or, write a new archive to disk to a temp file, then delete/rename the files. For this test this API is fine.
    status = mz_zip_add_mem_to_archive_file_in_place(archive_filename, file_name, data, strlen(data), s_pComment, (uint16)strlen(s_pComment), MZ_BEST_COMPRESSION);
    if (!status)
    {
      printf("mz_zip_add_mem_to_archive_file_in_place failed!\n");
      free(data);
      return EXIT_FAILURE;
    }
  free(data);
  remove(file_path); //deleting original file
  printf("Success.\n");
  return EXIT_SUCCESS;
}

//************************************************************************************
//unzip function
//************************************************************************************
//will unzip a file from a zip archive
//////////////////////////////
int unzip(char* archive_prefix_name, char* file_name)
{
  long long uncomp_size;
  int i;
  mz_zip_archive zip_archive;
  mz_bool status;
  char archive_filename[1024]=""; //name of the file you want to name your data 
  const char *s_pComment = "This is a comment";
  void *p;
  FILE *fp;
  strcat(archive_filename,archive_prefix_name);
  strcat(archive_filename, ".zip");
  fp=fopen(file_name, "w");
  // Now try to open the archive.
  memset(&zip_archive, 0, sizeof(zip_archive));
  status = mz_zip_reader_init_file(&zip_archive, archive_filename, 0);
  if (!status)
  {
    printf("mz_zip_reader_init_file() failed!\n");
    return EXIT_FAILURE;
  }
  printf("done1\n");
//  for (i = 0; i < (int)mz_zip_reader_get_num_files(&zip_archive); i++)
//  {
    // Try to extract all the files to the heap.
    p = mz_zip_reader_extract_file_to_heap(&zip_archive, file_name, &uncomp_size, 0);
  printf("done2\n");
//    mz_zip_extract_archive_file_to_heap(archive_filename, filename_name,
//        long long *pSize, mz_uint zip_flags);
    if (!p)
    {
      printf("mz_zip_reader_extract_file_to_heap() failed!\n");
      mz_zip_reader_end(&zip_archive);
      return EXIT_FAILURE;
    }
    printf("Successfully extracted file \"%s\", size %u\n", file_name, (uint)uncomp_size);
    fprintf(fp,"%s", (const char *)p);

    // We're done.
    mz_free(p);
    fclose(fp);

//  }

    // Close the archive, freeing any resources it was using
  mz_zip_reader_end(&zip_archive);
  printf("Success.\n");
  return EXIT_SUCCESS;

}

//************************************************************************************
//zip_encode_files function will encode repeat, fn,fp,bf files of an encoded file
//************************************************************************************
int zip_encoded_files(char* archive_prefix_name)
{
  char archive_filename[1024]=""; //name of the file you want to name your data 
  char file_name[1024]=""; 
  char buffer[1024]="";
  
  strcat(file_name, archive_prefix_name);
  strcat(file_name, "_repeat.txt");
 // zip(file_name ,archive_prefix_name, file_name);
  snprintf(buffer, sizeof(buffer), "gzip -f %s", file_name);
  system(buffer);
 // strcat(archive_filename,archive_prefix_name);
//  strcat(archive_filename, ".zip");



  memset(&file_name[0], 0, sizeof(file_name));
  strcat(file_name, archive_prefix_name);
  strcat(file_name, "_fn_unique.txt");
//  zip(file_name ,archive_prefix_name, file_name);
  memset(&buffer[0], 0, sizeof(buffer));
  snprintf(buffer, sizeof(buffer), "gzip -f %s", file_name);
  system(buffer);


  memset(&file_name[0], 0, sizeof(file_name));
  strcat(file_name, archive_prefix_name);
  strcat(file_name, "_fp_unique.txt");
//  zip(file_name ,archive_prefix_name, file_name);
  memset(&buffer[0], 0, sizeof(buffer));
  snprintf(buffer, sizeof(buffer), "gzip -f %s", file_name);
  system(buffer);


  memset(&file_name[0], 0, sizeof(file_name));
  strcat(file_name, archive_prefix_name);
  strcat(file_name, "_unique_bf.txt");
 // zip(file_name ,archive_prefix_name, file_name);
  memset(&buffer[0], 0, sizeof(buffer));
  snprintf(buffer, sizeof(buffer), "gzip -f %s", file_name);
  system(buffer);

}

//************************************************************************************
//unzip_encode_files function will unzip repeat, fn,fp,bf files of an encoded file
//************************************************************************************
int unzip_encoded_files(char* archive_prefix_name)
{
  char archive_filename[1024]=""; //name of the file you want to name your data 
  char file_name[1024]="";
  char buffer[1024]="";

//  strcat(archive_filename,archive_prefix_name);
//  strcat(archive_filename, ".zip");

  strcat(file_name, archive_prefix_name);
  strcat(file_name, "_repeat.txt.gz");
//  unzip(archive_prefix_name, file_name);
  snprintf(buffer, sizeof(buffer), "gunzip -f %s", file_name);
  system(buffer);


  memset(&file_name[0], 0, sizeof(file_name));
  strcat(file_name, archive_prefix_name);
  strcat(file_name, "_fn_unique.txt.gz");
//  unzip(archive_prefix_name, file_name);
  memset(&buffer[0], 0, sizeof(buffer));
  snprintf(buffer, sizeof(buffer), "gunzip -f %s", file_name);
  system(buffer);



  memset(&file_name[0], 0, sizeof(file_name));
  strcat(file_name, archive_prefix_name);
  strcat(file_name, "_fp_unique.txt.gz");
//  unzip(archive_prefix_name, file_name);
  memset(&buffer[0], 0, sizeof(buffer));
  snprintf(buffer, sizeof(buffer), "gunzip -f %s", file_name);
  system(buffer);


  memset(&file_name[0], 0, sizeof(file_name));
  strcat(file_name, archive_prefix_name);
  strcat(file_name, "_unique_bf.txt.gz");
//  unzip(archive_prefix_name, file_name);
  memset(&buffer[0], 0, sizeof(buffer));
  snprintf(buffer, sizeof(buffer), "gunzip -f %s", file_name);
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
    printf("done iterating.\n");
    return(1);
}




void copy_file(FILE* source, FILE* target){
   char ch;
   while( ( ch = fgetc(source) ) != EOF )
      fputc(ch, target);
   printf("File copied successfully.\n");
}
//************************************************************************************
//make_repeat_and_unique_tries function
//************************************************************************************
//gets a file path conataining reads and two empty tries, to fill them with:
//1.unique trie
//2.repeat trie
//************************************************************************************


int make_repeat_and_unique_tries(char* reads_file_path, hattrie_t* trie_unique, hattrie_t* trie_repeat, int* results) {
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
void check_accept_read(char* read, hattrie_t* trie_true,hattrie_t* trie_true_accept, FILE* fp_file, long long len) {
   char* m_key; // result of key when checking if trie_to_check is in trie_true
   char* to_check_true_key;
   m_key = hattrie_tryget(trie_true, read, len);
   if(m_key==NULL){ //if not in trie_true - means false positive, then insert to FP trie, and also remove it from 
      fprintf(fp_file, "%s", read);

   }
   else { //means it is true
     if (trie_true_accept!= NULL){
       to_check_true_key = hattrie_get(trie_true_accept, read, len);
       *to_check_true_key = 1;
     }
   }
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
int query_bf_with_genome(BloomFilter* bf_unique, FILE* genome_file ,hattrie_t* trie_genome_uniqe, int read_size) {
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



  //  if ((line_number+cur_window_index)%1000000==0) {
  //    printf("cur line number is %d line_number\n", line_number);
 //   }
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
  printf("there where total of %lld windows, in which %lld of original strands and %lld of complementary strands gave true for being in the BF \n",count_windows, count_in_bf, count_comp_in_bf);
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
int query_bf_with_genome_2(BloomFilter* bf_unique, FILE* genome_file ,hattrie_t* trie_unique, int read_size, hattrie_t* trie_true_accept,char* m_label, char* output_label) {
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
        check_accept_read(cur_window, trie_unique, trie_true_accept, fp_file, strlen(cur_window));
      }
      in_bf_comp = bloom_filter_query(bf_unique, complementary_window);
      if (in_bf_comp!=0) {
        count_comp_in_bf++;
        check_accept_read(complementary_window, trie_unique, trie_true_accept, fp_file, strlen(complementary_window));

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
  printf("done querying unique BF through genome \n");
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
    printf("we had %lld false_positives out of %lld reads \n", count, total_count);
    hattrie_iter_free(i);
    fclose(fp);
    printf("done check_fp\n");
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
  bloom_filter_read(bf, bf_array);
  while (index<(array_size)){
    fprintf(fp, "%c",bf_array[index]);
    index++;
  }
  sleep(3);
  printf("freein bf_array\n");
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
  return(1);
} 





//************************************************************************************
//encode
//************************************************************************************
//gets trie_unique with uniqe reads, genome referemce, and pointer to empty bloofilter,FN-trie,FP-trie, label for ouput, and parameters for bloom filter
//and will load the bloom filter, the false negative trie and false positive trie
// and also wll result in printing bloom-filter, false negatives, false positives into files
//************************************************************************************

int encode(hattrie_t* trie_unique, FILE* genome, BloomFilter* bf, hattrie_t* trie_fp, hattrie_t* trie_fn ,int read_size, char* label, long long bf_table_size,int num_of_hash_func) {
    hattrie_t* trie_genome_unique; //put 'accepts' (everything that uniqe BF says yes that it's in genome) into a trie
    hattrie_t* trie_genome_true; //holds reads that really mapped into the genome reference
    printf("start encode function\n");
    system("date"); 
    system("smem");
    printf("hasing trie into bf \n");
    hash_trie_into_bf(trie_unique, bf);
    
   // trie_genome_unique = hattrie_create(); //contain all the accepts windows
   // query_bf_with_genome(bf, genome,trie_genome_unique, read_size);
   trie_genome_unique = hattrie_create(); //will contain all the accepts windows with no FP
   trie_genome_true = hattrie_create();     //reads that really maps to the genome (true accepts)
   if (query_bf_with_genome_2(bf, genome,trie_unique, read_size,trie_genome_true,"fp_unique", label)) {
    if (MEM_CHECK){ 
     sleep(20);
    system("date");
     system("smem");
     sleep(1);
     system("smem");
    }
   }
   

//    printf("start checking for false positive \n");
//    check_fp_fn(trie_unique,trie_genome_unique, trie_genome_true,"fp_unique", label);
     
//    printf("done checking for false positve, printing it \n");
//    system("smem");
//    printf("freeing genome_unqiue \n");
//    hattrie_free(trie_genome_unique);
//    system("free");
    printf("start checking for false negative \n");
    if (check_fp_fn(trie_genome_true,trie_unique, NULL,"fn_unique", label)){
  //  check_fn(trie_unique,trie_genome_true, trie_fn);
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
    print_bf(bf, bf_table_size,num_of_hash_func, label, "unique_bf");
    printf("done printing bloom filter\n");
    printf("done encode function\n");
}


//************************************************************************************
//load_bf function
//************************************************************************************
//gets bf file name path a pointer to a bloom filter pointer, and a integer array size 2
//it loads the bloom filter file into the bloom filter pointer,
//and put into the array the bloom filter table-size and the number of hash func
//************************************************************************************
int load_bf(char* bf_path, BloomFilter** bf, int* bf_results) {
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
  system("date");
  system("smem");
  printf("this is the trie %d\n", reads_trie);
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
  return(1);
}



/////////////////////////////////
//decode_unique_reads_from_genome
/////////////////////////////////
//this function receives a path to the genome file reference, a pointer to fp trie,a pointer to an empty decoded_readds trie,
// a pointer to a bloom filter, read size, and decode the unique reads and loads it into the decoded_reads trie, based on the FP and the BF.
//if the BF accepts a read from the shifting window in the genome, and it doesn't exist in FP it will load it do decoded-reads
/////////////////////////////////
int decode_unique_reads_from_genome(char* genome_file_path, hattrie_t* trie_fp, hattrie_t* trie_decoded_reads, BloomFilter* bf, int read_size){
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

      in_bf = bloom_filter_query(bf, cur_window);
      if (in_bf!=0) { //if it says read in bloom filter check first that it's not already in (it's been before or it was in repeats)
        count_in_bf++;
        m_key = hattrie_tryget(trie_decoded_reads, cur_window, strlen(cur_window));
        if(m_key==NULL) { //it it's not there, check to see if it's in false positives
          m_key = hattrie_tryget(trie_fp, cur_window, strlen(cur_window));
          if(m_key==NULL) { //if it's not in false positve, then it's a true read and need to be inserted
            m_key = hattrie_get(trie_decoded_reads, cur_window, strlen(cur_window));
            *m_key=1;
           }
         }
       }
      //check for reverse complementary stranad
      in_bf_comp = bloom_filter_query(bf, complementary_window);
      if (in_bf_comp!=0) { //if it says read in bloom filter check first that it's not already in (it's been before or it was in repeats)
        count_comp_in_bf++;
        m_key = hattrie_tryget(trie_decoded_reads, complementary_window, strlen(complementary_window));
        if(m_key==NULL) { //it it's not there, check to see if it's in false positives
          m_key = hattrie_tryget(trie_fp, complementary_window, strlen(complementary_window));
          if(m_key==NULL) { //if it's not in false positve, then it's a true read and need to be inserted
            m_key = hattrie_get(trie_decoded_reads, complementary_window, strlen(complementary_window));
            *m_key=1;
           }
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
  printf("done decode_unique_reads_from_genome\n");
  return(1);
}     
        
//////////////////////////////////////////////////
//decode
//////////////////////////////////////////////////
//this function does the full decoding, it gets paths to a bloom filter with the genome reference accepts, repeated reads trie , genome referece file, FN trie, FP trie as well as read size and output label, and prints out the decoded reads into a file
int decode(char* bf_path, char* repeat_file_path, char* genome_file_path, char* fn_file_path, char* fp_file_path, int read_size, char* label){
  BloomFilter* bf_dec;
  hattrie_t* trie_decoded_reads;
  hattrie_t* trie_fp;
  int bf_results[2];
  FILE* repeat_file;
  FILE* decoded_file;
  FILE* fn_file;
  char output_path[1024]="";
  printf("start decoding \n");
  printf("copying repeat file to decoded file\n"); 
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
  if (load_bf(bf_path, &bf_dec, bf_results)) {
    if (MEM_CHECK){
      sleep(20);
    system("date");
      system("smem");
      sleep(1);
      system("smem");
    }
  }
  //load repeats and false negatives into decoded trie
//  printf("loading repeat file to trie\n");
//  load_file_to_trie(repeat_file_path, trie_decoded_reads);
//  printf("loading fn file to trie \n");
//  load_file_to_trie(fn_file_path, trie_decoded_reads);
  printf("loading fp file to trie \n");
  if (load_file_to_trie(fp_file_path, trie_fp)){
    if (MEM_CHECK){
      sleep(20);
    system("date");
      system("smem");
      sleep(1);
      system("smem");  
     }
   }
  if (decode_unique_reads_from_genome(genome_file_path, trie_fp, trie_decoded_reads, bf_dec, read_size)){
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
  printf("freeing bf_dec \n");
  bloom_filter_free(bf_dec);
  if (MEM_CHECK){
    if (bf_dec){
     sleep(20);
    system("date");
     system("smem");
     sleep(1);
     system("smem");
    }
  }
  printf("freeing decoded reads \n");
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
  return(1);
}

