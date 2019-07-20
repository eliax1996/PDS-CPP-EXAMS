//
//  exchanger.cpp
//  malnati
//
//  Created by Elia Migliore on 19/07/2019.
//  Copyright © 2019 Elia Migliore. All rights reserved.
//

#include <iostream>
#include <mutex>
#include <thread>

template <typename T>

class Exchanger {
    T exchanged;
    bool is_exchanged;
    std::mutex m_protect_data;
    std::mutex m_protect_sleep;
    std::condition_variable cv;
    
public:
    
    Exchanger() : is_exchanged(false) {
        
    }
    
    void exchange(T& exchanged) {
        std::unique_lock<std::mutex> l_s(m_protect_data);
        
        
        if (is_exchanged){
            std::swap(exchanged, this->exchanged);
            is_exchanged=false;
            cv.notify_one();
        } else {
            
            // exchange the item and set to true the variable
            // that represent the exchanged flag
            this->exchanged = exchanged;
            is_exchanged = true;
            
            // release the lock for protecting the variable
            //l.unlock();
            
            // wait for the other thread to wake me up when
            // another element has arrived and the exchanged
            // item is exchanged
            
            std::cout << "going to sleep" << std::endl;
            
            // unique lock per il paradisma RAII
            //std::unique_lock<std::mutex> l_s(m_protect_data);
            while(!is_exchanged) cv.wait(l_s);
            
            //std::swap(exchanged, this->exchanged);
            exchanged = this->exchanged;
            
            // now RAII paradigm release the second lock for me
        }
        // output to indicate the success
        std::cout << "element to exchange is: " << typeid(exchanged).name() << std::endl;
        
        
        // now RAII paradigm release the first lock for me
    }
    
    
};
