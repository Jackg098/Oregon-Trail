//John Giebe
//Assignment 2.2: language constructs
//date: 9/06/2025

#include <iostream>
#include <string> //includes the data for string paragraph
#include <cstdlib>
#include <ctime>
int main()
{
    std::srand(static_cast<unsigned int>(time(0)));
    int riderChance = rand() % 100;
    int eatingChance = rand() % 100;
    int fortChance = rand() % 100;

    int riderChoice, eatingChoice, fortChoice;




    std::string answer;

    while (true) {
        std::cout << "Do you need instructions (yes/no) ? \n";
        std::cin >> answer;


        if (answer == "yes") {
            std::string message = R"(
This program simulates a trip over the Oregon Trail from
    Independence, Missouri to Oregon City, Oregon in 1847.
    Your family of five will cover the 2000 mile Oregon Trail
    in 5-6 months --- if you make it alive.

    You had saved $900 to spend for the trip, and you've just
        paid $200 for a wagon.
    You will need to spend the rest of your money on the
        following items:

        Oxen - You can spend $200 to $300 on your team.
            The more you spend, the faster you'll go
            because you'll have better oxen

        Food - The more you have, the less chance there
            is of getting sick

        Ammunition - $1 buys a belt of 50 bullets.
            You will need bullets for attacks by animals
            and bandits, and for hunting food

        Clothing - This is especially important for the cold
            weather you will encounter when crossing
            the mountains

        Miscellaneous supplies - This includes medicine and
            other things you will need for sickness
            and emergency repairs

    You can spend all your money before you start your trip
        or you can save some of your cash to spend at forts along
        the way when you run low. However, items cost more at
        the forts. You can also go hunting along the way to get
        more food.

    Whenever you have to use your trusty rifle along the way,
        you will see the words: TYPE BANG. The faster you type
        in the word 'BANG' and hit the 'RETURN' key, the better
        luck you'll have with your gun.
        When asked to enter money amounts, don't use a '$'.

    Good luck!!!

)";
            std::cout << message << std::endl;

            break;
        }
        else if (answer == "no") {
            std::cout << "\n";
            break;
        }
        else {
            std::cout << "please enter 'yes' or 'no'. \n";
        }
    }

    std::cout << "let's begin your journey on the oregon trail... \n";

    // Prompt for rider response
    std::cout << "RIDERS AHEAD. DO YOU WANT TO:\n";
    std::cout << "(1) RUN\n(2) ATTACK\n(3) CONTINUE\n(4) CIRCLE THE WAGONS\n";
    std::cout << "Enter your choice (1-4): ";
    std::cin >> riderChoice;
    if (riderChance < 100) {
        while (true) {
            std::cout << "RIDERS AHEAD. DO YOU WANT TO:\n";
            std::cout << "(1) RUN\n(2) ATTACK\n(3) CONTINUE\n(4) CIRCLE THE WAGONS\n";
            std::cout << "Enter your choice (1-4): ";
            std::cin >> riderChoice;
            if (riderChoice <= 1 && riderChoice >= 4) break;
            std::cout << "Invalid choice. Please enter a number between 1 and 4.\n";
        }

        switch (riderChoice) {
        case 1:
            std::cout << "You try to outrun the riders. You lose some supplies in the rush.\n";
            break;
        case 2:
            std::cout << "You prepare to attack. Your ammo decreases, but you scare them off.\n";
            break;
        case 3:
            std::cout << "You continue calmly. The riders pass without incident.\n";
            break;
        case 4:
            std::cout << "You circle the wagons defensively. Everyone feels safer.\n";
            break;
        default:
            std::cout << "Invalid rider choice.\n";
        }
    }
    else {
        std::cout << "No rider encounter this turn.\n";
    }


    while (true) {
        std::cout << "\nDO YOU WANT TO EAT:\n";
        std::cout << "(1) POORLY\n(2) MODERATELY\n(3) WELL\n";
        std::cout << "Enter your choice (1-3): ";
        std::cin >> eatingChoice;

        if (eatingChoice >= 1 && eatingChoice <= 3) break;
        std::cout << "Invalid choice. Please enter a number between 1 and 3.\n";
    }

    switch (eatingChoice) {
    case 1:
        std::cout << "You eat poorly. Morale drops and health may suffer.\n";
        break;
    case 2:
        std::cout << "You eat moderately. Everyone stays steady.\n";
        break;
    case 3:
        std::cout << "You eat well. Spirits are high, but food supplies decrease faster.\n";
        break;
    default:
        std::cout << "Invalid eating choice.\n";
    }


    if (fortChance < 50) {
        while (true) {
            std::cout << "\nTHESE'S A FORT ALONG THE TRAIL.\n";
            std::cout << "DO YOU WANT TO:\n";
            std::cout << "(1) STOP AT THE FORT\n(2) HUNT\n(3) CONTINUE\n";
            std::cout << "Enter your choice (1-3): ";
            std::cin >> fortChoice;
            if (fortChoice >= 1 && fortChoice <= 3) break;
            std::cout << "INVALID CHOICE. TRY AGAIN \n";
        }

        // Fort logic
        switch (fortChoice) {
        case 1:
            std::cout << "You stop at the fort. Prices are high, but you rest and resupply.\n";
            break;
        case 2:
            std::cout << "You go hunting. You gain food but lose time.\n";
            break;
        case 3:
            std::cout << "You continue on the trail. You save time but risk running low on supplies.\n";
            break;
        default:
            std::cout << "Invalid fort choice.\n";
        }
    }
    else {
        std::cout << "no fort encounter this turn. \n";
    }

    return 0;


}
