#ifndef BFS_H
#define BFS_H

#include <iostream>
#include "gladiator.h"

struct QueueEntry {
    MazeSquare* square;
    MazeSquare* path[20];
    int pathLength;
};

struct Queue {
    QueueEntry data[144];
    int front, rear;
    Queue() : front(0), rear(0) {}
    bool isEmpty() { return front == rear; }
    void push(QueueEntry entry) { data[rear++] = entry; }
    QueueEntry pop() { return data[front++]; }
    int size() { return rear - front; }
};

MazeSquare** bfsPathFinder(MazeSquare* start, MazeSquare* goal, int& pathLength);

#endif