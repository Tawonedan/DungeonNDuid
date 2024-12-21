#include <iostream>
#include <fstream>
#include <string>
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

// Replace vector with arrays
const int MAX_INVENTORY_SIZE = 100;
const int MAX_MERCHANT_ITEMS = 10;
InventoryItem playerInventory[MAX_INVENTORY_SIZE];
int playerInventorySize = 3; // Initial inventory size
Item merchantItems[MAX_MERCHANT_ITEMS] = { {"Long Sword", 50}, {"Shield", 30}, {"Potion", 10} };
int merchantItemsSize = 3;

// Initialize player inventory
void initializePlayerInventory() {
    playerInventory[0] = {"Antique Hourglass", 20};
    playerInventory[1] = {"Old Coin", 15};
    playerInventory[2] = {"Old Coin", 15};
}

void assignDefaultPricesToInventory() {
    for (int i = 0; i < playerInventorySize; ++i) {
        if (playerInventory[i].name == "Antique Hourglass") playerInventory[i].price = 20;
        else if (playerInventory[i].name == "Old Coin") playerInventory[i].price = 15;
        else if (playerInventory[i].name == "Long Sword") playerInventory[i].price = 50;
        else if (playerInventory[i].name == "Shield") playerInventory[i].price = 30;
        else if (playerInventory[i].name == "Potion") playerInventory[i].price = 10;
    }
}

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
    initializePlayerInventory();
    assignDefaultPricesToInventory();
    titleScreen();
    return 0;
}

void titleScreen() {
    int choice;
    cout << "==================================================\n";
    cout << "|                 Dungeon n Duid                 |\n";
    cout << "==================================================\n";
    cout << "| 1. Load game                                   |\n";
    cout << "| 2. New Game                                    |\n";
    cout << "| 3. Keluar                                      |\n";
    cout << "==================================================\n";
    cout << "Pilih: ";
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
        playerInventorySize = 0;
        while (getline(loadFile, line)) {
            if (line.find("PlayerName:") == 0) {
                playerName = line.substr(12);
            } else if (line.find("Health:") == 0) {
                playerHealth = stoi(line.substr(8));
            } else if (line.find("Inventory:") == 0) {
                string inventoryLine = line.substr(11);
                size_t pos = 0;
                while ((pos = inventoryLine.find(", ")) != string::npos) {
                    playerInventory[playerInventorySize++] = {inventoryLine.substr(0, pos), 0};
                    inventoryLine.erase(0, pos + 2);
                }
                if (!inventoryLine.empty()) {
                    playerInventory[playerInventorySize++] = {inventoryLine, 0};
                }
            } else if (line.find("Currency:") == 0) {
                gold = stoi(line.substr(10));
            }
        }
        loadFile.close();
        assignDefaultPricesToInventory();
        cout << "Mantap, Ke Load rek!\n";
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
    cout << "==================================================\n";
    cout << "|                    Prologue                   |\n";
    cout << "==================================================\n";
    cout << "| Di dunia Duid, sebuah tanah penuh reruntuhan   |\n";
    cout << "| kuno dan ruang bawah tanah mematikan, kisah    |\n";
    cout << "| tentang kekayaan dan bahaya bergema di seluruh |\n";
    cout << "| guild.                                         |\n";
    cout << "|                                                |\n";
    cout << "| Sebagai seorang petualang pemula, Anda tiba di |\n";
    cout << "| Adventurers' Guild dengan semangat membara.    |\n";
    cout << "| Hari ini, perjalanan Anda dimulai. Apakah Anda |\n";
    cout << "| akan mencapai kejayaan, atau menjadi jiwa yang |\n";
    cout << "| terlupakan di kegelapan?                       |\n";
    cout << "==================================================\n";
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
    cout << "==================================================\n";
    cout << "|                    Guild Menu                 |\n";
    cout << "==================================================\n";
    cout << "| 1. Resepsionis (Pilih dungeon)                |\n";
    cout << "| 2. Pedagang Sibuk (Jual inventaris)           |\n";
    cout << "| 3. Istirahat (Simpan permainan)               |\n";
    cout << "| 4. Kembali ke menu utama                      |\n";
    cout << "==================================================\n";
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
        saveFile << "Strength: 20\n";
        saveFile << "Inventory: ";
        for (int i = 0; i < playerInventorySize; ++i) {
            saveFile << playerInventory[i].name;
            if (i < playerInventorySize - 1) saveFile << ", ";
        }
        saveFile << "\n";
        saveFile << "Weapon: Long Sword, Rogue knife\n";
        saveFile << "ChoosenWeapon: Rogue knife\n";
        saveFile << "Location: Cave\n";
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
    for (int i = 0; i < playerInventorySize; ++i) {
        cout << i + 1 << ". " << playerInventory[i].name 
             << " - " << playerInventory[i].price << " gold\n";
    }
    cout << "Enter the number to sell an item or 0 to exit: ";
    int choice;
    cin >> choice;

    if (choice > 0 && choice <= playerInventorySize) {
        cout << "You sold " << playerInventory[choice - 1].name << " for " << playerInventory[choice - 1].price << " gold.\n";
        gold += playerInventory[choice - 1].price;

        // Remove item by shifting array
        for (int i = choice - 1; i < playerInventorySize - 1; ++i) {
            playerInventory[i] = playerInventory[i + 1];
        }
        --playerInventorySize;
    }
    guild();
}

void selectDungeon() {
    int choice;
    cout << "==================================================\n";
    cout << "|               Pilih Dungeon                  |\n";
    cout << "==================================================\n";
    cout << "| 1. Skibidi Castle                             |\n";
    cout << "| 2. El Macho Prison                            |\n";
    cout << "| 3. Sigma Temple Ruin                          |\n";
    cout << "==================================================\n";
    cout << "Pilih: ";
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
    cout << "==================================================\n";
    cout << "|                  Perkemahan                   |\n";
    cout << "==================================================\n";
    cout << "| 1. Bonfire    (Simpan permainan)              |\n";
    cout << "| 2. Interaksi Fellow Adventurer                |\n";
    cout << "| 3. Masuk ke dungeon                           |\n";
    cout << "| 4. Travelling Merchant (Beli senjata)         |\n";
    cout << "| 5. Kembali ke Guild                           |\n";
    cout << "==================================================\n";
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
    for (int i = 0; i < merchantItemsSize; ++i) {
        cout << i + 1 << ". " << merchantItems[i].name << " (" << merchantItems[i].price << " gold)\n";
    }
    cout << "Enter the number to buy an item or 0 to exit: ";
    int choice;
    cin >> choice;

    if (choice > 0 && choice <= merchantItemsSize) {
        if (gold >= merchantItems[choice - 1].price) {
            gold -= merchantItems[choice - 1].price;
            cout << "You bought " << merchantItems[choice - 1].name << ".\n";
            playerInventory[playerInventorySize++] = {merchantItems[choice - 1].name, merchantItems[choice - 1].price};
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
