/*
List class implementation 
*/

/*
    Default Constructor
*/
template <class ItemType>
Node<ItemType>::Node() : next_{nullptr} {}

// parameterized constructor
template <class ItemType>
Node<ItemType>::Node(const ItemType& an_item) : item_{an_item}, next_{nullptr} {}

/*
    Parametarized Constructor
    @param item - the item to be constructed
    @param next_node - the next node to point to
 */
template <class ItemType>
Node<ItemType>::Node(const ItemType& item, Node<ItemType> *next_node) : item_{item}, next_{next_node} {}


/*
    Setter function for node item
    @param anItem - the item to be set in item_
*/
template <class ItemType>
void Node<ItemType>::setItem(const ItemType &anItem) {
    item_ = anItem;
}

/*
    Setter function for pointer to next node in chain
    @param nextNodePtr - the nextNodePtr to be set in next_
*/
template <class ItemType>
void Node<ItemType>::setNext(Node<ItemType> *nextNodePtr){
    next_ = nextNodePtr;
}

/**@param previous_node_ptr a pointer to the previous node in the chain*/
template <class ItemType>
void Node<ItemType>::setPrevious(Node<ItemType> *previous_node_ptr) {
    previous_ = previous_node_ptr;
}


/*
    @return returns the value for item_
*/
template <class ItemType>
ItemType Node<ItemType>::getItem() const { return item_; }


/*
    @return returns the value for next_
*/
template <class ItemType>
Node<ItemType> *Node<ItemType>::getNext() const { return next_; }

/*
    @return returns the value for previous_
*/
template <class ItemType>
Node<ItemType> *Node<ItemType>::getPrevious() const { return previous_; }
