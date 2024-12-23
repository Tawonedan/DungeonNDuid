#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime> 
#include <cmath>
#include <cstdint>
#include <cstring>
#include <string>
#include <io.h>
#include <fcntl.h>
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


const int MAX_INVENTORY_SIZE = 100;
const int MAX_WEAPONS_SIZE = 50;
const int MAX_MERCHANT_ITEMS = 10;
const int MAX_SIZE = 10; 
const int MAX_ITEMS = 100; 
const int numItems = sizeof(itemList) / sizeof(itemList[0]);

int playerHealth = 100;
int gold = 50;
int weaponCount = 0;
int currentDepth = 0;
int maxDepth = 0;
int playerInventorySize = 3; 
int weaponListSize = 0;      
int merchantItemsSize = 3;
int count = 0;
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

Player dataList[MAX_SIZE];
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
void startFinalBattle();
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
    weaponListSize = 0; 
    chosenWeapon = "";  
    ofstream file("weapons.txt", ios::out | ios::trunc);
    if (file.is_open()) {
        file.close();
    }
}


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


            for (char &c : name) {
                if (c == ' ') c = '_';
            }

            file << name << "," << weaponList[i].damage << "\n";
        }
        if (!chosenWeapon.empty()) {
            file << "ChosenWeapon," << chosenWeapon << "\n";
        }
        file.close();
        cout << "Senjata berhasil disimpan!\n";
    } else {
        cout << "Error menyimpan senjata!\n";  
    }

}



bool ownsWeapon(const string &weaponName) {
    for (int i = 0; i < weaponListSize; ++i) {
        if (weaponList[i].name == weaponName) return true;
    }
    return false;
}


int randomRange(int min, int max) {
    return min + rand() % (max - min + 1);
}

Item getRandomItem() {
    int index = randomRange(0, numItems - 1);
    return itemList[index];
}

void increaseHealth(int healAmount) {
    int previousHealth = playerHealth; 
    playerHealth = min(playerHealth + healAmount, 100);
    cout << "Health: " << previousHealth << " -> " << playerHealth << "\n";
}

void displayDungeonMenu() {
    cout << "====================================\n";
    cout << "|           Opsi Dungeon           |\n";
    cout << "====================================\n";
    cout << "|1. Masuk lebih dalam.             |\n"; 
    cout << "|2. Cek sekeliling.                |\n";
    cout << "|3. Kembali.                       |\n";
    if (currentDepth % 4 == 0 && currentDepth != 0) {
        cout << "|4. Istirahat.                     |\n";
    }
    cout << "====================================\n\n";
    cout << "Masukkan pilihan: ";
}

void displayChest() {
    cout << "====================================\n";
    cout << "|       Kamu menemukan peti!       |\n";
    cout << "====================================\n";
    cout << "|Apa yang kamu lakukan?            |\n";
    cout << "|1. Buka peti.                     |\n";
    cout << "|2. Tinggalkan.                    |\n";
    cout << "====================================\n\n";
    cout << "Masukkan pilihan: ";
}

void displayChestResult(bool foundTreasure, const Item& treasure) {
    if (foundTreasure) {
        cout << "====================================\n";
        cout << "|       Kamu membuka petinya!      |\n";
        cout << "====================================\n";
        cout << "Kamu menemukan " << treasure.name << " seharga " 
             << treasure.value << " gold!\n";
        cout << "====================================\n\n";
    } else {
        cout << "=========================================\n";
        cout << "|            Isinya jebakan!            |\n";
        cout << "=========================================\n";
        cout << "Rupanya Mimic! Bersiap untuk bertarung! \n";
        cout << "=========================================\n\n";
    }
}

void addItemToInventory(const Item& newItem) {
    if (playerInventorySize < MAX_INVENTORY_SIZE) {
        playerInventory[playerInventorySize++] = {newItem.name, newItem.value};
    } else {
        cout << "Inventory penuh!\n";
    }
}

void handleChest() {
    displayChest();
    int choice;
    cin >> choice;

    if (choice == 1) {
        int outcome = randomRange(0, 3);
        if (outcome < 3) {
            Item treasure = getRandomItem();
            displayChestResult(true, treasure);
            addItemToInventory(treasure); 
        } else {
            displayChestResult(false, Item{"Mimic", 0});
            
            string EnemyName = "Mimic Chest";
            loadEnemyData(EnemyName);
            battle(playerName, EnemyName);
        }
    } else {
        cout << "Kamu memilih untuk meninggalkan peti itu.\n";
    }
}


