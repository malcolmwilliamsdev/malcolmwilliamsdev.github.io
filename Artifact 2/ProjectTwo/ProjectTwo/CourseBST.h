#pragma once

#include "Course.h"

class CourseBST
{
public:
	CourseBST();
	~CourseBST();

    void Insert(Course* course);
    Course* Search(const string& courseNumber);
    void PrintAllSorted();
    bool Empty();
    void Clear();

private:

    struct Node {
        Course* course;
        Node* left;
        Node* right;

        Node() : course(nullptr), left(nullptr), right(nullptr) {};
        Node(Course* course) : course(course), left(nullptr), right(nullptr) {};
    };

    Node* root = nullptr;

    void deleteBST(Node* node);
    Node* insert(Node* node, Course* course);
    void inOrderPrint(Node* node);
};

