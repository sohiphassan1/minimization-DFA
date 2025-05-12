#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>

using namespace std;

struct DFA {
    int numStates;
    int startState;
    set<int> finalStates;
    vector<char> alphabet;
    map<pair<int, char>, int> transition;

    void readInput() {
        cout << "Enter number of states: ";
        cin >> numStates;

        cout << "Enter alphabet (e.g., ab): ";
        string alphaStr;
        cin >> alphaStr;
        alphabet.assign(alphaStr.begin(), alphaStr.end());

        cout << "Enter start state (0 to " << numStates - 1 << "): ";
        cin >> startState;

        cout << "Enter number of final states: ";
        int f;
        cin >> f;
        cout << "Enter final states: ";
        for (int i = 0; i < f; i++) {
            int fs;
            cin >> fs;
            finalStates.insert(fs);
        }

        cout << "Enter transitions (format: from char to):" << endl;
        for (int i = 0; i < numStates; i++) {
            for (char c : alphabet) {
                int to;
                cout << "δ(" << i << ", " << c << ") = ";
                cin >> to;
                transition[{i, c}] = to;
            }
        }
    }
};

int getGroup(const vector<set<int>>& partitions, int state) {
    for (int i = 0; i < partitions.size(); i++) {
        if (partitions[i].count(state)) return i;
    }
    return -1;
}

void minimizeDFA(const DFA& dfa) {
    vector<set<int>> partitions;

    // Step 1: Separate final and non-final
    set<int> final = dfa.finalStates;
    set<int> nonFinal;
    for (int i = 0; i < dfa.numStates; ++i) {
        if (!final.count(i)) nonFinal.insert(i);
    }

    if (!nonFinal.empty()) partitions.push_back(nonFinal);
    if (!final.empty()) partitions.push_back(final);

    bool updated;
    do {
        updated = false;
        vector<set<int>> newPartitions;

        for (auto& group : partitions) {
            map<vector<int>, set<int>> splitter;

            for (int state : group) {
                vector<int> key;

                for (char c : dfa.alphabet) {
                    int to = dfa.transition.at({state, c});
                    int toGroup = getGroup(partitions, to);
                    key.push_back(toGroup);
                }

                splitter[key].insert(state);
            }

            if (splitter.size() == 1) {
                newPartitions.push_back(group);
            } else {
                updated = true;
                for (auto& entry : splitter)
                    newPartitions.push_back(entry.second);
            }
        }

        partitions = newPartitions;

    } while (updated);

    // Output
    cout << "\nMinimized DFA:\n";
    cout << "States:\n";
    for (int i = 0; i < partitions.size(); i++) {
        cout << "P" << i << ": { ";
        for (int s : partitions[i]) cout << s << " ";
        cout << "}\n";
    }

    int startGroup = getGroup(partitions, dfa.startState);
    cout << "Start state: P" << startGroup << "\n";

    cout << "Final states: ";
    for (int i = 0; i < partitions.size(); i++) {
        for (int s : partitions[i]) {
            if (dfa.finalStates.count(s)) {
                cout << "P" << i << " ";
                break;
            }
        }
    }
    cout << "\n";

    cout << "Transitions:\n";
    for (int i = 0; i < partitions.size(); i++) {
        int rep = *partitions[i].begin(); // representative
        for (char c : dfa.alphabet) {
            int to = dfa.transition.at({rep, c});
            int toGroup = getGroup(partitions, to);
            cout << "P" << i << " --" << c << "--> P" << toGroup << "\n";
        }
    }
}

int main() {
    DFA dfa;
    dfa.readInput();
    minimizeDFA(dfa);
    return 0;
}
