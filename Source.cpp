#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <list>
#include <chrono>
#include <set>

using namespace std;

//Naive algorithm
int BruteForce(string text, string pattern)
{
	int occurances = 0;
	int patternLength = pattern.length();
	int textLength = text.length();
	for (int i = 0; i < text.length(); i++) {
		int j = 0;
		for (; j < patternLength && (i + j) < text.length(); )
		{
			if (pattern[j] == text[i + j])
				j++;
			else
				break;
		}

		if (j == patternLength)
			++occurances;
	}
	return occurances;
}


//BBST
struct AVLNode {
	string val;
	int height;
	AVLNode* left;
	AVLNode* right;
	AVLNode(string x) {
		val = x;
		left = right = nullptr;
		height = 0;
	}
};

int subtreeHeight(AVLNode* ptr)
{
	if (ptr == nullptr)
		return 0;
	else
		return ptr->height;
}

int getBalance(AVLNode* ptr)
{
	return subtreeHeight(ptr->right) - subtreeHeight(ptr->left);
}

void setBalance(AVLNode* ptr)
{
	ptr->height = 1 + max(subtreeHeight(ptr->left), subtreeHeight(ptr->right));
}

AVLNode* rightRotate(AVLNode* ptr)
{
	AVLNode* newPtr = ptr->left;
	ptr->left = newPtr->right;
	newPtr->right = ptr;
	setBalance(ptr);
	setBalance(newPtr);

	return newPtr;
}

AVLNode* leftRotate(AVLNode* ptr)
{
	AVLNode* newPtr = ptr->right;
	ptr->right = newPtr->left;
	newPtr->left = ptr;
	setBalance(ptr);
	setBalance(newPtr);

	return newPtr;
}

AVLNode* leftLeft(AVLNode* ptr)
{
	ptr = rightRotate(ptr);
	return ptr;
}

AVLNode* rightRight(AVLNode* ptr)
{
	ptr = leftRotate(ptr);
	return ptr;
}

AVLNode* leftRight(AVLNode* ptr)
{
	ptr->left = leftRotate(ptr->left);
	ptr = leftLeft(ptr);
	return ptr;
}

AVLNode* rightLeft(AVLNode* ptr)
{
	ptr->right = leftRotate(ptr->right);
	return rightRight(ptr);
}


AVLNode* insert(AVLNode* parent, string x) {
	if (parent == nullptr)
		return new AVLNode(x);
	if (x < parent->val)
		parent->left = insert(parent->left, x);
	else if (x > parent->val)
		parent->right = insert(parent->right, x);

	if (getBalance(parent) == 2)
		return (getBalance(parent->right) >= 0 ? rightRight(parent) : rightLeft(parent));
	else
		if (getBalance(parent) == -2)
			return (getBalance(parent->left) <= 0 ? leftLeft(parent) : leftRight(parent));

	setBalance(parent);
	return parent;
}


void printValue(AVLNode* node) {
	if (node == nullptr)
		cout << "nullptr";
	else
		cout << node->val;
	cout << "\n";
}

void printTree(AVLNode* node, bool isRight, string beginChar) {
	if (node != nullptr)
		printTree(node->left, false, beginChar + (isRight ? "  |      " : "         "));
	cout << beginChar;
	if (isRight)
		cout << "  '-";
	else
		cout << "  ,-";
	cout << "-----> ";
	printValue(node);
	if (node != nullptr)
		printTree(node->right, true, beginChar + (isRight ? "         " : "  |      "));
}

void printTree(AVLNode* node)
{
	if (node != nullptr)
	{
		printTree(node->left, false, "");
		printValue(node);
		printTree(node->right, true, "");
	}
	else
		throw logic_error("\nThe binary tree is empty!");
}

void delete_tree(AVLNode* node)
{
	if (node == nullptr)
		return;
	//cout << "\nDeleting tree" << endl;
	delete_tree(node->left);
	delete_tree(node->right);
	delete node;
}

bool search(AVLNode* parent, string x)
{
	while (parent != nullptr)
	{
		if (x < parent->val)
			parent = parent->left;
		else if (x > parent->val)
			parent = parent->right;
		else if (x == parent->val)
			return true;
	}

	return false;
}

