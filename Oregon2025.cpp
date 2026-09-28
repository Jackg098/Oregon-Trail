
//John Giebe
//Assignment 14:Advanced files
//date: 12/6/2025

#include <iostream>
#include <string>
#include <cstdlib>  
#include <ctime>    
#include <chrono>
#include <iomanip>
#include <fstream>

const std::string SAVEFILE{ "OREGON.TXT" };

bool died();
bool visitDoctor();

int shoot();
int distanceCurve(int);

void caughtInBlizzard();
void displayInventory();
void getInstructions();
void getInitialPurchase();
void hunt();
void showDate();
void saveFile();
void loadFile();
void flush();
void validateInventory();
void recordMiles();
void reachMountain();
void arrived();

bool getTurnChoice();
bool getFoodChoice();
bool encounterRiders();
bool isGameComplete();

void addCash(int amount);
void addOxen(int amount);
void addFood(int amount);
void addAmmo(int amount);
void addClothes(int amount);
void addSupplies(int amount);
void addTurn(int amount);
void addTotalMiles(int amount);
void triggerEvent( int eventID);
void riderDefense(int shootSpeed);

const int CHANCE_ENCOUNTER_RIDERS{ 4 };
const int CHANCE_RUGGED_MOUNTAINS{ 15 };
const int SouthPassMileage{ 950 };
const int BlueMountainsMileage{ 1700 };
const int OregonCityMileage{ 2040 };

const std::string events[] = {
   "WAGON BREAKS DOWN -- LOSE TIME AND SUPPLIES FIXING IT.",
   "OXEN INJURES LEG -- SLOWS YOU DOWN REST OF TRIP",
   "BAD LUCK -- YOUR DAUGHTER BROKE HER ARM.",
   "OX WANDERS OFF -- SPEND TIME LOOKING FOR IT.",
   "YOUR SON GETS LOST -- SPEND HALF THE DAY LOOKING FOR HIM.",
   "UNSAFE WATER -- LOSE TIME LOOKING FOR CLEAN SPRING.",
   "HEAVY RAINS -- TIME AND SUPPLIES LOST.",
   "BANDITS ATTACK",
   "THERE WAS A FIRE IN YOUR WAGON -- FOOD AND SUPPLIES DAMAGED.",
   "LOSE YOUR WAY IN HEAVY FOG -- TIME IS LOST.",
   "YOU KILLED A POISONOUS SNAKE AFTER IT BIT YOU.",
   "WAGON GETS SWAMPED FORDING RIVER -- LOSE FOOD AND CLOTHES.",
   "WILD ANIMALS ATTACK!",
   "COLD WEATHER -- BRRRRRR!",
   "HAIL STORM -- SUPPLIES DAMAGED",
   "HELPFUL INDIANS SHOW YOU WHERE TO FIND MORE FOOD."
};

const std::string choiceTurnResponses[] =
{
"STOPPING AT THE FORT",
"HUNTING",
"CONTINUING THE TRAIL",
"QUIT"
};

const std::string choiceEatResponses[] =
{
"EATING POORLY",
"EATING MODERATELY",
"EATING WELL"
};
const std::string choiceRidersResponses[] =
{
"RUNNING",
"ATTACKING",
"CONTINUING THE TRAIL",
"CIRCLING THE WAGONS"
};

const std::string Weekday[7] = {
    "Sunday", "Monday", "Tuesday",
    "Wednesday", "Thursday", "Friday", "Saturday"
};


struct Status {
    int cash{ 700 };
    int oxen{ 0 };
    int food{ 0 };
    int ammo{ 0 };
    int clothes{ 0 };
    int supplies{ 0 };
    int spendOxen;
    int spendFood;
    int spendAmmo;
    int spendClothes;
    int spendSupplies;
    int totalMiles{ 0 };
    int turn{ 0 };
    int milesLastTurn{ 0 };

    bool fortAvailable{ false };
    bool haveIllness{ false };
    bool haveInjury{ false };
    bool isDead{ false };
    bool hasQuit{ false };

    bool blueMountainCleared{ false };
    bool southPassCleared{ false };
};

 Status status;

 struct choice{
int choiceTurn {0};
int choiceEat {0};
int choiceRiders {0};
 };
 
 choice Choice;

int getRandom(int min, int max) {
    return rand() % (max - min + 1) + min;
}

