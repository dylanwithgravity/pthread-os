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
#include <map>
#include <sstream>
#include <time.h>
#include <cmath>
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
void *pinnacle(void *param);


int main(int argc, const char * argv[]) {
   
    string message = "";
    int result;
    
    // Define thread reference variable for Sifter thread
    pthread_t sifter_id;
    
    
    // Create  Sifter Thread to execute corresponding method
    result = pthread_create(&sifter_id, NULL, sifter, (void *) message.c_str());
    if(result == 0) {
        printf("Sifter Thread Created!\n");
        
    } else {
        printf("Error Creating Sifter Thread =(\n");
        return 1;
    }
    
    // wait for Sifter Thread to finish
    result = pthread_join(sifter_id, NULL);
    if(result == 0) {
        printf("Sifter Thread Joined\n");
    } else {
        printf("Error Joining Sifter Thread\n");
        return 1;
    }
    
    return 0;
}

/*
 Sifter thread method
 */
void *sifter(void *message_void_ptr) {
    
    string message;
    int total_chances = chances, result;
    pthread_t decoder_id;
    
    if(!valid_input) {
        chances--;
    }
    valid_input = true;
    
    // Ask user for input
    printf("[%d chances remaining]\nEnter a character string(press 'Return' to complete):\n", chances);
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
                printf("FAILED 1ST CHECKPOINT\n");
            } else {
                printf("PASSED 1ST CHECKPOINT\n");
                // Check message for two consecutive *s
                location = (int)user_input.find("**");
                if (location != string::npos) {
                    // Remove the two *s found at location to check again
                    user_input.replace(location, 2, "");
                    // Check user_input again for two consecutive *s
                    if (user_input.find("**") != string::npos) {
                        chances--;
                        printf("FAILED 2ND CHECKPOINT\n");
                    } else {
                        // Check message for one *
                        printf("PASSED 2ND CHECKPOINT\n");
                        location = (int)user_input.find("*");
                        if(location != string::npos) {
                            // Remove the one star found at location to check again
                            user_input.replace(location, 1, "");
                            // check user_input again for one star
                            if (user_input.find("*") != string::npos) {
                                chances--;
                                printf("FAILED 3RD CHECKPOINT\n");
                            } else {
                                printf("PASSED FINAL CHECKPOINT\n");
                            }
                        } else {
                            chances--;
                            printf("Message missing 1ST part(*)\n");
                        }
                    }
                } else {
                    chances--;
                    printf("Message missing 2ND part(**)\n");
                }
            }
        } else {
            chances--;
            printf("Message missing 3RD part(***)\n");
        }
        
    }
    

    
    
    return NULL;
}

/*
 Test decoder thread method
 */
void *decoder(void *message_void_ptr) {
    string *messaged_ptr = (string *)message_void_ptr;
    
    cout << "Decoder thread message: " << *messaged_ptr << endl;
    cout << "Decoder thread done! Success!" << endl;
    
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
 Test Pinnacle Thread method
 */
void *pinnacle(void *message_void_ptr) {
    string *message3_ptr = (string *)message_void_ptr;
    
    cout << "Pinnacle Thread message: " << *message3_ptr << endl;
    cout << "Pinnalce Thread done! Success!" << endl;
    
    
    return NULL;
}






























