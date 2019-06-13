#include "core.h"

/* Start block: DO NOT CHANGE
    * Please do not change anything in this code block (including whitespace, empty lines)
    * If changes, your submission is marked as zero.
*/

/*
    * Default constructor for Battle class
*/
Battle::Battle(MemoryManager* manager) {
    this->musketeers = NULL;
    this->events = NULL;
    this->manager = manager;
}

/*
    * Default destructor for Battle class
*/
Battle::~Battle() {
    this->finalize();
}

/*
    * Method: read data from input file and assign values to musketeers
    * Input:
    * - filename: the name of input file
    * Output:
    * - Returns true if read file successfully, otherwise returns false
*/
bool Battle::readDataFromFile(string filename) {
    fstream fs(filename, ios::in);

    if (!fs.is_open()) return false;

    this->musketeers = new Musketeer[NUM_OF_MUSKETEERS];

    int* cystalPointer = NULL;
    int cystal = 0;

    for (int i = 0; i < NUM_OF_MUSKETEERS; i++) {
        this->musketeers[i].setMemoryManger(this->manager);
        this->musketeers[i].setTeam(this->musketeers);
        
        for (int j = 0; j < NUM_OF_CYSTAL; j++) {
            fs >> cystal;

            if (cystal > 0) {
                manager->allocate(cystalPointer);
                *cystalPointer = cystal;
                this->musketeers[i].setCystalPointer(j + 1, cystalPointer);
            }
        }
    }
    
    fs >> this->firstMusketeer;

    string eventsInText = "";
    fs.ignore();
    getline(fs, eventsInText);

    int numOfSpaces = 0;
    for (int i = 0; i < eventsInText.length(); i++) {
        if (eventsInText.at(i) == ' ') numOfSpaces++;
    }

    int numOfEvents = numOfSpaces + 1;

    this->events = new int[numOfEvents];

    stringstream ss(eventsInText);

    for (int i = 0; i < numOfEvents; i++)
        ss >> this->events[i];
    
    this->numOfEvents = numOfEvents;
    
    return true;
}

void Battle::display() {
    for (int i = 0; i < NUM_OF_MUSKETEERS; i++) {
        cout << this->musketeers[i].getHP() << " ";

        for (int j = 0; j < NUM_OF_CYSTAL; j++) {
            if (this->musketeers[i].getCystalPointer(j + 1) != NULL)
                cout << *(this->musketeers[i].getCystalPointer(j + 1));
            else cout << 0;
            if (j < NUM_OF_CYSTAL - 1) cout << " ";
        }
    
        cout << endl;
    }

    cout << this->manager->getNumOfAllocation();
}
/* End block: DO NOT CHANGE */

/* Start block: TO DO
    * You have to complete only one method of Battle class
*/

/*
    * Method: Emulate this battle
    * Input: None
    * Output: None
*/
void Battle::struggle() {

    // init the current musketeer
    int currentMusketeer = this->firstMusketeer;
    int musketeerShareCrystal = this->firstMusketeer;

    // init HP for musketeer
    this->musketeers[0].setHP(999);
    this->musketeers[1].setHP(900);
    this->musketeers[2].setHP(888);
    this->musketeers[3].setHP(777);



    for(int i = 0; i < this->numOfEvents; i++){
        // pick up a Crystal
        if(this->events[i] / 10 == 1 || this->events[i] / 10 == 2 || this->events[i] / 10 == 3){
            int typeOfCrystal = this->events[i] / 10;
            int levelOfCrystal = this->events[i] %10;
            if(this->musketeers[currentMusketeer].getCystalPointer(typeOfCrystal) == NULL){
                int *newCrystal = NULL;
                this->manager->allocate(newCrystal);
                *newCrystal = levelOfCrystal;
                this->musketeers[currentMusketeer].setCystalPointer(typeOfCrystal,newCrystal);
            }else{
                if(*(this->musketeers[currentMusketeer].getCystalPointer(typeOfCrystal)) < levelOfCrystal){
                    *(this->musketeers[currentMusketeer].getCystalPointer(typeOfCrystal)) = levelOfCrystal;
                }
            }

        // Sapphire - Sky
        // Emerald - Land
        // Aquamarine - Sea
        }
        else{
            int typeOfCrystal = (this->events[i] % 100)*-1 / 10;
            bool checkCrystal = false;
            for(int j = 0; j < 4; j++){
                if(this->musketeers[musketeerShareCrystal].getCystalPointer(typeOfCrystal) != NULL){
                    checkCrystal = true;
                    break;
                }      
                if(++musketeerShareCrystal > 3){
                    musketeerShareCrystal = 0;
                }
            }
            // has crystal suitable
            if(checkCrystal){
                if(*(this->musketeers[musketeerShareCrystal].getCystalPointer(typeOfCrystal)) > 0){
                    *this->musketeers[musketeerShareCrystal].getCystalPointer(typeOfCrystal) -= 1 ;
                    
                }
                if(*(this->musketeers[musketeerShareCrystal].getCystalPointer(typeOfCrystal)) == 0){
                    this->manager->deallocate(this->musketeers[musketeerShareCrystal].getCystalPointer(typeOfCrystal));
                    this->musketeers[musketeerShareCrystal].setCystalPointer(typeOfCrystal,NULL);
                }
            }
            // don't have crystal

            // initial monster power
            float powerOfMonster;
            switch (this->events[i] % 100)
            {
            case -11:
                powerOfMonster = 0.65;
                break;
            case -12:
                powerOfMonster = 0.95;
                break;
            case -21:
                powerOfMonster = 0.85;
                break;
            case -22:
                powerOfMonster = 0.9;
                break;
            case -31:
                powerOfMonster = 0.4;
                break;
            case -32:
                powerOfMonster = 1;
                break;
            } 
            // calculate the pow
            int pow = powerOfMonster;
            for(int j = 0; j < currentMusketeer; j++){
                pow *= powerOfMonster;
            }
            // calculate K
            int K = (this->events[i]+1)*(-1);
            int count = this->events[i];
            while(K > 0){
                bool check = true;
                for(int j = K-1; j > 1; j--){
                    if(K % j == 0){
                        check = false;
                        break;
                    }
                }
                if(check)
                    break;
                K--;
            }
            int damage = (this->events[i]*(-1)*powerOfMonster + pow*K);
            damage = damage % 100;
        }
    }
    

    
    // TO-DO
}

/*
    * Method: finalize a battle, deallocate any dynamic memories. 
    * Input: None
    * Output: None
*/
void Battle::finalize() {
    free(this->manager);
    free(this->musketeers);
    free(this->events);
    // TO-DO
}
/* End block: TO DO */
