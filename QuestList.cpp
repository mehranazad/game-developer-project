/*
 * File Title: QuestList.cpp
 * Author: Mehran Azad
 * Date: November 10, 2023
 * Course: CSCI-235
 * Instructor: Tiziana Ligorio
 * Assignment: Project 5
 * This file implements a class called QuestList
 */


#include "QuestList.hpp"

/**
    Default Constructor
*/

QuestList::QuestList() : DoublyLinkedList<Quest *>()
{
}

/**
    @param: a reference to string name of an input file
    @pre: Formatting of the csv file is as follows:
        Title: A string
        Description: A string
        Completion Status: 0 (False) or 1 (True)
        Experience Points: A non negative integer
        Dependencies: A list of Quest titles of the form [QUEST1];[QUEST2], where each quest is separated by a semicolon. The value may be NONE.
        Subquests: A list of Quest titles of the form [QUEST1];[QUEST2], where each quest is separated by a semicolon. The value may be NONE.
    Notes:
        - The first line of the input file is a header and should be ignored.
        - The dependencies and subquests are separated by a semicolon and may be NONE.
        - The dependencies and subquests may be in any order.
        - If any of the dependencies or subquests are not in the list, they should be created as new quests with the following information:
            - Title: The title of the quest
            - Description: "NOT DISCOVERED"
            - Completion Status: False
            - Experience Points: 0
            - Dependencies: An empty vector
            - Subquests: An empty vector
        - However, if you eventually encounter a quest that matches one of the "NOT DISCOVERED" quests while parsing the file, you should update all the quest details.
        Hint: update as needed using addQuest()


    @post: Each line of the input file corresponds to a quest to be added to the list. No duplicates are allowed.
*/

QuestList::QuestList(const std::string &input_file_name) : DoublyLinkedList<Quest *>()
{
    std::string junk, line, title, description, completion_status_str, experience_points_str, dependencies, subquests, current_dependencies_str, current_subquests_str;
    Quest *current_dependencies;
    Quest *current_subquests;
    bool completion_status;
    int experience_points;
    std::vector<Quest *> dependencies_vector;
    std::vector<Quest *> subquests_vector;
    std::ifstream fin(input_file_name); 
    
    if (fin.fail())                     
    {
        std::cerr << "File cannot be opened for reading." << std::endl;
        exit(1);
    }
    getline(fin, junk);
    while (getline(fin, line))
    {
        std::istringstream quest(line);
        getline(quest, title, ',');
        getline(quest, description, ',');
        getline(quest, completion_status_str, ',');
        completion_status = std::stoi(completion_status_str);
        getline(quest, experience_points_str, ',');
        experience_points = std::stoi(experience_points_str);
        getline(quest, dependencies, ',');
        std::istringstream dependencies_name(dependencies);
        while (getline(dependencies_name, dependencies, ';'))
        {
            if (dependencies.empty() || dependencies == "NONE")
            {
                Quest *newQuest = new Quest(title, description = "NOT DISCOVERED", completion_status = false, experience_points = 0, dependencies_vector = {}, subquests_vector = {});
                insert(item_count_, newQuest);
            }
            else
            {
                Quest *newQuest = getItem((getPosOf(dependencies)));
                dependencies_vector.push_back(newQuest);
            }
        }
        getline(quest, subquests, ',');
        std::istringstream subquests_name(subquests);
        while (getline(subquests_name, subquests, ';'))
        {
            if (subquests.empty() || subquests == "NONE")
            {
                Quest *newSubquest = new Quest(title, description = "NOT DISCOVERED", completion_status = false, experience_points = 0, dependencies_vector = {}, subquests_vector = {});
                insert(item_count_, newSubquest);
            }
            else if (contains(subquests))
            {
                Quest *newSubquest = getItem((getPosOf(subquests)));
                subquests_vector.push_back(newSubquest);
            }
        }
        if (!contains(title))
        {
            Quest *newQuest = new Quest(title, description, completion_status, experience_points, dependencies_vector, subquests_vector);
            insert(item_count_, newQuest);
        }
        else
        {
            Quest *updateQuest = getItem(getPosOf(title));
            if (updateQuest->description_ == "NOT DISCOVERED")
            {
                addQuest(updateQuest);
            }
        }
    }
    fin.close();
}