//Trie
class trie {
	struct node {
		unsigned int counter;
		struct node* kids[127];
		node();
	} *root;
	unsigned int count;
public:
	trie();
	~trie();
	void insert(const string&);
	unsigned int search(const string&);
	void remove(const string&);
	unsigned int size();
};

void trie::remove(const string& str) {
	node* nodeptr = root;
	for (int i = 0; i < str.length(); i++) {
		if (nodeptr->kids[i] == nullptr)
			return;
		nodeptr = nodeptr->kids[i];
	}
	if (nodeptr->counter > 0)
		nodeptr->counter--;
	else
		return;
}

trie::trie() {
	count = 0;
	root = nullptr;
}

trie::~trie() {};

trie::node::node() {
	counter = 0;
	for (int i = 0; i < 127; i++)
		kids[i] = nullptr;
}

void trie::insert(const string& str) {
	if (root == nullptr)
		root = new node;
	node* nodeptr = root;
	for (int i = 0; i < str.length(); i++) {
		char c = str[i];
		if (nodeptr->kids[c] == nullptr)
			nodeptr->kids[c] = new node;
		nodeptr = nodeptr->kids[c];
	}
	nodeptr->counter++;
	count++;
}

unsigned int trie::search(const string& str) {
	if (root == nullptr)
		return 0;
	node* nodeptr = root;
	for (int i = 0; i < str.length(); i++) {
		char c = str[i];
		if (nodeptr->kids[c] == nullptr)
			return 0;
		nodeptr = nodeptr->kids[c];
	}
	return nodeptr->counter;
}

unsigned int trie::size() {
	return count;
}


//Hashtable
class Hashtable {
private:
	const static int hashSize = 2017;
	list<string> table[hashSize];
public:
	bool isEmpty() const;
	int hashFunction(string s);
	void insertString(string s);
	void removeString(string s);
	bool searchTable(string s);
	void printTable();
	~Hashtable();
};

bool Hashtable::isEmpty() const {
	int sum = 0;
	for (int i = 0; i < hashSize; i++)
	{
		sum += table[i].size();
	}

	if (sum == 0)
		return true;

	return false;
}

int Hashtable::hashFunction(string s) {
	int hash = 0;
	for (int i = 0; i < s.length(); i++)
	{
		hash = hash + (int)pow(s[i], s.length() - i);
		hash = hash % hashSize;
	}

	return hash;
}

void Hashtable::insertString(string s) {
	int hash = hashFunction(s);
	auto& cell = table[hash];
	auto it = begin(cell);
	for (; it != end(cell); it++)
	{
		if (*it == s)
			return;
	}

	cell.emplace_back(s);

	return;
}

void Hashtable::removeString(string s) {
	int hash = hashFunction(s);
	auto& cell = table[hash];
	auto it = begin(cell);
	for (; it != end(cell); it++)
	{
		if (*it == s)
		{
			it = cell.erase(it);
			return;
		}
	}

	cout << "String was not removed." << endl;
}

void Hashtable::printTable() {
	for (int i = 0; i < hashSize; i++)
	{
		if (table[i].size() == 0) continue;

		auto it = table[i].begin();
		for (; it != table[i].end(); it++)
		{
			cout << "Key: " << i << " String: " << *it << endl;
		}
	}
}

bool Hashtable::searchTable(string s) {
	int hash = hashFunction(s);
	auto& cell = table[hash];
	auto it = begin(cell);
	for (; it != end(cell); it++)
	{
		if (*it == s)
		{
			return true;
		}
	}

	return false;
}

Hashtable::~Hashtable() {
	if (isEmpty())
		return;
	for (int i = 0; i < hashSize; i++)
	{
		//cout << i << endl;
		auto& cell = table[i];
		auto it = begin(cell);
		while(it != end(cell))
		{
			it = cell.erase(it);
		}
	}
	table->clear();
}




