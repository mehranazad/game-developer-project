/*
Mehran Azad
Inventory.cpp declares the Inventory class and implements its private and public members
*/

#include "Inventory.hpp"


/**
 * @param : A Pointer to the Item object to be added to the inventory.
 * @post  : The Item is added to the inventory, preserving the BST structure. The BST property is based on (ascending) alphabetical order of the item's name.
 *          If the item is type UNKNOWN, WEAPON, or ARMOR and is already in the inventory, it is not added.
      However, if another instance of an Item of type CONSUMABLE is being added (an item with the same name), its quantity is updated to the sum of the quantities of the two objects, 
      and the time_picked_up_ variable is updated to that of the latest instance of the item being added.
 * @return true if the item was added to the inventory or updated, false otherwise.
 */



bool Inventory::addItem(Item* item_)
{
    if (((item_->getType() == "UNKNOWN") || (item_->getType() == "WEAPON") || (item_->getType() == "ARMOR")) && contains(item_))

    {
        return false;
    }

    if (contains(item_))

    {
        if (item_->getType() == "CONSUMABLE")

        {
            ((findItem(item_->getName()))->setQuantity(item_->getQuantity()+(findItem(item_->getName()))->getQuantity()));

            (findItem(item_->getName()))->updateTimePickedUp();
            return true;

        }

        
    }

add(item_);
return true;


}




/**
 * @param   : A reference to string name of the item to be removed from the inventory.
 * @return  : True if the item was found and removed or updated successfully, false otherwise.
 * @post    : If the item is found in the inventory, it is removed while preserving the BST structure.
              If a CONSUMABLE is removed, its quantity is decremented by one, but its time_picked_up_ remains the same. 
              However, if it is the last item (it's quantity is 1 and is being removed), the item should be removed. 
              Non-CONSUMABLE items should always be removed when they are found.
 */


bool Inventory::removeItem(const std::string& item_)

{
    Item* example_ = new Item(item_, UNKNOWN, 0, 0, 0);

    if (contains(example_))
    {
        if (findItem(example_->getName())->getType() != "CONSUMABLE")
          {
            remove(example_);
            return true;
          }

        else if (findItem(example_->getName())->getType() == "CONSUMABLE")
        {
            if (findItem(example_->getName())->getQuantity() > 1)
            {
                findItem(example_->getName())->setQuantity(findItem(example_->getName())->getQuantity()-1);
                return true;
            }
            remove(example_);
            return true;
        }
        
    }

    return false;

}



/**
 * @param   : A reference to string name of the item to be found.
 * @return  : An Item pointer to the found item, or nullptr if the item is not in the inventory.
 */


Item* Inventory::findItem(const std::string& item_) const
{
    Item* example_ = new Item(item_, UNKNOWN, 0, 0, 0);

    if (contains(example_))
    {
        return getPointerTo(example_)->getItem();

    }
    return nullptr;

}


/**
 * @return    : The total sum of gold values of all items in the inventory.
                Note: consumable items may have quantity >1 and gold value must be
                      added for each item
 */

int Inventory::getTotalGoldValue() 
{
    return traverseInventory(getRoot());

}


int Inventory::traverseInventory(std::shared_ptr <BinaryNode<Item*>> child_)
{
    if (child_ == nullptr)
    {
        return 0;
    }
    if ((child_->getItem()->getType() == "CONSUMABLE") && (child_->getItem()->getQuantity() > 1))
    {
        return ((child_->getItem()->getGoldValue())*child_->getItem()->getQuantity()) + traverseInventory(child_->getLeftChildPtr()) + traverseInventory(child_->getRightChildPtr());
    }
    return ((child_->getItem()->getGoldValue()) + traverseInventory(child_->getLeftChildPtr()) + traverseInventory(child_->getRightChildPtr()));
}


/**
 * @post    : The names of all items in the Inventory are printed in ascending order.
 *            This function performs an in-order traversal of the binary search tree and prints the details of each item in the following format. 
        NOTE: QUANTITY ONLY NEEDS TO BE DISPLAYED FOR CONSUMABLE ITEMS.

 *            [NAME] ([TYPE])
              Level: [LEVEL]
              Value: [VALUE]
              Quantity: [QUANTITY]
   
              Example:
              TIRED GAUNTLETS (ARMOR)
              Level: 3
              Value: 25
        
              SMALL HEALTH POTION (CONSUMABLE)
              Level: 1
              Value: 10
              Quantity: 2
 * 
 */

void Inventory::printInventoryInOrder()

{
    std::vector<Item*> inventory;
    inorderTraversal(inventory, getRoot());
    std::sort(inventory.begin(), inventory.end(), [this](Item* a, Item* b)
    {
        return a-> operator<(*b);
    });

    for (int i = 0; i < inventory.size(); i++)
    {
        printOrder(inventory[i]);
    }


}

void Inventory::printOrder(Item* item_)
{
    std::cout << item_->getName() << " "
               << "(" << item_->getType() << ")" << std::endl;
     std::cout << "Level: " << item_->getLevel() << std::endl;
     std::cout << "Value: " << item_->getGoldValue() << std::endl;
     if (item_->getType() == "CONSUMABLE")
     {
          std::cout << "Quantity: " << item_->getQuantity() << std::endl;
     }
     std::cout << std::endl;

}

void Inventory::inorderTraversal(std::vector<Item*> &ordereditems_, std::shared_ptr <BinaryNode<Item*>> unordereditems_)
{
    if (unordereditems_ == nullptr)
    {
        return;
    }
    
    inorderTraversal(ordereditems_, unordereditems_->getLeftChildPtr());
    ordereditems_.push_back(unordereditems_->getItem());
    inorderTraversal(ordereditems_, unordereditems_->getRightChildPtr());

}


