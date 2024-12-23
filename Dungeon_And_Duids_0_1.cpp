#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime> 
#include <cmath>
#include <cstdint>
#include <cstring>
#include <string>

using namespace std;

////////// Declare Struct ////////////
struct Player {
    string name;
    int health;
    int strength;
    string inventory;
    string location;
    int currency;
};

struct Enemy {
    string name;
    int health;
    int strength;
};

struct InventoryItem {
    string name;
    int price;
};

struct Weapon {
    string name;
    int damage;
};
struct Item {
    string name;
    int value;
};

////////// Declare Struct ////////////


////////// Array Declare ////////////
const Item itemList[] = {
    {"Golden Necklace", 50},
    {"Silver Boot", 15},
    {"Old Wine", 10},
    {"Gilded Watch", 20},
    {"Antique Belt", 25},
    {"Golden Coin", 30}
};

////////// Array Declare ////////////


////////// General Variable Declare ////////////
const int MAX_INVENTORY_SIZE = 100;
const int MAX_WEAPONS_SIZE = 50;
const int MAX_MERCHANT_ITEMS = 10;
const int MAX_SIZE = 10; // Batas maksimum data
const int MAX_ITEMS = 100; // Batas maksimum data
const int numItems = sizeof(itemList) / sizeof(itemList[0]);

int playerHealth = 100;
int gold = 50;
int weaponCount = 0;
int currentDepth = 0;
int maxDepth = 0;
int playerInventorySize = 3; // Initial inventory size
int weaponListSize = 0;      // Initial weapon list size
int merchantItemsSize = 3;
int count = 0; // Variabel global untuk jumlah elemen aktif
int currentWeaponDamage;

bool hasCheckedAround = false;
bool itemInitialize = false;
bool battleOngoing = false;

string playerName;

string chosenWeapon = "";
string items[MAX_INVENTORY_SIZE];
string inventoryLine;
string currentEnemyName;
////////// General Variable Declare ////////////


////////// Struct Variable Declare ////////////
InventoryItem playerInventory[MAX_INVENTORY_SIZE];
InventoryItem merchantItems[MAX_MERCHANT_ITEMS] = { {"Long Sword", 20}, {"Rogue Knife", 35}, {"pisau", 10}, };

Weapon weaponList[MAX_WEAPONS_SIZE];

Player dataList[MAX_SIZE]; // Array untuk menyimpan data
Player* loadAllPlayers(int& playerCount);
Player currentPlayer;


Enemy loadEnemyData(string enemyName);
Enemy currentEnemy;


////////// Struct Variable Declare ////////////


////////// Function Declare ////////////
void titleScreen();
void loadGame();
void newGame();
void exitGame();
void saveGame();

void prologue();
void inputName();

void idleGuild();
void guild();
void receptionist();
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
void sellItems();
void checkInventory();

void dungeon();
void goDeeper();
void checkAround();
void battle(const string& playerName, const string& enemyName);
void handleChest();
void handleSpecialEvent();
void displayChest();
void displayChestResult(bool foundTreasure, const Item& treasure);
void displayDungeonMenu();
void eventKolam();
void eventBadut();
Item getRandomItem();

string generateCommonEnemy();

////////// Function Declare ////////////




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

void resetWeapons() {
    weaponListSize = 0; // Reset weapon inventory
    chosenWeapon = ""; // Reset chosen weapon
    ofstream file("weapons.txt", ios::out | ios::trunc); // Overwrite file
    if (file.is_open()) {
        file.close();
    }
}

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
    ofstream file("weapons.txt", ios::out | ios::trunc);
    if (file.is_open()) {
        for (int i = 0; i < weaponListSize; ++i) {
            string name = weaponList[i].name;

            // Ganti spasi dengan garis bawah untuk menyimpan format yang konsisten
            for (char &c : name) {
                if (c == ' ') c = '_';
            }

            file << name << "," << weaponList[i].damage << "\n";
        }
        if (!chosenWeapon.empty()) {
            file << "ChosenWeapon," << chosenWeapon << "\n";
        }
        file.close();
        cout << "Weapons saved successfully!\n"; // Debugging log
    } else {
        cout << "Error saving weapons!\n"; // Debugging log
    }

}