bool saveFileExists(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    return file.good();
}

bool getSick() {
    int sickFactor;
    sickFactor = getRandom(0, 100);
    if (sickFactor < 10 + 35 * (Choice.choiceEat - 1)) {
        std::cout << "MILD ILLNESS -- MEDICINE\n";
    }
    else if (sickFactor < 100 - (40 / pow(4, Choice.choiceEat - 1))) {
        std::cout << "BAD ILLNESS -- MEDICINE used\n";
        addTotalMiles(-5);
        addSupplies(-5);
    }
    else {
        std::cout << "SERIOUS ILLNESS -- YOU MUST STOP FOR MEDICAL ATTENTION\n";
        addSupplies(-10);
    }
    if (status.supplies < 0) {
        status.isDead = true;
        died();
    }
       return status.haveIllness; 
   }

int main()
{
   
    
    getInstructions();


    if (saveFileExists("game_save.txt")) {
        char choice;
        while (true) {
            std::cout << "A SAVED GAME WAS FOUND. LOAD IT? (Y/N): ";
            std::cin >> choice;
            flush();

            choice = tolower(choice);
            if (choice == 'y') {
                loadFile();
                break;
            }
            else if (choice == 'n') {
                getInitialPurchase();
                break;
            }
            else {
                std::cout << "PLEASE ENTER 'Y' OR 'N'.\n";
            }
        }
    }
    else {
        std::cout << "No saved game found. Starting a new game.\n";
        getInitialPurchase();
    }




   
   srand(static_cast<unsigned int>(time(0)));

bool invalid = false;

   do {
       
       addTurn(1);

       showDate();

       if (status.food < 12) {
           std:: cout << "YOU'D BETTER DO SOME HUNTING OR BUY FOOD AND SOON!!!!\n";
       }

       validateInventory();

       recordMiles();

       displayInventory();

       if (visitDoctor())
       {
           died();
           break;
       }

       if (getTurnChoice()) {
           break;
      }

       if (getFoodChoice()) {
           break;
       }

       status.totalMiles += 200 + (status.oxen - 220) / 5 + getRandom(0,11);

       std::cout << "\nMILES: " << status.totalMiles << "\n";
       reachMountain();
       if (encounterRiders()) {
           break;
       }

       int randomEvent = getRandom(0, 15);

       std::cout << events[randomEvent] << "\n";

       triggerEvent(randomEvent + 1); 

       if (status.isDead) break;

       status.haveIllness = getRandom(0, 100) < 100 - (Choice.choiceEat - 1) * 25;

       if (status.haveIllness) {
           std::cout << "YOU HAVE AN ILLNESS \n";
       }

       saveFile();
      
   } while (!isGameComplete());

   if (status.isDead) {
      died(); 
   }
   else {
       arrived();
   }
       

  return EXIT_SUCCESS;
}

