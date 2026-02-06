#include <iostream>
#include <vector>
using namespace std;

int nProcess, nResource;

vector<vector<int>> alloc;
vector<vector<int>> maxNeed;
vector<vector<int>> need;
vector<int> available;

// calculate need matrix
void calcNeed() {
    need.assign(nProcess, vector<int>(nResource));

    for (int i = 0; i < nProcess; i++)
        for (int j = 0; j < nResource; j++)
            need[i][j] = maxNeed[i][j] - alloc[i][j];
}

// display matrix
void showMatrix(vector<vector<int>> &mat, string name) {
    cout << "\n" << name << " Matrix\n";

    for (int i = 0; i < nProcess; i++) {
        cout << "P" << i << ": ";
        for (int j = 0; j < nResource; j++)
            cout << mat[i][j] << " ";
        cout << endl;
    }
}

// safe state check
bool checkSafe() {
    vector<int> work = available;
    vector<bool> done(nProcess, false);
    vector<int> safeSeq;

    int completed = 0;

    while (completed < nProcess) {
        bool found = false;

        for (int i = 0; i < nProcess; i++) {
            if (!done[i]) {
                bool possible = true;

                for (int j = 0; j < nResource; j++) {
                    if (need[i][j] > work[j]) {
                        possible = false;
                        break;
                    }
                }

                if (possible) {
                    for (int j = 0; j < nResource; j++)
                        work[j] += alloc[i][j];

                    safeSeq.push_back(i);
                    done[i] = true;
                    completed++;
                    found = true;
                }
            }
        }

        if (!found)
            break;
    }

    if (completed != nProcess) {
        cout << "\nSystem is NOT SAFE.\n";
        return false;
    }

    cout << "\nSystem is SAFE.\nSafe sequence: ";
    for (int x : safeSeq)
        cout << "P" << x << " ";
    cout << endl;

    return true;
}

// resource request
void requestResource() {
    int pid;
    cout << "Enter process number: ";
    cin >> pid;

    vector<int> req(nResource);
    cout << "Enter resource request: ";

    for (int j = 0; j < nResource; j++)
        cin >> req[j];

    // check validity
    for (int j = 0; j < nResource; j++) {
        if (req[j] > need[pid][j]) {
            cout << "Request exceeds process need.\n";
            return;
        }
        if (req[j] > available[j]) {
            cout << "Resources not available.\n";
            return;
        }
    }

    // temporary allocation
    for (int j = 0; j < nResource; j++) {
        available[j] -= req[j];
        alloc[pid][j] += req[j];
        need[pid][j] -= req[j];
    }

    if (!checkSafe()) {
        cout << "Request denied (unsafe). Rolling back.\n";

        for (int j = 0; j < nResource; j++) {
            available[j] += req[j];
            alloc[pid][j] -= req[j];
            need[pid][j] += req[j];
        }
    } else {
        cout << "Request granted.\n";
    }
}

int main() {
    cout << "Enter number of processes and resources: ";
    cin >> nProcess >> nResource;

    alloc.assign(nProcess, vector<int>(nResource));
    maxNeed.assign(nProcess, vector<int>(nResource));
    available.assign(nResource, 0);

    cout << "\nEnter allocation matrix:\n";
    for (auto &row : alloc)
        for (int &x : row)
            cin >> x;

    cout << "\nEnter max requirement matrix:\n";
    for (auto &row : maxNeed)
        for (int &x : row)
            cin >> x;

    cout << "\nEnter available resources:\n";
    for (int &x : available)
        cin >> x;

    calcNeed();

    int choice;

    do {
        cout << "\n1. Show matrices";
        cout << "\n2. Check safe state";
        cout << "\n3. Request resources";
        cout << "\n4. Exit";
        cout << "\nChoice: ";
        cin >> choice;

        if (choice == 1) {
            showMatrix(alloc, "Allocation");
            showMatrix(maxNeed, "Max Need");
            showMatrix(need, "Need");
        }
        else if (choice == 2)
            checkSafe();
        else if (choice == 3)
            requestResource();

    } while (choice != 4);

    return 0;
}
