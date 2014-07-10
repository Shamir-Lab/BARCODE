m_file_name='barcode'
echo -e 'data_path =' $1 '\nlabel_name =' $2 | cat - barcode.cmd > $m_file_name'_'$2'.cmd'
condor_submit $m_file_name'_'$2'.cmd'