/**
 * @param   : a reference to bool if the items are to be printed in ascending order. 
 * @param   : a reference to string attribute which defines the order by which the items are to be printed. 
 *            You may assume that the given parameter will be in one of the following forms: ["NAME"/"TYPE"/"LEVEL"/"VALUE"/"TIME"]
 * @post    : All items in the Inventory are printed in the order specified by the parameter.
 *          Example usage: inventory.printInventory(false, "LEVEL");
            NOTE: QUANTITY ONLY NEEDS TO BE DISPLAYED FOR CONSUMABLE ITEMS:

            FEATHER DUSTER (WEAPON)
            Level: 5
            Value: 100

            TIRED GAUNTLETS (ARMOR)
            Level: 3
            Value: 50

            SMALL HEALTH POTION (CONSUMABLE)
            Level: 1
            Value: 10
            Quantity: 2

            NOODLES (CONSUMABLE)
            Level: 0
            Value: 120
            Quantity: 5
 */


void Inventory::printInventory(const bool &order_, const std::string &attribute_)
{
    if (attribute_ == "NAME")
    {
        if (order_ == true)
        {
            printAscendingAttributes("NAME");
        }
        printDescendingAttributes("NAME");
    }

    else if (attribute_ == "TYPE")
    {
        if (order_ == true)
        {
            printAscendingAttributes("TYPE");
        }
        printDescendingAttributes("TYPE");
    }

    else if (attribute_ == "LEVEL")
    {
        if (order_ == true)
        {
            printAscendingAttributes("LEVEL");
        }
        printDescendingAttributes("LEVEL");
    }

    else if (attribute_ == "VALUE")
    {
        if (order_ == true)
        {
            printAscendingAttributes("VALUE");
        }
        printDescendingAttributes("VALUE");
    }

    else if (attribute_ == "TIME")
    {
        if (order_ == true)
        {
            printAscendingAttributes("TIME");
        }
        printDescendingAttributes("TIME");
    }


}

void Inventory::printAscendingAttributes(const std::string& attribute)
{
    std::vector<Item*> inventory;
    inorderTraversal(inventory, getRoot());
    if (attribute == "NAME")
    {
        std::sort(inventory.begin(), inventory.end(), [this](Item* a, Item* b)
        {
            return a-> operator<(*b);
        });

        for (int i = 0; i < inventory.size(); i++)
        {
            printOrder(inventory[i]);
        }
    }
    else if (attribute == "TYPE")
    {
        std::sort(inventory.begin(), inventory.end(), [this](Item* a, Item* b)
        {
            return a->getType() < (b->getType());
        });

        for (int i = 0; i < inventory.size(); i++)
        {
            printOrder(inventory[i]);
        }
    }
    else if (attribute == "LEVEL")
    {
        std::sort(inventory.begin(), inventory.end(), [this](Item* a, Item* b)
        {
            return a->getLevel() < (b->getLevel());
        });

        for (int i = 0; i < inventory.size(); i++)
        {
            printOrder(inventory[i]);
        }
    }
    else if (attribute == "VALUE")
    {
        std::sort(inventory.begin(), inventory.end(), [this](Item* a, Item* b)
        {
            return a->getGoldValue() < (b->getGoldValue());
        });

        for (int i = 0; i < inventory.size(); i++)
        {
            printOrder(inventory[i]);
        }
    }
    else if (attribute == "TIME")
    {
        std::sort(inventory.begin(), inventory.end(), [this](Item* a, Item* b)
        {
            return a->getTimePickedUp() < (b->getTimePickedUp());
        });

        for (int i = 0; i < inventory.size(); i++)
        {
            printOrder(inventory[i]);
        }
    }

}


void Inventory::printDescendingAttributes(const std::string& attribute)
{
    std::vector<Item*> inventory;
    inorderTraversal(inventory, getRoot());
    if (attribute == "NAME")
    {
        std::sort(inventory.begin(), inventory.end(), [this](Item* a, Item* b)
        {
            return a-> operator>(*b);
        });

        for (int i = 0; i < inventory.size(); i++)
        {
            printOrder(inventory[i]);
        }
    }
    else if (attribute == "TYPE")
    {
        std::sort(inventory.begin(), inventory.end(), [this](Item* a, Item* b)
        {
            return a->getType() > (b->getType());
        });

        for (int i = 0; i < inventory.size(); i++)
        {
            printOrder(inventory[i]);
        }
    }
    else if (attribute == "LEVEL")
    {
        std::sort(inventory.begin(), inventory.end(), [this](Item* a, Item* b)
        {
            return a->getLevel() > (b->getLevel());
        });

        for (int i = 0; i < inventory.size(); i++)
        {
            printOrder(inventory[i]);
        }
    }
    else if (attribute == "VALUE")
    {
        std::sort(inventory.begin(), inventory.end(), [this](Item* a, Item* b)
        {
            return a->getGoldValue() > (b->getGoldValue());
        });

        for (int i = 0; i < inventory.size(); i++)
        {
            printOrder(inventory[i]);
        }
    }
    else if (attribute == "TIME")
    {
        std::sort(inventory.begin(), inventory.end(), [this](Item* a, Item* b)
        {
            return a->getTimePickedUp() > (b->getTimePickedUp());
        });

        for (int i = 0; i < inventory.size(); i++)
        {
            printOrder(inventory[i]);
        }
    }

}
