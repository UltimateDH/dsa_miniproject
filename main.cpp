#include <iostream>
using namespace std;

// ===== FRAME STRUCTURE (Linked List Node) =====
struct Frame {
    int frameNumber;
    int timestamp;
    Frame* next;
};

// Global head pointer
Frame* head = NULL;
int frameCounter = 0;

// ===== STACK NODE =====
struct StackNode {
    Frame data;
    StackNode* next;
};

StackNode* top = NULL;

// ===== QUEUE NODE =====
struct QueueNode {
    Frame data;
    QueueNode* next;
};

QueueNode* front = NULL;
QueueNode* rear = NULL;

// Function declarations
void addFrame();
void displayFrames();
void deleteFrame(int frameNumber);
void undoDelete();
void processFrames();
void enqueue(Frame frame); 

void push(Frame deletedFrame);
Frame pop();
bool isStackEmpty();

int main() {

    int choice;

    while (true) {
       cout << "\n1. Add Frame\n";
       cout << "2. Display Frames\n";
       cout << "3. Delete Frame\n";
       cout << "4. Undo Delete\n";
       cout << "5. Process Frames\n";
       cout << "6. Exit\n";
       cout << "Enter choice: ";
       cin >> choice;

        switch (choice) {
            case 1:
                addFrame();
                break;
            case 2:
                displayFrames();
                break;
            case 3: {
                    int num;
                    cout << "Enter frame number to delete: ";
                    cin >> num;
                    deleteFrame(num);
                break;
            }
            case 4:
                undoDelete();
            break;
            case 5:
                processFrames();
            break;
            case 6:
                return 0;
            default:
                cout << "Invalid choice!\n";
        }
    }

    return 0;
}

void addFrame() {
    Frame* newFrame = new Frame;

    frameCounter++;
    newFrame->frameNumber = frameCounter;
    newFrame->timestamp = frameCounter * 2; // simulated timestamp
    newFrame->next = NULL;

    if (head == NULL) {
        head = newFrame;
    } else {
        Frame* temp = head;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newFrame;
    }

    cout << "Frame " << newFrame->frameNumber << " added.\n";
}

void displayFrames() {
    if (head == NULL) {
        cout << "No frames available.\n";
        return;
    }

    Frame* temp = head;

    cout << "\nFrame List:\n";
    while (temp != NULL) {
        cout << "Frame " << temp->frameNumber
             << " | Timestamp: " << temp->timestamp << endl;
        temp = temp->next;
    }
}

void push(Frame deletedFrame) {
    StackNode* newNode = new StackNode;
    newNode->data = deletedFrame;
    newNode->next = top;
    top = newNode;
}

bool isStackEmpty() {
    return (top == NULL);
}

Frame pop() {
    if (isStackEmpty()) {
        cout << "Nothing to undo!\n";
        Frame emptyFrame;
        emptyFrame.frameNumber = -1;
        return emptyFrame;
    }

    StackNode* temp = top;
    Frame poppedData = temp->data;
    top = top->next;
    delete temp;

    return poppedData;
}


void deleteFrame(int frameNumber) {
    if (head == NULL) {
        cout << "No frames to delete.\n";
        return;
    }

    Frame* temp = head;
    Frame* prev = NULL;

    while (temp != NULL && temp->frameNumber != frameNumber) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        cout << "Frame not found.\n";
        return;
    }

    // Push deleted frame into stack
    push(*temp);

    if (prev == NULL) {
        head = temp->next;
    } else {
        prev->next = temp->next;
    }

    delete temp;

    cout << "Frame " << frameNumber << " deleted.\n";
}


void undoDelete() {
    Frame restored = pop();

    if (restored.frameNumber == -1)
        return;

    Frame* newFrame = new Frame;
    *newFrame = restored;
    newFrame->next = NULL;

    // Sorted insertion by frameNumber
    if (head == NULL || head->frameNumber > newFrame->frameNumber) {
        newFrame->next = head;
        head = newFrame;
        cout << "Undo successful. Frame "
             << restored.frameNumber << " restored.\n";
        return;
    }

    Frame* temp = head;

    while (temp->next != NULL &&
           temp->next->frameNumber < newFrame->frameNumber) {
        temp = temp->next;
    }

    newFrame->next = temp->next;
    temp->next = newFrame;

    cout << "Undo successful. Frame "
         << restored.frameNumber << " restored.\n";
}

void enqueue(Frame frame) {
    QueueNode* newNode = new QueueNode;
    newNode->data = frame;
    newNode->next = NULL;

    if (rear == NULL) {
        front = rear = newNode;
        return;
    }

    rear->next = newNode;
    rear = newNode;
}

Frame dequeue() {
    Frame emptyFrame;
    emptyFrame.frameNumber = -1;

    if (front == NULL) {
        cout << "Queue empty!\n";
        return emptyFrame;
    }

    QueueNode* temp = front;
    Frame data = temp->data;

    front = front->next;

    if (front == NULL)
        rear = NULL;

    delete temp;

    return data;
}

void processFrames() {
    Frame* temp = head;

    while (temp != NULL) {
        enqueue(*temp);
        temp = temp->next;
    }

    cout << "\nProcessing Frames:\n";

    while (front != NULL) {
        Frame processed = dequeue();

        if (processed.frameNumber != -1) {
            cout << "Processing Frame "
                 << processed.frameNumber << endl;
        }
    }
}