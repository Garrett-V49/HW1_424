//Garrett Varvil
//AERSP 424 HW1

#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <utility>
#include <ctime>
#include <cstdlib>
#include <iomanip>
#include<numeric>

using namespace std;

//function to calcualte the moment given arm and weight 
double calculate_moment(double weight, double arm) { 
    return weight * arm;
}

class Plane; //Only way to make Pilot class be able to reference Plane class

class Pilot { //Question 6
private:
    std::string name;
public:
    Plane *myPlane;

    //Constrcutor 
    Pilot(const std::string& pilotname);

    //Destructor 
    ~Pilot();

    //Getter for name
    std::string getName() const;
    
    //assignPlane Functo set myPlane pointer
    void assignPlane(Plane* plane);

    //Friend declaration to let it access Plane Class to be able to point to it
    friend class Plane;
};

//Implement of func to set myplane pointer
void Pilot::assignPlane(Plane* plane) {
    myPlane = plane;
}

//Question 3 Plane Class
class Plane { 
private: 
    double pos, vel, distance;
    bool at_SCE;
    std::string origin, destination;
    std::map<std::pair<std::string, std::string>, int> FlightsDetails;

public:
    //constructor 
    Plane(const std::string& from, const std::string& to);

    //Destructor 
    ~Plane();

    //Function named Operate
    void operate(double dt);

    //get functions
    double getPos() const;
    double getVel() const;
    double getDistance() const;
    bool getAtSCE() const; 
    std::string getOrigin() const;
    std::string getDestination() const;

    //Set function for vel
    void setVel(double newVel);
};

//Question 4
Plane::Plane(const std::string& from, const std::string& to) :
    pos(0.0), vel(0.0), distance(0.0), at_SCE(true), origin(from), destination(to) {

    //Question 2
    FlightsDetails[std::make_pair("SCE", "PHL")] = 160;
    FlightsDetails[std::make_pair("SCE", "EWR")] = 220;
    FlightsDetails[std::make_pair("SCE", "ORD")] = 640; 
    FlightsDetails[std::make_pair("PHL", "SCE")] = 160;
    FlightsDetails[std::make_pair("ORD", "SCE")] = 640;
    FlightsDetails[std::make_pair("EWR", "SCE")] = 220;

    //Calc distance between origin and destination
    auto it = FlightsDetails.find(std::make_pair(origin, destination));
    if (it != FlightsDetails.end()) {
        distance = it->second;
    }
    else {
        distance = 0.0; // Set default value if airports not found in container
    }
    std::cout << "Plane Created with a Tail number " << this << std::endl;
}

//Destructor implementation
Plane::~Plane() {
    std::cout << "Plane Destroyed" << std::endl;
}

//Member Function to perform operations 
void Plane::operate(double dt) {
    if (pos < distance) {
        pos += vel * (dt / 3600);
        at_SCE = false;
    }
    else if (destination == "SCE") {
        at_SCE = true;
        std::swap(origin, destination);
        pos = 0.0;
    }
    else {
        std::swap(origin, destination);
        pos = 0.0;
    }
}

//Getter Functions
double Plane::getPos() const {
    return pos;
}
double Plane::getVel() const {
    return vel;
}
double Plane::getDistance() const {
    return distance; 
}
bool Plane::getAtSCE() const {
    return at_SCE;
}
std::string Plane::getOrigin() const {
    return origin;
}
std::string Plane::getDestination() const {
    return destination;
}

//Setter function for velocity 
void Plane::setVel(double newVel) {
    vel = newVel;
}

//Implement Pilot Class
Pilot::Pilot(const std::string& pilotName) : name(pilotName), myPlane(nullptr) {
    std::cout  << name << " with certificate number " << this << " is at the gate, ready to board the plane." << std::endl;
}

Pilot::~Pilot() {
    std::cout << name << " is out of the plane." << std::endl;
}

std::string Pilot::getName() const {
    return name;
}

