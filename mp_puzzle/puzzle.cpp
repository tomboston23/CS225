/**
 * @file puzzle.cpp
 * Implementation of puzzle class.
 */
#include "puzzle.h"

#include "puzzle.h"
#include <unordered_set>

// Constructor
PuzzleState::PuzzleState() {
    for (uint8_t i = 0; i < 16; ++i) {
        positions[i] = (i+1)%16;
    }
}

PuzzleState::PuzzleState(const std::array<uint8_t, 16> state) : positions(state) {}

std::array<uint8_t, 16> PuzzleState::asArray() const {
    return positions;
}

bool PuzzleState::operator==(const PuzzleState &rhs) const {
    return positions == rhs.positions;
}

bool PuzzleState::operator!=(const PuzzleState &rhs) const {
    return positions != rhs.positions;
}

bool PuzzleState::operator<(const PuzzleState &rhs) const {
    return positions < rhs.positions;
}

PuzzleState PuzzleState::getNeighbor(Direction direction) const {
    PuzzleState neighborState = *this;
    std::array<uint8_t, 16> zeros = {0};
    PuzzleState zero = PuzzleState(zeros);
    int emptyIndex = 0;
    for (int i =0; i < (int)positions.size(); i++){
        if(positions[i] == 0){
            emptyIndex = i;
            break;
        }       
    }
    int row = emptyIndex/4;
    int col = emptyIndex%4;
    int neighborIndex;


    switch (direction) {
        case Direction::UP:
            if(row == 3) return zero;
            neighborIndex = emptyIndex + 4;
            break;
        case Direction::DOWN:
            if(row == 0) return zero;
            neighborIndex = emptyIndex - 4;
            break;
        case Direction::LEFT:
            if(col == 3) return zero;
            neighborIndex = emptyIndex + 1;
            break;
        case Direction::RIGHT:
            if(col == 0) return zero;
            neighborIndex = emptyIndex - 1;
            break;
    }

    if (neighborIndex >= 0 && neighborIndex < 16) {
        std::swap(neighborState.positions[emptyIndex], neighborState.positions[neighborIndex]);
    }

    return neighborState;
}

std::vector<PuzzleState> PuzzleState::getNeighbors() const {
    std::vector<PuzzleState> neighbors;
    std::array<uint8_t, 16> zeros = {0};
    PuzzleState zero = PuzzleState(zeros);

    for (int i = 0; i < 4; ++i) {
        PuzzleState neighborState = getNeighbor(static_cast<Direction>(i));
        if(neighborState == zero){
            continue;
        }
        neighbors.push_back(neighborState);
    }

    return neighbors;
}

int PuzzleState::manhattanDistance(const PuzzleState desiredState) const {
    int distance = 0;


    for (int i = 0; i < 16; ++i) {
        int num = positions[i];
        if(num == 0) continue;
        int desiredIndex;
        for(int j = 0; j < 16; ++j){
            if(desiredState.positions[j] == num) desiredIndex = j;
        }
        distance += abs(i / 4 - desiredIndex / 4) + abs(i % 4 - desiredIndex % 4);
    }
    return distance;
}

// std::vector<PuzzleState> solveAstar(const PuzzleState& startState, const PuzzleState &desiredState, size_t *iterations){
//     std::priority_queue<std::pair<int, PuzzleState>, std::vector<std::pair<int, PuzzleState>>, std::greater<std::pair<int, PuzzleState>>> vertices;
//     //this puts the PuzzleState with the lowest manhattan distance at the front of our queue
//     std::vector<PuzzleState> path;
//     std::map<PuzzleState, bool> visited;
//     std::map<PuzzleState, PuzzleState> parents;
//     //parents maps a puzzle state to the one that came before it
//     std::map<PuzzleState, int> distances; 
//     //this keeps track of the number of iterations that it took to get to a state
//     size_t len = 0;

//     vertices.push({startState.manhattanDistance(desiredState), startState});
//     visited[startState] = true;
//     while(!vertices.empty()){
//         PuzzleState curr = (vertices.top()).second;
//         vertices.pop();
//         len++;
//         if(curr == desiredState){
//             while(curr != startState){
//                 path.push_back(curr);
//                 curr = parents[curr];
//             } //push back our states in reverse order
//             path.push_back(startState);