void getInstructions() {
    std::string answer;


    while (true) {
        std::cout << "DO YOU NEED INSTRUCTION (YES/NO)? ";
        std::cin >> answer;

        for (char& c : answer) {
            c = toupper(c);
        }


        if (answer == "YES") {
            std::string message = R"(
THIS PROGRAM SIMULATES A TRIP OVER THE OREGON TRAIL FROM
    INDEPENDENCE, MISSOURI TO OREGON CITY, OREGON IN 1847.
    YOUR FAMILY OF FIVE WILL COVER THE 2000 MILE OREGON TRAIL
    IN 5-6 MONTHS --- IF YOU MAKE IT ALIVE.

    YOU HAD SAVED $900 TO SPEND FOR THE TRIP, AND YOU'VE JUST
        PAID $200 FOR A WAGON.
    YOU WILL NEED TO SPEND THE REST OF YOUR MONEY ON THE
        FOLLOWING ITEMS:

        OXEN - YOU CAN SPEND $200 TO $300 ON YOUR TEAM.
            THE MORE YOU SPEND, THE FASTER YOU'LL GO
            BECAUSE YOU'LL HAVE BETTER OXEN

        FOOD - THE MORE YOU HAVE, THE LESS CHANCE THERE
            IS OF GETTING SICK

        AMMUNITION - $1 BUYS A BELT OF 50 BULLETS.
            YOU WILL NEED BULLETS FOR ATTACKS BY ANIMALS
            AND BANDITS, AND FOR HUNTING FOOD

        CLOTHING - THIS IS ESPECIALLY IMPORTANT FOR THE COLD
            WEATHER YOU WILL ENCOUNTER WHEN CROSSING
            THE MOUNTAINS

        MISCELLANEOUS SUPPLIES - THIS INCLUDES MEDICINE AND
            OTHER THINGS YOU WILL NEED FOR SICKNESS
            AND EMERGENCY REPAIRS

    YOU CAN SPEND ALL YOUR MONEY BEFORE YOU START YOUR TRIP
        OR YOU CAN SAVE SOME OF YOUR CASH TO SPEND AT FORTS ALONG
        THE WAY WHEN YOU RUN LOW. HOWEVER, ITEMS COST MORE AT
        THE FORTS. YOU CAN ALSO GO HUNTING ALONG THE WAY TO GET
        MORE FOOD.

    WHENEVER YOU HAVE TO USE YOUR TRUSTY RIFLE ALONG THE WAY,
        YOU WILL SEE THE WORDS: TYPE BANG. THE FASTER YOU TYPE
        IN THE WORD 'BANG' AND HIT THE 'RETURN' KEY, THE BETTER
        LUCK YOU'LL HAVE WITH YOUR GUN.
        WHEN ASKED TO ENTER MONEY AMOUNTS, DON'T USE A '$'.

    GOOD LUCK!!!

    
)";
            std::cout << message << std::endl;
            break;
        }
        else if (answer == "NO") {
            std::cout << "\n";
            break;
        }
        else {
            std::cout << "PLEASE ENTER 'YES' OR 'NO'.\n";
        }
    }
   
    flush();
    std::cout << "\nLET'S BEGIN YOUR JOURNEY ON THE OREGON TRAIL...\n\n";
    
}

void displayInventory() {
   
    const int wideCol = 15;
    const int cashCol = 12;

    std::cout << "\n" << std::left << std::setw(wideCol) << "FOOD" << std::setw(wideCol) << "AMMO" << std::setw(wideCol) << "CLOTHING" << std::setw(wideCol) << "MISC. SUPP." << std::setw(cashCol) << "CASH";

    std::cout << std::left << "\n" << std::setw(wideCol) << status.food << std::setw(wideCol) << status.ammo << std::setw(wideCol) << status.clothes << std::setw(wideCol) << status.supplies << std::setw(cashCol) << status.cash << "\n\n";
}

void addCash(int amount) {
    status.cash += amount;
}

void addOxen(int amount) {
    status.oxen += amount;
}

void addFood(int amount) {
    status.food += amount;
}

void addAmmo(int amount) {
    status.ammo += amount;
}

void addClothes(int amount) {
    status.clothes += amount;
}

void addSupplies(int amount) {
    status.supplies += amount;
}

void addTurn(int amount) {
    status.turn += amount;
}

void addTotalMiles(int amount) {
    status.totalMiles += amount;
}

int getSpendAmount(std::string prompt, int min, int max) {
    bool invalid{ false };
    int tempValue{ 0 };
    do {
        invalid = false;
    std::cout << prompt;
    std::cin >> tempValue;
    if (tempValue < min || tempValue > max) {
        std::cout << "\nYOU OVERSPENT, TRY AGAIN\n";
        invalid = true;
    }

    } while (invalid);
    return tempValue;
}

