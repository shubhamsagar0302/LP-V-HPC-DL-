#include <iostream>
#include <queue>
#include <omp.h>  // Include OpenMP header

using namespace std;

class node {
public:
    int data;
    node *left, *right;
    
    node(int val) {  // Constructor for initializing a new node
        data = val;
        left = right = NULL;
    }
};

class Breadthfs {
public:
    node* insert(node*, int);
    void parallel_bfs(node*);
};

node* insert(node* root, int data) {
    if (!root) 
        return new node(data); // Create new root if tree is empty

    if (data < root->data)
        root->left = insert(root->left, data);
    else
        root->right = insert(root->right, data);

    return root;
}

void parallel_bfs(node* head) {
    if (!head) return;

    queue<node*> q;
    q.push(head);

    while (!q.empty()) {
        int qSize = q.size();

        #pragma omp parallel for
        for (int i = 0; i < qSize; i++) {
            node* currNode;

            // Critical section ensures only one thread modifies queue at a time
            #pragma omp critical
            {
                currNode = q.front();
                q.pop();
                cout << currNode->data << " ";
            }

            #pragma omp critical
            {
                if (currNode->left)
                    q.push(currNode->left);
                if (currNode->right)
                    q.push(currNode->right);
            }
        }
    }
}

int main() {
    node* root = NULL;
    int data;
    char ans;

    do {
        cout << "\nEnter data: ";
        cin >> data;

        root = insert(root, data);

        cout << "Do you want to insert one more node? (y/n): ";
        cin >> ans;

    } while (ans == 'y' || ans == 'Y');

    cout << "\nParallel Breadth-First Search (Level Order Traversal): ";
    parallel_bfs(root);

    return 0;
}
