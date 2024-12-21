#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
using namespace std;

// Global variables
string playerName;
int playerHealth = 100;
int gold = 50;
string chosenWeapon = "";

// Structs for inventory and weapons
struct InventoryItem {
    string name;
    int price;
};

struct Weapon {
    string name;
    int damage;
};

// Replace vector with arrays
const int MAX_INVENTORY_SIZE = 100;
const int MAX_WEAPONS_SIZE = 50;
const int MAX_MERCHANT_ITEMS = 10;
InventoryItem playerInventory[MAX_INVENTORY_SIZE];
Weapon weaponList[MAX_WEAPONS_SIZE];
int playerInventorySize = 3; // Initial inventory size
int weaponListSize = 0;      // Initial weapon list size

// Merchant items
InventoryItem merchantItems[MAX_MERCHANT_ITEMS] = { {"Long Sword", 50}, {"Shield", 30}, {"Potion", 10} };
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
        else if (playerInventory[i].name == "Shield") playerInventory[i].price = 30;
        else if (playerInventory[i].name == "Potion") playerInventory[i].price = 10;
    }
}

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
void checkInventory();

// Load and save weapons to file
void loadWeapons() {
    ifstream file("weapons.txt");
    if (!file.is_open()) return;

    weaponListSize = 0;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string name, damageStr;
        getline(ss, name, ',');
        getline(ss, damageStr);

        // Replace underscores with spaces
        for (char &c : name) {
            if (c == '_') c = ' ';
        }

        if (name == "ChosenWeapon") {
            chosenWeapon = damageStr;
        } else {
            weaponList[weaponListSize++] = {name, stoi(damageStr)};
        }
    }
    file.close();
}

void saveWeapons() {
    ofstream file("weapons.txt");
    for (int i = 0; i < weaponListSize; ++i) {
        string name = weaponList[i].name;

        // Replace spaces with underscores
        for (char &c : name) {
            if (c == ' ') c = '_';
        }

        file << name << "," << weaponList[i].damage << "\n";
    }
    if (!chosenWeapon.empty()) {
        file << "ChosenWeapon," << chosenWeapon << "\n";
    }
    file.close();
}

// Check if player owns a weapon
bool ownsWeapon(const string &weaponName) {
    for (int i = 0; i < weaponListSize; ++i) {
        if (weaponList[i].name == weaponName) return true;
    }
    return false;
}

// Main function
int main() {
    srand(time(0)); // Initialize random seed
    initializePlayerInventory();
    assignDefaultPricesToInventory();
    loadWeapons();
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
        saveFile << "Inventory: ";
        for (int i = 0; i < playerInventorySize; ++i) {
            saveFile << playerInventory[i].name;
            if (i < playerInventorySize - 1) saveFile << ", ";
        }
        saveFile << "\n";
        saveFile << "Currency: " << gold << "\n";
        saveFile.close();
        saveWeapons();
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
        if (ownsWeapon(playerInventory[i].name)) {
            continue; // Skip weapons
        }
        cout << i + 1 << ". " << playerInventory[i].name 
             << " - " << playerInventory[i].price << " gold\n";
    }
    cout << "Enter the number to sell an item or 0 to exit: ";
    int choice;
    cin >> choice;

    if (choice > 0 && choice <= playerInventorySize) {
        if (ownsWeapon(playerInventory[choice - 1].name)) {
            cout << "You cannot sell weapons!\n";
        } else {
            cout << "You sold " << playerInventory[choice - 1].name << " for " << playerInventory[choice - 1].price << " gold.\n";
            gold += playerInventory[choice - 1].price;

            // Remove item by shifting array
            for (int i = choice - 1; i < playerInventorySize - 1; ++i) {
                playerInventory[i] = playerInventory[i + 1];
            }
            --playerInventorySize;
        }
    }
    guild();
}

void travellingMerchant() {
    cout << "=== Travelling Merchant ===\n";
    for (int i = 0; i < merchantItemsSize; ++i) {
        cout << i + 1 << ". " << merchantItems[i].name;

        if (ownsWeapon(merchantItems[i].name)) {
            cout << " (Already owned)";
        } else {
            cout << " (" << merchantItems[i].price << " gold)";
        }

        cout << "\n";
    }
    cout << "Enter the number to buy an item or 0 to exit: ";
    int choice;
    cin >> choice;

    if (choice > 0 && choice <= merchantItemsSize) {
        if (ownsWeapon(merchantItems[choice - 1].name)) {
            cout << "You already own this weapon.\n";
        } else if (gold >= merchantItems[choice - 1].price) {
            gold -= merchantItems[choice - 1].price;
            cout << "You bought " << merchantItems[choice - 1].name << ".\n";

            // Add to weapon list if it's a weapon
            if (merchantItems[choice - 1].name == "Long Sword") {
                weaponList[weaponListSize++] = {"Long Sword", 50};
            }
        } else {
            cout << "Not enough gold!\n";
        }
    }
    camp();
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
    cout << "| 5. Cek Inventory                              |\n";
    cout << "| 6. Kembali ke Guild                           |\n";
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
            checkInventory();
            break;
        case 6:
            guild();
            break;
        default:
            cout << "Pilihan Invalid. Balik ke camp.\n";
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
    cout << "Interaksi dengan adventurer lain sedang tidak tersedia.\n";
}

void enterDungeon() {
    cout << "Memasuki dungeon. Fungsi belum diimplementasikan.\n";
}

void checkInventory() {
    cout << "=== Inventory ===\n";
    for (int i = 0; i < playerInventorySize; ++i) {
        cout << i + 1 << ". " << playerInventory[i].name << "\n";
    }

    cout << "\n=== Weapons ===\n";
    for (int i = 0; i < weaponListSize; ++i) {
        cout << i + 1 << ". " << weaponList[i].name << " (Damage: " << weaponList[i].damage << ")";
        if (weaponList[i].name == chosenWeapon) {
            cout << " [Chosen]";
        }
        cout << "\n";
    }

    cout << "\nChoose a weapon to equip (0 to cancel): ";
    int choice;
    cin >> choice;

    if (choice > 0 && choice <= weaponListSize) {
        chosenWeapon = weaponList[choice - 1].name;
        cout << "You have chosen " << chosenWeapon << ".\n";
        saveWeapons();
    }
    camp();
}
