//
//  main.cpp
//  PthreadProject
//
//  Created by Dylan Williamson on 10/20/17.
//  Copyright © 2017 Dylan Williamson. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

using namespace std;


// Number of chances user gets to input correct data
int chances = 3;

// all parts recieved sufficent parameters
bool valid_input = true;

// functions used by threads to start execution
void *sifter(void *param);
void *decoder(void *param);
void *fence(void *param);
void *hill(void *param);
void *pinnacle(void *param);


int main(int argc, const char * argv[]) {
   
    string message = "";
    
    
    // Define thread reference variable for Sifter thread
    pthread_t sifter_id;
    
    
    // Create  Sifter Thread to execute corresponding method
   
    if(pthread_create(&sifter_id, NULL, sifter, (void *) message.c_str())) {
        printf("Error Creating Sifter Thread =(\n");
    } else {
        printf("Sifter Thread Created!\n");
        
    }
    
    // Wait for Sifter Thread to finish
    if(pthread_join(sifter_id, NULL)) {
        printf("Error Joining Sifter Thread\n");
    } else {
        printf("Sifter Thread Joined\n");
        
    }
    
    return 0;
}

/*
 Sifter thread method
 Sifter asks user to enter a ciphered message as input. User gets 3 chances to enter valid message. If valid message is received sifter creates a thread called decoder and passed decoder the message.
 */
void *sifter(void *param) {
    
    string message;
    
    int total_chances = chances;
    
    // Define thread reference variable for Decoder Thread
    pthread_t decoder_id;
    
    if(!valid_input) {
        chances--;
    }
    valid_input = true;
    
    // Ask user for input
    printf("You have %d chances\nEnter a character string(press 'Enter' to submit):\n", chances);
    getline(cin, message);
    
    // Check if message has four consecutive *s
    if(message.find("****") == string::npos) {
        string user_input = message;
        // Check message for three conescutive *s
        int location = (int)message.find("***");
        if (location != string::npos) {
            // Remove the three consecutive *s found at location to check again
            user_input.replace(location, 3, "");
            // Check user_input again for three consecutive *s
            if (user_input.find("***") != string::npos) {
                chances--;
                } else {
                // Check message for two consecutive *s
                location = (int)user_input.find("**");
                if (location != string::npos) {
                    // Remove the two *s found at location to check again
                    user_input.replace(location, 2, "");
                    // Check user_input again for two consecutive *s
                    if (user_input.find("**") != string::npos) {
                        chances--;
                      } else {
                        // Check message for one *
                        location = (int)user_input.find("*");
                        if(location != string::npos) {
                            // Remove the one star found at location to check again
                            user_input.replace(location, 1, "");
                            // check user_input again for one star
                            if (user_input.find("*") != string::npos) {
                                chances--;
                            } else {
                                // Create Decoder Thread
                                if(pthread_create(&decoder_id, NULL, decoder, (void *) message.c_str())) {
                                    printf("Error creating Decoder Thread =(\n");
                                    // wait for Decoder thread to finish
                                } else {
                                    printf("Decoder Thread Created!\n");
                                    // Wait for Decoder thread to finish
                                    if(pthread_join(decoder_id, NULL)) {
                                        printf("Error Joining decoder thread\n");
                                    } else {
                                        printf("Decoder thread joined!\n");
                                    }
                                }
                            }
                            
                        } else {
                            chances--;
                        }
                    }
                } else {
                    chances--;
                }
            }
        } else {
            chances--;
        }
    } else {
        chances--;
    }
    
    // Determine if User still has chances
    if(chances < total_chances) {
        if(chances == 0){
            printf("You've used up all of your chances\n");
        return NULL;
        }
        int i = 0;
        while(i < 2) {
            printf("\n");
            i++;
        }
        printf("Invalid Input, Please try again\n");
        sifter(param);
    } else {
        // Reset User chances to 3
        chances = 3;
        int i = 0;
        while(i < 2) {
            printf("\n");
            i++;
        }
        sifter(param);
    }
    
    

    
    
    return NULL;
}

/*
 Test decoder thread method
 Decoder takes the message from sifter and divides it into its 3 parts each part seperated by asterisks(*, **, ***) the 3 parts are then fed to 3 seperate threads(Fence, Hill, and Pinnacle) to be decoded.
 */
