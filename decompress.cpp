#include <stdio.h>
#include <map>
#include <unordered_map>
#include <bitset>
#include <fstream>
#include <exception>

using namespace std;

unordered_map<string, char> code_char;

string char_to_binary(char input){
    return bitset<8>(input).to_string();
}

string huffman_code_to_string(string input){
    string output = "", temp = "";
    for(char i: input){
		temp += i;
		if(code_char.find(temp)!=code_char.end()){
			output+=code_char[temp];
			temp = "";
		}
	}
    return output;
}

struct file_name{
    string file_name;
    string extension;
};

file_name get_output_file_name(string input){
    int i;
    for(i=input.length();input[i] != '.';--i);
    file_name file_name_obj;
    file_name_obj.file_name = input.substr(0, i);
    file_name_obj.extension = "." + input.substr(i+1, input.length());
    return file_name_obj;
}

int main(int n, char** input_file_name){
    try{
        string output = "", output_file_name, extension;
        ifstream input_file(input_file_name[1], ios::binary);
        if(!input_file.is_open()){
            printf("error opening input file\n");
            return 1;
        }
        file_name file_name_obj = get_output_file_name(input_file_name[1]);
        output_file_name = "dc_" + file_name_obj.file_name;
        input_file >> extension; //  output file name
        ofstream output_file(output_file_name + "." + extension, ios::binary);
        if(!output_file.is_open()){
            printf("error opening output file\n");
            return 1;
        }
        printf("Decompressing...\n");
        int size, frequency;
        input_file >> size; // number of characters
        input_file.get();  //for space
		
        while(size--){
			char ch;
			string str;
			input_file.get(ch);
			input_file >> str;
			code_char[str] = ch;
			input_file.get();
        }

        int length;
        input_file >> length;   // length of string
        int extra_bits;
        input_file >> extra_bits; // extra bits
        input_file.get();
        char temp;
        for(int i=0;i<length;++i){
            input_file.get(temp);
            if(i == length-1 && extra_bits){
                output += char_to_binary(temp).substr(8-extra_bits);   // removing padding bits of last byte added in encoding
            }else{
                output += char_to_binary(temp);
            }
        }

		//printf("huffman_binary: %s", output.c_str());

        output = huffman_code_to_string(output);
        output_file << output;

        printf("Decompression success\n");
        input_file.close();
        output_file.close();
    }
    catch(exception& e){
        printf("%s", e.what());;
    }
	return 0;
}