void handleSpecialEvent() {
    int eventRoll = randomRange(1, 100); 

    if (eventRoll <= 25) { 
        cout << "====================================\n";
        cout << "  Kamu menemukan kolam misteriusl! |\n";
        cout << "====================================\n";
        cout << "Akankah kamu masuk ke kolam itu?   |\n";
        cout << "[1]Ya [2]Tidak                     |\n";
        cout << "====================================\n";
        cout << "Masukkan pilihan: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            if (randomRange(0, 1) == 0) {
                cout << "Kolam itu suci! Kamu memulihkan kesehatanmu.\n";
                playerHealth = 100;
            } else {
                cout << "Kolam itu beracun! Kamu kehilangan setengah kesehatanmu.\n";
                playerHealth = max(playerHealth / 2, 1);
            }
        } else {
            cout << "Kamu memutuskan untuk meninggalkan kolam itu dan lanjut.\n";
        }
    } else if (eventRoll <= 50) {
        cout << "===================================================\n";
        cout << "|           Kamu bertemu seorang badut!           |\n";
        cout << "===================================================\n";
        cout << "Badut itu menawarimu tawaran seharga 30 gold...   |\n";
        cout << "Terima tawaran abu abu itu?                       |\n"; 
        cout << "[1]Yes [2]No                                      |\n";
        cout << "===================================================\n";
        cout << "Masukkan Pilihan: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            if (gold >= 30) {
                gold -= 30;
                if (randomRange(0, 1) == 0) {
                    cout << "Badut itu memberimu 50 gold kembali!\n";
                    gold += 50;
                } else {
                    cout << "Badut itu hilang tanpa jejak!\n";
                }
            } else {
                cout << "Kamu tidak punya cukup gold! Badut itu menertawakanmu dan pergi.\n";
            }
        } else {
            cout << "Kamu menolak tawaran badut itu dan pergi.\n";
        }
    } else if (eventRoll <= 75) { 
        cout << "====================================\n";
        cout << "|  Kamu menemukan tumpukan mayat.  |\n";
        cout << "====================================\n";
        cout << "|Apa yang akan kamu lakukan?       |\n";
        cout << "|1. Interaksi/Cari.                |\n";
        cout << "|2. Tinggalkan.                    |\n";
        cout << "====================================\n";
        cout << "Masukkan Pilihan: ";
        int choice;
        cin >> choice;

        if (choice == 1) {
            int outcome = randomRange(0, 3);
            if (outcome < 2) {
                int foundGold = randomRange(10, 30);
                cout << "You found " << foundGold << " gold!\n";
                gold += foundGold;
            } else {
                cout << "Salah satu mayat itu bangkit kembali! Bersiap untuk bertarung!\n";
                string EnemyName = "Zombie";
                loadEnemyData(EnemyName);
                battle(playerName, EnemyName);
            }
        } else {
            cout << "Kamu meninggalkan tumpukan mayat itu dan pergi.\n";
        }
    } else {
        cout << "=====================================\n";
        cout << "| Kamu bertemu sesama petualang! |\n";
        cout << "=====================================\n";
        cout << "Ia menawarimu potion. Kamu menerima 20 poin kesehatan.\n";
        playerHealth = min(playerHealth + 20, 100);
        cout << "Darahmu saat ini: " << playerHealth << "\n";
    }
}


void navigateDepth(int depth) {

    if (depth < 1) {
        cout << "You are at the entrance. Returning to camp.\n";
        camp();
        return;
    }

    if (depth > maxDepth) {
        startFinalBattle();
        return;
    }
    currentDepth = depth;
    hasCheckedAround = false;
    cout << "Kedalaman: " << currentDepth << "\n";


    if (currentDepth % 4 == 0) {
        cout << "Kamu menemukan checkpoint! Kamu bisa istirahat disini.\n";
        dungeon();
        return;
    }


    int event = randomRange(0, 9);
    if (event < 6) {
        cout << "Kamu bertemu musuh!\n";


        string EnemyName = generateCommonEnemy();
        loadEnemyData(EnemyName);
        battle(playerName, EnemyName);
    } else if (event < 8) {
        cout << "Kamu menemukan peti!\n";
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
        cout << "Kamu telah mengecek sekitar.\n";
        dungeon();
        return;
    }

    hasCheckedAround = true;
    int event = randomRange(0, 9);
    if (event < 8) {
        cout << "Kamu tetap tidak menemukan apapun.\n";
    } else {
        cout << "kamu menemukan peti!\n";
        handleChest();
    }

    cout << "Kamu tetap berada pada kedalaman " << currentDepth << ".\n";
    dungeon();
}

////////////////////////////////// Inventory Function /////////////////////////////////// 
void displayInventory() {
    if (count == 0) {
        cout << "\nInventory kosong!" << endl;
    } else {
        cout << "\nInventory:" << endl;
        for (int i = 0; i < count; i++) {
            cout << i + 1 << ". " << items[i] << endl;
        }
    }
}
int convertInventory(const string& input, string items[], int max_size) {

    char cstr[input.size() + 1];
    strcpy(cstr, input.c_str());
    
    char* token = strtok(cstr, ",");
    int index = 0;

    while (token != nullptr && index < max_size) {
        while (*token == ' ') token++;

        items[index] = token;
        index++;
        
        token = strtok(nullptr, ",");
    }

    return index;

}


int removeItemFromArray(string items[], int count, int indexToRemove) {
    if (indexToRemove < 0 || indexToRemove >= count) {
        cout << "Invalid index!" << endl;
        return count; 
    }

    for (int i = indexToRemove; i < count - 1; i++) {
        items[i] = items[i + 1];
    }

    return count - 1; 
}

////////////////////////////////// Inventory Function /////////////////////////////////// 



////////////////////////////////// Battle Function /////////////////////////////////// 

