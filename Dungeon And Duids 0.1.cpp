#include <iostream>
#include <string>
#include <ctime>
using namespace std;

// Global variables
string playerName;
int playerHealth = 100;
int gold = 50;

// Function declarations
void titleScreen();
void loadGame();
void newGame();
void exitGame();
void prologue();
void inputName();
void idleGuild();
void guild();
void receptionist();
void busyMerchant();
void rest();
void selectDungeon();
void skibidiCastle();
void elMachoPrison();
void sigmaTempleRuin();
void camp();
void bonfire();
void interactFellowAdventurer();
void enterDungeon();
void travellingMerchant();
void dungeon();
void goDeeper();
void checkAround();

// Main function
int main() {
    srand(time(0)); // Initialize random seed
    titleScreen();
    return 0;
}

void titleScreen() {
    int choice;
    cout << "=== Dungeon and Duid ===\n";
    cout << "1. Load Game\n";
    cout << "2. New Game\n";
    cout << "3. Exit\n";
    cout << "Choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            loadGame();
            break;
        case 2:
            newGame();
            break;
        case 3:
            exitGame();
            break;
        default:
            cout << "Invalid choice. Returning to title screen.\n";
            titleScreen();
    }
}

void loadGame() {
    cout << "No saved games available yet. Returning to title screen.\n";
    titleScreen();
}

void newGame() {
    prologue();
    inputName();
    idleGuild();
}

void exitGame() {
    cout << "Exiting game. Goodbye!\n";
}

void prologue() {
    cout << "You are a brave adventurer starting your journey in the land of Duid.\n";
    cout << "Your goal is to explore dungeons, defeat enemies, and collect treasures.\n\n";
}

void inputName() {
    cout << "Enter your name: ";
    cin >> playerName;
    cout << "Welcome, " << playerName << "!\n";
}

void idleGuild() {
    guild();
}

void guild() {
    int choice;
    cout << "=== Guild ===\n";
    cout << "1. Receptionist (Choose dungeon)\n";
    cout << "2. Busy Merchant (Sell inventory)\n";
    cout << "3. Rest (Save game)\n";
    cout << "Choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            receptionist();
            break;
        case 2:
            busyMerchant();
            break;
        case 3:
            rest();
            break;
        default:
            cout << "Invalid choice. Returning to guild.\n";
            guild();
    }
}

void receptionist() {
    selectDungeon();
}

void busyMerchant() {
    cout << "You sold some items for 20 gold.\n";
    gold += 20;
    cout << "Current gold: " << gold << "\n";
    guild();
}

void rest() {
    cout << "You rest and save the game. Your health is fully restored.\n";
    playerHealth = 100;
    cout << "Current health: " << playerHealth << "\n";
    guild();
}

void selectDungeon() {
    int choice;
    cout << "=== Select Dungeon ===\n";
    cout << "1. Skibidi Castle\n";
    cout << "2. El Macho Prison\n";
    cout << "3. Sigma Temple Ruin\n";
    cout << "Choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            skibidiCastle();
            break;
        case 2:
            elMachoPrison();
            break;
        case 3:
            sigmaTempleRuin();
            break;
        default:
            cout << "Invalid choice. Returning to receptionist.\n";
            receptionist();
    }
}

void skibidiCastle() {
    cout << "You enter the mysterious Skibidi Castle.\n";
    camp();
}

void elMachoPrison() {
    cout << "You step into the dark El Macho Prison.\n";
    camp();
}

void sigmaTempleRuin() {
    cout << "You explore the ancient Sigma Temple Ruin.\n";
    camp();
}

void camp() {
    int choice;
    cout << "=== Camp ===\n";
    cout << "1. Bonfire (Save game)\n";
    cout << "2. Interact with fellow adventurer\n";
    cout << "3. Enter dungeon\n";
    cout << "4. Travelling Merchant (Buy weapons)\n";
    cout << "Choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            bonfire();
            break;
        case 2:
            interactFellowAdventurer();
            break;
        case 3:
            enterDungeon();
            break;
        case 4:
            travellingMerchant();
            break;
        default:
            cout << "Invalid choice. Returning to camp.\n";
            camp();
    }
}

void bonfire() {
    cout << "You save your game and feel refreshed by the warmth of the bonfire.\n";
    playerHealth = 100;
    cout << "Current health: " << playerHealth << "\n";
    camp();
}

void interactFellowAdventurer() {
    cout << "You exchange stories with a fellow adventurer and gain some wisdom.\n";
    camp();
}

void enterDungeon() {
    dungeon();
}

void travellingMerchant() {
    cout << "You buy a better sword for 30 gold.\n";
    if (gold >= 30) {
        gold -= 30;
        cout << "Current gold: " << gold << "\n";
    } else {
        cout << "Not enough gold!\n";
    }
    camp();
}

void dungeon() {
    int choice;
    cout << "=== Dungeon ===\n";
    cout << "1. Go deeper\n";
    cout << "2. Check around\n";
    cout << "3. Return\n";
    cout << "Choice: ";
    cin >> choice;

    switch (choice) {
        case 1:
            goDeeper();
            break;
        case 2:
            checkAround();
            break;
        case 3:
            camp();
            break;
        default:
            cout << "Invalid choice. Returning to dungeon.\n";
            dungeon();
    }
}

void goDeeper() {
    cout << "You venture deeper into the dungeon and encounter a monster!\n";
    int monsterHealth = rand() % 50 + 50;
    while (playerHealth > 0 && monsterHealth > 0) {
        int damage = rand() % 20 + 5;
        cout << "You deal " << damage << " damage to the monster.\n";
        monsterHealth -= damage;

        if (monsterHealth > 0) {
            damage = rand() % 15 + 5;
            cout << "The monster deals " << damage << " damage to you.\n";
            playerHealth -= damage;
        }
    }

    if (playerHealth > 0) {
        cout << "You defeated the monster and found 20 gold!\n";
        gold += 20;
    } else {
        cout << "You were defeated. Returning to title screen.\n";
        titleScreen();
    }

    dungeon();
}

void checkAround() {
    cout << "You find a treasure chest and gain 10 gold!\n";
    gold += 10;
    dungeon();
}