/**
    @param: A string reference to a quest title
    @return: The integer position of the given quest if it is in the QuestList, -1 if not found.
*/
int QuestList::getPosOf(const std::string &quest_title) const
{
    Node<Quest *> *head = first_;
    int pos = 0;
    while (head != nullptr)
    {
        if (head->getItem()->title_ == quest_title)
        {
            return pos;
        }
        head = head->getNext();
        pos++;
    }
    return -1;
}

/**
    @param: A string reference to a quest title
    @return: True if the quest with the given title is already in the QuestList
*/
bool QuestList::contains(const std::string &quest_title) const
{
    Node<Quest *> *head = first_;
    bool found = false;
    while (head != nullptr)
    {
        if (head->getItem()->title_ == quest_title)
        {
            found = true;
            break;
        }
        head = head->getNext();
    }
    return found;
}

/**
    @pre: The given quest is not already in the QuestList
    @param:  A pointer to a Quest object
    @post:  Inserts the given quest pointer into the QuestList. Each of its dependencies and subquests are also added to the QuestList IF not already in the list.
            If the quest is already in the list but is marked as "NOT DISCOVERED", update its details. (This happens when a quest has been added to the list through a dependency or subquest list)

    @return: True if the quest was added or updated successfully, False otherwise
*/
bool QuestList::addQuest(Quest *quest_object)
{
    if (!contains(quest_object->title_))
    {
        for (int i = 0; i < quest_object->dependencies_.size(); i++)
        {
            if (!contains(quest_object->dependencies_[i]->title_))
            {
                insert(item_count_, quest_object->dependencies_[i]);
            }
        }
        for (int i = 0; i < quest_object->subquests_.size(); i++)
        {
            if (!contains(quest_object->subquests_[i]->title_))
            {
                insert(item_count_, quest_object->subquests_[i]);
            }
        }
        insert(item_count_, quest_object);
        return true;
    }
    else if (contains(quest_object->title_) && quest_object->description_ == "NOT DISCOVERED")
    {
        Quest *existingQuest = getItem(getPosOf(quest_object->title_));
        existingQuest->description_ = quest_object->description_;
        existingQuest->completed_ = quest_object->completed_;
        existingQuest->experience_points_ = quest_object->experience_points_;
        existingQuest->dependencies_ = quest_object->dependencies_;
        existingQuest->subquests_ = quest_object->subquests_;
        return true;
    }
    return false;
}

/**
    @param:  A reference to string representing the quest title
    @param:  A reference to string representing the quest description
    @param:  A reference to boolean representing if the quest is completed
    @param:  An reference to int representing experience points the quest rewards upon completion
    @param:  A reference to vector of Quest pointers representing the quest's dependencies
    @param:  A reference to vector of Quest pointers representing the quest's subquests
    @post:   Creates a new Quest object and inserts a pointer to it into the QuestList.
             If the quest is already in the list but is marked as "NOT DISCOVERED", update its details. (This happens when a quest has been added to the list through a dependency or subquest list)
             Each of its dependencies and subquests are also added to the QuestList IF not already in the list.


    @return: True if the quest was added or updated successfully, False otherwise
*/
bool QuestList::addQuest(const std::string &quest_title, const std::string &quest_description, const bool &quest_completed, const int &quest_experience_points, const std::vector<Quest *> &quest_dependencies, const std::vector<Quest *> &quest_subquests)
{
    if (contains(quest_title) && quest_description == "NOT DISCOVERED")
    {
        Quest *existingQuest = getItem(getPosOf(quest_title));
        existingQuest->description_ = quest_description;
        existingQuest->completed_ = quest_completed;
        existingQuest->experience_points_ = quest_experience_points;
        existingQuest->dependencies_ = quest_dependencies;
        existingQuest->subquests_ = quest_subquests;
        for (int i = 0; i < existingQuest->dependencies_.size(); i++)
        {
            if (!contains(existingQuest->dependencies_[i]->title_))
            {
                insert(item_count_, existingQuest->dependencies_[i]);
            }
        }
        for (int i = 0; i < existingQuest->subquests_.size(); i++)
        {
            if (!contains(existingQuest->subquests_[i]->title_))
            {
                insert(item_count_, existingQuest->subquests_[i]);
            }
        }
        return true;
    }
    else
    {
        Quest *newQuest = new Quest(quest_title, quest_description, quest_completed, quest_experience_points, quest_dependencies, quest_subquests);
        insert(item_count_, newQuest);
        return true;
    }
    return false;
}

