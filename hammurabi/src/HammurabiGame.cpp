#include "HammurabiGame.h"

void HammurabiGame::play()
{
    cout << "=== ПРАВИТЕЛЬ ЕГИПТА ===\n";
    cout << "Древняя экономическая стратегия\n\n";

    if (checkForSavedGame())
    {
        if (askToContinue())
        {
            loadGame();
        }
        else
        {
            newGame();
        }
    }
    else
    {
        newGame();
    }

    while (state.year <= MAX_YEARS && state.population > 0)
    {
        cout << "\nГод " << state.year << " из " << MAX_YEARS << "\n";

        if (askToSaveAndExit())
        {
            saveGame();
            cout << "Игра сохранена. Возвращайтесь!\n";
            return;
        }

        playYear();

        if (state.starved > 0 &&
            (state.starved * 100.0 / state.population) > 45)
        {
            cout << "\n=== КАТАСТРОФА! ===\n";
            cout << "За этот год от голода умерло " << state.starved << " человек.\n";
            cout << "Это более 45% населения! Народ восстал и сверг вас!\n";
            cout << "Игра окончена.\n";
            remove(SAVE_FILE.c_str());
            return;
        }

        state.year++;
    }

    endGame();
}

void HammurabiGame::newGame()
{
    state = GameState();
    state.starvedEachYear.clear();
    state.populationEachYear.clear();
    cout << "Начинаем новую игру!\n";
}

bool HammurabiGame::checkForSavedGame()
{
    ifstream file(SAVE_FILE);
    return file.good();
}

bool HammurabiGame::askToContinue()
{
    char choice;
    while (true)
    {
        cout << "Найдена сохраненная игра. Продолжить? (y/n): ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 'y' || choice == 'Y')
        {
            return true;
        }
        else if (choice == 'n' || choice == 'N')
        {
            return false;
        }
        else
        {
            cout << "Пожалуйста, введите 'y' или 'n'.\n";
        }
    }
}

bool HammurabiGame::askToSaveAndExit()
{
    char choice;
    cout << "\nХотите сохранить и выйти? (y/n): ";
    cin >> choice;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return (choice == 'y' || choice == 'Y');
}

void HammurabiGame::saveGame()
{
    ofstream file(SAVE_FILE, ios::binary);
    if (file.is_open())
    {
        file.write(reinterpret_cast<char *>(&state.year), sizeof(state.year));
        file.write(reinterpret_cast<char *>(&state.population), sizeof(state.population));
        file.write(reinterpret_cast<char *>(&state.wheat), sizeof(state.wheat));
        file.write(reinterpret_cast<char *>(&state.acres), sizeof(state.acres));

        size_t starvedSize = state.starvedEachYear.size();
        size_t populationSize = state.populationEachYear.size();
        file.write(reinterpret_cast<char *>(&starvedSize), sizeof(starvedSize));
        file.write(reinterpret_cast<char *>(&populationSize), sizeof(populationSize));

        if (starvedSize > 0)
        {
            file.write(reinterpret_cast<char *>(state.starvedEachYear.data()),
                       starvedSize * sizeof(int));
        }
        if (populationSize > 0)
        {
            file.write(reinterpret_cast<char *>(state.populationEachYear.data()),
                       populationSize * sizeof(int));
        }

        file.close();
    }
}

void HammurabiGame::loadGame()
{
    ifstream file(SAVE_FILE, ios::binary);
    if (file.is_open())
    {
        file.read(reinterpret_cast<char *>(&state.year), sizeof(state.year));
        file.read(reinterpret_cast<char *>(&state.population), sizeof(state.population));
        file.read(reinterpret_cast<char *>(&state.wheat), sizeof(state.wheat));
        file.read(reinterpret_cast<char *>(&state.acres), sizeof(state.acres));

        size_t starvedSize, populationSize;
        file.read(reinterpret_cast<char *>(&starvedSize), sizeof(starvedSize));
        file.read(reinterpret_cast<char *>(&populationSize), sizeof(populationSize));

        state.starvedEachYear.resize(starvedSize);
        state.populationEachYear.resize(populationSize);

        if (starvedSize > 0)
        {
            file.read(reinterpret_cast<char *>(state.starvedEachYear.data()),
                      starvedSize * sizeof(int));
        }
        if (populationSize > 0)
        {
            file.read(reinterpret_cast<char *>(state.populationEachYear.data()),
                      populationSize * sizeof(int));
        }

        file.close();
        cout << "Игра загружена. Продолжаем с года " << state.year << "!\n";
    }
}

void HammurabiGame::playYear()
{
    state.landPrice = rand() % (MAX_LAND_PRICE - MIN_LAND_PRICE + 1) + MIN_LAND_PRICE;

    int populationBefore = state.population;

    printReport();

    getPlayerDecisions();

    processDecisions();

    calculateHarvest();

    calculateRatLoss();

    feedPopulation();

    calculateImmigration();

    checkPlague();

    updateStatistics(populationBefore);
}

