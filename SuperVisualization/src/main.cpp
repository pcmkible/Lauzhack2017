#include<iostream>
#include "brate/Display.h"

int main(){
    Display *mDisplay = Display::getInstance();

    mDisplay->draw();

    return 0;
}