// Check if player owns a weapon
bool ownsWeapon(const string &weaponName) {
    for (int i = 0; i < weaponListSize; ++i) {
        if (weaponList[i].name == weaponName) return true;
    }
    return false;
}

// Utility functions
int randomRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

Item getRandomItem() {
    int index = randomRange(0, numItems - 1);
    return itemList[index];
}

void increaseHealth(int healAmount) {
    int previousHealth = playerHealth; // Simpan nilai sebelum
    playerHealth = min(playerHealth + healAmount, 100); // Batas maksimum 100
    cout << "Health: " << previousHealth << " -> " << playerHealth << "\n";
}

void displayDungeonMenu() {
    cout << "====================================\n";
    cout << "|          Dungeon Options         |\n";
    cout << "====================================\n";
    cout << "|1. Go deeper into the dungeon.    |\n"; 
    cout << "|2. Check around for resources.    |\n";
    cout << "|3. Return to the previous depth.  |\n";
    if (currentDepth % 4 == 0 && currentDepth != 0) {
        cout << "|4. Rest.                          |\n";
    }
    cout << "====================================\n\n";
    cout << "Enter your choice: ";
}

void displayChest() {
    cout << "====================================\n";
    cout << "|         You found a chest!       |\n";
    cout << "====================================\n";
    cout << "|What would you like to do?        |\n";
    cout << "|1. Open the chest.                |\n";
    cout << "|2. Leave it alone.                |\n";
    cout << "====================================\n\n";
    cout << "Enter your choice: ";
}

void displayChestResult(bool foundTreasure, const Item& treasure) {
    if (foundTreasure) {
        cout << "====================================\n";
        cout << "|        You opened the chest!     |\n";
        cout << "====================================\n";
        cout << "You found a " << treasure.name << " worth " 
             << treasure.value << " gold!\n";
        cout << "====================================\n\n";
    } else {
        cout << "=========================================\n";
        cout << "|          It's a trap!                 |\n";
        cout << "=========================================\n";
        cout << "The chest was a mimic! Prepare to fight!\n";
        cout << "=========================================\n\n";
    }
}

void addItemToInventory(const Item& newItem) {
    if (playerInventorySize < MAX_INVENTORY_SIZE) {
        playerInventory[playerInventorySize++] = {newItem.name, newItem.value};
    } else {
        cout << "Inventory is full!\n";
    }
}

void handleChest() {
    displayChest();
    int choice;
    cin >> choice;

    if (choice == 1) {
        int outcome = randomRange(0, 3); // 3/4 peluang untuk mendapatkan item
        if (outcome < 3) {
            Item treasure = getRandomItem();
            displayChestResult(true, treasure);
            addItemToInventory(treasure); // Tambahkan item ke inventaris
        } else {
            displayChestResult(false, Item{"Mimic", 0});
            
            string EnemyName = "Mimic Chest";
            loadEnemyData(EnemyName);
            battle(playerName, EnemyName);
        }
    } else {
        cout << "You leave the chest behind.\n";
    }
}