//             //reverse the path
//             std::reverse(path.begin(), path.end());
//             break;
//         }
//         for(auto neighbor : curr.getNeighbors()){
//             if(visited[neighbor] == false){
//                 visited[neighbor] = true;
//                 parents[neighbor] = curr;
//                 //push our distance so we can utilize the shortest distance to desiredState
//                 vertices.push({neighbor.manhattanDistance(desiredState), neighbor});
//             }
//         }
        
//     }
//     if(iterations != nullptr) *iterations = len;
    
    

//     return path;
// }

std::vector<PuzzleState> solveAstar(const PuzzleState& startState, const PuzzleState& desiredState, size_t* iterations) {

    struct Node {
        PuzzleState state;
        Node* parent;
        int iterations; 
        int m_dist; 
        //use total to find the one closest to desired state while also not requiring many moves to reach
        int total() const { return iterations + m_dist; } 
    };

    auto cmp = [](const Node* node, const Node* other) { return (node->iterations + node->m_dist) > (other->iterations + other->m_dist); };
    //declare comparator to find the best puzzle state
    std::priority_queue<Node*, std::vector<Node*>, decltype(cmp)> queue(cmp);
    //put the "best" puzzle state at the front of the queue
    std::map<PuzzleState, Node*> nodes;

    //setup for start state
    Node* startNode = new Node{startState, nullptr, 0, startState.manhattanDistance(desiredState)};
    queue.push(startNode);
    nodes[startState] = startNode;
    if(iterations) *iterations = 0;


    while (!queue.empty()) {
        //get best node
        Node* current = queue.top();
        queue.pop();

        if(iterations) (*iterations)++;

        if (current->state == desiredState) {

            std::vector<PuzzleState> path;

            //this generates our path in reverse order
            for (Node* node = current; node->state != startState; node = node->parent) {
                path.push_back(node->state);
            }
            path.push_back(startState);

            //get path in correct order
            std::reverse(path.begin(), path.end());
            
            for(auto pair : nodes){
                delete pair.second; //delete the allocated node in each pair
            }

            return path;
        }

        for (PuzzleState neighbor : current->state.getNeighbors()) {
            int new_iterations = current->iterations + 1; 
            int dist = neighbor.manhattanDistance(desiredState);
            Node* child = new Node{neighbor, current, new_iterations, dist};

            auto it = nodes.find(neighbor);

            //check if it already has been searched
            if (it != nodes.end() && (it->second->iterations + it->second->m_dist) <= (child->iterations + child->m_dist)) {
                delete child;
                continue;
            }

            //if it doesn't exist or it's better than the one that's already there, insert it
            if (it == nodes.end() || (it->second->iterations + it->second->m_dist) > (child->iterations + child->m_dist)) {
                queue.push(child);
                //override the original if it exists
                nodes[neighbor] = child;
            } else {
                delete child; //shouldn't ever hit this? Just want to be sure
            }
        }
    }

    //case where you never reach desired state, meaning startState or desiredState are likely invalid
    for(auto pair : nodes){
        delete pair.second;
    }
    return {};
}

std::vector<PuzzleState> solveBFS(const PuzzleState &startState, const PuzzleState &desiredState, size_t *iterations){
    std::queue<PuzzleState> vertices;
    std::vector<PuzzleState> path;
    std::map<PuzzleState, bool> visited;
    std::map<PuzzleState, PuzzleState> parents;
    
    size_t len = 0;

    vertices.push(startState);
    visited[startState] = true;

    while(!vertices.empty()){
        PuzzleState curr = vertices.front();
        vertices.pop();
        len++;
        if(curr == desiredState){
            while(curr != startState){
                path.push_back(curr);
                curr = parents[curr];
            }
            path.push_back(startState);
            std::reverse(path.begin(), path.end());
            break;
        }
        for(auto neighbor : curr.getNeighbors()){
            if(visited[neighbor] == false){
                visited[neighbor] = true;
                parents[neighbor] = curr;
                vertices.push(neighbor);
            }
        }
    }
    if(iterations != nullptr) *iterations = len;

    return path;
}
