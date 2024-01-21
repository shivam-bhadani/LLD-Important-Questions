#include<bits/stdc++.h>
using namespace std;

#define IOS ios::sync_with_stdio(0); cin.tie(0); cout.tie(0);

class PaymentStrategy {
public:
    virtual double calculateCost(double hours) = 0;
};

class BikePayment: public PaymentStrategy {
public:
    double calculateCost(double hours) override {
        return 2 * hours;
    }
};

class CarPayment: public PaymentStrategy {
public:
    double calculateCost(double hours) override {
        return 3 * hours;
    }
};

class BusPayment: public PaymentStrategy {
public:
    double calculateCost(double hours) override {
        return 4 * hours;
    }
};

class Vehicle {
protected:
    PaymentStrategy *payment;
    chrono::time_point<chrono::system_clock> parkedTime;

public:
    virtual string getVehicleType() = 0;

    double calculateCost(double hours) {
        return payment->calculateCost(hours);
    }

    void setParkedTime() {
        parkedTime = chrono::system_clock::now();
    }

    std::chrono::time_point<std::chrono::system_clock> getParkedTime() {
        return parkedTime;
    }
};

class Bike : public Vehicle {
public:
    Bike() {
        payment = new BikePayment();
    }

    string getVehicleType() override {
        return "Bike";
    }
};

class Car : public Vehicle {
public:
    Car() {
        payment = new CarPayment();
    }

    string getVehicleType() override {
        return "Car";
    }
};

class Bus : public Vehicle {
public:
    Bus() {
        payment = new BusPayment();
    }

    string getVehicleType() override {
        return "Bus";
    }
};

class ParkingLot {
private:
    static ParkingLot *parkingLot;
    int floors, rows, spotPerRows;
    vector<vector<vector<Vehicle*>>> spots;

    ParkingLot(int _floors, int _rows, int _spotPerRows) {
        floors = _floors;
        rows = _rows;
        spotPerRows = _spotPerRows;
        spots.resize(floors, vector<vector<Vehicle*>>(rows, vector<Vehicle*>(spotPerRows, nullptr)));
    }

public:
    static ParkingLot *getInstance(int _floors, int _rows, int _spotPerRows) {
        if (parkingLot == NULL) {
            parkingLot = new ParkingLot(_floors, _rows, _spotPerRows);
        }
        return parkingLot;
    }

    double calculateParkedHours(Vehicle *vehicle) {
        for (auto& floor : spots) {
            for (auto& row : floor) {
                for (auto& spot : row) {
                    if (spot == vehicle) {
                        chrono::time_point<chrono::system_clock> nowTime = chrono::system_clock::now();
                        return std::chrono::duration_cast<std::chrono::hours>(nowTime - spot->getParkedTime()).count();
                    }
                }
            }
        }
        return 0;
    }

    bool park(Vehicle *vehicle, int floor, int row, int spot) {
        if (spots[floor][row][spot] != nullptr) {
            return false; // Spot is already occupied
        }
        spots[floor][row][spot] = vehicle;
        vehicle->setParkedTime();
        auto parkedTime = chrono::system_clock::to_time_t(vehicle->getParkedTime());
        cout << "Parked " << vehicle->getVehicleType() << " at floor " << floor << ", row " << row << ", spot " << spot << " at " << ctime(&parkedTime);
        return true;
    }

    bool leave(Vehicle *vehicle) {
        for (auto& floor : spots) {
            for (auto& row : floor) {
                for (auto& spot : row) {
                    if (spot == vehicle) {
                        double parkedHours = calculateParkedHours(vehicle);
                        cout << spot->getVehicleType() << " successfully left and its parking cost is: " << spot->calculateCost(parkedHours) << endl;
                        spot = nullptr;
                        return true;
                    }
                }
            }
        }
        cout << "Vehicle not found" << endl;
        return false;
    }
};

ParkingLot* ParkingLot::parkingLot = nullptr;

int32_t main() {
    IOS
    ParkingLot *parkingLot = ParkingLot::getInstance(4, 2, 8);

    Car *car = new Car();
    parkingLot->park(car, 0, 1, 1);
    parkingLot->leave(car);

    Bus *bus = new Bus();
    parkingLot->park(bus, 1, 0, 2);

    return 0;
}
