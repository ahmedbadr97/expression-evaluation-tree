#include <iostream>
#include <cstdlib>
#include <string>


using namespace std;
//__________________________BTREE _______________________________
typedef char ETYPE;
typedef struct node {
    ETYPE data;
    node *left;
    node *right;

    int Type;
    // this is a variable for distinguishing between  numbers  and operators and brackets (closing bracket and opening bracket)
    // 0 -> number  1 -> operator  2-> '('   3-> ')'
    //as 2 for pushing in a stack 3 pop from a stack

} Tnode;
typedef Tnode *BTREE;
//_______________________  Stack ________________________________
typedef Tnode *STYPE;
// Node
typedef struct list {
    STYPE data;
    list *next;
} Node;
typedef struct {
    Node *Top;
} Stack;


//_________________BTREE Methods_________________
void expTreeIni(BTREE *btree);

Tnode *createNode(ETYPE input);

void showNode(Tnode *node);

Tnode *insert(BTREE node, Tnode *input);

void inorder(BTREE tree);

void Preorder(BTREE tree);

void postOrder(BTREE tree);

int priority(Tnode *node);

void freeTree(BTREE tree);


//_______________Stack Functions______________________
Node *stackNode(STYPE element);

void stackIni(Stack *stack);

int isEmpty(Stack stack);

int isLast(Stack stack);

int push(Stack *stack, STYPE element);

int pop(Stack *stack, STYPE *element);

//___________Strlen__________
int Strlen(string x);


//___________Global Variables_______________
Tnode *Tree1 = (Tnode *) malloc(sizeof(Tnode));
Stack stack;
Stack priorityStack;


//__________________________  Main   ___________________________
int main() {
    int choice = -1;
    stackIni(&stack);
    expTreeIni(&Tree1);

    string element;
    int len = 0;

    do {
        cout << "EXP Tree\n1-insert \n2-In order\n3-PreOrder\n4-postOrder\n0-Exit\n";
        cin >> choice;
        switch (choice) {
            case 1:
                cout << "Input:";
                cin >> element;
                len = Strlen(element);
                for (int i = 0; i < len; i++) {
                    Tree1 = insert(Tree1, createNode(element[i]));
                }
                break;
            case 2:
                cout << "Output:";
                inorder(Tree1);
                cout << endl;
                break;
            case 3:
                cout << "Output:";
                Preorder(Tree1);
                cout << endl;
                break;
            case 4:
                cout << "Output:";
                postOrder(Tree1);
                cout << endl;
                break;


        }

    } while (choice != 0);
    freeTree(Tree1);
    return 0;
}

//____________________________Tree Functions _____________________________
void expTreeIni(BTREE *btree) {
    BTREE tree = *btree;
    tree->left = nullptr;
    tree->right = nullptr;
    tree->data = 'E';
    tree->Type = 0;

}

Tnode *createNode(ETYPE input) {
    Tnode *newnode = (Tnode *) malloc(sizeof(Tnode));
    if (newnode == nullptr) {
        cout << "No Memory available";
        return nullptr;

    }

    newnode->data = input;
    newnode->left = nullptr;
    newnode->right = nullptr;
    if (isdigit(input))
        newnode->Type = 0;
    else if (input == '*' || input == '/' || input == '+' || input == '-')
        newnode->Type = 1;
    else if (input == '(') {
        newnode->Type = 2;
        newnode->data = 'E';//Empty for adding an operator
    } else if (input == ')') {
        newnode->Type = 3;
        newnode->data = 'E';//Empty for adding an operator
    } else {
        cout << "Invalid Input";
        return nullptr;
    }
    return newnode;


}

Tnode *insert(BTREE node, Tnode *input) {
    if (node == nullptr) {
        node = input;
        return node;
    }
    if (input->Type == 0)//check put in the empty node left then right
    {
        if (node->left == nullptr) {
            node->left = insert(node->left, input);
        } else {
            node->right = insert(node->right, input);
            if (!isEmpty(priorityStack)) {
                pop(&priorityStack, &node);
            }

        }
    } else if (input->Type == 1)//set a new parent
    {
        if (node->data ==
            'E')//check if the parent is empty put the operator in it !!(for brackets case or the first node)
        {
            node->data = input->data;
            node->Type = 1;
        } else {
            if (priority(node) < priority(input)) {
                input->left = node->right;
                node->right = input;
                push(&priorityStack, node);
                node = input;
            } else {
                input->left = node;
                node = input;
            }


        }

    } else //brackets case
    {
        if (input->Type == 2) {
            //first making this node as new parent
            expTreeIni(&input);


            push(&stack, node);
            node = input;

        } else if (input->Type == 3) {
            Tnode *parent;
            pop(&stack, &parent);
            //assigning it to this node on it's left or right
            if (parent->left == nullptr)
                parent->left = insert(parent->left, node);
            else
                parent->right = insert(parent->right, node);
            // push the parent to stack
            node = parent;

        }


    }
    return node;
}

void showNode(Tnode *node) {
    if (node == nullptr)
        return;
    else
        cout << node->data;

}

void inorder(BTREE tree) {

    if (tree != nullptr) {
        inorder(tree->left);
        showNode(tree);
        inorder(tree->right);
    }

}

void Preorder(BTREE tree) {
    if (tree != nullptr) {
        showNode(tree);
        Preorder(tree->left);
        Preorder(tree->right);
    }
}

void postOrder(BTREE tree) {
    if (tree != nullptr) {
        postOrder(tree->left);
        postOrder(tree->right);
        showNode(tree);
    }

}

int priority(Tnode *node) {
    if (node->Type != 1) {
        cout << "Error at priority Function\n";
        return -1;
    }

    if (node->data == '*' || node->data == '/')
        return 2;
    else if (node->data == '+' || node->data == '-')
        return 1;

    else {
        cout << "Error at priority Function\n";
        return -1;
    }


}

void freeTree(BTREE tree) {
    if (tree != nullptr) {
        freeTree(tree->left);
        freeTree(tree->right);
        free(tree);
    }

}

//____________________________Stack Functions_______________________________
Node *stackNode(STYPE element) {
    Node *ptr = (Node *) malloc(sizeof(Node));
    if (ptr == nullptr) {
        return nullptr;
    }
    ptr->data = element;
    ptr->next = nullptr;
    return ptr;
}

void stackIni(Stack *stack) {
    stack->Top = nullptr;
}

int isEmpty(Stack stack) {
    if (stack.Top == nullptr)
        return 1;
    return 0;
}

int push(Stack *stack, STYPE element) {
    Node *newNode = stackNode(element);
    if (newNode == nullptr) {
        cout << "Insuficent Memory";
        return 0;
    }
    newNode->next = stack->Top;
    stack->Top = newNode;
    return 1;
}

int pop(Stack *stack, STYPE *element) {
    if (isEmpty(*stack)) {
        return 0;
    }
    *element = stack->Top->data;
    if (isLast(*stack)) {
        free(stack->Top);
        stack->Top = nullptr;
    } else {
        Node *ptr = stack->Top;
        stack->Top = stack->Top->next;
        free(ptr);
    }
    return 1;
}

int isLast(Stack stack) {
    if (stack.Top->next == nullptr)
        return 1;
    return 0;

}

//___________Strlen__________
int Strlen(string x) {
    int i=0;
    for (; x[i] != '\0'; i++) {
    }
    return i;
}