void handleSpecialEvent() {
    int eventRoll = randomRange(1, 100); // Angka acak antara 1-100

    if (eventRoll <= 25) { // Event Kolam
        cout << "====================================\n";
        cout << "    You found a mysterious pool!   |\n";
        cout << "====================================\n";
        cout << "Would you like to enter the pool?  |\n";
        cout << "1. Yes 2. No                       |\n";
        cout << "====================================\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            if (randomRange(0, 1) == 0) {
                cout << "The pool is sacred! Your health is fully restored.\n";
                playerHealth = 100;
            } else {
                cout << "The pool is poisonous! You lose 50% of your health.\n";
                playerHealth = max(playerHealth / 2, 1);
            }
        } else {
            cout << "You decide to avoid the pool and move on.\n";
        }
    } else if (eventRoll <= 50) { // Event Badut
        cout << "===================================================\n";
        cout << "|            You encounter a clown!               |\n";
        cout << "===================================================\n";
        cout << "The clown offers a mysterious deal for 30 gold.   |\n";
        cout << "Would you like to accept?                         |\n"; 
        cout << "1. Yes 2. No                                      |\n";
        cout << "===================================================";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            if (gold >= 30) {
                gold -= 30;
                if (randomRange(0, 1) == 0) {
                    cout << "The clown gives you 50 gold in return!\n";
                    gold += 50;
                } else {
                    cout << "The clown disappears without a trace!\n";
                }
            } else {
                cout << "You don't have enough gold! The clown laughs and leaves.\n";
            }
        } else {
            cout << "You decline the clown's offer and move on.\n";
        }
    } else if (eventRoll <= 75) { // Event Mayat
        cout << "====================================\n";
        cout << "|    You find a pile of corpses.   |\n";
        cout << "====================================\n";
        cout << "|What would you like to do?        |\n";
        cout << "|1. Interact/Search.               |\n";
        cout << "|2. Leave it alone.                |\n";
        cout << "====================================\n";
        cout << "Enter your choice: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            int outcome = randomRange(0, 3); // 2/4 chance positive
            if (outcome < 2) {
                int foundGold = randomRange(10, 30);
                cout << "You found " << foundGold << " gold!\n";
                gold += foundGold;
            } else {
                cout << "One of the corpses reanimates as a zombie! Prepare to fight!\n";
                string EnemyName = "Zombie";
                loadEnemyData(EnemyName);
                battle(playerName, EnemyName);
            }
        } else {
            cout << "You decide to leave the corpses alone and move on.\n";
        }
    } else { // Event Adventurer
        cout << "=====================================\n";
        cout << "| You encounter a fellow adventurer! |\n";
        cout << "=====================================\n";
        cout << "They offer you a potion. You restore 20 health points.\n";
        playerHealth = min(playerHealth + 20, 100);
        cout << "Your current health: " << playerHealth << "\n";
    }
}


void navigateDepth(int depth) {

    if (depth < 1) {
        cout << "You are at the entrance. Returning to camp.\n";
        camp();
        return;
    }

    if (depth > maxDepth) {
        cout << "You have reached the maximum depth! Prepare for the final challenge!\n";
        return;
    }

    currentDepth = depth;
    hasCheckedAround = false;
    cout << "Current Depth: " << currentDepth << "\n";

    // Cek apakah lantai adalah checkpoint (kelipatan 4)
    if (currentDepth % 4 == 0) {
        cout << "You found a checkpoint! You can rest here.\n";
        dungeon();
        return;
    }

    // Event probabilitas: 60% musuh, 20% peti, 20% spesial
    int event = randomRange(0, 9);
    if (event < 6) {
        cout << "You encounter an enemy!\n";


        string EnemyName = generateCommonEnemy();
        loadEnemyData(EnemyName);
        battle(playerName, EnemyName);
    } else if (event < 8) {
        cout << "You find a chest!\n";
        handleChest();
    } else {
        handleSpecialEvent();
    }

    if (!battleOngoing) {
    dungeon();
    }
    
}

void checkAround() {
    if (hasCheckedAround) {
        cout << "You have already checked around at this depth. Try something else.\n";
        dungeon();
        return;
    }

    hasCheckedAround = true;
    int event = randomRange(0, 9); // 80% no result, 20% find chest
    if (event < 8) {
        cout << "You searched around but found nothing of value.\n";
    } else {
        cout << "You found a chest!\n";
        handleChest();
    }

    cout << "You remain at depth " << currentDepth << ".\n";
    dungeon();
}

////////////////////////////////// Inventory Function /////////////////////////////////// 
void displayInventory() {
    if (count == 0) {
        cout << "\nInventory kosong!" << endl;
    } else {
        cout << "\nInventory items:" << endl;
        for (int i = 0; i < count; i++) {
            cout << i + 1 << ". " << items[i] << endl;
        }
    }
}
int convertInventory(const string& input, string items[], int max_size) {
    // Ubah string menjadi array karakter untuk strtok
    char cstr[input.size() + 1];
    strcpy(cstr, input.c_str());
    
    // Pisahkan string berdasarkan delimiter ","
    char* token = strtok(cstr, ",");
    int index = 0;

    while (token != nullptr && index < max_size) {
        // Hilangkan spasi di awal jika ada
        while (*token == ' ') token++;

        items[index] = token; // Masukkan token ke array
        index++;
        
        token = strtok(nullptr, ","); // Ambil token berikutnya
    }

    return index; // Kembalikan jumlah elemen yang diisi

}

