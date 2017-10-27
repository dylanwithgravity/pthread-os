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
#include <regex>


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
void *pinnacol(void *param);


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
                                // Check if Parts are Null
                                if(regex_match(message, regex("[* ]*"))) {
                                cout << " you entered in null" << endl;
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
   /*     // Reset User chances to 3
        chances = 3;
        int i = 0;
        while(i < 2) {
            printf("\n");
            i++;
        }
        sifter(param); */
    }
    
    

    
    
    return NULL;
}

/*
 Test decoder thread method
 */
void *decoder(void *param) {
    string message;
    string part1;
    string part2;
    string part3;
    string temp;
    
    
    if(param) {
        message = (char*) param;
        temp = message;
        // Erase anything before Single * if it is the first start encountered
        message.erase(message.begin(), message.begin() + message.find('*'));
        
        // Locations
        int loc_star3;
        int loc_star2;
        int loc_star1;
        
        // Check if User inputs only stars and spaces in
 /*       loc_star3 = (int)message.find("***");
        temp.replace(loc_star3, 3, "___");
        
        loc_star2 = (int)message.find("**");
        temp.replace(loc_star2, 2, "__");
        
        loc_star1 = (int)message.find("*");
      */
        // Regex to check if pattern is only stars and spaces
       
        
        
    }
    return NULL;
}

/*
 Test fence thread method
 */
void *fence(void *message_void_ptr) {
    string *message1_ptr = (string *)message_void_ptr;
    
    cout << "Fence Thread message: " << *message1_ptr << endl;
    cout << "Fence Thread done! Success!" << endl;
    
    
    return NULL;
}

/*
 Test Hill Thread method
 */
void *hill(void *message_void_ptr) {
    string *message2_ptr = (string *)message_void_ptr;
    
    cout << "Hill Thread message: " << *message2_ptr << endl;
    cout << "Hill Thread done! Success!" << endl;
    
    
    return NULL;
}

/*
 Test Pinnacol Thread method
 */
void *pinnacol(void *message_void_ptr) {
    string *message3_ptr = (string *)message_void_ptr;
    
    cout << "Pinnacol Thread message: " << *message3_ptr << endl;
    cout << "Pinnacol Thread done! Success!" << endl;
    
    
    return NULL;
}






