void HammurabiGame::printReport()
{
    cout << "\n=== ОТЧЕТ ЗА ГОД " << (state.year - 1) << " ===\n";

    if (state.starved > 0)
    {
        cout << state.starved << " человек умерли с голоду";
        if (state.immigrants > 0)
        {
            cout << ", и " << state.immigrants << " человек прибыли в наш великий город\n";
        }
        else
        {
            cout << "\n";
        }
    }
    else if (state.immigrants > 0)
    {
        cout << state.immigrants << " человек прибыли в наш великий город\n";
    }

    if (state.plague)
    {
        cout << "Чума уничтожила половину населения!\n";
        cout << "Погибло от чумы: " << state.diedFromPlague << " человек\n";
    }

    cout << "Население города сейчас составляет " << state.population << " человек\n";

    if (state.year > 1)
    {
        cout << "Мы собрали " << state.wheatFromLastHarvest << " бушелей пшеницы";
        if (state.wheatPerAcre > 0)
        {
            cout << ", по " << state.wheatPerAcre << " бушеля с акра\n";
        }
        else
        {
            cout << "\n";
        }

        if (state.wheatEatenByRats > 0)
        {
            cout << "Крысы истребили " << state.wheatEatenByRats
                 << " бушелей пшеницы\n";
        }
    }

    cout << "В амбарах " << state.wheat << " бушелей пшеницы\n";
    cout << "Город сейчас занимает " << state.acres << " акров\n";
    cout << "1 акр земли стоит сейчас " << state.landPrice << " бушелей\n";
}

void HammurabiGame::getPlayerDecisions()
{
    cout << "\n=== РЕШЕНИЯ НА ГОД " << state.year << " ===\n";

    int netAcres = 0;
    while (true)
    {
        cout << "Сколько акров земли повелеваешь купить? (0-"
             << (state.wheat / state.landPrice) << "): ";
        cin >> state.acresToBuy;

        cout << "Сколько акров земли повелеваешь продать? (0-"
             << state.acres << "): ";
        cin >> state.acresToSell;

        netAcres = state.acresToBuy - state.acresToSell;
        int cost = state.acresToBuy * state.landPrice;
        int income = state.acresToSell * state.landPrice;

        if (state.acresToBuy < 0 || state.acresToSell < 0)
        {
            cout << "О, повелитель, числа должны быть положительными!\n";
        }
        else if (cost > state.wheat)
        {
            cout << "О, повелитель, у нас только " << state.wheat
                 << " бушелей пшеницы!\n";
        }
        else if (state.acresToSell > state.acres)
        {
            cout << "О, повелитель, у нас только " << state.acres
                 << " акров земли!\n";
        }
        else if (state.acres + netAcres < 0)
        {
            cout << "О, повелитель, город не может иметь отрицательное количество земли!\n";
        }
        else
        {
            break;
        }
    }

    state.wheat += (state.acresToSell * state.landPrice) - (state.acresToBuy * state.landPrice);
    state.acres += netAcres;

    while (true)
    {
        cout << "Сколько бушелей пшеницы повелеваешь съесть? (0-"
             << state.wheat << "): ";
        cin >> state.wheatToFeed;

        if (state.wheatToFeed < 0)
        {
            cout << "О, повелитель, нельзя съесть отрицательное количество пшеницы!\n";
        }
        else if (state.wheatToFeed > state.wheat)
        {
            cout << "О, повелитель, у нас только " << state.wheat
                 << " бушелей пшеницы!\n";
        }
        else
        {
            break;
        }
    }

    while (true)
    {
        cout << "Сколько акров земли повелеваешь засеять? (0-"
             << min(state.acres, state.population * MAX_ACRES_PER_PERSON)
             << "): ";
        cin >> state.acresToPlant;

        int seedsNeeded = static_cast<int>(state.acresToPlant * SEEDS_PER_ACRE);

        if (state.acresToPlant < 0)
        {
            cout << "О, повелитель, нельзя засеять отрицательное количество земли!\n";
        }
        else if (state.acresToPlant > state.acres)
        {
            cout << "О, повелитель, у нас только " << state.acres
                 << " акров земли!\n";
        }
        else if (state.acresToPlant > state.population * MAX_ACRES_PER_PERSON)
        {
            cout << "О, повелитель, у нас только " << state.population
                 << " человек, каждый может обработать только "
                 << MAX_ACRES_PER_PERSON << " акров!\n";
        }
        else if (seedsNeeded > state.wheat - state.wheatToFeed)
        {
            cout << "О, повелитель, для посева нужно " << seedsNeeded
                 << " бушелей пшеницы, а у нас останется только "
                 << (state.wheat - state.wheatToFeed) << "!\n";
        }
        else
        {
            break;
        }
    }
}

