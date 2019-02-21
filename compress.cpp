#include <stdio.h>
#include <map>
#include <bitset>
#include <fstream>
#include <exception>
#include <math.h>

using namespace std;


struct node{
    int frequency;
    char value;
    struct node *left;
    struct node *right;
    node(int frequency, char value, struct node* left, struct node* right){
        this->frequency = frequency;
        this->value = value;
        this->left = left;
        this->right = right;
    }
};

node *head = nullptr;

map<char, string> huffman_char_code_map;
multimap<int, node*> frequency_node_map;
map<char, int> char_frequency_map;

// string char_to_binary(char input){
//     return bitset<8>(input).to_string();
// }

int binary_to_decimal(string input){
    int output = 0, b = 1;
    for(int i=input.length()-1;i>-1;--i){
        if(input[i] == '1')
            output += b;
        b *= 2;
    }
    return output;
}

map<char, int> char_count(string input){
    map<char, int> output;
    for(int i: input)
        output[i]++;
    return output;
}

void build_huffman_char_code_map(node* n, string st){
    if(n && !n->left && !n->right){
        huffman_char_code_map[n->value] = st;   // assigning custom code to character
    }
    else if(n){
        build_huffman_char_code_map(n->left, st + "0");
        build_huffman_char_code_map(n->right, st + "1");
    }
}

string string_to_huffman_binary(string input){
    string output;
    for(auto i: input)
        output += huffman_char_code_map[i];
    return output;
}

void build_huffman_tree(){
	if(frequency_node_map.size() == 1){
        head = new node(1, '*', new node(1, frequency_node_map.begin()->second->value, nullptr, nullptr), nullptr);
    }
    else
		while(frequency_node_map.size()>1){
			auto temp = frequency_node_map.begin();
			node *l = temp->second;
			frequency_node_map.erase(frequency_node_map.begin());
			temp = frequency_node_map.begin();
			node *r = temp->second;
			frequency_node_map.erase(frequency_node_map.begin());
			head = new node(l->frequency+r->frequency, '*', l, r);
			frequency_node_map.insert({head->frequency, head});
		}
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
    file_name_obj.extension = input.substr(i+1, input.length());
    return file_name_obj;
}

int main(int n, char ** input_file_name){
    try{
        ifstream input_file(input_file_name[1], ios::binary);
        file_name file_name_obj = get_output_file_name(input_file_name[1]);
        string output_file_name = file_name_obj.file_name;
        string output_file_extension = file_name_obj.extension;
        string extension = ".huff";
        ofstream output_file(output_file_name + extension, ios::binary);
        if(!input_file.is_open()){
            printf("error opening input file\n");
            return 1;
        }
        char temp;
        string input;
        input_file >> noskipws;
        while(input_file >> temp){
            input += temp;
        }
        if(!output_file.is_open()){
            printf("error opening output file\n");
            return 2;
        }
        printf("Compressing...\n");
        char_frequency_map = char_count(input);
        output_file << output_file_extension << " "; //original file extension
        
        for(auto i: char_frequency_map){
            frequency_node_map.insert({i.second, new node(i.second, i.first, nullptr, nullptr)});
            //output_file << " " << i.first << i.second;	//	character and frequency
        }

        build_huffman_tree();   

        build_huffman_char_code_map(head, "");

		output_file << huffman_char_code_map.size() << " ";  //length of chars

		for(auto &i: huffman_char_code_map){
			output_file << i.first << i.second << " ";
		}

        //string original_binary = string_to_binary(input);
		
        string huffman_binary = string_to_huffman_binary(input);
        output_file << (int)ceil(huffman_binary.length()/8.0) << " "; // length of characters
		string output_check = "";
        int extra_bits = huffman_binary.length()%8;
        output_file << extra_bits << " "; // extra bits count (extra bits left when total bits converted to bytes = total_bits % 8) this is needed for decoding the last byte correctly while decompressing
        for(int i=0;i<huffman_binary.length();){
            int c=0;
            string byte;
            while(c<8 && i<huffman_binary.length()){
				output_check += huffman_binary[i];
                byte += huffman_binary[i++];
                ++c;
            }
            char ch = binary_to_decimal(byte);
            output_file << ch;
        }
        //double comp_rate = ((double)huffman_binary.length()/original_binary.length())*100;
        printf("compression success\n");
        input_file.close();
        output_file.close();
    }
    catch(exception& e){
        printf("%s", e.what());
    }
	return 0;
}


/*
Output file format

Extension_of_original_file number_of_characers_in_dictionary (characterfrequency )... number_of_characters_in_encoded_string extra_bit_count encoded_string

*/