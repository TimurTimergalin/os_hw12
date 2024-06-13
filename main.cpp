#include <iostream>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <cerrno>
#include <cstring>

std::string base26(unsigned int i) {
    if (i == 0) {
        return "a";
    }

    std::string res;

    auto a = static_cast<unsigned int>('a');
    while (i != 0) {
        res += static_cast<char>(a + i % 26);
        i /= 26;
    }
    return res;
}

std::string join(const std::string &left, const std::string &right) {
    return left + '/' + right;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " dir_path\n";
        return 1;
    }

    int res1= creat(join(argv[1], base26(0)).c_str(), 0666);
    if (res1 < 0) {
        return 2;
    }
    unsigned int last = 0;
    for (;last < 43; ++last) {
//         std::cout << join(argv[1], base26(last + 1)) << '\n';
        
        errno = 0;
        int res = symlink(
            base26(last).c_str(),
            join(argv[1], base26(last + 1)).c_str()
        );
        if (res < 0) {
            std::cerr << "Error " << strerror(errno);
            return 1;
        }
        errno = 0;
        struct stat s{};
        res = stat(join(argv[1], base26(last + 1)).c_str(), &s);
        if (res >= 0) {
//             std::cout << "Not " << last << "\n";
             continue;
        }
        if (errno == ELOOP) {
            std::cout << last << '\n';
            return 0;
        } else {
       	    std::cerr << "Error " << strerror(errno);
            return 1;
        }
        
    }
}