int main() {
    // Get user inputs for all aircraft data
    double empty_weight, empty_weight_moment;
    int num_front_seat_occupants, num_rear_seat_occupants;

    cout << "Enter airplane empty weight (pounds): ";
    cin >> empty_weight;

    cout << "Enter airplane empty-weight moment (pounds-inches): ";
    cin >> empty_weight_moment;

    cout << "Enter the number of front seat occupants: ";
    cin >> num_front_seat_occupants;

    vector<double> front_seat_weights(num_front_seat_occupants);

    for (int i = 0; i < num_front_seat_occupants; ++i) {
        cout << "Enter weight of front seat occupant " << i + 1 << " (pounds): ";
        cin >> front_seat_weights[i];
    }

    double front_seat_moment_arm;
    cout << "Enter front seat moment arm (inches): ";
    cin >> front_seat_moment_arm;

    cout << "Enter the number of rear seat occupants: ";
    cin >> num_rear_seat_occupants;

    vector<double> rear_seat_weights(num_rear_seat_occupants);

    for (int i = 0; i < num_rear_seat_occupants; ++i) {
        cout << "Enter weight of rear seat occupant " << i + 1 << " (pounds): ";
        cin >> rear_seat_weights[i];
    }

    double rear_seat_moment_arm;
    cout << "Enter rear seat moment arm (inches): ";
    cin >> rear_seat_moment_arm;

    double gallons_of_fuel, fuel_weight_per_gallon, fuel_tank_moment_arm;

    cout << "Enter the number of gallons of usable fuel: ";
    cin >> gallons_of_fuel;

    cout << "Enter usable fuel weights per gallon (pounds): ";
    cin >> fuel_weight_per_gallon;

    cout << "Enter fuel tank moment arm (inches): ";
    cin >> fuel_tank_moment_arm;

    double baggage_weight, baggage_moment_arm;

    cout << "Enter baggage weight (pounds): ";
    cin >> baggage_weight;

    cout << "Enter baggage moment arm (inches): ";
    cin >> baggage_moment_arm;

    // Calculate total weight and moment
    double total_weight = (empty_weight +
        accumulate(front_seat_weights.begin(), front_seat_weights.end(), 0.0) +
        accumulate(rear_seat_weights.begin(), rear_seat_weights.end(), 0.0) +
        (gallons_of_fuel * fuel_weight_per_gallon) +
        baggage_weight);

    double total_moment = (empty_weight_moment +
        accumulate(front_seat_weights.begin(), front_seat_weights.end(), 0.0,
            [=](double acc, double weight) { return acc + calculate_moment(weight, front_seat_moment_arm); }) +
        accumulate(rear_seat_weights.begin(), rear_seat_weights.end(), 0.0,
            [=](double acc, double weight) { return acc + calculate_moment(weight, rear_seat_moment_arm); }) +
        (gallons_of_fuel * calculate_moment(fuel_weight_per_gallon, fuel_tank_moment_arm)) +
        calculate_moment(baggage_weight, baggage_moment_arm));

    double new_weight_fuel = fuel_weight_per_gallon * gallons_of_fuel;
    double new_CG = 0.0;
    double fuel_to_add_or_drain = 0.0;

    // Check if within design limits
    while (total_weight > 2950 || 82.1 > new_CG || new_CG > 84.7) {
        if (total_weight > 2950 || new_CG < 82.1) {
            new_weight_fuel -= 0.01;
            total_weight -= 0.01;
        }
        else if (new_CG > 84.7) {
            new_weight_fuel += 0.01;
            total_weight += 0.01;
        }
        fuel_to_add_or_drain = new_weight_fuel / fuel_weight_per_gallon;
        new_CG = total_moment / total_weight;
    }
        cout << "Aircraft was outside design limits. Adjusted fuel: " << fixed << setprecision(2) << fuel_to_add_or_drain << " gallons." << endl;
        cout << "New gross weight: " << fixed << setprecision(2) << total_weight << " lbs, New C.G. location: " << new_CG << " inches" << endl;
    

    cout << "\n #### End of Question 1 #### \n\n ####Start of Question 5 Below #### \n\n";

    //HW made it seem as though we choose a set value for these and that they were not randomized nor inputs
    double flight_speed = 450; //flight speed between 400-500mph
    double timestep = 50; //Timestep between 10-100
    int max_iterations = 2000; //Iteration amount
    double Time_Iteration = 0; 

    string origin, destination;
    cout << "Enter the origin airport ID: " << endl;
    cin >> origin;
    cout << "Enter the destination airport ID: " << endl;
    cin >> destination;

    Plane myAirplane(origin, destination); //Instantiate an object from Plane class MAYBE CHANGE TO HAVE INPUTS HERE IDK

    myAirplane.setVel(flight_speed); //Set the aircraft speed

    for (int iteration = 1; iteration <= max_iterations; ++iteration) {
        // Print airplane position at each timestep

        cout << "Time: " << Time_Iteration << "s, Position: " << myAirplane.getPos() << " miles" << endl;
        Time_Iteration += timestep;

        // Call the "operate" function with the timestep as an input
        myAirplane.operate(timestep);

        // Break the loop if the airplane is back at the departure airport
        if (myAirplane.getAtSCE()) {
            cout << "Airplane has returned to the departure airport." << endl;
            break;
        }
    }

    cout << "\n #### End of Question 5 #### \n\n ####Start of Question 7 Below #### \n\n"; 

    //Question 7 

    Pilot pilot1("Alpha");
    Pilot pilot2("Bravo");

    cout << "Plane created with tail number: " << &myAirplane << "\n" << endl;
    // Pilot 1 takes control

    pilot1.myPlane = &myAirplane;
    cout << pilot1.getName() << " with certificate number " << &pilot1 << " is in control of a plane : " << pilot1.myPlane << "." << endl;
    pilot2.myPlane = nullptr;
    cout << pilot2.getName() << " with certificate number " << &pilot2 << " is in control of a plane :" << pilot2.myPlane << "." << endl;

    for (int iteration = 1; iteration <= max_iterations; ++iteration) {

        // Call the "operate" function with the timestep as an input
        myAirplane.operate(timestep);

        // Check if the airplane is back at the departure airport
        if (myAirplane.getAtSCE() == 1) {
            cout << "The Plane: " <<&myAirplane << " is at SCE" << endl;

            // Pilot Swaps control
            swap(pilot1.myPlane, pilot2.myPlane);
            cout << pilot1.getName() << " with certificate number " << &pilot1 << " is in control of a plane : " << pilot1.myPlane << "." << endl;
            cout << pilot2.getName() << " with certificate number " << &pilot2 << " is in control of a plane :" << pilot2.myPlane << ".\n" << endl;
        }
    }

    //Question 8 in OLD CPP Style Below
    //Pilot* pilot1 = new Pilot("Alpha");
    //Pilot* pilot2 = new Pilot("Bravo");

    //std::cout << "Plane created with tail number: " << &myAirplane << "\n" << std::endl;

    //// Pilot 1 takes control
    //pilot1->assignPlane(&myAirplane);
    //std::cout << pilot1->getName() << " with certificate number " << pilot1 << " is in control of a plane: " << pilot1->myPlane << "." << std::endl;

    //// Pilot 2 takes control
    //pilot2->assignPlane(nullptr);
    //std::cout << pilot2->getName() << " with certificate number " << pilot2 << " is in control of a plane: " << pilot2->myPlane << "." << std::endl;

    //for (int iteration = 1; iteration <= max_iterations; ++iteration) {
    //    // Call the "operate" function with the timestep as an input
    //    myAirplane.operate(timestep);

    //    // Check if the airplane is back at the departure airport
    //    if (myAirplane.getAtSCE() == 1) {
    //        std::cout << "The Plane: " << &myAirplane << " is at SCE" << std::endl;

    //        // Pilot Swaps control
    //        Plane* temp = pilot1->myPlane;
    //        pilot1->myPlane = pilot2->myPlane;
    //        pilot2->myPlane = temp;
    //        std::cout << pilot1->getName() << " with certificate number " << pilot1 << " is in control of a plane: " << pilot1->myPlane << "." << std::endl;
    //        std::cout << pilot2->getName() << " with certificate number " << pilot2 << " is in control of a plane: " << pilot2->myPlane << ".\n" << std::endl;
    //    }
    //}

    //// Clean up allocated memory
    //delete pilot1;
    //delete pilot2;

return 0;
}