void *decoder(void *param) {
    string message;
    string part1;
    string part2;
    string part3;
    string temp;
    
    pthread_t fence_id;
    pthread_t hill_id;
    pthread_t pinnacle_id;
    
    vector<pthread_t> threads;
    
    if(param) {
        message = (char*) param;
        temp = message;
        // Erase anything before Single * if it is the first start encountered
        message.erase(message.begin(), message.begin() + message.find('*'));
        
        // Locations
        int loc_star3;
        int loc_star2;
        int loc_star1;
        
        // Find location of stars
        loc_star3 = (int)message.find("***");
        temp.replace(loc_star3, 3, "___");
        
        loc_star2 = (int)temp.find("**");
        temp.replace(loc_star2, 2, "__");

        loc_star1 = (int)temp.find("*");

       /*
        Get Pinnacol Message
        */
        
        //Check if Pinnacol Message is at the front
        if(loc_star3 == 0) {
            message.erase(message.begin(), message.begin() + 3);
            part3 = message.substr(0, message.find('*'));
            message.erase(message.begin(), message.begin() + message.find('*'));
        }
        // Check if Pinnacle Message is in the middle
        else if(loc_star3 < loc_star1 || loc_star3 < loc_star2) {
            message.erase(message.begin() + loc_star3, message.begin() + loc_star3 + 3);
            part3 = message.substr(loc_star3, message.find('*', loc_star3) - loc_star3);
            message.erase(message.begin() + loc_star3, message.begin() + message.find('*', loc_star3));
        }
        // If not middle, then must be last
        else {
            message.erase(message.begin() + loc_star3, message.begin() + loc_star3 + 3);
            part3 = message.substr(loc_star3, message.size());
            message.erase(message.begin() + loc_star3, message.end());
        }
        // Check if Part 3 is NULL
        if(part3[0] == ' ') {
            chances--;
            return NULL;
        }
        
        /*
         Get Hill Message
         */
        
        //Check if Hill Message is at front
        if(loc_star2 == 0) {
            message.erase(message.begin(), message.begin() + 2);
            part2 = message.substr(0, message.find('*'));
            message.erase(message.begin(), message.begin() + message.find('*'));
        }
        // If not first, then must be last
        else {
            message.erase(message.begin() + loc_star2, message.begin() + loc_star2 + 2);
            part2 = message.substr(loc_star2, message.size());
            message.erase(message.begin() + loc_star2, message.end());
        }
        // Check if Part 2 is NULL
        if(part2[0] == ' ') {
            chances--;
            return NULL;
        }
        
        /*
         Get Fence Message
         */
        // Only Fence Message remains
        message.erase(message.begin(), message.begin() + 1);
        part1 = message;
        // Check if Part 1 is NULL
        if(part1[0] == ' ') {
            chances--;
            return NULL;
        }
        
        /*
        cout << "p1: " << part1 << endl;
        cout << "p2: " << part2 << endl;
        cout << "p3: " << part3 << endl;
        */
        
        // Create Fence, Hill, and Pinnacle Threads
        if(pthread_create(&fence_id, NULL, fence, (void *) part1.c_str())) {
            cout << "Fence thread  not created" << endl;
        } else {
            cout << "Fence Thread Created" << endl;
        }
        //threads.push_back(fence_id);
        pthread_join(fence_id, NULL);
        
        if(pthread_create(&hill_id, NULL, hill, (void *) part2.c_str())) {
            cout << "Hill thread  not created" << endl;
        } else {
            cout << "Hill Thread Created" << endl;
        }
       // threads.push_back(hill_id);
        pthread_join(hill_id, NULL);
        
        if(pthread_create(&pinnacle_id, NULL, pinnacle, (void *) part3.c_str())) {
            cout << "Pinnacle thread  not created" << endl;
        } else {
            cout << "Pinnacle Thread Created" << endl;
        }
        //threads.push_back(pinnacle_id);
        pthread_join(pinnacle_id, NULL);
       
    }
    return NULL;
}

/*
 Test fence thread method
 Fence thread checks if parts received is valid:
 - No repition of digits in first part of message
 - First part ends when repetition of digits is encountered or a character is encountered
 - Highest Digit in first part dictates # of digits in first part. 0 Not included
 - Largest digit can be anywhere in the first part
 - Add spaces to second part R(second part of message) if R is not divisble by N(largest digit)
 - R is partitioned such that each section has K characters where K = R/N
 - Vertically rearrange sections to produce matrix
 - Rerrange columns using order of digits from first part
 */
