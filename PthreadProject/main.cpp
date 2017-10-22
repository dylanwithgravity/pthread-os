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

using namespace std;

int chances = 3;

/* functions used by threads to start execution */
void *sifter(void *param);
void *decoder(void *param);
void *fence(void *param);
void *hill(void *param);
void *pinnacle(void *param);


int main(int argc, const char * argv[]) {
    
    string message0 = "*first**second**third";
    string message1 = "*first";
    string message2 = "**second";
    string message3 = "***third";
    int result;
    
    
    
    
    /* Define thread reference variables for each thread being created */
    pthread_t sifterThread;
    pthread_t decoder;
    pthread_t fenceThread;
    pthread_t hillThread;
    pthread_t pinnacleThread;
    
    /* Create Thread to execute corresponding method */
    
    if(pthread_create(&sifterThread, NULL, sifter, &message0)) {
        cout << "Error creating thread" << endl;
        return 1;
    }
    
    pthread_create(&fenceThread, NULL, fence, &message1);
    pthread_create(&hillThread, NULL, hill, &message2);
    pthread_create(&pinnacleThread, NULL, pinnacle, &message3);
    
    
    
    /*wait for thread to finish */
    if(pthread_join(sifterThread, NULL)) {
        cout << "Error joining thread" << endl;
        
        return 1;
    }
    pthread_join(fenceThread, NULL);
    pthread_join(hillThread, NULL);
    pthread_join(pinnacleThread, NULL);
    
    
    cout << "Enter ciphered message: ";
    cin >> result;
    
    
    cout << "You entered: " << result << endl;
    
    
    
    
    
    return 0;
}

/* Test sifter thread method */
void *sifter(void *message_void_ptr) {
    string *message0_ptr = (string *)message_void_ptr;
    
    cout << "Sifter Thread message: " << *message0_ptr << endl;
    cout << "Sifter Thread done! Success!" << endl;
    
    
    return NULL;
}

/* Test decoder thread method */
void *decoder(void *message_void_ptr) {
    string *messaged_ptr = (string *)message_void_ptr;
    
    cout << "Decoder thread message: " << *messaged_ptr << endl;
    cout << "Decoder thread done! Success!" << endl;
}

/* Test fence thread method */
void *fence(void *message_void_ptr) {
    string *message1_ptr = (string *)message_void_ptr;
    
    cout << "Fence Thread message: " << *message1_ptr << endl;
    cout << "Fence Thread done! Success!" << endl;
    
    
    return NULL;
}

/* Test Hill Thread method */
void *hill(void *message_void_ptr) {
    string *message2_ptr = (string *)message_void_ptr;
    
    cout << "Hill Thread message: " << *message2_ptr << endl;
    cout << "Hill Thread done! Success!" << endl;
    
    
    return NULL;
}

/* Test Pinnacle Thread method */
void *pinnacle(void *message_void_ptr) {
    string *message3_ptr = (string *)message_void_ptr;
    
    cout << "Pinnacle Thread message: " << *message3_ptr << endl;
    cout << "Pinnalce Thread done! Success!" << endl;
    
    
    return NULL;
}






























