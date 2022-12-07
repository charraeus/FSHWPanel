/*********************************************************************************************************//**
 * @file helloworld.cpp
 * @author Christian Harraeus <christian@harraeus.de>
 * @brief Testprogramm
 * @version 0.1
 * @date 2020-04-16
 * 
 * Copyright © 2017 - 2020. All rights reserved.
 * 
 ************************************************************************************************************/

#include <iostream>
#include <vector>
#include <string>

using namespace std;

/*********************************************************************************************************//**
 * @brief Hauptprogramm
 * 
 * @return int 
 */
int main()
{
    vector<string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};
    for (const string& word : msg)
    {
        cout << word << " ";
    }
    cout << endl;
}