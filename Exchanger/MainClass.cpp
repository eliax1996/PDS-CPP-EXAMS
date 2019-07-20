#include <iostream>
#include <future>
#include <mutex>
#include <thread>
#include <vector>
#include <string>
#include <sstream>
#include "exchanger.cpp"

void output(int val){
    std::ostringstream ss;
    ss << std::this_thread::get_id();
    std::string idstr = ss.str();
    std::string str = idstr + std::string("-") + std::to_string(val)  + '\n';
    
    std::cout << str;
}


void excecute(int ex,Exchanger<int>& exc) {
    
    output(ex);
    
    exc.exchange(ex);
    
    output(ex);
    
}

std::thread thread_generator(int ex,Exchanger<int>& exc){
    std::thread t([ex,&exc]{
        excecute(ex, exc);
    });
    
    return t;
}

int main(){
    std::vector<std::thread> threads;
    Exchanger<int> exchanger{};
    int var1=11,var2=15;
    
    threads.push_back(thread_generator(var1,exchanger));
    threads.push_back(thread_generator(var2,exchanger));
    threads[0].join();
    threads[1].join();
}