/**
    @param:  A Quest pointer
    @return: A boolean indicating if all the given quest's dependencies are completed
*/
bool QuestList::dependenciesComplete(Quest *quest_object)
{
    for (int i = 0; i < quest_object->dependencies_.size(); i++)
    {
        if (!(quest_object->dependencies_[i]->completed_))
        {
            return false;
        }
    }
    return true;
}

/**
    @param: A Quest pointer
    @return: A boolean if the given quest is available.
    Note: For a quest to be available, it must not be completed, and its dependencies must be complete.
*/
bool QuestList::questAvailable(Quest *quest_object)
{
    if (!(quest_object->completed_) && dependenciesComplete(quest_object))
    {
        return true;
    }
    return false;
}

/**
    @param: A Quest pointer
    @post: Prints the quest title and completion status
    The output should be of the form:
    [Quest Title]: [Complete / Not Complete]
    [Quest Description]\n\n
*/
void QuestList::printQuest(Quest *quest_object)
{
    std::cout << quest_object->title_ << ":";
    if (quest_object->completed_)
    {
        std::cout << "Complete" << std::endl;
    }
    else
    {
        std::cout << "Not Complete" << std::endl;
    }
    std::cout << quest_object->description_ << std::endl << std::endl;
}

/**
    @param: A string reference to a quest title
    @post:  Prints a list of quests that must to be completed before the given quest can be started (incomplete dependencies).
            If any of the quest's incomplete dependencies have an incomplete dependency, recursively print the quests that need to be done in order, indenting incomplete quests.
            The indentation for incomplete quests is 2 spaces: "  "
            The format of the list should be of the following forms for each different case:

            Query: [Quest Title]
            No such quest.

            Query: [Quest Title]
            Quest Complete

            Query: [Quest Title]
            Ready: [Quest Title]

            Query: [Quest Title]
            Ready: [Dependency0]
              [Quest Title]

            Query: [Quest Title]
            Ready: [Dependency0]
            Ready: [Dependency1]
              [Quest Title]

            Query: [Quest Title]
            Ready: [Dependency0]
            Ready: [Dependency1]
              [Dependency2]
              [Quest Title]

     If the given quest title is not found in the list, print "No such quest."
*/
void QuestList::questQuery(const std::string& title)
{

    int questPos = getPosOf(title);

    std::cout << "Query: " << title << std::endl;

    if (questPos == -1) {
    
        std::cout << "No such quest.\n";
        return;
    
    }

    Quest* quest = getPointerTo(questPos)->getItem();

    if (quest->completed_) {
    
        std::cout << "Quest Complete\n";
    
    }
    else {
    
        recursiveQuery(quest);
    
    }

}

void QuestList::recursiveQuery(Quest* const& quest)
{

    if (quest->dependencies_.size() == 0 || dependenciesComplete(quest)) {
        
        std::cout << "Ready: " << quest->title_ << std::endl;
        return;
    
    }

    for (int i = 0; i < quest->dependencies_.size(); i++) {
    
        if (!quest->dependencies_[i]->completed_) {
        
            recursiveQuery(quest->dependencies_[i]);
        
        }
    
    }

    std::cout << "  " << quest->title_ << std::endl;

}

/**
    @return: An integer sum of all the experience gained
    Note: This should only include experience from completed quests
*/
int QuestList::calculateGainedExperience()
{
    int sum = 0;
    Node<Quest *> *head = first_;
    while (head != nullptr)
    {
        if (head->getItem()->completed_)
        {
            sum += head->getItem()->experience_points_;
        }
        head = head->getNext();
    }
    return sum;
}

