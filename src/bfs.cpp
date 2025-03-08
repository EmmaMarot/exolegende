#include "bfs.h"

MazeSquare** bfsPathFinder(MazeSquare* start, MazeSquare* goal, int& pathLength) {
    static MazeSquare* resultPath[20];  // Static array to store the path, max length 20
    pathLength = 0;
    
    Queue q;
    bool visited[144] = {false}; // To keep track of visited squares
    
    QueueEntry initial = {start, {}, 0};
    initial.path[0] = start;
    initial.pathLength = 1;
    q.push(initial);
    
    while (!q.isEmpty()) {
        QueueEntry entry = q.pop();
        MazeSquare* square = entry.square;
        
        if (square == goal) {
            for (int i = 0; i < entry.pathLength; i++) {
                resultPath[i] = entry.path[i];
            }
            pathLength = entry.pathLength;
            return resultPath;
        }
        
        int index = square->i * 12 + square->j;
        if (!visited[index]) {
            visited[index] = true;

            if (square->northSquare && !visited[square->northSquare->i * 12 + square->northSquare->j] && entry.pathLength < 20) {
                QueueEntry nextEntry = {square->northSquare, {}, entry.pathLength};
                for (int i = 0; i < entry.pathLength; i++)
                    nextEntry.path[i] = entry.path[i];
                nextEntry.path[nextEntry.pathLength++] = square->northSquare;
                q.push(nextEntry);
            }

            if (square->southSquare && !visited[square->southSquare->i * 12 + square->southSquare->j] && entry.pathLength < 20) {
                QueueEntry nextEntry = {square->southSquare, {}, entry.pathLength};
                for (int i = 0; i < entry.pathLength; i++)
                    nextEntry.path[i] = entry.path[i];
                nextEntry.path[nextEntry.pathLength++] = square->southSquare;
                q.push(nextEntry);
            }

            if (square->westSquare && !visited[square->westSquare->i * 12 + square->westSquare->j] && entry.pathLength < 20) {
                QueueEntry nextEntry = {square->westSquare, {}, entry.pathLength};
                for (int i = 0; i < entry.pathLength; i++)
                    nextEntry.path[i] = entry.path[i];
                nextEntry.path[nextEntry.pathLength++] = square->westSquare;
                q.push(nextEntry);
            }

            if (square->eastSquare && !visited[square->eastSquare->i * 12 + square->eastSquare->j] && entry.pathLength < 20) {
                QueueEntry nextEntry = {square->eastSquare, {}, entry.pathLength};
                for (int i = 0; i < entry.pathLength; i++)
                    nextEntry.path[i] = entry.path[i];
                nextEntry.path[nextEntry.pathLength++] = square->eastSquare;
                q.push(nextEntry);
            }
        }
    }

    pathLength = 0;
    return nullptr;
}