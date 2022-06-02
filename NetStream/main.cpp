#include <iostream>
#include "FlowNetwork.h"
using namespace std;


int main()
{
    //it just works
    FlowNetwork net("net.txt");
    cout << net.GetMaxFlow() << endl;
    net.DrawNetwork();
}