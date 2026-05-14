#include "Train.h"
#include <iomanip>

using namespace std;

TrainRegistry::TrainRegistry() : root(nullptr), count(0) {
}

TrainRegistry::~TrainRegistry() {
    clear();
}

int TrainRegistry::getNodeHeight(AVLNode* node) const {
    if (!node) {
        return 0;
    }
    return node->height;
}

int TrainRegistry::getBalanceFactor(AVLNode* node) const {
    if (!node) {
        return 0;
    }
    return getNodeHeight(node->left) - getNodeHeight(node->right);
}

AVLNode* TrainRegistry::rotateRight(AVLNode* pivot) {
    AVLNode* newRoot = pivot->left;
    AVLNode* subtree = newRoot->right;

    newRoot->right = pivot;
    pivot->left = subtree;

    int leftHeight = getNodeHeight(pivot->left);
    int rightHeight = getNodeHeight(pivot->right);
    pivot->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);

    leftHeight = getNodeHeight(newRoot->left);
    rightHeight = getNodeHeight(newRoot->right);
    newRoot->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);

    return newRoot;
}

AVLNode* TrainRegistry::rotateLeft(AVLNode* pivot) {
    AVLNode* newRoot = pivot->right;
    AVLNode* subtree = newRoot->left;

    newRoot->left = pivot;
    pivot->right = subtree;

    int leftHeight = getNodeHeight(pivot->left);
    int rightHeight = getNodeHeight(pivot->right);
    pivot->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);

    leftHeight = getNodeHeight(newRoot->left);
    rightHeight = getNodeHeight(newRoot->right);
    newRoot->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);

    return newRoot;
}