void HammurabiGame::processDecisions()
{
    state.wheat -= state.wheatToFeed;

    int seedsNeeded = static_cast<int>(state.acresToPlant * SEEDS_PER_ACRE);
    state.wheat -= seedsNeeded;
}

void HammurabiGame::calculateHarvest()
{
    if (state.acresToPlant > 0)
    {
        state.wheatPerAcre = rand() % (MAX_HARVEST - MIN_HARVEST + 1) + MIN_HARVEST;
        state.wheatHarvested = state.acresToPlant * state.wheatPerAcre;
        state.wheat += state.wheatHarvested;
        state.wheatFromLastHarvest = state.wheatHarvested;
    }
    else
    {
        state.wheatPerAcre = 0;
        state.wheatHarvested = 0;
        state.wheatFromLastHarvest = 0;
    }
}

void HammurabiGame::calculateRatLoss()
{
    if (state.wheat > 0)
    {
        float lossPercentage = static_cast<float>(rand()) / RAND_MAX * RAT_LOSS_MAX;
        state.wheatEatenByRats = static_cast<int>(state.wheat * lossPercentage);
        state.wheat -= state.wheatEatenByRats;
    }
    else
    {
        state.wheatEatenByRats = 0;
    }
}

void HammurabiGame::feedPopulation()
{
    int wheatNeeded = state.population * CONSUMPTION_PER_PERSON;

    if (state.wheatToFeed >= wheatNeeded)
    {
        state.starved = 0;
        state.wheat += (state.wheatToFeed - wheatNeeded);
    }
    else
    {
        state.starved = state.population - (state.wheatToFeed / CONSUMPTION_PER_PERSON);
        state.population -= state.starved;
        state.wheat = 0;
    }
}

void HammurabiGame::calculateImmigration()
{
    if (state.starved == 0 && state.wheatHarvested > 0)
    {
        int immigrants = state.starved / 2 +
                         (5 - state.wheatPerAcre) * state.wheat / 600 + 1;

        if (immigrants < 0)
            immigrants = 0;
        if (immigrants > MAX_IMMIGRANTS)
            immigrants = MAX_IMMIGRANTS;

        state.immigrants = immigrants;
        state.population += immigrants;
    }
    else
    {
        state.immigrants = 0;
    }
}

void HammurabiGame::checkPlague()
{
    float plagueRoll = static_cast<float>(rand()) / RAND_MAX;
    state.plague = (plagueRoll < PLAGUE_CHANCE);

    if (state.plague)
    {
        state.diedFromPlague = state.population / 2;
        state.population -= state.diedFromPlague;
    }
    else
    {
        state.diedFromPlague = 0;
    }
}

void HammurabiGame::updateStatistics(int populationBefore)
{
    state.starvedEachYear.push_back(state.starved);
    state.populationEachYear.push_back(populationBefore);
}

void HammurabiGame::endGame()
{
    cout << "\n=== ПРАВЛЕНИЕ ЗАВЕРШЕНО ===\n";
    cout << "Вы правили " << (state.year - 1) << " лет\n";

    double totalStarved = 0;
    for (int starved : state.starvedEachYear)
    {
        totalStarved += starved;
    }

    double avgStarvedPercentage = 0;
    if (!state.populationEachYear.empty())
    {
        for (size_t i = 0; i < state.populationEachYear.size(); i++)
        {
            if (state.populationEachYear[i] > 0)
            {
                avgStarvedPercentage += (state.starvedEachYear[i] * 100.0 /
                                         state.populationEachYear[i]);
            }
        }
        avgStarvedPercentage /= state.populationEachYear.size();
    }

    double acresPerPerson = (state.population > 0) ? static_cast<double>(state.acres) / state.population : 0;

    cout << "Среднегодовой процент умерших от голода: "
         << avgStarvedPercentage << "%\n";
    cout << "Акров земли на человека: " << acresPerPerson << "\n\n";

    if (avgStarvedPercentage > 33 && acresPerPerson < 7)
    {
        cout << "Из-за вашей некомпетентности в управлении, народ устроил бунт,\n";
        cout << "и изгнал вас из города. Теперь вы вынуждены влачить жалкое\n";
        cout << "существование в изгнании.\n";
    }
    else if (avgStarvedPercentage > 10 && acresPerPerson < 9)
    {
        cout << "Вы правили железной рукой, подобно Нерону и Ивану Грозному.\n";
        cout << "Народ вздохнул с облегчением, и никто больше не желает\n";
        cout << "видеть вас правителем.\n";
    }
    else if (avgStarvedPercentage > 3 && acresPerPerson < 10)
    {
        cout << "Вы справились вполне неплохо, у вас, конечно, есть\n";
        cout << "недоброжелатели, но многие хотели бы увидеть вас во главе\n";
        cout << "города снова.\n";
    }
    else
    {
        cout << "Фантастика! Карл Великий, Диэразли и Джефферсон вместе\n";
        cout << "не справились бы лучше!\n";
    }

    remove(SAVE_FILE.c_str());
}