Enemy loadEnemyData(string enemyName) {
    ifstream inFile("enemyData.txt");
    if (!inFile) {
        cerr << "Error: Could not open enemyAsset.txt!" << endl;
        return Enemy{"", 0, 0};
    }

    string line;

    bool foundEnemy = false;

    while (getline(inFile, line)) {
        stringstream ss(line);
        string key, value;

        getline(ss, key, ':'); 
        getline(ss, value);    

        if (!value.empty() && value[0] == ' ') {
            value = value.substr(1);
        }

        if (key == "EnemyName") {
            if (value == enemyName) {
                foundEnemy = true;
                currentEnemy = Enemy();
                currentEnemy.name = value;
            }
        } else if (key == "Health" && foundEnemy) {
            currentEnemy.health = stoi(value);
        } else if (key == "Strength" && foundEnemy) {
            currentEnemy.strength = stoi(value);
        }

        if (foundEnemy && key == "Strength") {
            break;
        }
    }

    inFile.close();

    if (!foundEnemy) {
        cerr << "Error: Enemy '" << enemyName << "' not found in enemyAsset.txt!" << endl;
        return Enemy{"", 0, 0}; 
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

void damageStr(const string& weaponName, int& targetHealth) {
    for (int i = 0; i < weaponListSize; ++i) {
        if (weaponList[i].name == weaponName) {
            int damage = weaponList[i].damage;
            targetHealth -= damage;
            if (targetHealth < 0) targetHealth = 0;

            wcout << L"\nYou attacked with " << weaponName.c_str() 
                  << L" and dealt " << damage << L" damage!\n";
            wcout << L"Target health is now: " << targetHealth << L".\n";

            return;
        }
    }

    wcout << L"Weapon not found! No damage dealt.\n";
}


void playerAttack() {
    currentEnemy.health -= currentPlayer.strength;
    cout << "You dealt " << currentPlayer.strength << " damage to the " << currentEnemy.name << ".\n";

}

void enemyAttack() {
    playerHealth -= currentEnemy.strength;
    cout << currentEnemy.name << " Menyerangmu sebesar " << currentEnemy.strength << " damage kepadamu.\n";
}

void useBlazingBuster() {
    currentEnemy.health -= 30;
    cout << "Kamu memberikan 30 damage kepada" << currentEnemy.name << " dengan Blazing Buster" <<".\n";
}

void useRevitalize() {
    increaseHealth(50);
    cout << "Kamu mengembalikan 50 darahmu dengan Revitalize"<< ".\n";
}

void playerSkill() {
    cout << "\n=======================================\n";
    cout << "|[1] Blazing Buster | [2] Revitalize  |\n";
    cout << "=======================================\n";
    cout << "Input Pilihan: ";
                
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
    cout << "\nPilihan Invalid.\n";
    }
}

void playerDefend() {
    int blockRate = randomRange(1, 4);

    int blockedPercentage = blockRate * 25;
    cout << "Kamu memilih untuk berlindung dan menangkis " << blockedPercentage << "% of the damage!\n";
    int incomingDamage = currentEnemy.strength;
    int blockedDamage = incomingDamage * (blockedPercentage / 100.0);
    int damageTaken = incomingDamage - blockedDamage;
    playerHealth -= damageTaken;
    cout << "Kamu menerima " << damageTaken << " Setelah menangkis.\n";

}

string useItems() {
    string usedItem;

    if (!itemInitialize) {
        string input = inventoryLine;
        count = convertInventory(input, items, MAX_INVENTORY_SIZE);
        itemInitialize = true;
    }

    while (true) {
        displayInventory();

        if (count == 0) {
            break;
        }

        int itemChoice;
        cout << "\n[0]Keluar\n \nInput pilihan item: ";
        cin >> itemChoice;

        if (itemChoice < 1 || itemChoice > count) {
            cout << "Pilihan tidak valid!" << endl;
            continue;
        }

        if (itemChoice == 0) {
            break;
        }

        if (itemChoice > 0 && itemChoice <= count) {
            cout << "Kamu memilih: " << items[itemChoice - 1] << endl;
        } else {
            cout << "Invalid selection!" << endl;
        }

        count = removeItemFromArray(items, count, itemChoice - 1);
        
        usedItem = items[itemChoice - 1];
        break;

    }

    if (usedItem == "Potion") {
        increaseHealth(30);
        cout << "Kamu menggunakan Potion.\n";
    }
    
}

void displayBattleHeader(const string& playerName, const string& enemyName) {
    cout << "\n======================================\n";
    cout << "|         Pertarungan mulai!         |\n";
    cout << "======================================\n";
    cout << playerName << " bertarung dengan " << enemyName << "!\n";
    cout << "====================================\n";
}

void displayBattleStatus(int playerHP, int enemyHP, const string& enemyName) {
    cout << "\n====================================\n";
    cout << "|            Battle Status         |\n";
    cout << "====================================\n";
    cout << "| HP-mu: " << playerHP << "                   \n";
    cout << "| " << enemyName << " HP: " << enemyHP << "        \n";
    cout << "====================================\n";
}

void displayBattleMenu() {
    cout << "\n===============================================================\n";
    cout << "|[1] Serang | [2] Berlindung | [3] Skill | [4] Item | [5] Kabur |\n";
    cout << "=================================================================\n";
    cout << "Input pilihan: ";
}

void displayGameOver(){
    cout << "\n===============================================================\n";
    cout << "|          Game Over! Kembali ke title screen.....            |\n";
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
                cout << "Kamu menggunakan skill\n";
                playerSkill();
                enemyAttack();
                break;
            }
            case 4: {
                cout << "Kamu menggunakan Item\n";
                useItems();
                break;
            }
            case 5: {
                cout << "Kamu mencoba kabur...\n";

                int escapeChance = randomRange(0, 5);

                if (escapeChance == 1) {
                    cout << "Kamu berhasil kabur\n";
                    battleOngoing = false;
                } else {
                    cout << "Percobaan kabur gagal..\n";
                    enemyAttack();
                }
                             
                    
                break;
            }
            default:
                cout << L"Invalid choice.\n";
        }

        if (playerHealth > 0 && currentEnemy.health <= 0) {
            cout << "\nKamu berhasil mengalahkan " << enemyName << "!.\n";
            battleOngoing = false;
        }

        if (playerHealth <= 0) {
            cout << "\nDikalahkan oleh " << enemyName << "..\n";
            battleOngoing = false;
            displayGameOver();


            currentEnemy = Enemy{"", 0, 0};
            playerHealth = 100;
            titleScreen();
        }
    }

}

