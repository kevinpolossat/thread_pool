//
// Created by kevin on 28/04/17.
//

#include <iostream>
#include <future>
#include "ThreadPool.hh"

int main()
{
    ThreadPool p(5);

    for (int i = 0; i < 100; ++i) {
        p.submit([i](){
            std::cout << i << std::endl;
        });
    }
    return 0;
}
