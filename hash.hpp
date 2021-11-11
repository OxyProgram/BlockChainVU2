#ifndef Hash_H
#define HEADER_H
#include <string>
#include <sstream>
#include <deque>
#include <bitset>
#include <cmath>
#include <limits>
#include <iomanip>
#include <bits/stdc++.h>

using namespace std;

deque<bitset<8> > convertStringToBits(const string& input);
deque<bitset<32> > from8to32Bits(deque<bitset<8> >& input);
deque<deque<uint32_t> > generateWordBlocks(const string & input);
uint32_t operation1(uint32_t word);
uint32_t operation2(uint32_t word);
uint32_t operation3(uint32_t input1, uint32_t input2, uint32_t input3);
uint32_t operation4(uint32_t input1, uint32_t input2, uint32_t input3);
string convertToString(uint32_t h[]);

class Hash {

    public:
        Hash();
        string generateHash(const string& input);
        ~Hash();
};

Hash::Hash() {

}

Hash::~Hash() {

}

deque<uint32_t> generateMessageSchedule(deque<deque<uint32_t> > wordBlocks) {
    deque<uint32_t> message;

    for(int i = 0; i < wordBlocks.size(); ++i) {
        deque<uint32_t> ms(wordBlocks.at(i));
        for(int j = 0; j < wordBlocks.at(i).size(); ++j) {
            uint32_t block = ms[i];
            message.push_back(block);
        }
    }
    return message;
}

deque<bitset<32> > from8to32Bits(deque<bitset<8> >& input) {
    deque<bitset<32> > strBits32;
    for (int i = 0; i < input.size(); i+=4) {
        bitset<32> bits32(0);
        for (int j = i; j-i < 4 && j < input.size(); ++j) {
            for (int k = 0; k < 8; ++k) {
                bits32[(j-i)*8+k] = input[j][k];
            }
        }
        strBits32.push_back(bits32);
    }

    for(int i = strBits32.size(); i < 64; ++i) {
        bitset<32> a(0);
        strBits32.push_back(a);
    }

    return strBits32;
}

deque<deque<uint32_t> > generateWordBlocks(const string & input) {
    deque<deque<uint32_t> > wordBlocks;

    if (input.length() == 0) {
        deque<uint32_t> wordBlock;
        wordBlock.push_back(0);
        wordBlock.push_back(1);
        while (wordBlock.size() < 16) {
            wordBlock.push_back(0);
        }
        wordBlocks.push_back(wordBlock);
    }

    deque<bitset<8> > strBits8;
    for (int i = 0; i < input.length(); ++i) {
        bitset<8> charBits(input[i]);
        strBits8.push_back(charBits);
    }

    deque<bitset<32> > strBits32;
    for (int i = 0; i < strBits8.size(); i+=4) {
        bitset<32> bits32(0);
        for (int j = i; j-i < 4 && j < strBits8.size(); ++j) {
            for (int k = 0; k < 8; ++k) {
                bits32[(j-i)*8+k] = strBits8[j][k];
            }
        }
        strBits32.push_back(bits32);
    }

    strBits8.clear();

    deque<uint32_t> wordBlock;
    for (int i = 0; i < strBits32.size(); i+=16) {
        for (int j = i; j - i < 16 && j < strBits32.size(); ++j) {
            wordBlock.push_back(static_cast<int>(strBits32[j].to_ulong()));
        }

        if (wordBlock.size() < 15)
            wordBlock.push_back(1);

        while (wordBlock.size() < 16) {
            if (wordBlock.size() == 15) {
                wordBlock.push_back(input.length());
            }
            else {
                wordBlock.push_back(0);
            }
        }

        wordBlocks.push_back(wordBlock);
        wordBlock.clear();
    }

    return wordBlocks;
}

string Hash::generateHash(const string& input) {

    uint32_t h[8] = {
            static_cast<uint32_t>(sqrt(2)*pow(2, 32)),
            static_cast<uint32_t>(sqrt(3)*pow(2, 32)),
            static_cast<uint32_t>(sqrt(5)*pow(2, 32)),
            static_cast<uint32_t>(sqrt(7)*pow(2, 32)),
            static_cast<uint32_t>(sqrt(11)*pow(2, 32)),
            static_cast<uint32_t>(sqrt(13)*pow(2, 32)),
            static_cast<uint32_t>(sqrt(17)*pow(2, 32)),
            static_cast<uint32_t>(sqrt(19)*pow(2, 32))
    };

    deque<deque<uint32_t> > wordBlocks = generateWordBlocks(input);
    deque<uint32_t> ms = generateMessageSchedule(wordBlocks);

    for (int i = 0; i < wordBlocks.size(); ++i) {

        uint32_t t1, t2;
        for (int j = 0; j < ms.size(); ++j) {
            t1 = operation1(h[4]) + operation3(h[4], h[5], h[6]) + h[7] + ms[j] + h[j%8];
            t2 = operation2(h[0]) + operation4(h[0], h[1], h[2]);

            for (int k = 1; k < 8; ++k) {
                h[k] = h[k-1];
            }

            h[0] = t1 + t2;
            h[4] += t1;
        }


        for(int j = 0; j < wordBlocks.at(i).size(); ++j) {
            h[j%8] = ms.at(j) ^ h[j%8];
        } 
    }

    return convertToString(h);

}

string convertToString(uint32_t h[]) {
    string output = "";
    for (int i = 0; i < 8; ++i) {
        bitset<32> word(h[i]);
        for (int j = 0; j < 8; ++j) {
            bitset<4> hexChar(0);
            for (int k = 0; k < 4; ++k) {
                hexChar[k] = word[j*4+k];
            }
            stringstream ss;
            ss << hex << hexChar.to_ulong();
            output += ss.str();
            ss.clear();

        }


    }
    return output;
}

uint32_t operation1(uint32_t input) {
    uint32_t a = _rotr(input, 7);
    uint32_t b = input >> 2;
    uint32_t c = _rotr(input, 5);

    return a^b^c;
}
uint32_t operation2(uint32_t input) {
    uint32_t a = input >> 7;
    uint32_t b = _rotr(input, 20);
    uint32_t c = _rotr(input, 17);

    return a^b^c;
}

uint32_t operation3(uint32_t input1, uint32_t input2, uint32_t input3) {
    bitset<32> b1(input1);
    bitset<32> b2(input2);
    bitset<32> b3(input3);
    bitset<32> output;

    for (int i = 0; i < 32; ++i) {
        if (b1[i]){
            output[i] = b2[i];
        } 
        else {
            output[i] = b3[i];
        }
    }
    return static_cast<uint32_t>(output.to_ulong());
}

uint32_t operation4(uint32_t input1, uint32_t input2, uint32_t input3) {
    bitset<32> b1(input1);
    bitset<32> b2(input2);
    bitset<32> b3(input3);
    bitset<32> output;

    for (int i = 0; i < 32; ++i) {
        if ((b1[i]&&b2[i])||(b1[i]&&b3[i])||(b2[i]&&b3[i])) {
            output[i] = 1;
        } else {
            output[i] = 0;
        } 
    }

    return static_cast<uint32_t>(output.to_ulong());
}


#endif