// Fungsi untuk menghapus elemen dari array
int removeItemFromArray(string items[], int count, int indexToRemove) {
    if (indexToRemove < 0 || indexToRemove >= count) {
        cout << "Invalid index!" << endl;
        return count; // Tidak ada yang dihapus
    }

    // Geser elemen ke kiri untuk menimpa elemen yang dihapus
    for (int i = indexToRemove; i < count - 1; i++) {
        items[i] = items[i + 1];
    }

    return count - 1; // Kurangi jumlah item aktif
}

////////////////////////////////// Inventory Function /////////////////////////////////// 



////////////////////////////////// Battle Function /////////////////////////////////// 

Enemy loadEnemyData(string enemyName) {
    ifstream inFile("enemyData.txt");
    if (!inFile) {
        cerr << "Error: Could not open enemyAsset.txt!" << endl;
        return Enemy{"", 0, 0}; // Mengembalikan enemy default jika file tidak ditemukan
    }

    string line;

    bool foundEnemy = false;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string key, value;

        getline(ss, key, ':');   // Baca key sebelum ':'
        getline(ss, value);     // Baca value setelah ':'

        if (!value.empty() && value[0] == ' ') {
            value = value.substr(1); // Hilangkan spasi awal jika ada
        }

        if (key == "EnemyName") {
            if (value == enemyName) {
                foundEnemy = true;
                currentEnemy = Enemy(); // Reset data enemy
                currentEnemy.name = value;
            }
        } else if (key == "Health" && foundEnemy) {
            currentEnemy.health = stoi(value);
        } else if (key == "Strength" && foundEnemy) {
            currentEnemy.strength = stoi(value);
        }

        // Jika sudah membaca semua data musuh yang diperlukan, keluar dari loop
        if (foundEnemy && key == "Strength") {
            break;
        }
    }

    inFile.close();

    if (!foundEnemy) {
        cerr << "Error: Enemy '" << enemyName << "' not found in enemyAsset.txt!" << endl;
        return Enemy{"", 0, 0}; // Mengembalikan enemy default jika tidak ditemukan
    }

    return currentEnemy;
}

string generateCommonEnemy() {
    int commonEnemyRate = randomRange(1, 4);

    switch (commonEnemyRate) {
    case 1:
        return "Skeletals";
        break;
    case 2:
        return  "Armed Skeletals";
        break;
    case 3:
        return  "Rogue Scales";
        break;
    case 4:
        return  "Feral Mandrake";
        break;
    default:
        break;
    }

}

void damageStr(const string& weaponName, int& enemyHealth) {
    for (int i = 0; i < weaponListSize; ++i) {
        if (weaponList[i].name == weaponName) {
            int damage = weaponList[i].damage;

            // Kurangi kesehatan musuh
            currentEnemy.health -= damage;

            // Tampilkan hasil serangan
            cout << "You attacked the enemy with " << weaponName 
                 << " and dealt " << damage << " damage!\n";
            cout << "Enemy health is now: " << max(enemyHealth, 0) << "\n";

            return;
        }
    }

    // Jika senjata tidak ditemukan
    cout << "Weapon not found! No damage dealt.\n";
}


void playerAttack() {
    currentEnemy.health -= currentPlayer.strength;
    cout << "You dealt " << currentPlayer.strength << " damage to the " << currentEnemy.name << ".\n";

}

void enemyAttack() {
    playerHealth -= currentEnemy.strength;
    cout << "The " << currentEnemy.name << " dealt " << currentEnemy.strength << " damage to you.\n";
}

void useBlazingBuster() {
    currentEnemy.health -= 30;
    cout << "You dealt 30 damage to the " << currentEnemy.name << " by using Blazing Buster" <<".\n";
}

