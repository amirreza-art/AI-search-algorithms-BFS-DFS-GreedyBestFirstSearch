// graph search for DFS and BFS AND Greedy Best First Search
#include <iostream>
#include <stack>
#include <vector>
#include <queue>

using namespace std;
typedef vector<vector<int>> Matrix;

void printMatrix(const Matrix &);
Matrix upAction(const Matrix &, pair<int, int>);
Matrix rightAction(const Matrix &, pair<int, int>);
Matrix downAction(const Matrix &, pair<int, int>);
Matrix leftAction(const Matrix &, pair<int, int>);
bool isInExplored(const vector<Matrix> &, const Matrix &);
pair<int, int> getIndex(const Matrix &, int);
int evaluation(const Matrix &, const Matrix &);
void sortByEvaluation(vector<Matrix> &, const Matrix &);
void DFS(stack<Matrix> &, const Matrix &);
void BFS(queue<Matrix> &, const Matrix &);
void greedyBestFirst(stack<Matrix> &, const Matrix &);

int main()
{
    // initial and goal matrix
    Matrix initial_state {{8,1,3}, {7,2,4}, {0,6,5}};
    Matrix goal_state {{1,2,3}, {8,0,4}, {7,6,5}};

    cout << "initial state : " << endl;
    printMatrix(initial_state);
    cout << "\ngoal state : " << endl;
    printMatrix(goal_state);

    cout << "\nselect the algorithm you want :\n"
             << "1 for BFS\n"
             << "2 for DFS\n"
             << "3 for Greedy Best First Search\n"
             << "4 for exit\n"; 
    unsigned int choice;
    while (true)
    {
        cout << "\nEnter your choice : ";
        cin >> choice;
        if (choice == 4)
        {
            break;
        }
        
        if (choice == 1) // BFS
        {
            queue<Matrix> frontier;
            frontier.push(initial_state);
            BFS(frontier, goal_state);
        }

        if (choice == 2) // DFS
        {
            stack<Matrix> frontier;
            frontier.push(initial_state);
            DFS(frontier, goal_state);
        }

        if (choice == 3) // Greedy Best First Search
        {
            stack<Matrix> frontier;    
            frontier.push(initial_state);
            greedyBestFirst(frontier, goal_state);
        }
    }
    return 0;
}

// display matrix
void printMatrix(const Matrix &state)
{
    for (auto rows : state)
    {
        for (auto columns : rows)
        {
            cout << columns << "\t";
        }
        cout << endl;
    }
    cout << "--------------" << endl;
}

// Actions, if the action was not applicable return input state
//----------------------------------------------------------------
Matrix upAction(const Matrix &state, pair<int, int> index)
{
    if (index.first == 0)
    {
        return state;
    }

    Matrix child(state);
    int row = index.first;
    int column = index.second;
    int temp = state[row-1][column];
    child[row-1][column] = 0;
    child[row][column] = temp;
    return child;
}

Matrix rightAction(const Matrix &state, pair<int, int> index)
{
    if (index.second == 2)
    {
        return state;
    }

    Matrix child(state);
    int row = index.first;
    int column = index.second;
    int temp = state[row][column+1];
    child[row][column+1] = 0;
    child[row][column] = temp;
    return child;  
}

Matrix downAction(const Matrix &state, pair<int, int> index)
{
    if (index.first == 2)
    {
        return state;
    }

    Matrix child(state);
    int row = index.first;
    int column = index.second;
    int temp = state[row+1][column];
    child[row+1][column] = 0;
    child[row][column] = temp;
    return child;     
}

Matrix leftAction(const Matrix &state, pair<int, int> index)
{
    if (index.second == 0)
    {
        return state;
    }
    
    Matrix child(state);
    int row = index.first;
    int column = index.second;
    int temp = state[row][column-1];
    child[row][column-1] = 0;
    child[row][column] = temp;
    return child;  
}
//---------------------------------------------------------------

// checks if a matrix is in the explored list
bool isInExplored(const vector<Matrix> &DB, const Matrix &state)
{
    for (auto item : DB)
    {
        if (item == state)
        {
            return true;
        }
    }
    return false;
}

// return index of an element in matrix
pair<int, int> getIndex(const Matrix &state, int number)
{
    int row;
    int column;
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (state[i][j] == number)
            {
                row = i;
                column = j;
                break;
            }
        }
    }
    return make_pair(row, column);
}

