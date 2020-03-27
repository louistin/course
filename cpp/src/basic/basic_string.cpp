#include "basic_string.h"

int basic_string_init()
{
    string s1 = "hello";
    string s2 = "world";
    string s3;

    s3 = s1;
    cout << s3 << endl;

    s3 = s1 + s2;
    cout << s3 << endl;

    cout << s3.size() << endl;

    return 0;
}