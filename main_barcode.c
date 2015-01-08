nt main(int argc, char *argv[]) {
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
    table_factor = 10; //arbitrary
    num_of_reads = line_number/2;
    bf_table_size = table_factor*num_of_reads;
    num_of_hash_func = (int) ceil(table_factor*0.69314);
    //print the keys of the uniqe and repaet tries
    hattrie_iteration(trie_unique, "unique", argv[3]);
    hattrie_iteration(trie_repeat, "repeat", argv[3]);
//hashing uniqe reads trie using bloom filter
    bf_unique = bloom_filter_new(bf_table_size, string_hash, num_of_hash_func);
    hash_trie_into_bf(trie_unique, bf_unique);
    check_if_trie_in_bf(trie_unique, bf_unique);
//create trie for all of the sliding windows in the genome reference which are in the unique reads according to the bf_unique
    read_size = size-2;
    trie_genome_unique = hattrie_create();
    query_bf_with_genome(bf_unique, genome_f ,trie_genome_unique, read_size);
    check_if_trie_in_bf(trie_unique, bf_unique);
    fclose(genome_f);
    hattrie_iteration(trie_genome_unique, "genome_unique", argv[3]);
    trie_fp = hattrie_create();
    trie_fn = hattrie_create();

    printf("start checking for false positive \n");
    check_fp(trie_unique,trie_genome_unique, trie_fp);
    hattrie_iteration(trie_fp, "fp_unique", argv[3]);

    printf("start checking for false negative \n");
    check_fn(trie_unique,trie_genome_unique, trie_fn);
    hattrie_iteration(trie_fn, "fn_unique", argv[3]);

    bloom_filter_free(bf_unique);
    free(buffer);
    hattrie_free(trie_unique);
    hattrie_free(trie_repeat);
    hattrie_free(trie_genome_unique);
    hattrie_free(trie_fn);
    hattrie_free(trie_fp);
    return 0;
}

