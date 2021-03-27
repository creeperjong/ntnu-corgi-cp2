#include<iostream>
#include<stdlib.h>
#include<string>

using namespace std;

struct node {
	string data;
	struct node* next;
};
typedef struct node node;

int main() {
	string input;
	node* now;
	now = (node*)malloc(sizeof(node));
	cin >> input;
	now->data = input;
	now->next = NULL;
    cout << now->data << endl;
}