void useRevitalize() {
    increaseHealth(50);
    cout << "You heal 50 health by using Revitalize"<< ".\n";
}

void playerSkill() {
    cout << "\n=======================================\n";
    cout << "|[1] Blazing Buster | [2] Revitalize  |\n";
    cout << "=======================================\n";
    cout << "Enter your choice: ";
                
    int skillChoice;
    cin >> skillChoice;

    switch (skillChoice) {
    case 1:
        useBlazingBuster();
    break;
    case 2:
        useRevitalize();
    break;

    default:
    cout << "\nInvalid skill choice.\n";
    }
}

void playerDefend() {
    int blockRate = randomRange(1, 4);

    int blockedPercentage = blockRate * 25;
    cout << "You choose to defend and blocked " << blockedPercentage << "% of the damage!\n";
    int incomingDamage = currentEnemy.strength;
    int blockedDamage = incomingDamage * (blockedPercentage / 100.0);
    int damageTaken = incomingDamage - blockedDamage;
    playerHealth -= damageTaken;
    cout << "You took " << damageTaken << " damage after blocking.\n";

}

string useItems() {
    string usedItem;

    if (!itemInitialize) { // Inisialisasi inventori hanya sekali
        string input = inventoryLine; // String input
        count = convertInventory(input, items, MAX_INVENTORY_SIZE);
        itemInitialize = true;
    }

    while (true) {
        displayInventory();

        if (count == 0) {
            break; // Keluar dari loop jika inventori kosong
        }

        int itemChoice;
        cout << "\nEnter the number of the item you want to remove (or 0 to exit): ";
        cin >> itemChoice;

        if (itemChoice < 1 || itemChoice > count) {
            cout << "Pilihan tidak valid!" << endl;
            continue;
        }

        if (itemChoice == 0) {
            break; // Keluar dari program jika pengguna memasukkan 0
        }

        if (itemChoice > 0 && itemChoice <= count) {
            cout << "You selected: " << items[itemChoice - 1] << endl;
        } else {
            cout << "Invalid selection!" << endl;
        }

        count = removeItemFromArray(items, count, itemChoice - 1);
        
        usedItem = items[itemChoice - 1];
        break;

    }

    cout << "\nUpdated inventory:" << endl;
    for (int i = 0; i < count; i++) {
        cout << i + 1 << ". " << items[i] << endl;
    }

    if (usedItem == "Potion") {
        increaseHealth(30);
        cout << "You use a Potion to heal.\n";
    }
    
}

void displayBattleHeader(const string& playerName, const string& enemyName) {
    cout << "\n====================================\n";
    cout << "|          Battle Begins!          |\n";
    cout << "====================================\n";
    cout << playerName << " faces off against " << enemyName << "!\n";
    cout << "====================================\n";
}

void displayBattleStatus(int playerHP, int enemyHP, const string& enemyName) {
    cout << "\n====================================\n";
    cout << "|            Battle Status         |\n";
    cout << "====================================\n";
    cout << "| Player HP: " << playerHP << "                   \n";
    cout << "| " << enemyName << " HP: " << enemyHP << "        \n";
    cout << "====================================\n";
}

void displayBattleMenu() {
    cout << "\n===============================================================\n";
    cout << "|[1] Attack | [2] Defend | [3] Skill | [4] Items | [5] Escape |\n";
    cout << "===============================================================\n";
    cout << "Enter your choice: ";
}

void displayGameOver(){
    cout << "\n===============================================================\n";
    cout << "|          Game Over! Returning to title screen.....          |\n";
    cout << "===============================================================\n";
}