void displayBossBattleStatus(int playerHP, int enemyHP, const string& enemyName) {
    _setmode(_fileno(stdout), _O_U16TEXT); 
    wcout << L"\n═════════════════════════════════════\n";
    wcout << L"⚔️       Status Final Boss        ⚔️\n";
    wcout << L"═════════════════════════════════════\n";
    wcout << L"🌟 Player HP: " << playerHP << L"                \n";
    wcout << L"👹 " << enemyName.c_str() << L" HP: " << enemyHP << L"          \n";
    wcout << L"═════════════════════════════════════\n";
}
void displayBossBattleMenu() {
    _setmode(_fileno(stdout), _O_U16TEXT); 
    wcout << L"\n═══════════════════════════════════════════════════════════════\n";
    wcout << L"🔮 Pilih Aksi:\n";
    wcout << L"═══════════════════════════════════════════════════════════════\n";
    wcout << L"⚔️ [1] Serang  🛡 [2] Berlindung  ✨ [3] Skill  💊 [4] Item   \n";
    wcout << L"═══════════════════════════════════════════════════════════════\n";
    wcout << L" Input Pilihan ";
}
void playerBossSkill() {
    _setmode(_fileno(stdout), _O_U16TEXT); 
    wcout << "\n=======================================\n";
    wcout << "|[1] Blazing Buster | [2] Revitalize  |\n";
    wcout << "=======================================\n";
    wcout << "Input Pilihan: ";
                
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
    cout << "\nPilihan Invalid.\n";
    }
}
void battleFinalBoss() {
    string bossName = "Lynanien Darkborn"; 
    _setmode(_fileno(stdout), _O_U16TEXT);
    int bossHealth = 250; 
    int bossStrength = 20; 
    int bossSkillDamage = 40; 
    int bossSkillTurn = 3; 

    battleOngoing = true;
    int currentTurn = 0;

    while (battleOngoing && playerHealth > 0 && bossHealth > 0) {
        currentTurn++;
        wcout << L"\n═══════════════════════════════════════════════\n";
        wcout << L"                  Final Boss \n";
        wcout << L"═══════════════════════════════════════════════\n";
        wcout << L"| Anda menghadapi: " << bossName.c_str() << L" |\n";
        wcout << L"⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡛⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n";
        wcout << L"⣿⣿⢿⣿⣿⢿⣿⣿⢿⣿⣿⣿⣧⠘⢿⣿⣿⢿⣿⣿⢿⣿⣿⢿⣿⣿⢿⣿⣻⣿⣿⣻⣿⣿⣻⣿⣿⣻⣿⣿⣻⣿⣿⣻⣿⣿⣻⣿⣿⣻\n";
        wcout << L"⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣿⣿⡆⠠⢉⢻⣿⣿⣿⣿⢿⣛⣯⣿⣾⣿⣞⣷⣯⣟⣻⢿⣿⣿⡿⣿⣿⣿⢿⣿⣿⢿⣿⣿⢿⣿⣿⢿⣿\n";
        wcout << L"⣿⣾⣿⣿⣾⣿⣿⣾⣿⣿⣻⠿⣿⣿⠀⠂⠄⡉⠑⢏⣿⣿⣿⣿⣿⣿⣿⣿⣿⣾⣿⣟⣿⣶⣯⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n";
        wcout << L"⣿⣿⣟⣿⣿⣟⣿⣿⣟⣿⣿⣷⡈⠿⣆⠡⠈⠄⣉⢸⣿⣿⣿⣿⣿⣿⣿⣿⣻⣿⣯⣿⣿⣿⣿⣿⣯⡿⣿⡽⢺⣿⣿⣯⣿⣿⣯⣿⣿⣯\n";
        wcout << L"⣿⡿⣿⣿⡿⣿⣿⡿⣿⣿⣿⣿⣷⡀⠌⢁⠐⠠⠘⣷⣿⣿⢿⣽⣿⣿⣿⣿⣿⣿⣿⣷⣿⣿⣿⣿⣿⣿⣍⠒⢻⣿⣿⣿⣿⣟⣿⣿⣟⣿\n";
        wcout << L"⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣿⣿⣿⣷⡀⠉⠤⠁⢄⠈⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡀⠼⣿⣿⣿⣽⣿⣿⣿⣿⣿\n";
        wcout << L"⣿⣿⣯⣿⣿⣯⣿⣿⣯⣿⣿⣿⣻⣿⣟⣳⢈⠰⢮⡀⠌⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⣼⣿⣿⣿⣿⣿⢿⣯⣿⣷\n";
        wcout << L"⣿⣟⣿⣿⣟⣿⣿⣟⣿⣿⣿⣽⣿⢿⣽⣿⣿⣶⡄⣿⣾⣦⣿⣿⣟⣿⣿⣿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣟⣾⣿⣿⣿⣾⣿⣿⣿⣿⣿\n";
        wcout << L"⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣻⡿⣯⣿⣿⣻⣽⣷⡿⢿⣿⣿⣿⣿⣽⣿⢿⣹⣾⣟⣿⣿⣿⣿⣿⣿⣿⣿⢯⣟⣾⣿⣿⣽⣿⣿⡿⣟⣯⣷\n";
        wcout << L"⣿⣿⢿⣾⣿⣿⣾⣿⣷⣟⣯⣟⣯⣷⣾⣷⡯⣞⡽⣿⣾⣿⣽⡇⠉⠒⠯⢳⡻⢾⣿⣿⣿⣿⣿⣿⣿⣻⣿⢯⣷⣿⣿⣿⣿⣻⣿⣿⣿⣿\n";
        wcout << L"⣟⣿⣿⣿⣿⣻⣿⣯⣿⣿⣿⡿⣟⣽⣿⣿⣷⡯⣟⣼⣿⣽⣿⡽⣆⡅⣆⣦⣝⣻⣿⣿⣿⢟⡾⢋⣴⣿⡿⣯⣿⣾⣿⣿⣽⣿⣿⣿⢿⣟\n";
        wcout << L"⣿⣿⣿⣿⣻⣿⣿⣿⣽⣫⣷⣽⢾⣿⣿⣿⣿⣿⡙⢾⣿⣿⠏⡔⠉⡈⢩⡿⣾⣟⣯⣟⡷⣞⣳⠞⣛⣾⣿⡷⣿⡷⣿⣿⣿⣿⣟⣿⣿⣿\n";
        wcout << L"⡿⣟⣯⣿⣿⣿⣿⣻⣿⣿⡿⣯⣿⣿⣿⡿⠋⢠⣟⣿⣿⣿⠁⢈⠐⣄⡙⢿⣳⣿⢾⣽⣞⣿⠁⢤⣿⣻⣿⣽⣷⢿⣻⣿⣿⣾⣿⣿⣿⡿\n";
        wcout << L"⣿⣿⣿⣿⣿⣿⣽⣿⣿⢯⡿⣷⣿⣿⣿⣶⣴⣿⣻⣿⣿⣽⠀⠀⢂⡈⣳⣮⣿⡽⣯⣿⡞⣣⡜⣮⢳⣿⣿⣎⣿⣿⡽⣿⣿⣿⢿⣟⣿⣿\n";
        wcout << L"⣿⣿⡿⣿⣷⣿⣿⡿⣽⣿⣻⣿⣿⣿⣿⣿⢿⡱⣯⣿⣿⣷⠀⠀⠀⠘⢦⡙⠳⢿⣛⢶⣙⢧⡝⣮⢿⣿⣿⡲⣽⣿⡽⣿⣿⣿⣿⣿⣿⣿\n";
        wcout << L"⣷⣿⣿⣿⣿⡿⢯⣽⣿⣳⡿⣿⣿⡿⠟⠋⠾⣱⢻⣿⣿⣿⡀⠀⠄⠀⠰⣅⠣⣄⠹⢮⡝⣮⡝⣮⢿⣿⣿⡵⣣⢿⣗⣿⣿⣿⣽⣿⡿⣷\n";
        wcout << L"⣿⣿⣿⣿⣝⣿⣿⣿⣻⣿⣇⠘⢿⠀⠠⠐⠀⠀⠉⢉⠻⣿⣧⡀⠀⡈⠐⢨⡓⡜⢦⣍⠘⠁⠻⠹⣾⣿⣿⠞⣵⢫⣿⣿⣿⢿⣿⣿⣿⣿\n";
        wcout << L"⡿⣿⣾⣳⣿⣿⢿⣾⣿⣿⡏⠠⢈⠐⡀⠆⡀⠀⠀⠂⠀⠄⠙⢿⣷⣶⣌⢡⣟⢬⡳⢬⠓⣦⠀⢼⡋⢿⣿⡆⠈⢷⡹⣿⣿⣿⣿⣷⣿⣿\n";
        wcout << L"⡿⣟⣼⣿⣿⣻⣿⣿⣿⡍⠿⢂⠁⢂⠐⡀⠀⠐⠀⠀⢈⠐⠠⢸⢻⣿⣿⢧⣋⣄⢻⡎⢋⠐⠩⣀⢳⢦⡹⢷⣄⡸⣷⡹⣿⣿⣯⣿⣿⡿\n";
        wcout << L"⢫⣿⣿⣿⣳⣿⣿⣿⣿⣷⡈⢄⠂⠄⠂⠐⠠⠀⠀⠀⠀⠀⠐⣸⣤⢿⣿⢯⣟⣞⡾⣽⣤⣟⣦⣌⠳⣮⣟⣿⣼⣻⣷⣛⣶⢇⣛⠻⣿⣿\n";
        wcout << L"⣿⣿⢟⣾⣿⣿⣿⣿⣿⣿⣧⠀⠊⠀⠀⡀⠀⢀⠀⠀⠀⠀⣲⣷⢯⣿⣯⠟⠮⣽⢻⡟⣿⢾⣱⠾⣿⣻⣞⡷⣻⡷⣯⡟⣧⠟⠀⠀⠀⠙ \n";
        wcout << L"⣿⣽⣿⣿⣿⣿⣿⣿⣿⡿⠁⠀⠀⠄⠀⠀⠀⠀⡀⠀⠀⢰⣯⡿⣿⣳⠃⠀⠀⠀⠀⠈⠄⠙⠪⢿⡱⢏⡻⡵⣱⢟⣣⠝⡀⠀⠀⠀⠀⠀  \n";
        wcout << L"═══════════════════════════════════════════════\n";

        displayBossBattleStatus(playerHealth, bossHealth, bossName);
        displayBossBattleMenu();
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: 
                if (!chosenWeapon.empty()) {
                    damageStr(chosenWeapon, bossHealth);
                } else {
                    cout << "No weapon equipped! You deal no damage.\n";
                }
                break;
            case 2: 
                playerDefend();
                break;
            case 3: 
                playerBossSkill();
                break;
            case 4: 
                useItems();
                break;
            case 5: { 
                int escapeChance = randomRange(0, 5);
                if (escapeChance == 1) {
                    cout << "You managed to escape\n";
                    battleOngoing = false;
                    return;
                } else {
                    cout << "You failed to escape\n";
                }
                break;
            }
            default:
                cout << "Invalid choice.\n";
        }

        if (bossHealth > 0) {
            if (currentTurn % bossSkillTurn == 0) {
                playerHealth -= bossSkillDamage;
                wcout << L"\n" << bossName.c_str() << L" menggunakan skill mematikan dan memberikan "
                      << bossSkillDamage << L" damage!\n";
            } else {
                playerHealth -= bossStrength;
                wcout << L"\n" << bossName.c_str() << L" menyerang Anda dan memberikan "
                      << bossStrength << L" damage!\n";
            }
        }
 
        if (bossHealth <= 0) {
            wcout << L"\n════════════════════════════════════════════════════════════════════════════════════════════════════════\n";
            wcout << L"          Selamat Anda Mengalahkan Final Boss!! Terima Kasih Telah Memainkan Game ini......             \n";
            wcout << L"════════════════════════════════════════════════════════════════════════════════════════════════════════\n";
            battleOngoing = false;
            exit (0);
        } else if (playerHealth <= 0) {
            wcout << L"\n════════════════════════════════════════════════════════════════════════════════════════════════════════\n";
            wcout << L"          Kamu telah dikalahkan Lyanien Darkborn! Coba Lagi Dengan Equipment Yang Lebih Mumpuni         \n";
            wcout << L"════════════════════════════════════════════════════════════════════════════════════════════════════════\n";
            battleOngoing = false;
            exit (0);
        }
    }
}
void startFinalBattle() {
    cout << "Prepare yourself for the ultimate challenge!\n";
    battleFinalBoss();
}

