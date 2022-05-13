

#include <iostream>
#include "TransportNetwork.h"
using namespace std;
int main()
{
    TransportNetwork net("net.txt");
    net.DrawNetwork();
    net.CalculateMaxFlow();
    net.DrawNetwork();
}