void getInitialPurchase() {
    int tempCash, tempOxen, tempFood, tempAmmo, tempClothes, tempSupplies;
    std::cout << " YOU HAVE $700 TO SPEND ON SUPPLIES FOR YOUR JOURNEY.\n";
    if (status.turn == 0) {

        do {
            tempOxen = getSpendAmount("\nHOW MUCH DO YOU WANT TO SPEND ON YOUR OXEN TEAM ?\n", 200, 300);
            tempFood = getSpendAmount("HOW MUCH WILL YOU SPEND ON FOOD? \n", 0, 1000);
            tempAmmo = getSpendAmount("HOW MUCH WILL YOU SPEND ON AMMO? \n", 0, 1000);
            tempClothes = getSpendAmount("HOW MUCH WILL YOU SPEND ON CLOTHES? \n", 0, 1000);
            tempSupplies = getSpendAmount("HOW MUCH WILL YOU SPEND ON SUPPLIES? \n", 0, 1000);
            tempCash = status.cash - tempOxen - tempFood - tempAmmo - tempClothes - tempSupplies;
            if (tempCash < 0) {
                std::cout << "\nYOU OVERSPENT, TRY AGAIN\n";
            }
        } while (tempCash < 0);

        addOxen(tempOxen);
        addFood(tempFood);
        addAmmo(tempAmmo * 50);
        addClothes(tempClothes);
        addSupplies(tempSupplies);
        status.cash = tempCash;


        std::cout << "\nAFTER ALL YOUR PURCHASES, YOU NOW HAVE $" << status.cash << " DOLLARS LEFT. \n\n";
    }

 else {
     do {
         tempCash = 0;
         tempFood = getSpendAmount("\nHOW MUCH WOULD YOU LIKE TO SPEND ON FOOD: ", 0, status.cash);
         tempAmmo = getSpendAmount("HOW MUCH WOULD YOU LIKE TO SPEND ON AMMO: ", 0, status.cash);
         tempClothes = getSpendAmount("HOW MUCH WOULD YOU LIKE TO SPEND ON CLOTHES: ", 0, status.cash);
         tempSupplies = getSpendAmount("HOW MUCH WOULD YOU LIKE TO SPEND ON MISCELLANEOUS SUPPLIES: ", 0, status.cash);
         tempCash = tempFood + tempAmmo + tempClothes + tempSupplies;
         if (tempCash > status.cash) {
             std::cout << "\nYOU OVERSPENT, TRY AGAIN\n ";
         }
     } while (tempCash > status.cash);
     addFood(tempFood * 2 / 3);
     addAmmo(tempAmmo * 50 * 2 / 3);
     addClothes(tempClothes * 2 / 3);
     addSupplies(tempSupplies * 2 / 3);
     addCash(-tempCash);
     }
    flush();
}

void showDate() {
    std::string dates[18] = { 
        "MARCH 29", 
        "APRIL 12", 
        "APRIL 26", 
        "MAY 10", 
        "MARCH 24", 
        "JUNE 7", 
        "JUNE 21", 
        "JULY 5", 
        "JULY 19", 
        "AUGUST 2", 
        "AUGUST 16", 
        "AUGUST 31", 
        "SEPTEMBER 13",
        "SEPTEMBER 27",
        "OCTOBER 11", 
        "OCTOBER  25", 
        "NOVEMBER 8", 
        "NOVEMBER 22"
    };

        std::cout << "\nMONDAY, " << dates[ status.turn - 1 ] << " 1847\n";

}

void caughtInBlizzard() {
    std::cout << "\nBLIZZARD IN MOUNTAIN PASS -- TIME AND SUPPLIES LOST\n";
    addFood(- 25);
    addSupplies ( -10);
    addAmmo( - 300);
    addTotalMiles(- getRandom(30,40));
   
    if (status.clothes < getRandom(18,20)) {

        getSick();
    }
}

int shoot() {
    
    auto start = std::chrono::high_resolution_clock::now();
   
    std::string getInput;
    std::cout << "\nTYPE BANG \n"; 
    std::cin >> getInput;
   
    for (char& c : getInput) {
        c = toupper(c);
    }

    std::chrono::duration<float> diff = std::chrono::high_resolution_clock::now() - start;
    return static_cast<int>(getInput == "BANG" ? diff.count() : 7);
}

void hunt() {
    
    status.totalMiles -= 45;

    
    double shootSpeed = shoot();

    
    

    if (shootSpeed <= 1.0) {
        std::cout << "\nRIGHT BETWEEN THE EYES--YOU GOT A BIG ONE!!!!\n";

        int foodGain = 52 + getRandom(0,6);      
        int ammoLoss = 10 + getRandom(0,4);      

        status.food += foodGain;
        status.ammo -= ammoLoss;

    }
    else {
        int chance = getRandom(0,100);             

        if (chance > static_cast<int>(shootSpeed * 13)) {
            int foodGain = 48 - static_cast<int>(shootSpeed * 2);
            int ammoLoss = 10 + static_cast<int>(shootSpeed * 3);

            std::cout << "\nNICE SHOT--RIGHT THROUGH THE NECK--FEAST TONIGHT!!\n";

            status.food += foodGain;
            status.ammo -= ammoLoss;

        }
        else {
            std::cout << "\nSORRY---NO LUCK TODAY\n";
            
        }
    }
}              
           
