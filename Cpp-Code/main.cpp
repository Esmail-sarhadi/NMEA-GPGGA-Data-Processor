#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

struct GPGGAData {
    std::string time;
    std::string latitude;
    std::string latitude_direction;
    std::string longitude;
    std::string longitude_direction;
    std::string satellite_count;
};
bool process_gpgga_line(const std::string& line, GPGGAData& data) {
    std::stringstream ss(line);
    std::string token;
    std::vector<std::string> parts;

    while (std::getline(ss, token, ',')) {
        parts.push_back(token);
    }

    if (parts.size() < 15 || parts[0] != "$GPGGA") {
        return false;
    }

    try {
        data.time = parts[1].substr(0, 2) + ":" + parts[1].substr(2, 2) + ":" + parts[1].substr(4, 2);
        data.latitude = parts[2];
        data.latitude_direction = parts[3];
        data.longitude = parts[4];
        data.longitude_direction = parts[5];
        data.satellite_count = parts[7];
    } catch (const std::exception&) {
        return false;
    }

    return true;
}
std::vector<GPGGAData> read_and_process_file(const std::string& file_path) {
    std::vector<GPGGAData> gpgga_data;
    std::ifstream file(file_path);
    std::string line;
    char ch;

    if (!file.is_open()) {
        std::cerr << "Unable to open file" << std::endl;
        return gpgga_data;
    }

    line.reserve(100);
    while (file.get(ch)) {
        if (ch == '\n') {
            if (line.find("$GPGGA") != std::string::npos) {
                GPGGAData data;
                if (process_gpgga_line(line, data)) {
                    gpgga_data.push_back(data);
                }
            }
            line.clear();
        } else {
            line += ch;
        }
    }

    if (!line.empty() && line.find("$GPGGA") != std::string::npos) {
        GPGGAData data;
        if (process_gpgga_line(line, data)) {
            gpgga_data.push_back(data);
        }
    }

    file.close();
    return gpgga_data;
}
int main() {
    std::string file_path = "nmea_messages.txt";
    std::vector<GPGGAData> gpgga_data = read_and_process_file(file_path);
    for (const auto& data : gpgga_data) {
        std::cout << "Time: " << data.time << ", "
                  << "Latitude: " << data.latitude << " " << data.latitude_direction << ", "
                  << "Longitude: " << data.longitude << " " << data.longitude_direction << ", "
                  << "Satellite Count: " << data.satellite_count << std::endl;
    }
    return 0;
}
