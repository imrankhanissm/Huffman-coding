# Huffman-coding

Compressor and Decompressor takes full file path with extension as a command line arguement.

## Compression:

1.Compile compress.cpp  command->(__g++ -s compress.cpp -o compress__)

2.To compress the file  command->(__./compress "full_path_of_the_file_you_want_to_compress_with_extension"__) gives you the compressed file with _your_file_name.huff_ in the same directory.

## Decompression:

3.Compile decompress.cpp->(__g++ -s decompress.cpp -o decompress__)

4.To decompress the file->(__./decompress "full_path_of_the_file_you_want_to_decompress_with_extension"__) gives you the original file in the same directory.