/**
    @param: A quest pointer to a main quest
    @return: An integer sum of all the experience that can be gained from completing the main quest AND all its subquests.
    Note: Also consider the potential experience if a subquest itself has subquests.
*/
int QuestList::calculateProjectedExperience(Quest *main_quest)
{
    int projected_experience = main_quest->experience_points_;
    for (int i = 0; i < main_quest->subquests_.size(); i++)
    {
        Quest *subquest = main_quest->subquests_[i];
        projected_experience += calculateProjectedExperience(subquest);
    }
    return projected_experience;
}

/**
    @param: A quest pointer to a main quest
    @return: An integer sum of all the experience that has already been gained by completing the given quest's subquests.
    Note: Also consider the experience gained if a completed subquest itself has subquests.
*/
int QuestList::calculatePathwayExperience(Quest *main_quest)
{
    int pathway_experience = 0;
    if (main_quest->completed_)
    {
        pathway_experience += main_quest->experience_points_;
    }
    for (int i = 0; i < main_quest->subquests_.size(); i++)
    {
        Quest *subquest = main_quest->subquests_[i];
        pathway_experience += calculatePathwayExperience(subquest);
    }
    return pathway_experience;
}

/**
    @param: A string reference to a filter with a default value of "NONE".
    @post: With default filter "NONE": Print out every quest in the list.
           With filter "COMPLETE":   Only print out the completed quests in the list.
           With filter "INCOMPLETE": Only print out the incomplete quests in the list.
           With filter "AVAILABLE":  Only print out the available quests in the list.
           If an invalid filter is passed, print "Invalid Filter\n"
    Printing quests should be of the form:
    [Quest title]: [Complete / Not Complete]
    [Quest description]\n
*/
void QuestList::questHistory(const std::string &quest_filter)
{
    Node<Quest *> *head = first_;
    if (quest_filter != "NONE" && quest_filter != "COMPLETE" && quest_filter != "INCOMPLETE" && quest_filter != "AVAILABLE")
    {
        std::cout << "Invalid Filter" << std::endl;
        return;
    }
    while (head != nullptr)
    {
        if (quest_filter == "NONE")
        {
            if (contains(head->getItem()->title_))
            {
                printQuest(head->getItem());
            }
        }
        else if (quest_filter == "COMPLETE")
        {
            if (head->getItem()->completed_)
            {
                printQuest(head->getItem());
            }
        }
        else if (quest_filter == "INCOMPLETE")
        {
            if (!(head->getItem()->completed_))
            {
                printQuest(head->getItem());
            }
        }
        else if (quest_filter == "AVAILABLE")
        {
            if (questAvailable(head->getItem()))
            {
                printQuest(head->getItem());
            }
        }
        head = head->getNext();
    }
}

/**
    @param: A quest pointer to a main quest
    @post:  Outputs subquest pathway. Print quest names with two spaces ("  ") of indentation for each subquest, recursively.
            Also print the percentage of experience gained in this pathway, rounded down to the lower integer.
            The format should be of the form:
            [Main Quest] ([Pathway XP] / [Projected XP]% Complete)
                [Subquest0]: [Complete / Not Complete]
                    [Sub-Subquest01]: [Complete / Not Complete]
                        [Sub-Subquest011]: [Complete / Not Complete]
                    [Subquest02]: [Complete / Not Complete]
            Hint: You can write a helper function to print the subquests recursively. If the given quest is already marked as completed, you can assume that all of its subquests are also completed.
*/
void QuestList::printQuestDetails(Quest *main_quest)
{
    if (!main_quest)
    {
        return;
    }
    int pathway_total = calculatePathwayExperience(main_quest);
    int projected_total = calculateProjectedExperience(main_quest);
    int experience_percentage = 0;
    if (projected_total > 0)
    {
        experience_percentage = floor(((double)pathway_total / projected_total) * 100);
    }
    std::cout << main_quest->title_ << " (" << experience_percentage << "% Complete)" << std::endl;
    for (int i = 0; i < main_quest->subquests_.size(); i++)
    {
        Quest *subquest = main_quest->subquests_[i];
        std::cout << "  ";
        std::cout << subquest->title_ << ": ";
        if (subquest->completed_)
        {
            std::cout << "Complete" << std::endl;
        }
        else
        {
            std::cout << "Not Complete" << std::endl;
        }
        printQuestDetails(subquest);
    }
}