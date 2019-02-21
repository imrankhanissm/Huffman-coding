How to use(Windows):

0.change directory to where you downloaded the source files(compress.cpp, decompress.cpp)
Compression:

1.compile compress.cpp	command->(g++ -s compress.cpp -o compress)	

2.to compress the file	command->(./compress "full_path_of_the_file_you_want_to_compress_with_extension") gives you the compressed file with "your_file_name.huff" in the same directory.

Decompression:

3.compile decompress.cpp->(g++ -s decompress.cpp -o decompress)

4.to decompress the file->(./decompress "full_path_of_the_file_you_want_to_decompress_with_extension") gives you the original file in the same directory.