bool died() {
    int deathChoice;

    if (status.isDead == true) {

        if (status.haveInjury) {
            std::cout << "\nYOU DIED OF INJURIES\n";
        }
        else if (status.haveIllness) {
            std::cout << "\nYOU DIED OF PNEUMONIA\n";
        }


        std::cout << "DUE TO YOUR UNFORTUNATE SITUATION, THERE ARE A FEW\n";
        std::cout << "FORMALITIES WE MUST GO THROUGH\n";

        std::cout << "WOULD YOU LIKE A MINISTER? (1 = YES, 0 = NO): ";
        std::cin >> deathChoice;

        std::cout << "WOULD YOU LIKE A FANCY FUNERAL? (1 = YES, 0 = NO): ";
        std::cin >> deathChoice;

        std::cout << "WOULD YOU LIKE US TO INFORM YOUR NEXT OF KIN? (1 = YES, 0 = NO): ";
        std::cin >> deathChoice;

        if (deathChoice == 0) {
            std::cout << "YOUR AUNT NELLIE IN ST. LOUIS IS ANXIOUS TO HEAR\n";
        }
        std::string message = R"(
WE THANK YOU FOR THIS INFORMATION AND WE ARE SORRY YOU
DIDN'T MAKE IT TO THE GREAT TERRITORY OF OREGON
BETTER LUCK NEXT TIME
                              SINCERELY
                 THE OREGON CITY CHAMBER OF COMMERCE
     )";
    std::cout << message;
    }
    return true;
}

void saveFile() {

    std::ofstream outFile("game_save.txt", std::ios::binary);

    if (!outFile) {
        std::cerr << "ERROR: Unable to save game.\n";
        return;
    }

    outFile.write(reinterpret_cast<char*>(&status), sizeof(status));
    outFile.write(reinterpret_cast<char*>(&Choice), sizeof(Choice));

    outFile.close();
    std::cout << "Game saved successfully.\n";
}

void loadFile() {
    std::ifstream inFile("game_save.txt", std::ios::binary);

    if (!inFile) {
        std::cerr << "\nNo saved game found. Starting a new game.\n";
        return;
    }

    inFile.read(reinterpret_cast<char*>(&status), sizeof(status));
    inFile.read(reinterpret_cast<char*>(&Choice), sizeof(Choice));

    inFile.close();
    std::cout << "Game loaded successfully.\n";
}

void flush()
{
    using std::numeric_limits;
    using std::streamsize;

    std::cin.clear();

    std::cin.ignore(numeric_limits<streamsize>::max(), '\n');

    
}

bool getTurnChoice()
{
    const std::string CHOICE_TURN[]{
        "STOPPING AT THE FORT",
        "HUNTING",
        "CONTINUING THE TRAIL",
        "QUIT"
    };

    do {
        Choice.choiceTurn = 0;
        int fortAvailable = status.turn % 2;
        std::cout << (fortAvailable ?
            "\nDO YOU WANT TO (1) STOP AT THE NEXT FORT, (2) HUNT, (3) CONTINUE, (4) QUIT? " :
            "\nDO YOU WANT TO (1) HUNT, (2) CONTINUE, (3) QUIT? ");

        std::cin >> Choice.choiceTurn;
        flush();

        if (Choice.choiceTurn < 1 || Choice.choiceTurn > 4)
        {
            Choice.choiceTurn = 2 + fortAvailable;
        }

        Choice.choiceTurn += 1 - fortAvailable;

        if (Choice.choiceTurn == 2 && status.ammo < 40)
        {
            std::cout << "TOUGH -- YOU NEED MORE BULLETS TO GO HUNTING \n";
            Choice.choiceTurn = 0;
        }

    } while (Choice.choiceTurn == 0);

   
    std::cout << CHOICE_TURN[Choice.choiceTurn - 1] << "\n";
    switch (Choice.choiceTurn)
    {
    case 1:
    {
        getInitialPurchase();
    }
    break;
    case 2:
    {
        hunt();
    }
    break;
    case 3:
    {
        
    }
    break;
    case 4:
    {
        return true;
    }
    }
    return false;
}

bool getFoodChoice()