int main()
{
	string word;
	ifstream words("D:\\UL\\Advanced algorithms\\proj2_part1\\words.txt");
	using chrono::nanoseconds;
	nanoseconds totalNaive(0), totalTree(0), totalTrie(0), totalHashtable(0), dictionaryNaive(0),dictionaryTree(0), dictionaryTrie(0), dictionaryHashtable(0);
	ofstream results("ex1_results.txt"), dictionariesResults("ex1_results_dictionaries.txt");
	results << "size		Naive		BBST		Trie		HashTable" << endl;
	dictionariesResults << "size		Naive		BBST		Trie		Hashtable" << endl;

	int dictionarySize = 0;

	while (getline(words, word))
	{
		++dictionarySize;
	}
	words.clear();
	words.seekg(0);

	int sizes = 1;

	cout << dictionarySize << endl;

	AVLNode* tree = new AVLNode("a");
	trie trie1;
	Hashtable table;
	list<string> naive;

	//set<string> tree;

	//Dictionary building
	int j = 0;
	for (int i = 1; i <= 4; i++)
	{
		for (; j < 2000 * i; )
		{
			getline(words, word);

			//Naive
			auto start = chrono::steady_clock::now();
			naive.push_back(word);
			auto end = chrono::steady_clock::now();
			dictionaryNaive = dictionaryNaive + end - start;
			
			//BBST
			start = chrono::steady_clock::now();
			tree = insert(tree, word);
			//tree.insert(word);
			end = chrono::steady_clock::now();
			dictionaryTree = dictionaryTree + end - start;


			//Trie
			start = chrono::steady_clock::now();
			trie1.insert(word);
			end = chrono::steady_clock::now();
			dictionaryTrie = dictionaryTrie + end - start;

			//Hashtable
			start = chrono::steady_clock::now();
				table.insertString(word);
			end = chrono::steady_clock::now();
			dictionaryHashtable = dictionaryHashtable + end - start;

			j += word.length();
		}
		dictionariesResults << j << "		" << dictionaryNaive.count() << "		" << dictionaryTree.count()
			<< "	" << dictionaryTrie.count() << "		" << dictionaryHashtable.count() << endl;
	}

	//Search
	ifstream text("D:\\UL\\Advanced algorithms\\proj2_part1\\text.txt");
	string spell, wordsFromSpell;
	int resultNaive;

	int sizeSpell = 0;
	while (getline(text, spell))
	{
		++sizeSpell;
	}
	text.clear();
	text.seekg(0);

	int sizeSpellIterate = sizeSpell / 4;
	int sizeSpellChecked = 0;

	for (int i = 1; i <= 4; i++)
	{
		for (j = 0; j < sizeSpellIterate * i; j++)
		{
			getline(text, spell);
			sizeSpellChecked += spell.length();
			for (int k = 0; k < spell.length(); k++)
			{
				if ((spell[k] >= 65 && spell[k] <= 90) || (spell[k] >= 97 && spell[k] <= 122))
				{
					wordsFromSpell += spell[k];
					continue;
				}

				words.clear();
				words.seekg(0);

				//Naive algorithm
				auto start = chrono::steady_clock::now();
				for (list<string>::iterator it = naive.begin(); it != naive.end(); it++)
				{
					if (*it == wordsFromSpell)
						break;
				}
				auto end = chrono::steady_clock::now();
				totalNaive += end - start;

				//BBST
				start = chrono::steady_clock::now();
				search(tree, wordsFromSpell);
				//tree.find(wordsFromSpell);
				end = chrono::steady_clock::now();
				totalTree += end - start;

				//Trie
				start = chrono::steady_clock::now();
				trie1.search(wordsFromSpell);
				end = chrono::steady_clock::now();
				totalTrie += end - start;


				//Hashtable
				start = chrono::steady_clock::now();
				table.searchTable(wordsFromSpell);
				end = chrono::steady_clock::now();
				totalHashtable += end - start;

				wordsFromSpell = "";
			}
		}
		
		results << sizeSpellChecked << "    " << totalNaive.count() << "		" << totalTree.count() << "		"
			<< totalTrie.count() << "		" << totalHashtable.count() << endl;
	}
	
	words.close();
	return 0;
}