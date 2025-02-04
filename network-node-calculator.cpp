#include <iostream>
#include <cmath>
#include <vector>
#include <string>

// Haversine formula to calculate distance between two points on the Earth (latitude, longitude)
double haversine(double lat1, double lon1, double lat2, double lon2) {
    const double R = 6371.0;  // Earth radius in kilometers
    double phi1 = lat1 * M_PI / 180.0;
    double phi2 = lat2 * M_PI / 180.0;
    double delta_phi = (lat2 - lat1) * M_PI / 180.0;
    double delta_lambda = (lon2 - lon1) * M_PI / 180.0;

    double a = sin(delta_phi / 2) * sin(delta_phi / 2) +
               cos(phi1) * cos(phi2) * sin(delta_lambda / 2) * sin(delta_lambda / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return R * c;  // Distance in kilometers
}

// Function to calculate the new node location based on distance and bearing
void calculate_new_location(double lat, double lon, double distance, double bearing, double &new_lat, double &new_lon) {
    // Convert bearing to radians
    bearing = bearing * M_PI / 180.0;

    // Radius of the Earth in kilometers
    const double R = 6371.0;

    // New latitude in radians
    new_lat = asin(sin(lat * M_PI / 180.0) * cos(distance / R) +
                   cos(lat * M_PI / 180.0) * sin(distance / R) * cos(bearing));

    // New longitude in radians
    new_lon = (lon * M_PI / 180.0) + atan2(sin(bearing) * sin(distance / R) * cos(lat * M_PI / 180.0),
                                             cos(distance / R) - sin(lat * M_PI / 180.0) * sin(new_lat));

    // Convert back to degrees
    new_lat = new_lat * 180.0 / M_PI;
    new_lon = new_lon * 180.0 / M_PI;
}

int main() {
    std::cout << "Ad Hoc Network Node Location Calculation\n";

    double lat, lon;
    std::cout << "Enter the initial latitude of the node (in degrees):";
    std::cin >> lat;
    std::cout << "Enter the initial longitude of the node (in degrees):";
    std::cin >> lon;

    int num_nodes;
    std::cout << "Enter the number of nearby nodes:";
    std::cin >> num_nodes;

    std::vector<std::pair<double, double>> nodes;

    for (int i = 0; i < num_nodes; ++i) {
        std::cout << "\nNode " << i + 1 << ":\n";
        double distance, bearing;
        std::cout << "Enter the distance from this node (in kilometers): ";
        std::cin >> distance;
        std::cout << "Enter the bearing (direction) from the node in degrees (0-360): ";
        std::cin >> bearing;

        double new_lat, new_lon;
        calculate_new_location(lat, lon, distance, bearing, new_lat, new_lon);
        nodes.push_back({new_lat, new_lon});

        std::cout << "Node " << i + 1 << " location (Lat, Lon): " << new_lat << ", " << new_lon << "\n";
    }

    // For visualization, we'll write the locations to a file that can be plotted using gnuplot or any other plotting tool
    std::ofstream plot_file("node_locations.dat");
    plot_file << lat << " " << lon << "\n";  // Write the initial node location

    for (const auto& node : nodes) {
        plot_file << node.first << " " << node.second << "\n";  // Write each node's location
    }

    plot_file.close();
    std::cout << "Node locations have been written to 'node_locations.dat'. Use a plotting tool (e.g., gnuplot) to visualize them.\n";

    return 0;
}
