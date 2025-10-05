#include "CourseBST.h"

/// <summary>
/// Default constructor
/// </summary>
CourseBST::CourseBST(){}

/// <summary>
/// Destructor.
/// Deallocate used memory.
/// </summary>
CourseBST::~CourseBST()
{
	deleteBST(root);
}

/// <summary>
/// Place a course into the tree
/// </summary>
/// <param name="course">Address of course to be added</param>
void CourseBST::Insert(Course* course)
{
	root = insert(root, course);
}

/// <summary>
/// Returns the course whose number matches the courseNumber paramater.
/// Returns a null pointer if not found.
/// </summary>
/// <param name="courseNumber"> Course number to search for</param>
/// <returns></returns>
Course* CourseBST::Search(const string& courseNumber)
{
	// begin search at root
	Node* currentNode = root;
	while (currentNode != nullptr) {
		// found a match
		if (currentNode->course->GetNumber() == courseNumber) {
			// return the match
			return currentNode->course; 
		}
		// course number is less than current so traverse left
		else if (courseNumber < currentNode->course->GetNumber()) {
			currentNode = currentNode->left;
		}
		// course number is greater or equal so traverse right
		else {
			currentNode = currentNode->right;
		}
	}

	// not found, return null pointer
	return nullptr;
}

/// <summary>
/// Print all courses by course number sorted least to greatest
/// </summary>
void CourseBST::PrintAllSorted()
{
	inOrderPrint(root);
}

/// <summary>
/// Returns true if tree is empty
/// </summary>
/// <returns></returns>
bool CourseBST::Empty()
{
	return root == nullptr;
}

/// <summary>
/// Delete all nodes
/// </summary>
void CourseBST::Clear()
{
	deleteBST(root);
	root = nullptr;
}

/// <summary>
/// Traverse postorder and destruct.
/// Start from root.
/// </summary>
/// <param name="node">Current node</param>
void CourseBST::deleteBST(Node* node)
{
	// base case (start unfolding)
	if (node == nullptr) {
		return;
	}

	// traverse down tree postorder
	deleteBST(node->left);
	deleteBST(node->right);

	// delete given node after all children are gone
	delete node;
}

/// <summary>
/// Recursively insert a course into the tree
/// </summary>
/// <param name="node"></param>
/// <param name="course"></param>
/// <returns></returns>
CourseBST::Node* CourseBST::insert(Node* node, Course* course) {
	// leaf is null, place course here
	if (node == nullptr) {
		// Create and return a new node
		return new Node(course); 
	}

	// course is less than this node, traverse left for a null leaf
	if (course->GetNumber() < node->course->GetNumber()) {
		node->left = insert(node->left, course); 
	}
	// course is greater or equal to this node, traverse right for a null leaf
	else {
		node->right = insert(node->right, course);
	}

	// return the current node
	return node;
}


/// <summary>
/// Taverse tree in order and print course information
/// </summary>
/// <param name="node">Current node</param>
void CourseBST::inOrderPrint(Node* node)
{
	// base case no node here
	if (node == nullptr) {
		return;
	}

	// traverse left to start with least
	inOrderPrint(node->left);

	// print node info
	node->course->PrintSelf();

	//traverse right to reach greatest
	inOrderPrint(node->right);
}