{
    if (status.food < 13)
    {
        std::cout << "\nYOU RAN OUT OF FOOD AND STARVED TO DEATH \n";
        status.isDead = true;
    }
    else
    {

        const std::string FOOD_CHOICE[]{ "POORLY", "MODERATELY", "WELL" };

        do
        {
            do
            {
                std::cout << "DO YOU WANT TO EAT (1) POORLY, (2) MODERATELY, (3) WELL? ";
                std::cin >> Choice.choiceEat;
            } while (Choice.choiceEat < 1 || Choice.choiceEat > 3);

            if (status.food - 8 - 5 * Choice.choiceEat < 0)
            {
                std::cout << "YOU CAN'T EAT THAT WELL.\n";
                Choice.choiceEat = 0;
            }
        } while (Choice.choiceEat == 0);

        addFood(-8 - 5 * Choice.choiceEat);

        std::cout << "YOU ARE EATING " << FOOD_CHOICE[Choice.choiceEat - 1] << "\n";

        status.haveIllness = getRandom(0, 100) < 100 - (Choice.choiceEat - 1) * 25;

        
    }
    return status.isDead;
}

void riderDefense(int shootSpeed) {
    if (shootSpeed <= 1) {
        std::cout << "NICE SHOOTING -- YOU DROVE THEM OFF\n";
    }
    else if (shootSpeed <= 4) {
        std::cout << "KINDA SLOW WITH YOUR COLT .45\n";
    }
    else {
        std::cout << "LOUSY SHOT -- YOU GOT KNIFED\n";
        std::cout << "YOU HAVE TO SEE OL' DOC BLANCHARD\n";
        status.haveInjury = true;
    }
    return;
    
}

bool encounterRiders()
{
    

    if (getRandom(0, 11) > distanceCurve(CHANCE_ENCOUNTER_RIDERS) - 1) return false;

    const std::string RIDERS_RESPONSE[4]{
        "RUNNING\n",
        "ATTACKING\n",
        "CONTINUING THE TRAIL\n",
        "CIRCLING THE WAGONS\n"
    };

    bool friendly = getRandom(0, 100) < 20;
    std::cout << "\nRIDERS AHEAD. ";
    std::cout << "THEY " << (friendly ? "DON'T" : "") << " LOOK HOSTILE.\n" ;

    std::cout << "\nTACTICS\n" ;
    do
    {

        if (getRandom(0, 100) < 20) friendly = !friendly;

        std::cout << "IF YOU RUN YOU'LL GAIN TIME BUT WEAR DOWN YOUR OXEN\n";
        std::cout << "IF YOU CIRCLE YOU'LL LOSE TIME\n";
        std::cout << "DO YOU WANT TO (1) RUN, (2) ATTACK, (3) CONTINUE, (4) CIRCLE THE WAGONS?\n ";

        std::cin >> Choice.choiceRiders;
    } while (Choice.choiceRiders < 1 || Choice.choiceRiders > 4);

    std::cout << RIDERS_RESPONSE[Choice.choiceRiders - 1] << "\n";

    if (!friendly)
    {
        switch (Choice.choiceRiders)
        {
        case 1:
        {
            addTotalMiles(20);
            addSupplies(-15);
            addAmmo(-150);
            addOxen(-40);
            break;
        }
        case 2:
        {
            int shootSpeed = shoot();
            addAmmo(-shootSpeed * 40 - 80);
            riderDefense(shootSpeed);
            break;
        }
        case 3:
            std::cout << "CONTINUING THE TRAIL\n";
        {
      
            if (getRandom(0, 100) < 20)
            {
                std::cout << "THEY DID NOT ATTACK\n";
                return false;
            }
            else
            {
                addAmmo(-150);
                addSupplies(-15);
            }
            break;
        }
        case 4:
        {
            int shootSpeed = shoot();
            addAmmo(-shootSpeed * 30 - 80);
            addTotalMiles(-25);
            riderDefense(shootSpeed);
        }
        }
    }
    else
    {
        switch (Choice.choiceRiders)
        {
        case 1:
        std::cout << "RUNNING \n";
        {
            addTotalMiles(15);
            addOxen(-10);
            break;
        }
        case 2:
            std::cout << "ATTACKING \n";
        {
            addTotalMiles(-5);
            addAmmo(-100);
            break;
        }
        case 3:
            std::cout << "CONTINUING THE TRAIL\n";
        {
            break;
        }
        case 4:
            std::cout << "CIRCLING THE WAGON \n";
        {
            addTotalMiles(-20);
        }
        }
    }

    std::cout << (friendly ?
        "RIDERS WERE FRIENDLY, BUT CHECK FOR POSSIBLE LOSSES\n" :
        "RIDERS WERE HOSTILE -- CHECK FOR LOSSES\n");

    if (status.ammo < 0)
    {
        std::cout << "YOU RAN OUT OF BULLETS AND GOT MASSACRED BY THE RIDERS\n";
        status.isDead = true;
    }

    return status.isDead;
}

