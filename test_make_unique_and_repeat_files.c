#include <barcode.c>
//************************************************************************************
//main function
//************************************************************************************

void test_make_unique_and_repeat_files(char *argv[]) {
    FILE *genome_f;
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
    make_repeat_and_unique_tries((argv[1], "r"), trie_unique, trie_repeat);
    hattrie_iteration(trie_unique, "unique", argv[1]);
    hattrie_iteration(trie_repeat, "repeat", argv[1]);

}

int main(int argc, char *argv[]) {
test_make_unique_and_repeat_files(argv);
}

