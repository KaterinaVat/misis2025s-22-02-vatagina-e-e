#include "../semcv/semcv.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <filesystem>

std::string get_format(const std::string& filename) {
    size_t dotPos = filename.find_first_of('.');
    std::string format = filename.substr(dotPos + 1);
    if (format == "jpg") {
        return "JPG";
    }
    if (format == "png") {
        return "PNG";
    }
    if (format == "tiff") {
        return "TIFF";
    }
    return "";
}
std::string get_from_file(const std::string& filename) {
    std::filesystem::path currentPath = std::filesystem::current_path() / "output_images" / filename;
    std::ifstream file(currentPath, std::ios::binary);
    if (!file) {
        return "";
    }

    std::vector<char> buffer(8);
    file.read(buffer.data(), buffer.size());

    std::string signature;
    for (char byte : buffer) {
        signature += byte;
    }
    return signature;
}

std::string get_actual_format(const std::string& signature) {
    if (signature.substr(0, 2) == "\xFF\xD8") {
        return "JPG";
    }
    if (signature.substr(0, 8) == "\x89PNG\r\n\x1A\n") {
        return "PNG";
    }
    if (signature.substr(0, 2) == "II" || signature.substr(0, 2) == "MM") {
        return "TIFF";
    }

    return "";
}


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << argv[0] << " <path to lst file>" << std::endl;
        return 1;
    }

    std::ifstream lst_file(argv[1]);
    if (!lst_file.is_open()) {
        std::cerr << "Error openning file" << argv[1] << std::endl;
        return 1;
    }

    std::string filename;
    while (std::getline(lst_file, filename)) {
        std::string expected_format = get_format(filename);
        if (expected_format.empty()) {
            std::cerr << "Error: unknown format " << filename << std::endl;
            continue;
        }

        std::string signature = get_from_file(filename);
        if (signature.empty()) {
            std::cerr << "Error reading file " << filename << std::endl;
            continue;
        }

        std::string actual_format = get_actual_format(signature);
        if (actual_format.empty()) {
            std::cerr << "Error: unknown format " << filename << std::endl;
            continue;
        }

        if (expected_format == actual_format) {
            std::cout << filename << "\tgood" << std::endl;
        }
        else {
            std::cout << filename << "\tbad, should be " << actual_format << std::endl;
        }
    }

    lst_file.close();
    return 0;
}