void triggerEvent(int eventID) {
    
    
    switch (eventID) {
    case 1:
        addTotalMiles(-getRandom(15, 20));
        addSupplies(-8);
        break;

    case 2:
        addTotalMiles(-25);
        addOxen(-20);
        break;

    case 3:
        addTotalMiles(-getRandom(5, 9));
        addSupplies(-getRandom(2, 5));
        break;

    case 4:
        addTotalMiles(-17);
        break;

    case 5:
        addTotalMiles(-10);
        break;

    case 6:
        addTotalMiles(-getRandom(10, 12));
        break;

    case 7:
        if (status.totalMiles < 950) {
            addFood(-10);
            addAmmo(-500);
            addSupplies(-15);
            addTotalMiles(-getRandom(10, 15));
        }
        break;

    case 8: {
        std::cout << "\nBANDIT ATTACK!\n";
        double shootSpeed = shoot();
        addAmmo(-static_cast<int>(20 * shootSpeed));

        if (status.ammo < 1) {
            std::cout << "\nYOU RAN OUT OF BULLETS -- THEY GET LOTS OF CASH\n";
            status.cash = status.cash / 3;
        }
        else if (shootSpeed <= 1) {
            std::cout << "\nQUICKEST DRAW OUTSIDE OF DODGE CITY!!! YOU GOT 'EM!\n";
        }
        else {
            std::cout << "\nYOU GOT SHOT IN THE LEG AND THEY TOOK ONE OF YOUR OXEN. BETTER HAVE DOCK LOOK AT YOUR WOUND.\n";
            status.haveInjury = true;
            addSupplies(-5);
            addOxen(-20);
        }
        break;
    }

    case 9:
        addFood(-40);
        addAmmo(-400);
        addTotalMiles(-15);
        addSupplies(-getRandom(3, 11));
        break;

    case 10:
        addTotalMiles(-getRandom(10, 15));
        break;

    case 11:
        addAmmo(-10);
        addSupplies(-5);
        if (status.supplies <= 0) {
            std::cout << "\nYOU DIED OF SNAKEBITE SINCE YOU HAVE NO MEDICINE\n";
            status.isDead = true;
        }
        break;

    case 12:
        addFood(-30);
        addAmmo(-20);
        addTotalMiles(-getRandom(20, 40));
        break;

    case 13: {
        std::cout << "\nWILD ANIMALS ATTACK!\n";
        double shootSpeed = shoot();

        if (status.ammo < 40) {
            std::cout << "\nYOU WERE TOO LOW ON BULLETS. THE WOLVES OVERPOWERED YOU\n";
            status.haveInjury = true;
            status.isDead = true;
        }
        else {
            if (shootSpeed <= 2) {
                std::cout << "\nNICE SHOOTING PARDNER -- THEY DIDN'T GET MUCH\n";
            }
            else {
                std::cout << "\nSLOW ON THE DRAW -- THEY GOT AT YOUR FOOD AND CLOTHES\n";
            }
            addAmmo(-(20 + static_cast<int>(shootSpeed)));
            addClothes(-static_cast<int>(4 * shootSpeed));
            addFood(-static_cast<int>(8 * shootSpeed));
        }
        break;
    }

    case 14: {
        int threshold = getRandom(22, 26);
        if (status.clothes < threshold) {
            std::cout << "\nYOU DON'T HAVE ENOUGH CLOTHING TO KEEP WARM\n";
            status.haveIllness = true;
            getSick();
        }
        else {
            std::cout << "\nYOU'RE WARM ENOUGH TO SURVIVE THE COLD\n";

            if (status.haveIllness) getSick();
        }
        break;
    }

    case 15:
        addTotalMiles(-getRandom(5, 15));
        addAmmo(-200);
        addSupplies(-getRandom(4, 7));
        break;

    case 16:
        addFood(14);
        break;
    }
}

