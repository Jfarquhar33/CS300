//============================================================================
// Name        : 7-1 Project Two Farquhar.cpp
// Author      : Justin Farquhar
// Version     : 1.0
// Copyright   : Copyright © 2017 SNHU COCE
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <time.h>

#include "CSVparser.hpp"

using namespace std;

// forward declarations
double strToDouble(string str, char ch);

// Define Structure to hold course information
struct Course {
    string courseId;
    string courseName;
    string coursePreReq;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/*
* Define a class containing data members and methods to
* implement a binary search tree
*/
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void inOrder(Node* node);
    Node* removeNode(Node* node, string courseId);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void InOrder();
    void Insert(Course course);
    void Remove(string courseId);
    Course Search(string courseId);
};

/**
Default constructor
*/
BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

/**
Destructor
*/
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
}

/**
* Traverse the tree in order
*/
void BinarySearchTree::InOrder() {
    // call inOrder function and pass root
    this->inOrder(root);
}

/**
* Insert a course
*/
void BinarySearchTree::Insert(Course course) {
    // if root equal to null ptr
    if (root == nullptr) {
        root = new Node(course);
    }
    else {
        // add Node root and course
        this->addNode(root, course);
    }
}

/**
* Remove a course
*/
void BinarySearchTree::Remove(string courseId) {
    // remove node root courseId
    this->removeNode(root, courseId);
}

/**
* Search for a course
*/
Course BinarySearchTree::Search(string courseId) {
    // set current node equal to root
    Node* current = root;
    // keep looping downwards until bottom reached or matching course
    while (current != nullptr) {
        // if match found, return current course
        if (current->course.courseId == courseId) {
            return current->course;
        }
        // if course is smaller than current node, traverse left
        if (courseId < current->course.courseId) {
            current = current->left;
        }
        // else larger, so traverse right
        else {
            current = current->right;
        }
    }
    Course course;
    return course;
}

/**
* Add a course to some node (recursive)
* 
* @param node Current node in tree
* @param course Course to be added
*/
void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (node->course.courseId.compare(course.courseId) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        // else recurse down the left node
        else {
            this->addNode(node->left, course);
        }
    }
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        // else recurse down the right node
        else {
            this->addNode(node->right, course);
        }
    }
}

/**
* Ordering in alphanumeric order
*/
void BinarySearchTree::inOrder(Node* node) {
    // if node is not equal to null ptr
    if (node != nullptr) {
        inOrder(node->left);
        // output courseID, courseName, prereqs
        cout << node->course.courseId << ": "
            << node->course.courseName << " | "
            << node->course.coursePreReq << endl;
        //inOrder right
        inOrder(node->right);

    }
}

Node* BinarySearchTree::removeNode(Node* node, string courseId) {
    // if node is null then return to avoid crashing
    if (node == nullptr) {
        return node;
    }

    // recurse down left tree
    if (courseId.compare(node->course.courseId) < 0) {
        node->left = removeNode(node->left, courseId);
    }
    else if (courseId.compare(node->course.courseId) > 0) {
        node->right = removeNode(node->right, courseId);
    }
    else {
        // no children, so a leaf node
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }
        // one child on left
        else if (node->left != nullptr && node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
        }
        // one child on right
        else if (node->right != nullptr && node->left == nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
        }
        // two children
        else {
            Node* temp = node->right;
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            node->course = temp->course;
            node->right = removeNode(node->right, temp->course.courseId);
        }
    }
    return node;
}

//============================================================================
// Static methods used for testing
//============================================================================

/**
 * Display the course information to the console (std::out)
 *
 * @param course struct containing the bid info
 */
void displayCourse(Course course) {
    cout << course.courseId << ": " << course.courseName << " | " 
         << course.coursePreReq << endl;
    return;
}

/**
 * Load a CSV file containing courses into a container
 *
 * @param csvPath the path to the CSV file to load
 * @return a container holding all the courses read
 */
void loadCourses(string csvPath, BinarySearchTree* bst) {
    cout << "Loading CSV file " << csvPath << endl;

    // initialize the CSV Parser using the given path
    csv::Parser file = csv::Parser(csvPath);

    // read and display header row - optional
    vector<string> header = file.getHeader();
    for (auto const& c : header) {
        cout << c << " | ";
    }
    cout << "" << endl;

    try {
        // loop to read rows of a CSV file
        for (unsigned int i = 0; i < file.rowCount(); i++) {

            // Create a data structure and add to the collection of bids
            Course course;
            course.courseId = file[i][0];
            course.courseName = file[i][1];
            if (!file[i][2].empty()) {
                course.coursePreReq = file[i][2];
            }
            /*int j = 2;
            while(!file[i][j].empty()) {
                course.coursePreReq.append(file[i][j]);
                j++;
            }
            j = 2;
            */

            // push this course to the end
            bst->Insert(course);
        }
    }
    catch (csv::Error& e) {
        std::cerr << e.what() << std::endl;
    }
}




int main(int argc, char* argv[])
{
    // Defining timer variable
    clock_t ticks;
    string csvPath;
    string courseKey;

    // Defining a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;
    
    int choice = 0;
    while (choice != 9) {
        cout << "Welcome to the course planner" << endl;
        cout << endl;
        cout << "  1. Load Data Structure." << endl;
        cout << "  2. Print Course List." << endl;
        cout << "  3. Print Course." << endl;
        cout << "  9. Exit." << endl;
        cout << endl;
        cout << "What would you like to do?" << endl;
        cin >> choice;

        switch (choice) {
        case 1:

            // Initialize a timer variable before loading courses
            ticks = clock();

            // Getting csv input
            cout << "Enter the file name: ";
            cin >> csvPath;

            // Complete method call to load courses
            loadCourses(csvPath, bst);

            //cout << bst->Size() << " courses read" << endl;

            // Calculate elapsed time and display result
            ticks = clock() - ticks; // current clock ticks minus starting ticks
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            cout << endl;
            break;

        case 2:
            cout << "Here is a sample schedule:" << endl;
            cout << endl;

            bst->InOrder();
            cout << endl;
            break;

        case 3:
            ticks = clock();
            //string courseKey;

            cout << "What course do you want to know about?" << endl;
            cin >> courseKey;

            course = bst->Search(courseKey);

            ticks = clock() - ticks; // current ticks minus starting ticks

            if (!course.courseId.empty()) {
                displayCourse(course);
            }
            else {
                cout << "Course Id " << courseKey << " not found." << endl;
            }
            cout << endl;
            cout << "time: " << ticks << " clock ticks" << endl;
            cout << "time: " << ticks * 1.0 / CLOCKS_PER_SEC << " seconds" << endl;
            cout << endl;
            break;

        default:
            cout << endl << choice << " is not a valid option." << endl;
            cout << endl;
            break;
        }
    }

    cout << "Thank you for using the course planner!" << endl;

    return 0;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu


