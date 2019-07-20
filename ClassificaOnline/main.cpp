//
//  main.cpp
//  ClassificaOnline
//
//  Created by Elia Migliore on 19/07/2019.
//  Copyright © 2019 Elia Migliore. All rights reserved.
//

#include <iostream>
#include <vector>
#include <shared_mutex>
#include <iterator>
//#include <bits/stdc++.h>
#include "stdc++.h"


class Giocatore {
public:
    Giocatore(std::string name, int punti) {
        this->nickname = name;
        this->punteggi = punti;
    }

    void setPunti(int punti){
        if (punti>punteggi){
            this->punteggi = punti;
        }
    }

    bool operator < (const Giocatore g) const {
        return this->punteggi < g.punteggi;
    }

    bool operator > (const Giocatore g) const {
        return this->punteggi > g.punteggi;
    }

    // dio bastardo non dimenticare più il const
    // nell'override degli operatori, non possono
    // cambiare un cazzo sull'oggetto a cui si applicano!
    // CONST SIA NELL'OGGETTO CHE DOPO LA SIGNATURE!!
    bool operator == (const Giocatore g) const {
        return (g.nickname == this->nickname);
    }

    std::string nickname;
    int punteggi;
};

class Classifica {
private:
    std::shared_mutex sm;
    std::mutex m;
    std::vector<std::unique_ptr<Giocatore>>vect;
    std::condition_variable cv;

    bool cambiamenti;

public:
    Classifica() : vect(),cambiamenti(false),cv() {}


    std::unique_ptr<std::vector<std::unique_ptr<Giocatore>>> getClassifica(){
        std::shared_lock<std::shared_mutex> l(sm);
        // return copy of classifica

        std::unique_ptr<std::vector<std::unique_ptr<Giocatore>>> ret = std::make_unique<std::vector<std::unique_ptr<Giocatore>>>();

        std::copy(vect.begin(),vect.end(),std::back_inserter(*ret));

        return ret;
    }

    void insertPunteggio(std::string name,int punteggio){
        std::unique_lock<std::shared_mutex> l(sm);

        auto gamer = std::make_unique<Giocatore>(name,punteggio);
        auto res = std::upper_bound(vect.begin(),vect.end(),gamer);

        if (**res==*(gamer.get())){
            (*res)->punteggi=punteggio;
        } else {
            // difference between .push_back and emplace_back is
            // that emplace_back forward the argument while push_back
            // generate a copy and after put into the memory
            vect.emplace_back(std::make_unique<Giocatore>(name,punteggio));
        }

        std::sort(vect.begin(),vect.end(),[](std::unique_ptr<Giocatore>& g1,std::unique_ptr<Giocatore>& g2) -> bool {
            return g1.get()->punteggi > g2.get()->punteggi;
        });



        cv.notify_all();
    }

    void wait_variations(){
        std::unique_lock<std::mutex> l(m);

        if (cambiamenti){
            cambiamenti=false;
        }

        cv.wait(l,[this]{
            return cambiamenti;
        });
    }

};

int main(int argc, const char * argv[]) {
    // insert code here...

    Classifica c{};
    c.insertPunteggio("mario",12);
    c.insertPunteggio("giovanni",15);
    c.insertPunteggio("elia",14);
    c.insertPunteggio("malnati",1200000000);

    return 0;
}

