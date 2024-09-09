/*
CSCI235 Fall 2023
File Title: "Tavern.cpp"
Project 4 - 
Author: Mehran Azad
Date: 10/27/23
Implementation of Tavern Class of it's functions and constructors.
*/
#include "Tavern.hpp"

/** Default Constructor **/
Tavern::Tavern() : ArrayBag<Character*>(), level_sum_{0}, num_enemies_{0}
{
}

/**
    @param: the name of an input file
    @pre: Formatting of the csv file is as follows (each numbered item appears separated by comma, only one value for each numbered item):
    1. Name: An uppercase string
    2. Race: An uppercase string [HUMAN, ELF, DWARF, LIZARD, UNDEAD]
    3. Subclass: An uppercase string [BARBARIAN, MAGE, SCOUNDREL, RANGER]
    4. Level/Vitality/Armor: A positive integer
    5. Enemy: 0 (False) or 1 (True)
    6. Main: Uppercase string or strings representing the main weapon (Barbarian and Mage), Dagger type (Scoundrel), or arrows (Ranger). 
    - A ranger's arrows are of the form [TYPE] [QUANTITY];[TYPE] [QUANTITY], where each arrow type is separated by a semicolon, and the type and its quantity are separated with a space.
    7. Offhand: An uppercase string that is only applicable to Barbarians, and may be NONE if the Barbarian does not have an offhand weapon, or if the character is of a different subclass.
    8. School/Faction: Uppercase strings that represent a Mage's school of magic: [ELEMENTAL, NECROMANCY, ILLUSION] or a Scoundrel's faction: [CUTPURSE, SHADOWBLADE, SILVERTONGUE], and NONE where not applicable
    9. Summoning: 0 (False) or 1 (True), only applicable to Mages (summoning an Incarnate) and Rangers (Having an Animal Companion)
    10. Affinity: Only applicable to Rangers. Affinities are of the form [AFFINITY1];[AFFINITY2] where multiple affinities are separated by a semicolon. Th value may be NONE for a Ranger with no affinities, or characters of other subclasses.
    11. Disguise: 0 (False) or 1 (True), only applicable to Scoundrels, representing if they have a disguise.
    12. Enraged: 0 (False) or 1 (True), only applicable to Barbarians, representing if they are enraged.
    @post: Each line of the input file corresponds to a Character subclass and dynamically allocates Character derived objects, adding them to the Tavern.
    */


Tavern::Tavern(const std::string &input_file_name) : ArrayBag<Character*>()

{
  std::string junk, line, name, race, subclass, level_str, vitality_str, armor_str, enemy_str, main, offhand, school_or_faction, summoning_str, affinity, disguise_str, enraged_str, quantity_str, Current_affinity; // variables to read each header of the csv file.
  int level, vitality, armor, quantity;                                                                                                                                                                              // variables to read each header of the csv file.
  bool enemy, summoning, disguise, enraged;                                                                                                                                                                          // variables to read each header of the csv file.
  std::vector<Arrows> Arrows_vector;                                                                                                                                                                                 // vector to store the arrows
  Arrows Current_arrows;                                                                                                                                                                                             // arrow object that contains the Ranger's character main weapon.
  std::vector<std::string> Affinities_vector;                                                                                                                                                                        // vector to store the affinities
  Character *character_ptr;                                                                                                                                                                                          // pointer of the character that is dynamically allocated in each line of the csv file depending on the name of the character.
  std::ifstream fin(input_file_name);                                                                                                                                                                                // opens the input file
  if (fin.fail())                                                                                                                                                                                                    // error message if input file doesn't open
  {
    std::cerr << "File cannot be opened for reading." << std::endl;
    exit(1);
  }
  getline(fin, junk);        
  while (getline(fin, line)) 
  {
    std::istringstream character1(line); 
    getline(character1, name, ',');      
    getline(character1, race, ',');      
    getline(character1, subclass, ',');
    getline(character1, level_str, ',');
    level = std::stoi(level_str);
    getline(character1, vitality_str, ',');
    vitality = std::stoi(vitality_str);
    getline(character1, armor_str, ',');
    armor = std::stoi(armor_str);
    getline(character1, enemy_str, ',');
    enemy = std::stoi(enemy_str);
    if (subclass != "RANGER")
    {
      getline(character1, main, ',');
    }
    else 
    {    
      getline(character1, main, ',');
      std::istringstream main_weapon(main);
      while (getline(main_weapon, main, ';'))
      {
        std::istringstream Arrows_name(main);
        getline(Arrows_name, Current_arrows.type_, ' ');
        getline(Arrows_name, quantity_str, ';');
        quantity = std::stoi(quantity_str);
        Current_arrows.quantity_ = quantity;
        Arrows_vector.push_back(Current_arrows);
      }
    }
    getline(character1, offhand, ',');
    getline(character1, school_or_faction, ',');
    getline(character1, summoning_str, ',');
    summoning = std::stoi(summoning_str);
    getline(character1, affinity, ',');
    std::istringstream Affinity_stream(affinity); 
    while (getline(Affinity_stream, affinity, ';'))
    {
      std::istringstream Affinity_name(affinity);
      getline(Affinity_name, Current_affinity, ';');
      Affinities_vector.push_back(Current_affinity);
    }
    getline(character1, disguise_str, ',');
    disguise = std::stoi(disguise_str);
    getline(character1, enraged_str, ',');
    enraged = std::stoi(enraged_str);
    if (subclass == "MAGE")
    {
      character_ptr = new Mage(name, race, vitality, armor, level, enemy, school_or_faction, main, summoning);
    }
    else if (subclass == "BARBARIAN")
    {
      character_ptr = new Barbarian(name, race, vitality, armor, level, enemy, main, offhand, enraged);
    }
    else if (subclass == "SCOUNDREL")
    {
      character_ptr = new Scoundrel(name, race, vitality, armor, level, enemy, main, school_or_faction, disguise);
    }
    else if (subclass == "RANGER")
    {
      character_ptr = new Ranger(name, race, vitality, armor, level, enemy, Arrows_vector, Affinities_vector, summoning);
    }
    enterTavern(character_ptr); 
    character_ptr = nullptr;    
    Arrows_vector.clear();
    Affinities_vector.clear();      
  }
  fin.close(); 

}



