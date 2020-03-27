#include "basic_reference.h"

void swap(int& x, int& y) {
    int temp = x;
    x = y;
    y = temp;
}

int basic_reference_init()
{
    int i;
    double d;

    int& r = i;
    double& s = d;

    i = 5;
    cout << i << endl;
    cout << r << endl;

    d = 11.7;
    cout << d << endl;
    cout << s << endl;

    int x = 10;
    int y = 20;
    swap(x, y);

    cout << "x: " << x << ", y: " << y << endl;

    return 0;
}