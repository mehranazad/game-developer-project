/*
 * File Title: Tavern.hpp
 * Author: Mehran Azad
 * Date: November 10, 2023
 * This file is the interface of a class called QuestList
 */
#ifndef QUESTLIST_HPP_
#define QUESTLIST_HPP_

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <fstream>
#include <sstream>
#include <cmath>
#include "DoublyLinkedList.hpp"


struct Quest
{
    std::string title_;                
    std::string description_;         
    bool completed_;                    
    int experience_points_;            
    std::vector<Quest *> dependencies_; 
    std::vector<Quest *> subquests_;    

    /**
        Default Constructor
        @post: Creates a new Quest object with default values (zero-initialized)
    */
    Quest() : title_{""}, description_(""), completed_{false}, experience_points_{0}, dependencies_{}, subquests_{} {};
    /*
    @param: A reference to string reference to a quest title
    @param: A reference to string reference to a quest description
    @param: A reference to boolean representing if the quest is completed
    @param: An reference to int indicating the experience points
    @param: A reference to vector of Quest pointers representing the quest's dependencies
    @param: A reference to vector of Quest pointers representing the quest's subquests
    @post: Creates a new Quest object with the given parameters
    */
    Quest(const std::string &title, const std::string &description, const bool &completed, const int &experience_points, const std::vector<Quest *> &dependencies, const std::vector<Quest *> &subquests) : title_{title}, description_{description}, completed_{completed}, experience_points_{experience_points}, dependencies_{dependencies}, subquests_{subquests} {};
};
class QuestList : public DoublyLinkedList<Quest *>
{
public:

    /**
    Default Constructor
    */
    QuestList();

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
    QuestList(const std::string &input_file_name);

    /**
    @param: A string reference to a quest title
    @return: The integer position of the given quest if it is in the QuestList, -1 if not found.
    */
    int getPosOf(const std::string &quest_title) const;

    /**
    @param: A string reference to a quest title
    @return: True if the quest with the given title is already in the QuestList
    */
    bool contains(const std::string &quest_title) const;

    /**
    @pre: The given quest is not already in the QuestList
    @param:  A pointer to a Quest object
    @post:  Inserts the given quest pointer into the QuestList. Each of its dependencies and subquests are also added to the QuestList IF not already in the list.
            If the quest is already in the list but is marked as "NOT DISCOVERED", update its details. (This happens when a quest has been added to the list through a dependency or subquest list)

    @return: True if the quest was added or updated successfully, False otherwise
    */
    bool addQuest(Quest *quest_object);

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
    bool addQuest(const std::string &quest_title, const std::string &quest_description, const bool &quest_completed, const int &quest_experience_points, const std::vector<Quest *> &quest_dependencies, const std::vector<Quest *> &quest_subquests);

    /**
    @param:  A Quest pointer
    @return: A boolean indicating if all the given quest's dependencies are completed
    */
    bool dependenciesComplete(Quest *quest_object);

    /**
    @param: A Quest pointer
    @return: A boolean if the given quest is available.
    Note: For a quest to be available, it must not be completed, and its dependencies must be complete.
    */
    bool questAvailable(Quest *quest_object);

    /**
    @param: A Quest pointer
    @post: Prints the quest title and completion status
    The output should be of the form:
    [Quest Title]: [Complete / Not Complete]
    [Quest Description]\n\n
    */
    void printQuest(Quest *quest_object);

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
    void questQuery(const std::string &quest_title);

    /**
    @return: An integer sum of all the experience gained
    Note: This should only include experience from completed quests
    */
    int calculateGainedExperience();

    /**
    @param: A quest pointer to a main quest
    @return: An integer sum of all the experience that can be gained from completing the main quest AND all its subquests.
    Note: Also consider the potential experience if a subquest itself has subquests.
    */
    int calculateProjectedExperience(Quest *main_quest);

    /**
    @param: A quest pointer to a main quest
    @return: An integer sum of all the experience that has already been gained by completing the given quest's subquests.
    Note: Also consider the experience gained if a completed subquest itself has subquests.
    */
    int calculatePathwayExperience(Quest *main_quest);

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
    void questHistory(const std::string &quest_filter = "NONE");

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
    void printQuestDetails(Quest *main_quest);

    void recursiveQuery(Quest* const& quest);
    
};

#endif