bool hasRestedAtCheckpoint = false;
void dungeon() {
    int choice;
    displayDungeonMenu();
    cin >> choice;

    switch (choice) {
        case 1: 
            hasRestedAtCheckpoint = false; 
            navigateDepth(currentDepth + 1);
            break;
        case 2:
            checkAround();
            break;
        case 3: 
            navigateDepth(currentDepth - 1);
            break;
        case 4: 
            if (currentDepth % 4 == 0 && currentDepth != 0) {
                if (!hasRestedAtCheckpoint) {
                    cout << "Kamu beristirahat dan recover health.\n";
                    increaseHealth(50); 
                    hasRestedAtCheckpoint = true; 
                } else {
                    cout << "Kamu sudah beristirahat di checkpoint ini!\n";
                }
            } else {
                cout << "Pilihan Invalid. Kembali ke dungeon menu.\n";
            }
            dungeon(); 
            break;
        default: 
            cout << "Pilihan Invalid. Coba Lagi.\n";
            navigateDepth(currentDepth);
    }
}



int main() {
    srand(time(0));
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
            } else if (line.find("Currency:") == 0) {
                gold = stoi(line.substr(10));
            }
        }
        loadFile.close();
        loadWeapons();
        cout << "Game sukses di load!\n";
        idleGuild();
    } else {
        cout << "Ga ada game yang di simpan. Memulai petualangan baru.\n";
        newGame();
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
    exit(0);
}

