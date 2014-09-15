m_file_name='barcode'
echo -e 'test_number='$1 '\ndata_path =' $3 '\ngenome_path =' $4 '\nlabel_name =' $2 | cat - barcode.cmd > $m_file_name'_'$2'_test_'$1'.cmd'
condor_submit $m_file_name'_'$2'_test_'$1'.cmd'
