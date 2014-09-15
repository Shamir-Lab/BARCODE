filename           = barcode_$(label_name)_test_$(test_number)

#ExDIR           = /usr/local/bin

Executable      = /usr/bin/time

arguments       = "-v '/home/nasheran/yaronein/projects/BARCODE/scripts/./test_barcode' $(test_number) $(label_name) $(data_path) $(genome_path)"
Error           = /home/nasheran/yaronein/projects/BARCODE/scripts/$(filename).error

Log             = /home/nasheran/yaronein/projects/BARCODE/scripts/$(filename).log

Output          = /home/nasheran/yaronein/projects/BARCODE/scripts/$(filename).output 

#notification    = Always

notification    = never

#requirements    = Machine == "rack-bio-eran-02.cs.tau.ac.il"

Queue 1