void prologue() {
    cout << "==================================================\n";
    cout << "|                    Prologue                    |\n";
    cout << "==================================================\n";
    cout << "| Di ohiolandia, sebuah tanah penuh reruntuhan   |\n";
    cout << "| kuno dan ruang bawah tanah mematikan, kisah    |\n";
    cout << "| tentang kekayaan dan bahaya bergema di seluruh |\n";
    cout << "| guild.                                         |\n";
    cout << "|                                                |\n";
    cout << "| Sebagai seorang petualang pemula, Anda tiba di |\n";
    cout << "| Adventurers' Guild dengan semangat membara.    |\n";
    cout << "| Hari ini, perjalanan Anda dimulai. Apakah Anda |\n";
    cout << "| akan mencapai kejayaan, atau menjadi jiwa yang |\n";
    cout << "| terlupakan dalam kegelapan Dungeon?            |\n";
    cout << "==================================================\n";
}

void inputName() {
    cout << "Masukkan nama: ";
    cin >> playerName;
    cout << "Selamat datang di guild, " << playerName << "!\n";
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
            cout << "Pilihan Salah. Kembali ke guild.\n";
            guild();
    }
}

void saveGame() {
    ofstream saveFile("savegame.txt", ios::out | ios::trunc);
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
        cout << "Game berhasil di save!\n"; 
    } else {
        cout << "Error saving the game!\n"; 
    }
}

void rest() {
    cout << "Kamu beristirahat sejenak. Kamu merasakan tubuhmu kembali bertenaga\n";
    increaseHealth(100);
    saveGame();
    guild();
}

void receptionist() {
    selectDungeon();
}