void *fence(void *param) {
    string fence_crypt = (char *)param;
    string fence_decrypt;
    string digits;
    string chars;
    int n = 0;
    int k = 0;
    
    
    
    // Check if no numbers in String
    
    if(!isdigit(fence_crypt.at(0))){
        printf("Invalid Fence Thread\n");
        return NULL;
    }
    
    
    // Get all digits in first part
    int num_loc = 0;
    while(num_loc < fence_crypt.length() && (isdigit(fence_crypt.at(num_loc)))) {
        if(fence_crypt.at(num_loc) == '0') {
            printf("Invalid Fence Thread\n");
            return NULL;
        } else {
        digits += fence_crypt.at(num_loc);
        num_loc++;
        }
    }
    
    //Cut where duplicate occurs
    for(int i = 0; i < digits.length() - 1; i++){
        for(int j = i+1; j < digits.length(); j++){
            if(digits[i] == digits[j]){
                digits.erase(digits.begin() + j, digits.end());
            }
        }
    }
    
    // Determine largest digit
   int num = 0;
    char x[1];
    for(int i = 0; i < digits.size();i++){
        x[0] = digits[i];
        num = atoi(x);
        if(num > n) {
            n = num;
        }
    }
    
    // Check if there are N digits
    if(n != digits.size()) {
        printf("Invalid fence thread\n");
        return NULL;
    }
    
    // get 2nd part
    chars = fence_crypt;
    chars.erase(chars.begin(), chars.begin() + num_loc);

    
    // Find out what K is
    if((int)chars.size() % n == 0){
        k = (int)chars.size() / n;
    } else {
    while(chars.size() % n != 0) {
        chars.append(" ");
        }
        k = (int)chars.size() / n;
    }
    
    // Create and fill matrix with characters
    char message_array[k][n];
    char decrypted_fence[k][n];
    int i = 0;
    for(int col = 0; col < n; col++){
        for(int row = 0; row < k; row++, i++){
            message_array[row][col] = chars.at(i);
        }
    }
    
    // Transfer string of digits to array of int
    int digit_array[n];
    for(int i = 0; i < n; i++) {
        digit_array[i] = digits[i] - '0';
    }
    
    
    int pos_one = 0;
    int pos_two = 0;
    int counter = 1;
    
    
    // Fill decrypted Matrix
    while(counter <= n) {
        if(pos_one > n) {
            pos_one = 0;
        }
        if(digit_array[pos_one] == counter) {
            string temp = "";
            pos_two = counter - 1;
            for(int z = 0; z < k; z++) {
                temp += message_array[z][pos_two];
            }
            
            for(int r = 0; r < k; r++) {
                decrypted_fence[r][pos_one] = temp.at(r);
            }
            counter++;
            pos_one = 0;
        }
        pos_one++;
    }
    
    // Transfer values from decrypted matrix to string
    for(int i = 0; i < k; i++){
        for(int j = 0; j < n; j++) {
            fence_decrypt += decrypted_fence[i][j];
        }
    }
    
    
    printf("Fence decrypted message: %s \n", fence_decrypt.c_str());
    
    
    return NULL;
}

/*
 Hill Thread method
 - 2 parts
 - digits have spaces and can be larger then 9
 - deciphers 2 characters at a time
 - character generate 2x1 matrix with values corresponding to location in alphabet
 - Only use first 4 numbers
 - Build matrix with the 4 numbers, 2x2
 - Multiply the two matrices then the result is modded by 26
 - resulting vector is location of decrypted characters
 - Must have atleast 4 values in 2nd part or invalid
 - 1st part must have even # of characters to be valid
 */
void *hill(void *param) {
    string hill_crypt = (char *)param;
    string hill_decrypt;
    string chars;
    vector<int> nums;
    int c[2];
    int s[2][2];
    
    // Get Characters from 1st part of String
    int alpha_loc = 0;
    while(alpha_loc < hill_crypt.length() && (isalpha(hill_crypt.at(alpha_loc)) || hill_crypt.at(alpha_loc) == ' ')) {
        if (hill_crypt.at(alpha_loc) != ' ') {
            chars += hill_crypt.at(alpha_loc);
        }
        alpha_loc++;
    }
    
    // Check if even amount of Chars
    if(chars.length() % 2 != 0) {
        printf("Invalid Hill message\n");
        return NULL;
    }
    
    cout << chars << endl;
    
    //Check rest of string for Characters and get numbers
    for(int i = alpha_loc; i < hill_crypt.length(); i++) {
        if(isalpha(hill_crypt.at(i))){
        printf("Invalid Hill message\n");
            return NULL;
        }
        if(i < hill_crypt.length() && hill_crypt.at(i) != ' ') {
            string temp;
            while(i < hill_crypt.length() && hill_crypt.at(i) != ' ') {
                temp += hill_crypt.at(i);
                i++;
            }
            if(!temp.empty() && nums.size() < 4) {
                nums.push_back(stoi(temp));
            }
        }
    }
    
    // Check if vector has atleast 4 nums
    if(nums.size() != 4) {
        printf("Invalid Hill message\n");
        return NULL;
    }
    
    // Fill 2x2 Matrix, S, with first 4 Numbers from message
    for(int row = 1; row  >= 0; row--) {
        for(int col = 1; col >= 0; col--) {
            s[row][col] = nums.back();
            nums.pop_back();
        }
    }
    
    // Convert Letters to Position number and Postion to Letters
    
    for(int i = 0; i < chars.length(); i = i + 2) {
        if(isupper(chars[i])) {
            c[0] = chars[i] - 65;
        } else {
            c[0] = chars[i] - 97;
        }
        if(isupper(chars[i+1])) {
            c[1] = chars[i+1] - 65;
        } else {
            c[1] = chars[i+1] - 97;
        }
        hill_decrypt += (65 + (((s[0][0] * c[0] + s[0][1] * c[1]) % 26 + 26) % 26));
        hill_decrypt += (65 + (((s[1][0] * c[0] + s[1][1] * c[1]) % 26 + 26) % 26));
    }
   
   printf("Hill decrypted message: %s \n", hill_decrypt.c_str());
    return NULL;
}

