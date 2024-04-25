#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Car {
    string brand;
    string type;
    string licensePlate;
    int serviceYears;
    struct {
        string color;
        double payload;
    } details;
};

void writeToFile(const string& filename, const vector<Car>& cars) {
    ofstream file(filename, ios::binary);
    if (!file) {
        cerr << "Failed to open file for writing.\n";
        return;
    }
    for (const auto& car : cars)
        file.write(reinterpret_cast<const char*>(&car), sizeof(Car));
    file.close();
}

void appendToFile(const string& filename, const Car& car) {
    ofstream file(filename, ios::binary | ios::app);
    if (!file) {
        cerr << "Failed to open file for appending.\n";
        return;
    }
    file.write(reinterpret_cast<const char*>(&car), sizeof(Car));
    file.close();
}

void replaceInFile(const string& filename, const Car& car, int index) {
    fstream file(filename, ios::binary | ios::in | ios::out);
    if (!file) {
        cerr << "Failed to open file for replacing.\n";
        return;
    }
    file.seekp(index * sizeof(Car));
    file.write(reinterpret_cast<const char*>(&car), sizeof(Car));
    file.close();
}

void deleteFromFile(const string& filename, int maxServiceYears) {
    vector<Car> cars;
    ifstream file(filename, ios::binary);
    if (!file) {
        cerr << "Failed to open file for reading.\n";
        return;
    }
    Car car;
    while (file.read(reinterpret_cast<char*>(&car), sizeof(Car)))
        if (car.serviceYears <= maxServiceYears)
            cars.push_back(car);
    file.close();

    ofstream outFile(filename, ios::binary);
    if (!outFile) {
        cerr << "Failed to open file for writing.\n";
        return;
    }
    for (const auto& c : cars)
        outFile.write(reinterpret_cast<const char*>(&c), sizeof(Car));
    outFile.close();
}

void findCarsByPayload(const vector<Car>& cars, double minPayload) {
    bool found = false;
    for (const auto& car : cars)
        if (car.type == "truck" && car.details.payload >= minPayload) {
            found = true;
            cout << "License Plate: " << car.licensePlate << ", Brand: " << car.brand << ", Payload: " << car.details.payload << " tons, Service Years: " << car.serviceYears << endl;
        }
    if (!found)
        cout << "No cars with payload greater than or equal to " << minPayload << " tons were found." << endl;
}

void findCarsByTypeAndColor(const vector<Car>& cars, const string& type, const string& color) {
    bool found = false;
    for (const auto& car : cars)
        if (car.type == type && (type == "car" ? car.details.color == color : false)) {
            found = true;
            cout << "License Plate: " << car.licensePlate << ", Brand: " << car.brand << ", Service Years: " << car.serviceYears << endl;
        }
    if (!found)
        cout << "No cars of specified type and color were found." << endl;
}

int main() {
    vector<Car> cars;

    while (true) {
        cout << "1. Add car to file\n";
        cout << "2. Replace data in file\n";
        cout << "3. Delete cars with service years more than five\n";
        cout << "4. Find cars with payload greater than or equal to specified payload\n";
        cout << "5. Find cars of specified type and color\n";
        cout << "6. Exit\n";
        cout << "Choose an option: ";

        int choice;
        cin >> choice;

        switch (choice) {
        case 1: {
            Car newCar;
            cout << "Enter data for the new car:" << endl;
            cout << "Brand: ";
            cin >> newCar.brand;
            cout << "Type (truck/car): ";
            cin >> newCar.type;
            cout << "License Plate: ";
            cin >> newCar.licensePlate;
            cout << "Service Years: ";
            cin >> newCar.serviceYears;
            if (newCar.type == "car") {
                cout << "Color: ";
                cin >> newCar.details.color;
            }
            else if (newCar.type == "truck") {
                cout << "Payload (tons): ";
                cin >> newCar.details.payload;
            }
            cars.push_back(newCar);
            appendToFile("cars.bin", newCar);
            break;
        }
        case 2: {
            int index;
            cout << "Enter the index of the car to replace: ";
            cin >> index;
            if (index >= 0 && index < cars.size()) {
                Car newData;
                cout << "Enter new data for the car:" << endl;
                cout << "Brand: ";
                cin >> newData.brand;
                cout << "Type (truck/car): ";
                cin >> newData.type;
                cout << "License Plate: ";
                cin >> newData.licensePlate;
                cout << "Service Years: ";
                cin >> newData.serviceYears;
                if (newData.type == "car") {
                    cout << "Color: ";
                    cin >> newData.details.color;
                }
                else if (newData.type == "truck") {
                    cout << "Payload (tons): ";
                    cin >> newData.details.payload;
                }
                cars[index] = newData;
                replaceInFile("cars.bin", newData, index);
            }
            else
                cout << "Invalid car index." << endl;
            break;
        }
        case 3: {
            deleteFromFile("cars.bin", 5);
            cout << "Cars with service years more than five have been successfully deleted from the file." << endl;
            break;
        }
        case 4: {
            double minPayload;
            cout << "Enter the minimum payload (tons): ";
            cin >> minPayload;
            findCarsByPayload(cars, minPayload);
            break;
        }
        case 5: {
            string type, color;
            cout << "Enter the type of car (truck/car): ";
            cin >> type;
            if (type == "car") {
                cout << "Enter the color: ";
                cin >> color;
            }
            else
                color = "";
            findCarsByTypeAndColor(cars, type, color);
            break;
        }
        case 6:
            return 0;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}