/** 
    @param:   A reference to a Character entering the Tavern
    @return:  returns true if a Character was successfully added to items_, false otherwise
    @post:    adds Character to the Tavern and updates the level sum and the enemy count if the character is an enemy.
**/
bool Tavern::enterTavern(Character* a_character)
{
  if(add(a_character))
  {
    level_sum_ += a_character->getLevel();
    if(a_character->isEnemy())
      num_enemies_++;
     
    return true;
  }
  else
  {
    return false;
  }
}

/** @param:   A reference to a Character leaving the Tavern  
    @return:  returns true if a character was successfully removed from items_, false otherwise
    @post:    removes the character from the Tavern and updates the level sum and the enemy count if the character is an enemy.
**/
bool Tavern::exitTavern(Character* a_character)
{
  if(remove(a_character))
  {
    level_sum_ -= a_character->getLevel();
    if(a_character->isEnemy())
      num_enemies_--;
      
    return true;
  }
  return false;
}



/** 
    @return:  The integer level count of all the characters currently in the Tavern
    **/
    int Tavern::getLevelSum()
    {
      return level_sum_;
    }



/** 
    @return:  The average level of all the characters in the Tavern
    @post:    Considers every character currently in the Tavern, updates the average level of the Tavern rounded to the NEAREST integer, and returns the integer value.
**/
int Tavern::calculateAvgLevel()
{
   return (level_sum_>0) ? round(double(level_sum_) / item_count_) : 0.0;

}



/** 
    @return:  The integer enemy count of the Tavern
    **/
    int Tavern::getEnemyCount()
    {
      return num_enemies_;
    }



/** 
    @return:  The percentage (double) of all the enemy characters in the Tavern
    @post:    Considers every character currently in the Tavern, updates the enemy percentage of the Tavern rounded to 2 decimal places, and returns the double value.
**/
double Tavern::calculateEnemyPercentage()
{
  double enemy_percent = (num_enemies_>0) ?  (double(num_enemies_) / item_count_) * 100: 0.0;
  return std::ceil(enemy_percent*100.0) / 100.0; //round up to to decimal places
 
}


/** 
    @param:   A string reference to a race 
    @return:  An integer tally of the number of characters in the Tavern of the given race
**/
int Tavern::tallyRace(const std::string &race)
{
  int frequency = 0;
  int curr_index = 0;   
  while (curr_index < item_count_)
  {
    if (items_[curr_index]->getRace() == race)
    {
      frequency++;
    } 

    curr_index++; 
  }

  return frequency;
}


/**
  @post:    Outputs a report of the characters currently in the tavern in the form:
  "Humans: [x] \nElves: [x] \nDwarves: [x] \nLizards: [x] \nUndead: [x] \n\nThe average level is: [x] \n[x]% are enemies.\n\n"

  Example output: 
  Humans: 5
  Elves: 8
  Dwarves: 3
  Lizards: 7
  Undead: 2

  The average level is: 16
  24% are enemies.
*/
void Tavern::tavernReport()
{
  int humans = tallyRace("HUMAN");
  int elves = tallyRace("ELF");
  int dwarves = tallyRace("DWARF");
  int lizards = tallyRace("LIZARD");
  int undead = tallyRace("UNDEAD");
  
  std::cout << "Humans: " << humans << std::endl;
  std::cout << "Elves: " << elves << std::endl;
  std::cout << "Dwarves: " << dwarves << std::endl;
  std::cout << "Lizards: " << lizards << std::endl;
  std::cout << "Undead: " << undead << std::endl;
  std::cout << "\nThe average level is: " << calculateAvgLevel() << std::endl;
  std::cout << std::fixed << std::setprecision(2) << calculateEnemyPercentage() << "% are enemies.\n\n";
}

/**
    @post: For every character in the tavern, displays each character's information
*/

void Tavern::displayCharacters()
{
  for (int i = 0; i < getCurrentSize(); i++)
  {
    items_[i]->display(); 
  }
}

/**
    @param: a string reference to a race
    @post: For every character in the tavern of the given race (only exact matches to the input string), displays each character's information

*/

void Tavern::displayRace(const std::string &race)
{
  for (int i = 0; i < getCurrentSize(); i++)
  {
    if (items_[i]->getRace() == race) 
    {
      items_[i]->display();
    }
  }
}

/**
    @post: Every character in the tavern eats a tainted stew.
*/

void Tavern::taintedStew()
{
  for (int i = 0; i < getCurrentSize(); i++)
  {
    items_[i]->eatTaintedStew(); 
  }
}