/*
 Test Pinnacle Thread method
 - 2 parts
 - digits have spaces and can be larger then 9
 - deciphers 3 characters at a time
 - character generate 3x1 matrix with values corresponding to location in alphabet
 - Only use first 3 numbers
 - Build matrix with the 9 numbers, 3x3
 - Multiply the two matrices then the result is modded by 26
 - resulting vector is location of decrypted characters
 - Must have atleast 9 values in 2nd part or invalid
 */
void *pinnacle(void *param) {
    string pinnacle_crypt = (char *)param;
    string pinnacle_decrypt;
    string chars;
    vector<int> nums;
    int c[3];
    int s[3][3];
    
    
    
    
    
    // Get Characters from 1st part of String
    int alpha_loc = 0;
    while(alpha_loc < pinnacle_crypt.length() && (isalpha(pinnacle_crypt.at(alpha_loc)) || pinnacle_crypt.at(alpha_loc) == ' ')) {
        if (pinnacle_crypt.at(alpha_loc) != ' ') {
            chars += pinnacle_crypt.at(alpha_loc);
        }
        alpha_loc++;
    }
    
    // Check if even amount of Chars
    if(chars.length() % 3 != 0) {
        printf("Invalid Pinnacle message\n");
        return NULL;
    }
    
    //Check rest of string for Characters and get numbers
    for(int i = alpha_loc; i < pinnacle_crypt.length(); i++) {
        if(isalpha(pinnacle_crypt.at(i))){
            printf("Invalid Hill message\n");
            return NULL;
        }
        if(i < pinnacle_crypt.length() && pinnacle_crypt.at(i) != ' ') {
            string temp;
            while(i < pinnacle_crypt.length() && pinnacle_crypt.at(i) != ' ') {
                temp += pinnacle_crypt.at(i);
                i++;
            }
            if(!temp.empty() && nums.size() < 9) {
                nums.push_back(stoi(temp));
            }
        }
    }
    
    // Check if vector has atleast 4 nums
    if(nums.size() != 9) {
        printf("Invalid Pinnacle message\n");
        return NULL;
    }
    
    // Fill 3x3 Matrix, S, with first 4 Numbers from message
    for(int row = 2; row  >= 0; row--) {
        for(int col = 2; col >= 0; col--) {
            s[row][col] = nums.back();
            nums.pop_back();
        }
    }
    
    // Convert Chars to Position number and Postion to Letters
    
    for(int i = 0; i < chars.length(); i = i + 3) {
        if(isupper(chars[i])) {
            c[0] = chars[i] - 65;
        } else {
            c[0] = chars[i] - 97;
        }
        if(isupper(chars[i+1])) {
            c[1] = chars[i+1] - 65;
        } else {
            c[1] = chars[i+1] - 97;
        }
        if(isupper(chars[i+2])) {
            c[2] = chars[i+2] - 65;
        } else {
            c[2] = chars[i+2] - 97;

        }
        pinnacle_decrypt += (65 + (((s[0][0] * c[0] + s[0][1] * c[1] + s[0][2] * c[2]) % 26 + 26) % 26));
        pinnacle_decrypt += (65 + (((s[1][0] * c[0] + s[1][1] * c[1] + s[1][2] * c[2]) % 26 + 26) % 26));
        pinnacle_decrypt += (65 + (((s[2][0] * c[0] + s[2][1] * c[1] + s[2][2] * c[2]) % 26 + 26) % 26));
        
    }
    
    printf("Pinnacle decrypted message: %s \n", pinnacle_decrypt.c_str());
    return NULL;
}