void sellItems() {
    cout << "\n========================================\n";
    cout << "            Pedagang Sibuk           \n";
    cout << "========================================\n";
    if (playerInventorySize == 0) {
        cout << "Inventaris Anda kosong. Tidak ada item untuk dijual.\n";
    } else {
        cout << "Daftar Item di Inventaris Anda:\n";
        for (int i = 0; i < playerInventorySize; ++i) {
            int itemPrice = playerInventory[i].price > 0 ? playerInventory[i].price : 0;
            cout << i + 1 << ". " << playerInventory[i].name 
                 << " - " << itemPrice << " gold\n";
        }
        cout << "0. Kembali ke menu sebelumnya.\n";
    }
    cout << "========================================\n";
    cout << "Masukkan angka untuk menjual item, atau 0 untuk kembali: ";
    int choice;
    cin >> choice;
    if (choice > 0 && choice <= playerInventorySize) {
        int itemPrice = playerInventory[choice - 1].price > 0 ? playerInventory[choice - 1].price : 0;
        cout << "\n Anda menjual " << playerInventory[choice - 1].name 
             << " seharga " << itemPrice << " gold.\n";
        gold += itemPrice;
        for (int i = choice - 1; i < playerInventorySize - 1; ++i) {
            playerInventory[i] = playerInventory[i + 1];
        }
        --playerInventorySize;
    } else if (choice == 0) {
        cout << "\nKembali ke menu sebelumnya.\n";
    } else {
        cout << "\n Pilihan tidak valid. Coba lagi.\n";
    }
    cout << "\n========================================\n";
    guild();
}


void travellingMerchant() {
    cout << "\n========================================\n";
    cout << "          Travelling Merchant         \n";
    cout << "========================================\n";
    for (int i = 0; i < merchantItemsSize; ++i) {
        cout << i + 1 << ". " << merchantItems[i].name;
        if (ownsWeapon(merchantItems[i].name)) {
            cout << " (Sudah dimiliki)";
        } else {
            cout << " (" << merchantItems[i].price << " gold)";
        }
        cout << "\n";
    }
    cout << "0. Kembali ke menu sebelumnya.\n";
    cout << "========================================\n";
    cout << "Masukkan pilihan Anda: ";
    int choice;
    cin >> choice;

    if (choice > 0 && choice <= merchantItemsSize) {
        if (ownsWeapon(merchantItems[choice - 1].name)) {
            cout << "\n  Anda sudah memiliki " << merchantItems[choice - 1].name << ".\n";
        } else if (gold >= merchantItems[choice - 1].price) {
            gold -= merchantItems[choice - 1].price;
            cout << "\n Anda membeli " << merchantItems[choice - 1].name << ".\n";
            if (weaponListSize < MAX_WEAPONS_SIZE) {
                weaponList[weaponListSize++] = {merchantItems[choice - 1].name, merchantItems[choice - 1].price};
                saveWeapons(); 
            } else {
                cout << "\n Inventory penuh! Tidak bisa menambahkan senjata baru.\n";
            }
        } else {
            cout << "\n Uang Anda tidak cukup untuk membeli " << merchantItems[choice - 1].name << ".\n";
        }
    }
    cout << "\n========================================\n";
    camp();
}


void selectDungeon() {
    int choice;
    cout << "==================================================\n";
    cout << "|               Pilih Dungeon                    |\n";
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
    cout << "Kamu pergi skibidi castle yang misterius.\n";
    maxDepth = 10;
    currentDepth = 0;
    camp();
}

void elMachoPrison() {
    cout << "Kamu pergi El macho prison yang gelap.\n";
    maxDepth = 20;
    currentDepth = 0;
    camp();
}

void sigmaTempleRuin() {
    cout << "Kamu pergi ke sigma temple ruin yang kuno.\n";
    maxDepth = 30;
    currentDepth = 0;
    camp();
}