int distanceCurve(int eventType) {

    return static_cast<int>((pow(status.totalMiles / 100 - eventType, 2) + 72) / (pow(status.totalMiles / 100 - eventType, 2) + 12));

}

void validateInventory() {

    if (status.ammo < 0) status.ammo = 0;
    if (status.cash < 0) status.cash = 0;
    if (status.clothes < 0) status.clothes = 0;
    if (status.food < 0) status.food = 0;
    if (status.supplies < 0) status.supplies = 0;

}

bool isGameComplete() {
        if (status.totalMiles >= OregonCityMileage || died) {
            return true;
        }
        return false;

}

bool visitDoctor()
{
    if (status.haveIllness || status.haveInjury)
    {
        addCash(-20);
        if (status.cash < 0)
        {
            std::cout << "You can't afford a doctor. You died of ";
            std::cout << (status.haveIllness ? "pneumonia" : "injury") << "\n";
            status.isDead = true;
        }
        else
        {
            std::cout << "Doctor's bill is $20\n";
            status.haveIllness = status.haveInjury = false;
        }
    }
    return status.isDead;
}

void recordMiles() {
   
    status.milesLastTurn = status.totalMiles;
    return;
}

void reachMountain() {


    if (status.totalMiles >= SouthPassMileage) {
        if (getRandom(0, 10) < 9 - distanceCurve(CHANCE_RUGGED_MOUNTAINS)) {
            std::cout << "RUGGED MOUNTAINS\n";

            if (getRandom(0, 100) < 10) {
                if (getRandom(0, 100) < 11) {
                    std::cout << "THE GOING GETS SLOW\n";
                    addTotalMiles(-getRandom(45, 90));
                }
                else {
                    std::cout << "WAGON DAMAGED! -- LOSE TIME AND SUPPLIES\n";
                    addSupplies(-5);
                    addAmmo(-200);
                    addTotalMiles(-getRandom(20, 30));
                }
            }
            else {
                std::cout << "YOU GOT LOST -- LOSE VALUABLE TIME TRYING TO FIND THE TRAIL!\n";
                addTotalMiles(-60);
            }
        }

        if (!status.southPassCleared) {
            status.southPassCleared = true;
            if (getRandom(0, 100) < 80) {
                caughtInBlizzard();
            }
            else {
                std::cout << "YOU MADE IT SAFELY THROUGH THE SOUTH PASS -- NO SNOW\n";
            }
        }

        if (status.totalMiles >= BlueMountainsMileage) {
            if (!status.blueMountainCleared) {
                status.blueMountainCleared = true;
                if (getRandom(0, 100) < 70) {
                    caughtInBlizzard();
                }
            }
        }
    }
}

void arrived() {
    if (!died) {
        std::cout << "YOU FINALLY ARRIVED AT OREGON CITY\n";
        std::cout << "AFTER " << OregonCityMileage << " LONG MILES -- HOORAY!!\n\n";

        double fractionalMilesLastTurn =
            (double)(OregonCityMileage - status.milesLastTurn) / (status.totalMiles - status.milesLastTurn);

        addFood((1 - fractionalMilesLastTurn) * (8 + 5 * Choice.choiceEat));

        fractionalMilesLastTurn *= 14;

        int daysTraveled = status.turn * 14 + (int)fractionalMilesLastTurn;

        int dayOfWeek = ((int)fractionalMilesLastTurn + 6) % 7;

        std::cout << Weekday[dayOfWeek] << "\n";

        if (daysTraveled < 125) {              
            daysTraveled -= 93;
            std::cout << "JULY ";
        }
        else if (daysTraveled < 156) {       
            daysTraveled -= 124;
            std::cout << "AUGUST ";
        }
        else if (daysTraveled < 186) {       
            daysTraveled -= 155;
            std::cout << "SEPTEMBER ";
        }
        else if (daysTraveled < 217) {       
            daysTraveled -= 185;
            std::cout << "OCTOBER ";
        }
        else {                               
            daysTraveled -= 216;
            std::cout << "NOVEMBER ";
        }
        std::cout << daysTraveled << " 1847\n\n";

        displayInventory();

        std::string message = R"(

         PRESIDENT JAMES K. POLK SENDS YOU HIS 
               HEARTIEST CONGRATULATIONS 
        
                    AND WISHES YOU A PROSPEROUS LIFE AHEAD 
        
                               AT YOUR NEW HOME 
     )";
        std::cout << message;


    }

}
       
