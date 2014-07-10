filename           = barcode_$(label_name)

#ExDIR           = /usr/local/bin

Executable      = /usr/bin/time

arguments       = "-v '/home/nasheran/yaronein/projects/BARCODE/scripts/./barcode' $(data_path) $(label_name)"
Error           = /home/nasheran/yaronein/projects/BARCODE/scripts/$(filename).error

Log             = /home/nasheran/yaronein/projects/BARCODE/scripts/$(filename).log

Output          = /home/nasheran/yaronein/projects/BARCODE/scripts/$(filename).output 

#notification    = Always

notification    = never

#requirements    = Machine == "rack-bio-eran-02.cs.tau.ac.il" || Machine == "rack-bio-eran-03.cs.tau.ac.il"

Queue 1