void battle(const string& playerName, const string& enemyName) {
    battleOngoing = true;
    displayBattleHeader(playerName, currentEnemy.name);

    while (battleOngoing && playerHealth > 0 && currentEnemy.health > 0) {
        displayBattleStatus(playerHealth, currentEnemy.health, currentEnemy.name);
        displayBattleMenu();
        int battleChoice;
        cin >> battleChoice;
        switch (battleChoice) {
            case 1: {
               if (!chosenWeapon.empty()) {
                    damageStr(chosenWeapon, currentEnemy.health);
                } else {
                    cout << "No weapon equipped! You deal no damage.\n";
                }
                if (currentEnemy.health > 0) {
                    enemyAttack();
                }
                break;
            }
            case 2: {
                playerDefend();
                break;
            }
            case 3: {
                cout << "You choose to use skill\n";
                playerSkill();
                enemyAttack();
                break;
            }
            case 4: {
                cout << "You Used item\n";
                useItems();
                break;
            }
            case 5: {
                cout << "You choose to escape\n";

                int escapeChance = randomRange(0, 5);

                if (escapeChance == 1) {
                    cout << "You managed to escape\n";
                    battleOngoing = false;
                } else {
                    cout << "You failed to escape\n";
                    enemyAttack();
                }
                             
                    
                break;
            }
            default:
                cout << L"Invalid choice.\n";
        }

        if (playerHealth > 0 && currentEnemy.health <= 0) {
            cout << "\nYou have defeated the enemy!.\n";
            battleOngoing = false;
        }

        if (playerHealth <= 0) {
            cout << "\nYou are defeated by the enemy.....\n";
            battleOngoing = false;
            displayGameOver();

            // Reset status untuk permainan baru
            currentEnemy = Enemy{"", 0, 0};
            playerHealth = 100;
            titleScreen();
        }
    }

}
////////////////////////////////// Battle Function /////////////////////////////////// 


void dungeon() {
    int choice;
    displayDungeonMenu();
    cin >> choice;

    switch (choice) {
        case 1: // Go deeper
            navigateDepth(currentDepth + 1);
            break;
        case 2:
            checkAround();
            break;
        case 3: // Return to previous depth
            navigateDepth(currentDepth - 1);
            break;
        case 4: // Rest 
            if (currentDepth % 4 == 0 && currentDepth != 0) {
                cout << "You rest and recover health.\n";
                increaseHealth(50); // Pulihkan 50 HP
            } else {
                cout << "Invalid choice. Returning to dungeon menu.\n";
            }
            dungeon(); // Kembali ke menu dungeon
            break;
        default: // Input tidak valid
            cout << "Invalid choice. Try again.\n";
            navigateDepth(currentDepth);
    }
}

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
    cout << "| 1. New Game                                    |\n";
    cout << "| 2. Load Game                                   |\n";
    cout << "| 3. Keluar                                      |\n";
    cout << "==================================================\n";
    cout << "Pilih: ";
    cin >> choice;

    switch (choice) {
        case 1:
            newGame();
            break;
        case 2:
            loadGame();
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
                inventoryLine = line.substr(11);
                // size_t pos = 0;
                // while ((pos = inventoryLine.find(";")) != string::npos) {
                //     string item = inventoryLine.substr(0, pos);
                //     size_t commaPos = item.find(",");
                //     playerInventory[playerInventorySize++] = {
                //         item.substr(0, commaPos), 
                //         stoi(item.substr(commaPos + 1))
                //     };
                //     inventoryLine.erase(0, pos + 1);
                // }
                // if (!inventoryLine.empty()) {
                //     size_t commaPos = inventoryLine.find(",");
                //     playerInventory[playerInventorySize++] = {
                //         inventoryLine.substr(0, commaPos), 
                //         stoi(inventoryLine.substr(commaPos + 1))
                //     };
                // }
            } else if (line.find("Currency:") == 0) {
                gold = stoi(line.substr(10));
            }
        }
        loadFile.close();
        loadWeapons(); // Load weapons including chosen weapon
        cout << "Game loaded successfully!\n";
        idleGuild(); // Lanjutkan ke guild setelah memuat game
    } else {
        cout << "No save game found. Starting a new game.\n";
        newGame(); // Mulai permainan baru jika file save tidak ditemukan
    }
}



void newGame() {
    prologue();
    inputName();
    resetWeapons();
    idleGuild();
}

