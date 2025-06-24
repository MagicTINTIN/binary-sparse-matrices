#include <iostream>
#include <vector>
#include <cstdint>
#include <cstdlib>
#include <ctime>

int main(int argc, char* argv[]) {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    uint32_t height = 0;
    uint32_t width = 0;
    double p = 0.0;

    if (argc > 1) {
        if (argc != 4) {
            std::cout << "Usage: " << argv[0] << " <height> <width> <proba>\n";
            return 0;
        }
        height = static_cast<uint32_t>(std::stoi(argv[1]));
        width  = static_cast<uint32_t>(std::stoi(argv[2]));
        p      = std::stod(argv[3]);
    } else {
        std::cout << "Matrix height: ";
        std::cin  >> height;
        std::cout << "Matrix width: ";
        std::cin  >> width;
        std::cout << "1-value probability: ";
        std::cin  >> p;
    }

    std::vector<uint32_t> pointers;
    std::vector<uint32_t> columns;
    pointers.reserve(height + 1);
    pointers.push_back(0);
    columns.reserve(1.5*p*(height*width));

    uint32_t nnz = 0;
    uint32_t maxStep = static_cast<uint32_t>(1.0 / p);

    for (uint32_t i = 0; i < height; ++i) {
        uint32_t col = -1;
        while (col < width - 1 || col == -1) {
            uint32_t step = (std::rand() % maxStep) + 1;
            col += step;
            if (col < width) {
                ++nnz;
                columns.emplace_back(col);
            }
        }
        pointers.emplace_back(nnz);
    }
    std::cerr << "matrix generated.\nWriting it out...\n";

    std::cout << "u_int32_t mat_p[" << pointers.size() << "] = {";
    for (size_t i = 0; i < pointers.size(); ++i) {
        if (i > 0) std::cout << ",";
        std::cout << pointers[i];
    }
    std::cout << "};\n\n";

    std::cout << "u_int32_t mat_j[" << columns.size() << "] = {";
    for (size_t i = 0; i < columns.size(); ++i) {
        if (i > 0) std::cout << ",";
        std::cout << columns[i];
    }
    std::cout << "};\n";

    return 0;
}
