#ifndef HASH_H
#define HASH_H

#include <iostream>
#include <cmath>
#include <random>
#include <chrono>

typedef std::size_t HASH_INDEX_T;

struct MyStringHash {
    HASH_INDEX_T rValues[5] { 983132572, 1468777056, 552714139, 984953261, 261934300 };
    MyStringHash(bool debug = true)
    {
        if(false == debug){
            generateRValues();
        }
    }
    // hash function entry point (i.e. this is h(k))
    HASH_INDEX_T operator()(const std::string& k) const
    {
        unsigned long long w_vals[5] = {0, 0, 0, 0, 0};


        int len = (int)k.length();
        int numLeft = len;
        int str_index = 0;

        int w_index = 4;
        if (len % 6 == 0){
            w_index = 5 - (len/6);
        } else {
            w_index = 5 - (len/6) - 1;
        }

        while(numLeft > 0){
            int toUse;
            std::string substr;

            //get first substring which may be short
            if (numLeft == len && len % 6 != 0){
                toUse = len % 6;
                substr = k.substr(str_index,toUse);
                numLeft -= (len % 6);
                str_index += (len % 6);
            } else {
                toUse = 6;
                substr = k.substr(str_index,toUse);
                numLeft -= 6;
                str_index += 6;
            }

            unsigned long long this_w = w(substr);
            //std::cout << this_w << std::endl;
            w_vals[w_index] = this_w;
            w_index += 1;
        }

        unsigned long long answer = 0;

        //now there is a list of unsigned longs
        for (int i = 0; i < 5; i++){
            //std::cout << rValues[i] << " " << w_vals[i] << std::endl;
            answer += (rValues[i] * w_vals[i]);
        }


        return answer;
        // Add your code here
    }

    //takes 6 letters
    unsigned long long w(const std::string& substr) const
    {
        HASH_INDEX_T a[6] = {0, 0, 0, 0, 0, 0};

        int a_index = 5;
        for (int i = (int)substr.length() - 1; i >= 0; i--){
            a[a_index] = letterDigitToNumber(substr[i]);
            a_index--;
        }

        unsigned long long answer = ((((((((((a[0]*36)+a[1])*36)+a[2])*36)+a[3])*36)+a[4])*36)+a[5]);

        return answer;
    }

    // A likely helper function is to convert a-z,0-9 to an integral value 0-35
    HASH_INDEX_T letterDigitToNumber(char letter) const
    {
        HASH_INDEX_T conversion;
        if (letter >= 65 && letter <= 90){ //uppercase
            letter = letter + 32;
            conversion = (HASH_INDEX_T)letter - 97;
        } else if (letter >= 97 && letter <= 122){ //lowercase
            conversion = (HASH_INDEX_T)letter - 97;
        } else if (letter >= 48 && letter <= 57){ //number
            conversion = (HASH_INDEX_T)letter - 22;
        }
        return conversion;

    }

    // Code to generate the random R values
    void generateRValues()
    {
        // obtain a seed from the system clock:
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 generator (seed);  // mt19937 is a standard random number generator

        // Simply call generator() [it has an operator()] to get another random number
        for(int i{ 0 }; i < 5; ++i)
        {
            rValues[i] = generator();
        }
    }
};

#endif