AVLNode* TrainRegistry::rebalance(AVLNode* node) {
    if (!node) {
        return nullptr;
    }

    int leftHeight = getNodeHeight(node->left);
    int rightHeight = getNodeHeight(node->right);
    node->height = 1 + (leftHeight > rightHeight ? leftHeight : rightHeight);

    int balance = getBalanceFactor(node);

    // left heavy
    if (balance > 1 && getBalanceFactor(node->left) >= 0) {
        return rotateRight(node);
    }

    if (balance > 1 && getBalanceFactor(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // right heavy
    if (balance < -1 && getBalanceFactor(node->right) <= 0) {
        return rotateLeft(node);
    }

    if (balance < -1 && getBalanceFactor(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

AVLNode* TrainRegistry::insertHelper(AVLNode* node, int id, const char* name, bool& added) {
    if (!node) {
        added = true;
        count++;
        return new AVLNode(id, name);
    }

    if (id < node->trainID) {
        node->left = insertHelper(node->left, id, name, added);
    }
    else if (id > node->trainID) {
        node->right = insertHelper(node->right, id, name, added);
    }
    else {
        added = false;
        return node;
    }

    return rebalance(node);
}

bool TrainRegistry::insert(int id, const char* name) {
    if (id <= 0) {
        printError("Train ID must be positive.");
        return false;
    }
    bool added = false;
    root = insertHelper(root, id, name, added);
    if (!added) {
        printError("Train with this ID already exists.");
    }
    return added;
}

AVLNode* TrainRegistry::findMinNode(AVLNode* node) const {
    while (node && node->left) {
        node = node->left;
    }
    return node;
}

AVLNode* TrainRegistry::findMaxNode(AVLNode* node) const {
    while (node && node->right) {
        node = node->right;
    }
    return node;
}

// uses predecessor (max of left subtree) for two children case
AVLNode* TrainRegistry::removeHelper(AVLNode* node, int id, bool& removed) {
    if (!node) {
        removed = false;
        return nullptr;
    }

    if (id < node->trainID) {
        node->left = removeHelper(node->left, id, removed);
    } else if (id > node->trainID) {
        node->right = removeHelper(node->right, id, removed);
    } else {
        removed = true;

        if (!node->left || !node->right) {
            AVLNode* child = node->left ? node->left : node->right;
            if (!child) {
                delete node;
                count--;
                return nullptr;
            } else {
                node->trainID = child->trainID;
                strCopyN(node->name, child->name, 100);
                delete node->coaches;
                delete node->seats;
                node->coaches = child->coaches;
                node->seats = child->seats;
                child->coaches = nullptr;
                child->seats = nullptr;
                node->left = child->left;
                node->right = child->right;
                delete child;
                count--;
            }
        } else {
            AVLNode* predecessor = findMaxNode(node->left);
            node->trainID = predecessor->trainID;
            strCopyN(node->name, predecessor->name, 100);
            delete node->coaches;
            delete node->seats;
            node->coaches = predecessor->coaches;
            node->seats = predecessor->seats;
            predecessor->coaches = nullptr;
            predecessor->seats = nullptr;
            node->left = removeHelper(node->left, predecessor->trainID, removed);
        }
    }

    if (!node) {
        return nullptr;
    }
    return rebalance(node);
}

bool TrainRegistry::remove(int id) {
    bool removed = false;
    root = removeHelper(root, id, removed);
    if (!removed) {
        printError("Train not found.");
    }
    return removed;
}

AVLNode* TrainRegistry::searchHelper(AVLNode* node, int id) const {
    if (!node) {
        return nullptr;
    }
    if (id == node->trainID) {
        return node;
    }
    if (id < node->trainID) {
        return searchHelper(node->left, id);
    }
    return searchHelper(node->right, id);
}

AVLNode* TrainRegistry::search(int id) const {
    return searchHelper(root, id);
}

void TrainRegistry::inOrderHelper(AVLNode* node) const {
    if (!node) {
        return;
    }
    inOrderHelper(node->left);
    cout << "  " << CLR_GREEN << setw(8) << left << node->trainID << CLR_RESET;
    cout << " " << CLR_WHITE << setw(25) << left << node->name << CLR_RESET;
    cout << " " << CLR_CYAN << setw(8) << node->coaches->getCount() << CLR_RESET;
    cout << " " << CLR_MAGENTA << node->coaches->getTotalSeats() << CLR_RESET << endl;
    inOrderHelper(node->right);
}

void TrainRegistry::preOrderHelper(AVLNode* node) const {
    if (!node) {
        return;
    }
    cout << "  " << CLR_GREEN << setw(8) << left << node->trainID << CLR_RESET;
    cout << " " << CLR_WHITE << setw(25) << left << node->name << CLR_RESET;
    cout << " " << CLR_CYAN << setw(8) << node->coaches->getCount() << CLR_RESET;
    cout << " " << CLR_MAGENTA << node->coaches->getTotalSeats() << CLR_RESET << endl;
    preOrderHelper(node->left);
    preOrderHelper(node->right);
}

void TrainRegistry::postOrderHelper(AVLNode* node) const {
    if (!node) {
        return;
    }
    postOrderHelper(node->left);
    postOrderHelper(node->right);
    cout << "  " << CLR_GREEN << setw(8) << left << node->trainID << CLR_RESET;
    cout << " " << CLR_WHITE << setw(25) << left << node->name << CLR_RESET;
    cout << " " << CLR_CYAN << setw(8) << node->coaches->getCount() << CLR_RESET;
    cout << " " << CLR_MAGENTA << node->coaches->getTotalSeats() << CLR_RESET << endl;
}

void TrainRegistry::inOrder() const {
    if (isEmpty()) {
        printWarning("No trains registered.");
        return;
    }
    cout << endl;
    cout << CLR_BOLD << CLR_CYAN << "  In-Order Traversal (Sorted by ID):" << CLR_RESET << endl;
    printSeparator('-', 55);
    cout << CLR_BOLD << "  TrainID  Name                      Coaches  Seats" << CLR_RESET << endl;
    printSeparator('-', 55);
    inOrderHelper(root);
    printSeparator('-', 55);
}

void TrainRegistry::preOrder() const {
    if (isEmpty()) {
        printWarning("No trains registered.");
        return;
    }
    cout << endl;
    cout << CLR_BOLD << CLR_CYAN << "  Pre-Order Traversal:" << CLR_RESET << endl;
    printSeparator('-', 55);
    cout << CLR_BOLD << "  TrainID  Name                      Coaches  Seats" << CLR_RESET << endl;
    printSeparator('-', 55);
    preOrderHelper(root);
    printSeparator('-', 55);
}

void TrainRegistry::postOrder() const {
    if (isEmpty()) {
        printWarning("No trains registered.");
        return;
    }
    cout << endl;
    cout << CLR_BOLD << CLR_CYAN << "  Post-Order Traversal:" << CLR_RESET << endl;
    printSeparator('-', 55);
    cout << CLR_BOLD << "  TrainID  Name                      Coaches  Seats" << CLR_RESET << endl;
    printSeparator('-', 55);
    postOrderHelper(root);
    printSeparator('-', 55);
}

void TrainRegistry::displayAll() const {
    if (isEmpty()) {
        printWarning("No trains registered.");
        return;
    }
    inOrder();
    cout << CLR_DIM << "  Total trains: " << count << CLR_RESET << endl;
}

void TrainRegistry::displayTrainDetails(int id) const {
    AVLNode* node = search(id);
    if (!node) {
        printError("Train not found.");
        return;
    }

    cout << endl;
    printDoubleSeparator(55);
    cout << CLR_BOLD << CLR_YELLOW << "  Train Details" << CLR_RESET << endl;
    printSeparator('-', 55);
    cout << "  " << CLR_CYAN << "Train ID:   " << CLR_WHITE << CLR_BOLD << node->trainID << CLR_RESET << endl;
    cout << "  " << CLR_CYAN << "Name:       " << CLR_WHITE << CLR_BOLD << node->name << CLR_RESET << endl;
    cout << "  " << CLR_CYAN << "Coaches:    " << CLR_WHITE << node->coaches->getCount() << CLR_RESET << endl;
    cout << "  " << CLR_CYAN << "Total Seats:" << CLR_WHITE << " " << node->coaches->getTotalSeats() << CLR_RESET << endl;

    if (node->seats && node->seats->isInitialized()) {
        cout << "  " << CLR_CYAN << "Booked:     ";
        cout << CLR_RED << node->seats->getBookedCount() << CLR_RESET;
        cout << " / " << CLR_GREEN << node->seats->getTotalSeats() << CLR_RESET << endl;
    } else {
        cout << "  " << CLR_CYAN << "Seating:    " << CLR_DIM << "Not initialized" << CLR_RESET << endl;
    }

    printSeparator('-', 55);
    if (node->coaches->getCount() > 0) {
        cout << CLR_BOLD << CLR_CYAN << "  Coach Configuration:" << CLR_RESET << endl;
        node->coaches->traverseForward();
    }
    printDoubleSeparator(55);
}

int TrainRegistry::countNodes(AVLNode* node) const {
    if (!node) {
        return 0;
    }
    return 1 + countNodes(node->left) + countNodes(node->right);
}

int TrainRegistry::getCount() const {
    return count;
}

bool TrainRegistry::isEmpty() const {
    return root == nullptr;
}

void TrainRegistry::freeTree(AVLNode* node) {
    if (!node) {
        return;
    }
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

void TrainRegistry::clear() {
    freeTree(root);
    root = nullptr;
    count = 0;
}
