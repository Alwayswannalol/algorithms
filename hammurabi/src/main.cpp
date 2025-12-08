#include <iostream>
#include "HammurabiGame.h"

using namespace std;

int main() {
    setlocale(LC_ALL, "Russian");
    
    HammurabiGame game;
    game.play();
    
    cout << "\nСпасибо за игру!\n";
    return 0;
}