#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class TrafficSignal {
private:
    int signalID;
    string location;
    int trafficDensity; // 0-100% congestion level
    int signalTiming; // Green light duration in seconds
    bool isCongested; // True if congestion level > 80%

public:
    TrafficSignal(int id, string loc, int density, int timing) {
        signalID = id;
        location = loc;
        trafficDensity = density;
        signalTiming = timing;
        isCongested = (trafficDensity > 80);
    }

    int getSignalID() const { return signalID; }
    string getLocation() const { return location; }
    int getTrafficDensity() const { return trafficDensity; }
    int getSignalTiming() const { return signalTiming; }
    bool getIsCongested() const { return isCongested; }

    void updateTrafficDensity(int density) {
        trafficDensity = density;
        isCongested = (trafficDensity > 80);
    }

    void updateSignalTiming(int timing) {
        signalTiming = timing;
    }

    void displaySignal() const {
        cout << "Signal ID: " << signalID << " | Location: " << location
             << " | Traffic Density: " << trafficDensity << "%"
             << " | Green Light: " << signalTiming << "s"
             << " | Status: " << (isCongested ? "Congested ??" : "Normal ?") << endl;
    }

    void saveToFile(ofstream &outfile) const {
        outfile << signalID << "," << location << "," << trafficDensity << "," << signalTiming << "," << isCongested << endl;
    }

    static TrafficSignal loadFromFile(ifstream &infile) {
        int id, density, timing;
        string location;
        bool congested;
        infile >> id;
        infile.ignore();
        getline(infile, location, ',');
        infile >> density >> timing >> congested;
        return TrafficSignal(id, location, density, timing);
    }
};

// Function to check if Signal ID is unique
bool isSignalIDUnique(vector<TrafficSignal> &signals, int id) {
    for (const auto &s : signals) {
        if (s.getSignalID() == id) {
            return false;
        }
    }
    return true;
}

// Function to find a signal by ID
TrafficSignal* findSignal(vector<TrafficSignal> &signals, int id) {
    for (auto &s : signals) {
        if (s.getSignalID() == id) {
            return &s;
        }
    }
    return nullptr;
}

// Save traffic signals to file
void saveSignalsToFile(vector<TrafficSignal> &signals) {
    ofstream outfile("traffic_signals.txt", ios::trunc);
    for (const auto &s : signals) {
        s.saveToFile(outfile);
    }
    outfile.close();
}

// Load traffic signals from file
void loadSignalsFromFile(vector<TrafficSignal> &signals) {
    ifstream infile("traffic_signals.txt");
    if (!infile) return;

    while (!infile.eof()) {
        int id, density, timing;
        string location;
        bool congested;

        infile >> id;
        infile.ignore();
        if (getline(infile, location, ',') && infile >> density >> timing >> congested) {
            signals.push_back(TrafficSignal(id, location, density, timing));
        }
    }
    infile.close();
}

// Function to delete a traffic signal record
void deleteSignal(vector<TrafficSignal> &signals, int id) {
    for (auto it = signals.begin(); it != signals.end(); ++it) {
        if (it->getSignalID() == id) {
            signals.erase(it);
            cout << "Traffic signal deleted successfully!\n";
            saveSignalsToFile(signals);
            return;
        }
    }
    cout << "Traffic signal not found!\n";
}

int main() {
    vector<TrafficSignal> signals;
    loadSignalsFromFile(signals);

    int choice;
    while (true) {
        cout << "\n===== Smart Traffic Management System =====\n";
        cout << "1. Register a Traffic Signal\n";
        cout << "2. View All Signals\n";
        cout << "3. Update Traffic Density\n";
        cout << "4. Update Signal Timing\n";
        cout << "5. Delete a Traffic Signal\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int id, density, timing;
            string location;
            cout << "Enter Signal ID: ";
            cin >> id;
            cin.ignore();

            if (!isSignalIDUnique(signals, id)) {
                cout << "Error! Signal ID already exists.\n";
                continue;
            }

            cout << "Enter Location: ";
            getline(cin, location);
            cout << "Enter Initial Traffic Density (0-100%): ";
            cin >> density;
            cout << "Enter Green Light Duration (in seconds): ";
            cin >> timing;

            signals.push_back(TrafficSignal(id, location, density, timing));
            saveSignalsToFile(signals);
            cout << "Traffic Signal registered successfully!\n";

        } else if (choice == 2) {
            cout << "\n===== Traffic Signals List =====\n";
            for (const auto &s : signals) {
                s.displaySignal();
            }

        } else if (choice == 3) {
            int id, density;
            cout << "Enter Signal ID to update density: ";
            cin >> id;
            TrafficSignal* signal = findSignal(signals, id);

            if (signal) {
                cout << "Enter New Traffic Density (0-100%): ";
                cin >> density;
                signal->updateTrafficDensity(density);
                saveSignalsToFile(signals);
                cout << "Traffic density updated successfully!\n";
            } else {
                cout << "Traffic signal not found!\n";
            }

        } else if (choice == 4) {
            int id, timing;
            cout << "Enter Signal ID to update timing: ";
            cin >> id;
            TrafficSignal* signal = findSignal(signals, id);

            if (signal) {
                cout << "Enter New Green Light Duration (in seconds): ";
                cin >> timing;
                signal->updateSignalTiming(timing);
                saveSignalsToFile(signals);
                cout << "Signal timing updated successfully!\n";
            } else {
                cout << "Traffic signal not found!\n";
            }

        } else if (choice == 5) {
            int id;
            cout << "Enter Signal ID to delete: ";
            cin >> id;
            deleteSignal(signals, id);

        } else if (choice == 6) {
            cout << "Exiting Smart Traffic Management System. Thank you!\n";
            break;
        } else {
            cout << "Invalid choice! Try again.\n";
        }
    }
    return 0;
}