// evaluation function for greedy best first
int evaluation(const Matrix &state, const Matrix &goal)
{
    unsigned int temp;
    int h = 0;
    pair<int, int> p1;
    pair<int, int> p2;
    for (int i = 1; i < 9; i++)
    {
        temp = 0;
        p1 = getIndex(state, i);
        p2 = getIndex(goal, i);
        temp = abs(p1.first-p2.first) + abs(p1.second-p2.second);
        h = h + temp;
    }
    return h;
}

// sort chile of an state by evaluation function
void sortByEvaluation(vector<Matrix> &DB, const Matrix &goal)
{
    for (int i = 0; i < DB.size(); i++)
    {
        for (int j = DB.size() - 1; j > i; j--)
        {
            if (evaluation(DB.at(j), goal) < evaluation(DB.at(j-1),goal))
            {
                swap(DB.at(j), DB.at(j-1));
            }
        }
    }
}

// Algorithms
//------------------------------------------------------
// Breadth First Search
void BFS(queue<Matrix> &frontier, const Matrix &goal)
{
    int count = 0; // count of actions
    vector<Matrix> explored;
    while (true)
    {
        if (frontier.empty())
        {
            break;
        }
        // pop the state and push in explored
        auto now = frontier.front();
        explored.push_back(now);
        frontier.pop();
        // goal test
        if (now == goal)
        {
            cout << "Found" <<endl;
            printMatrix(now);
            break;
        }
        // index of zero
        pair<int, int> mypair = getIndex(now, 0);

        Matrix child;
        // first in first out, for this reason up action first of all it applies
        // expand child if action is applicable
        
        child = upAction(now, mypair);
        if (!(isInExplored(explored, child)))
        {
            frontier.push(child);
        }

        child = rightAction(now, mypair);
        if (!(isInExplored(explored, child)))
        {
            frontier.push(child);
        }

        child = downAction(now, mypair);
        if (!(isInExplored(explored, child)))
        {
            frontier.push(child);
        }

        child = leftAction(now, mypair);
        if (!(isInExplored(explored, child)))
        {
            frontier.push(child);
        }
        
        count++;
    }
    cout << "count of actions : " << count << endl;
}
// Depth First Search
void DFS(stack<Matrix> &frontier, const Matrix &goal)
{
    int count = 0; // count of actions
    vector<Matrix> explored;
    while (true)
    {
        if (frontier.empty())
        {
            break;
        }
        // pop the state and push in explored
        auto now = frontier.top();
        explored.push_back(now);
        frontier.pop();
        // goal test
        if (now == goal)
        {
            cout << "Found" << endl;
            printMatrix(now);
            break;
        }
        // find index of zero 
        pair<int, int> mypair = getIndex(now, 0);
        
        Matrix child;
        // last in first out, for this reason up action last of all it applies
        // expand if action is applicable

        child = leftAction(now, mypair);
        if (!(isInExplored(explored, child)))
        {
            frontier.push(child);
        }

        child = downAction(now, mypair);
        if (!(isInExplored(explored, child)))
        {
            frontier.push(child);
        }
        child = rightAction(now, mypair);
        if (!(isInExplored(explored, child)))
        {
            frontier.push(child);
        }
        child = upAction(now, mypair);
        if (!(isInExplored(explored, child)))
        {
            frontier.push(child);
        }

        count++;
    }
    cout << "count of actions : " << count << endl;
}


void greedyBestFirst(stack<Matrix> &frontier, const Matrix &goal)
{
    int count = 0;
    vector<Matrix> explored;
    while (true)
    {
        if (frontier.empty())
        {
            break;
        }
        
        auto now = frontier.top();
        explored.push_back(now);
        frontier.pop();
        if (now == goal)
        {
            cout << "Found" << endl;
            printMatrix(now);
            break;
        }
    
        pair<int, int> mypair = getIndex(now, 0);
        Matrix child;

        vector<Matrix> temp;

        child = leftAction(now, mypair);
        if (!(isInExplored(explored, child)))
        {
            temp.push_back(child);
        }
        child = downAction(now, mypair);
        if (!(isInExplored(explored, child)))
        {
            temp.push_back(child);
        }
        child = rightAction(now, mypair);
        if (!(isInExplored(explored, child)))
        {
            temp.push_back(child);
        }
        child = upAction(now, mypair);
        if (!(isInExplored(explored, child)))
        {
            temp.push_back(child);
        }

        sortByEvaluation(temp, goal);
        
        while (!temp.empty())
        {
            frontier.push(temp.back());
            temp.pop_back();
        }
        
        count++;
    }
    cout << "count of actions : " << count << endl;
}
// --------------------------------------------------------------
