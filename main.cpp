#include "raylib.h"
#include <iostream>
#include <string>
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

int getLastFrameNumber() {
    if (head == NULL) return -1;

    Frame* temp = head;
    while (temp->next != NULL)
        temp = temp->next;

    return temp->frameNumber;
}



int main() {

    InitWindow(900, 600, "Video Frame Management System");
    SetTargetFPS(60);

    Rectangle addBtn     = {100, 100, 140, 40};
    Rectangle deleteBtn  = {260, 100, 140, 40};
    Rectangle undoBtn    = {420, 100, 140, 40};
    Rectangle processBtn = {580, 100, 140, 40};

    while (!WindowShouldClose()) {

    Vector2 mouse = GetMousePosition();

    // --- Button Click Detection ---
    if (CheckCollisionPointRec(mouse, addBtn) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        addFrame();
    }

    if (CheckCollisionPointRec(mouse, deleteBtn) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        int last = getLastFrameNumber();
        if (last != -1)
            deleteFrame(last);
        else
            cout << "No frames to delete\n";
            }

    if (CheckCollisionPointRec(mouse, undoBtn) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        undoDelete();
    }

    if (CheckCollisionPointRec(mouse, processBtn) &&
        IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        processFrames();
    }

    BeginDrawing();
    ClearBackground(RAYWHITE);

    // --- Title ---
    DrawText("Video Frame Management System", 250, 30, 30, BLACK);

    // --- Draw Buttons ---
    DrawRectangleRec(addBtn, LIGHTGRAY);
    DrawText("Add Frame", 115, 110, 18, BLACK);

    DrawRectangleRec(deleteBtn, LIGHTGRAY);
    DrawText("Delete", 290, 110, 18, BLACK);

    DrawRectangleRec(undoBtn, LIGHTGRAY);
    DrawText("Undo", 460, 110, 18, BLACK);

    DrawRectangleRec(processBtn, LIGHTGRAY);
    DrawText("Process", 605, 110, 18, BLACK);

    // ==============================
    // ✅ ADD FRAME DISPLAY HERE
    // ==============================

    Frame* temp = head;
    int y = 200;

    while (temp != NULL) {
        DrawText(
            TextFormat("Frame %d | Timestamp %d",
            temp->frameNumber,
            temp->timestamp),
            120, y, 20, DARKBLUE);

        y += 30;
        temp = temp->next;
    }

    EndDrawing();
}

    CloseWindow();
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


void deleteFrame(int id) {
    if (head == NULL) {
        cout << "No frames found to delete\n";
        return;
    }

    Frame* temp = head;
    Frame* prev = NULL;

    // If head is the frame to delete
    if (temp->frameNumber == id) {

        head = temp->next;

        // Push deleted frame data into stack
        push(*temp);

        delete temp;

        cout << "Frame " << id << " deleted\n";
        return;
    }

    // Search for frame
    while (temp != NULL && temp->frameNumber != id) {
        prev = temp;
        temp = temp->next;
    }

    // If not found
    if (temp == NULL) {
        cout << "Frame not found\n";
        return;
    }

    // Remove from list
    prev->next = temp->next;

    // Push deleted frame data into stack
    push(*temp);

    delete temp;

    cout << "Frame " << id << " deleted\n";
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