void camp() {
    int choice;
    cout << "==================================================\n";
    cout << "|                  Perkemahan                   |\n";
    cout << "==================================================\n";
    cout << "| 1. Masuk ke dungeon                           |\n";
    cout << "|                                               |\n";
    cout << "| 2. Bonfire    (Simpan permainan)              |\n";
    cout << "| 3. Cek Inventory                              |\n";
    cout << "|                                               |\n";
    cout << "| 4. Interaksi Fellow Adventurer                |\n";
    cout << "| 5. Travelling Merchant (Beli senjata)         |\n";
    cout << "|                                               |\n";
    cout << "| 6. Kembali ke Guild                           |\n";
    cout << "==================================================\n";
    cout << "Pilih: ";
    cin >> choice;

    switch (choice) {
        case 1:
            enterDungeon();
            break;
        case 2:
            bonfire();
            break;
        case 3:
            checkInventory();
            break;
        case 4:
            interactFellowAdventurer();
            break;
        case 5:
            travellingMerchant();
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
    cout << "Kamu menulis kisahmu sembari merasakan hangatnya api unggun [Menyimpan File].\n";
    increaseHealth(100);
    saveGame();
    camp();
}

bool hasInteracted = false; 
void interactFellowAdventurer() {
    if (hasInteracted) {
        cout << "\n=============================================\n";
        cout << "||      Anda sudah berbicara sebelumnya.   ||\n";
        cout << "=============================================\n";
        camp();
        return;
    }
    hasInteracted = true; 
    int interactionIndex = randomRange(0, 2); 
    cout << "\n=============================================\n";
    cout << "||       Interaksi Fellow Adventurer        ||\n";
    cout << "=============================================\n";

    if (maxDepth == 10) {
        if (interactionIndex == 0) {
            cout << "| Petualang:                                |\n";
            cout << "| 'Skibidi Castle itu aneh banget,          |\n";
            cout << "|   aku hampir nyasar di sana!'             |\n";
            cout << "|                                           |\n";
            cout << "| Tips:                                     |\n";
            cout << "| 'Coba periksa tiap sudut, kadang ada      |\n";
            cout << "|   harta karun tersembunyi.'               |\n";
        } else if (interactionIndex == 1) {
            cout << "| Petualang:                                |\n";
            cout << "| 'Hati-hati sama suara langkah yang        |\n";
            cout << "|   nggak jelas di Skibidi Castle.'         |\n";
            cout << "|                                           |\n";
            cout << "| Peringatan:                               |\n";
            cout << "| 'Kadang ada perangkap tersembunyi di      |\n";
            cout << "|   lorong sempit.'                         |\n";
        } else {
            cout << "| Petualang:                                |\n";
            cout << "| 'Dinding yang retak di Skibidi Castle     |\n";
            cout << "|   biasanya menyimpan rahasia.'            |\n";
            cout << "|                                           |\n";
            cout << "| Rahasia:                                  |\n";
            cout << "| 'Coba periksa lebih dekat.'               |\n";
        }
    } else if (maxDepth == 20) {
        if (interactionIndex == 0) {
            cout << "| Petualang:                                |\n";
            cout << "| 'El Macho Prison itu penuh jebakan,       |\n";
            cout << "|   aku hampir celaka di sana.'             |\n";
            cout << "|                                           |\n";
            cout << "| Tips:                                     |\n";
            cout << "| 'Perhatikan lantai yang terlihat aneh,    |\n";
            cout << "|   mungkin itu jebakan.'                   |\n";
        } else if (interactionIndex == 1) {
            cout << "| Petualang:                                |\n";
            cout << "| 'Gelap banget di El Macho Prison, jadi    |\n";
            cout << "|   siapkan penerangan yang cukup.'         |\n";
            cout << "|                                           |\n";
            cout << "| Peringatan:                               |\n";
            cout << "| 'Musuh sering menyerang dari bayangan,    |\n";
            cout << "|   jadi tetap waspada.'                    |\n";
        } else {
            cout << "| Petualang:                                |\n";
            cout << "| 'Ada harta karun di lantai terdalam El    |\n";
            cout << "|   Macho Prison, tapi penjagaannya ketat.' |\n";
            cout << "|                                           |\n";
            cout << "| Rahasia:                                  |\n";
            cout << "| 'Jebakan biasanya ada di sekitar peti.'   |\n";
        }
    } else if (maxDepth == 30) {
        if (interactionIndex == 0) {
            cout << "| Petualang:                                |\n";
            cout << "| 'Sigma Temple Ruin itu penuh teka-teki,   |\n";
            cout << "|   jangan terburu-buru.'                   |\n";
            cout << "|                                           |\n";
            cout << "| Tips:                                     |\n";
            cout << "| 'Perhatikan lingkungan sekitar untuk      |\n";
            cout << "|   menemukan petunjuk.'                    |\n";
        } else if (interactionIndex == 1) {
            cout << "| Petualang:                                |\n";
            cout << "| 'Aku pernah lihat golem penjaga di Sigma  |\n";
            cout << "|   Temple Ruin. Mereka sangat kuat.'       |\n";
            cout << "|                                           |\n";
            cout << "| Peringatan:                               |\n";
            cout << "| 'Jangan melawan tanpa senjata yang kuat.' |\n";
        } else {
            cout << "| Petualang:                                |\n";
            cout << "| 'Ada ruangan rahasia di Sigma Temple      |\n";
            cout << "|   Ruin penuh artefak kuno.'               |\n";
            cout << "|                                           |\n";
            cout << "| Rahasia:                                  |\n";
            cout << "| 'Menyalakan semua obor bisa membukanya.'  |\n";
        }
    } else {
        cout << "| Petualang:                                |\n";
        cout << "| 'Aku belum pernah sejauh ini. Hati-hati.' |\n";
    }
    cout << "=============================================\n";
    camp();
}
void enterDungeon() {
    hasInteracted = false;
    dungeon();
}

void checkInventory() {
    cout << "\nCurrency: " << gold << " gold\n";
    cout << "\n========================================\n";
    cout << "               INVENTORY              \n";
    cout << "========================================\n";
    if (playerInventorySize == 0) {
        cout << "Inventaris kosong. Tidak ada item yang tersedia.\n";
    } else {
        cout << " Daftar Item:\n";
        for (int i = 0; i < playerInventorySize; ++i) {
            cout << i + 1 << ". " << playerInventory[i].name << "\n";
        }
    }
    cout << "\n========================================\n";
    cout << "                WEAPONS                  \n";
    cout << "========================================\n";
    if (weaponListSize == 0) {
        cout << "Tidak ada senjata di inventaris.\n";
    } else {
        cout << "  Daftar Senjata:\n";
        for (int i = 0; i < weaponListSize; ++i) {
            cout << i + 1 << ". " << weaponList[i].name << " (Damage: " << weaponList[i].damage << ")";
            if (weaponList[i].name == chosenWeapon) {
                cout << " [Dipilih]";
            }
            cout << "\n";
        }
    }
    cout << "\n========================================\n";
    cout << "Pilih senjata untuk digunakan (0 untuk batal): ";
    int choice;
    cin >> choice;
    if (choice > 0 && choice <= weaponListSize) {
        chosenWeapon = weaponList[choice - 1].name;
        cout << "\n Anda memilih senjata: " << chosenWeapon << ".\n";
        saveWeapons(); 
    } else if (choice == 0) {
        cout << "\nAnda membatalkan pemilihan senjata.\n";
    } else {
        cout << "\n Pilihan tidak valid. Coba lagi.\n";
    }
    cout << "\n========================================\n";
    camp();
}
