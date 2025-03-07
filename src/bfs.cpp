#include <iostream>

struct MazeSquare {
    int i, j;
    MazeSquare* northSquare = nullptr;
    MazeSquare* southSquare = nullptr;
    MazeSquare* westSquare = nullptr;
    MazeSquare* eastSquare = nullptr;
};

struct QueueEntry {
    MazeSquare* square;
    MazeSquare* path[20]; // Path with a maximum length of 20
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

// Return type is now a pointer to the raw array of path and the length of the path.
MazeSquare** bfsPathFinder(MazeSquare* start, MazeSquare* goal, int& pathLength) {
    static MazeSquare* resultPath[20];  // Static array to store the path, max length 20
    pathLength = 0;
    
    Queue q;
    bool visited[144] = {false}; // To keep track of visited squares
    
    // Initialize the queue with the start square and an empty path
    QueueEntry initial = {start, {}, 0};
    initial.path[0] = start;
    initial.pathLength = 1;
    q.push(initial);
    
    while (!q.isEmpty()) {
        QueueEntry entry = q.pop();
        MazeSquare* square = entry.square;
        
        // If we reach the goal, store the path and return
        if (square == goal) {
            for (int i = 0; i < entry.pathLength; i++) {
                resultPath[i] = entry.path[i];
            }
            pathLength = entry.pathLength;
            return resultPath;  // Return the path
        }
        
        // Get the index in the visited array based on the row and column
        int index = square->i * 12 + square->j;
        if (!visited[index]) {
            visited[index] = true;

            // Explore each of the four directions (north, south, west, east)
            // Check if the square in each direction exists and is not visited

            // Check north (if exists and not visited)
            if (square->northSquare && !visited[square->northSquare->i * 12 + square->northSquare->j] && entry.pathLength < 20) {
                QueueEntry nextEntry = {square->northSquare, {}, entry.pathLength};
                for (int i = 0; i < entry.pathLength; i++)
                    nextEntry.path[i] = entry.path[i];
                nextEntry.path[nextEntry.pathLength++] = square->northSquare;
                q.push(nextEntry);
            }

            // Check south (if exists and not visited)
            if (square->southSquare && !visited[square->southSquare->i * 12 + square->southSquare->j] && entry.pathLength < 20) {
                QueueEntry nextEntry = {square->southSquare, {}, entry.pathLength};
                for (int i = 0; i < entry.pathLength; i++)
                    nextEntry.path[i] = entry.path[i];
                nextEntry.path[nextEntry.pathLength++] = square->southSquare;
                q.push(nextEntry);
            }

            // Check west (if exists and not visited)
            if (square->westSquare && !visited[square->westSquare->i * 12 + square->westSquare->j] && entry.pathLength < 20) {
                QueueEntry nextEntry = {square->westSquare, {}, entry.pathLength};
                for (int i = 0; i < entry.pathLength; i++)
                    nextEntry.path[i] = entry.path[i];
                nextEntry.path[nextEntry.pathLength++] = square->westSquare;
                q.push(nextEntry);
            }

            // Check east (if exists and not visited)
            if (square->eastSquare && !visited[square->eastSquare->i * 12 + square->eastSquare->j] && entry.pathLength < 20) {
                QueueEntry nextEntry = {square->eastSquare, {}, entry.pathLength};
                for (int i = 0; i < entry.pathLength; i++)
                    nextEntry.path[i] = entry.path[i];
                nextEntry.path[nextEntry.pathLength++] = square->eastSquare;
                q.push(nextEntry);
            }
        }
    }

    // If no path is found, return nullptr
    pathLength = 0;
    return nullptr;
}
