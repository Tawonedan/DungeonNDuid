#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
using namespace std;

// Global variables
string playerName;
int playerHealth = 100;
int gold = 50;

// Structs for inventory and merchant items
struct InventoryItem {
    string name;
    int price;
};

struct Item {
    string name;
    int price;
};

vector<InventoryItem> playerInventory = { {"Antique Hourglass", 20}, {"Old Coin", 15}, {"Old Coin", 15} };
vector<Item> merchantItems = { {"Long Sword", 50}, {"Shield", 30}, {"Potion", 10} };

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
void saveGame();
void selectDungeon();
void skibidiCastle();
void elMachoPrison();
void sigmaTempleRuin();
void camp();
void bonfire();
void interactFellowAdventurer();
void enterDungeon();
void travellingMerchant();
void sellItems();
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
    cout << "Pilih : ";
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
            cout << "Invalid Boy\n";
            titleScreen();
    }
}

void loadGame() {
    ifstream loadFile("savegame.txt");
    if (loadFile.is_open()) {
        string line;
        while (getline(loadFile, line)) {
            if (line.find("PlayerName:") == 0) {
                playerName = line.substr(12);
            } else if (line.find("Health:") == 0) {
                playerHealth = stoi(line.substr(8));
            } else if (line.find("Strength:") == 0) {
                // Strength could be used if added
            } else if (line.find("Inventory:") == 0) {
                string inventoryLine = line.substr(11);
                playerInventory.clear();
                size_t pos = 0;
                while ((pos = inventoryLine.find(", ")) != string::npos) {
                    string itemName = inventoryLine.substr(0, pos);
                    playerInventory.push_back({itemName, 0}); // Default price to be updated
                    inventoryLine.erase(0, pos + 2);
                }
                if (!inventoryLine.empty()) {
                    playerInventory.push_back({inventoryLine, 0});
                }
                // Assign prices to items (example: adjust based on your logic)
                for (auto &item : playerInventory) {
                    if (item.name == "Antique Hourglass") item.price = 20;
                    else if (item.name == "Old Coin") item.price = 15;
                    else if (item.name == "Long Sword") item.price = 50;
                    // Add more mappings if needed
                }
            } else if (line.find("Currency:") == 0) {
                gold = stoi(line.substr(10));
            }
        }
        loadFile.close();
        cout << "Mantap Ke Load rek!\n";
        idleGuild();
    } else {
        cout << "G ad sv game. Starting a new game.\n";
        titleScreen();
    }
}


void newGame() {
    prologue();
    inputName();
    idleGuild();
}

void exitGame() {
    cout << "Balik lagi ya. Dada >////<!\n";
}

void prologue() {
    cout << "Welcome to the world of Duid, where adventurers seek fortune and fame.\n";
    cout << "You are a novice adventurer seeking to make a name for yourself.\n";
    cout << "Your journey begins at the Adventurers' Guild, where your choices shape your destiny.\n";
}

void inputName() {
    cout << "Masukkan nama: ";
    cin >> playerName;
    cout << "Welkam, " << playerName << "!\n";
}

void idleGuild() {
    guild();
}

void guild() {
    int choice;
    cout << "=== Guild ===\n";
    cout << "1. Receptionist (Pilih Dungeon)\n";
    cout << "2. Busy Merchant (Sell inventory)\n";
    cout << "3. Rest (Save game)\n";
    cout << "4. Exit ke Menu Utama\n";
    cout << "Pilih: ";
    cin >> choice;

    switch (choice) {
        case 1:
            receptionist();
            break;
        case 2:
            sellItems();
            break;
        case 3:
            rest();
            break;
        case 4:
            titleScreen();
            break;
        default:
            cout << "Pilihan Salah. Balik to guild.\n";
            guild();
    }
}

void saveGame() {
    ofstream saveFile("savegame.txt");
    if (saveFile.is_open()) {
        saveFile << "PlayerName: " << playerName << "\n";
        saveFile << "Health: " << playerHealth << "\n";
        saveFile << "Strength: 20\n"; // Example value for strength
        saveFile << "Inventory: ";
        for (size_t i = 0; i < playerInventory.size(); ++i) {
            saveFile << playerInventory[i].name;
            if (i < playerInventory.size() - 1) saveFile << ", ";
        }
        saveFile << "\n";
        saveFile << "Weapon: Long Sword, Rogue knife\n"; // Example weapons
        saveFile << "ChoosenWeapon: Rogue knife\n"; // Example chosen weapon
        saveFile << "Location: Cave\n"; // Example location
        saveFile << "Currency: " << gold << "\n";
        saveFile.close();
        cout << "Game saved successfully!\n";
    } else {
        cout << "Error saving the game!\n";
    }
}

void rest() {
    cout << "You rest and save the game. Your health is fully restored.\n";
    playerHealth = 100;
    saveGame();
    guild();
}

void receptionist() {
    selectDungeon();
}

void sellItems() {
    cout << "=== Sell Items ===\n";
    for (size_t i = 0; i < playerInventory.size(); ++i) {
        cout << i + 1 << ". " << playerInventory[i].name 
             << " - " << playerInventory[i].price << " gold\n";
    }
    cout << "Enter the number to sell an item or 0 to exit: ";
    int choice;
    cin >> choice;

    if (choice > 0 && choice <= playerInventory.size()) {
        cout << "You sold " << playerInventory[choice - 1].name << " for " << playerInventory[choice - 1].price << " gold.\n";
        gold += playerInventory[choice - 1].price;
        playerInventory.erase(playerInventory.begin() + (choice - 1));
    }
    guild();
}

void selectDungeon() {
    int choice;
    cout << "=== Select Dungeon ===\n";
    cout << "1. Skibidi Castle\n";
    cout << "2. El Macho Prison\n";
    cout << "3. Sigma Temple Ruin\n";
    cout << "Pilih : ";
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
            cout << "Pilihan invalid. Balik to receptionist.\n";
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
    cout << "5. Return to Guild\n";
    cout << "Pilih: ";
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
        case 5:
            guild();
            break;
        default:
            cout << "Pilihan Invalid. Balek to camp.\n";
            camp();
    }
}

void bonfire() {
    cout << "You save your game and feel refreshed by the warmth of the bonfire.\n";
    playerHealth = 100;
    saveGame();
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
    cout << "=== Travelling Merchant ===\n";
    for (size_t i = 0; i < merchantItems.size(); ++i) {
        cout << i + 1 << ". " << merchantItems[i].name << " (" << merchantItems[i].price << " gold)\n";
    }
    cout << "Enter the number to buy an item or 0 to exit: ";
    int choice;
    cin >> choice;

    if (choice > 0 && choice <= merchantItems.size()) {
        if (gold >= merchantItems[choice - 1].price) {
            gold -= merchantItems[choice - 1].price;
            cout << "You bought " << merchantItems[choice - 1].name << ".\n";
            playerInventory.push_back({merchantItems[choice - 1].name, merchantItems[choice - 1].price});
        } else {
            cout << "Not enough gold!\n";
        }
    }
    camp();
}

void dungeon() {
    int choice;
    cout << "=== Dungeon ===\n";
    cout << "1. Go deeper\n";
    cout << "2. Check around\n";
    cout << "3. Return\n";
    cout << "Pilih : ";
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
            cout << "Pilihan Invalid. Balek to dungeon.\n";
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
