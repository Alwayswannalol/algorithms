#ifndef HAMMURABI_GAME_H
#define HAMMURABI_GAME_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>

using namespace std;

struct GameState {
    int year = 1;
    int population = 100;
    int wheat = 2800;
    int acres = 1000;
    int wheatHarvested = 0;
    int wheatPerAcre = 0;
    int wheatEatenByRats = 0;
    int landPrice = 0;
    int immigrants = 0;
    int starved = 0;
    bool plague = false;
    int diedFromPlague = 0;
    int acresToBuy = 0;
    int acresToSell = 0;
    int wheatToFeed = 0;
    int acresToPlant = 0;
    int wheatFromLastHarvest = 0;
    
    vector<int> starvedEachYear;
    vector<int> populationEachYear;
};

class HammurabiGame {
private:
    GameState state;
    const string SAVE_FILE = "hammurabi_save.dat";
    
    const int MAX_YEARS = 10;
    const int CONSUMPTION_PER_PERSON = 20;
    const int MAX_ACRES_PER_PERSON = 10;
    const float SEEDS_PER_ACRE = 0.5f;
    const int MIN_LAND_PRICE = 17;
    const int MAX_LAND_PRICE = 26;
    const int MIN_HARVEST = 1;
    const int MAX_HARVEST = 6;
    const float RAT_LOSS_MAX = 0.07f;
    const float PLAGUE_CHANCE = 0.15f;
    const int MAX_IMMIGRANTS = 50;
    
public:
    HammurabiGame() {
        srand(static_cast<unsigned>(time(nullptr)));
    }
    
    void play();
    
private:
    void newGame();
    
    bool checkForSavedGame();
    
    bool askToContinue();
    
    bool askToSaveAndExit();
    
    void saveGame();
    
    void loadGame();
    
    void playYear();
    
    void printReport();
    
    void getPlayerDecisions();
    
    void processDecisions();
    
    void calculateHarvest();
    
    void calculateRatLoss();
    
    void feedPopulation();
    
    void calculateImmigration();
    
    void checkPlague();
    
    void updateStatistics(int populationBefore);
    
    void endGame();
};

#endif // HAMMURABI_GAME_H