void exitGame() {
    cout << "Balik lagi ya. Dada >////<!\n";
    exit(0); // Menutup program sepenuhnya
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
    ofstream saveFile("savegame.txt", ios::out | ios::trunc); // Gunakan mode trunc
    if (saveFile.is_open()) {
        saveFile << "PlayerName: " << playerName << "\n";
        saveFile << "Health: " << playerHealth << "\n";
        saveFile << "Inventory: ";
        for (int i = 0; i < playerInventorySize; ++i) {
            saveFile << playerInventory[i].name << "," << playerInventory[i].price;
            if (i < playerInventorySize - 1) saveFile << ";";
        }
        saveFile << "\n";
        saveFile << "Currency: " << gold << "\n";
        saveFile.close();
        cout << "Game saved successfully!\n"; // Debugging log
    } else {
        cout << "Error saving the game!\n"; // Debugging log
    }
}

void rest() {
    cout << "You rest and save the game. Your health is fully restored.\n";
    increaseHealth(100);
    saveGame();
    guild();
}

void receptionist() {
    selectDungeon();
}

void sellItems() {
    cout << "=== Sell Items ===\n";
    for (int i = 0; i < playerInventorySize; ++i) {
        int itemPrice = playerInventory[i].price > 0 ? playerInventory[i].price : 0;
        cout << i + 1 << ". " << playerInventory[i].name 
             << " - " << itemPrice << " gold\n";
    }
    cout << "Enter the number to sell an item or 0 to exit: ";
    int choice;
    cin >> choice;

    if (choice > 0 && choice <= playerInventorySize) {
        int itemPrice = playerInventory[choice - 1].price > 0 ? playerInventory[choice - 1].price : 0;
        cout << "You sold " << playerInventory[choice - 1].name 
             << " for " << itemPrice << " gold.\n";
        gold += itemPrice;

        // Remove item by shifting array
        for (int i = choice - 1; i < playerInventorySize - 1; ++i) {
            playerInventory[i] = playerInventory[i + 1];
        }
        --playerInventorySize;

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

            // Tambahkan ke weaponList
            if (weaponListSize < MAX_WEAPONS_SIZE) {
                weaponList[weaponListSize++] = {merchantItems[choice - 1].name, merchantItems[choice - 1].price};
                saveWeapons(); // Simpan senjata ke file
            } else {
                cout << "Your weapon inventory is full!\n";
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
    maxDepth = 10;
    currentDepth = 0;
    camp();
}

void elMachoPrison() {
    cout << "You step into the dark El Macho Prison.\n";
    maxDepth = 20;
    currentDepth = 0;
    camp();
}

void sigmaTempleRuin() {
    cout << "You explore the ancient Sigma Temple Ruin.\n";
    maxDepth = 30;
    currentDepth = 0;
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
    increaseHealth(100);
    saveGame();
    camp();
}

void interactFellowAdventurer() {
    cout << "=== Interaksi dengan Fellow Adventurer ===\n";

    if (maxDepth == 10) {
        cout << "Seorang petualang berkata: 'Skibidi Castle benar-benar aneh, aku hampir kehilangan arah di sana!'\n";
        cout << "Dia menawarkan beberapa tips: 'Jangan lupa periksa setiap sudut, terkadang ada harta karun.'\n";
    } else if (maxDepth == 20) {
        cout << "Seorang petualang berkata: 'El Macho Prison penuh jebakan, berhati-hatilah.'\n";
        cout << "Dia memperingatkan: 'Musuh di dalam gelap sangat mematikan, bawa cukup senjata!'\n";
    } else if (maxDepth == 30) {
        cout << "Seorang petualang berkata: 'Sigma Temple Ruin penuh misteri dan teka-teki.'\n";
        cout << "Dia berbagi: 'Aku pernah melihat harta karun yang dijaga golem, tapi belum cukup kuat untuk mengambilnya.'\n";
    } else {
        cout << "Seorang petualang berkata: 'Aku belum pernah sejauh ini, hati-hati ya.'\n";
    }

    camp();
}


void enterDungeon() {
    dungeon();
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
            // currentWeaponDamage = weaponListSize[i].damage;
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
        saveWeapons(); // Simpan senjata yang dipilih
    }
    camp();
}
