//
// Created by kevin on 28/04/17.
//

#include <iostream>
#include <future>
#include <exception>

#include "ThreadPool.hh"


static int f() { throw std::runtime_error("future error"); }

static int g(int a, int b) { return a * b; }

int main()
{
    ThreadPool p(5);

    auto lifeAns    = p.submit([](){
        return 42;
    });
    auto fe         = p.submit(f);
    auto fm         = p.submit(g, 5, 42);

    std::cout << "life answer: " << lifeAns.get() << std::endl;
    std::cout << "life answer * 5: " << fm.get() << std::endl;
    try {
        fe.get();
    } catch (std::runtime_error const & re) {
        std::cout << re.what() << std::endl;
    }
    return 0;
}
