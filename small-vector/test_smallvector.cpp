#include <cstddef>
#include<iostream>
#include "smallvector.h"

using namespace std;
void testFunc(){
    SmallVector<int, 4> vec;
    for(int i=0;i<4;i++){
        vec.push_back(i);
    }
    for(size_t i=0;i<vec.size();i++){
        cout<<vec[i]<<" ";
    }
    cout<<endl;
}
int main(){
    testFunc();
    return 0;
}