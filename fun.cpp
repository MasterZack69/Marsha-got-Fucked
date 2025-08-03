// Marsha got Fucked - An interactive choice-based narrative
// Copyright (C) 2025 MasterZack

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published
// by the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.

// IN NO EVENT UNLESS REQUIRED BY APPLICABLE LAW OR AGREED TO IN WRITING
// WILL ANY COPYRIGHT HOLDER, OR ANY OTHER PARTY WHO MODIFIES AND/OR CONVEYS
// THE PROGRAM AS PERMITTED ABOVE, BE LIABLE TO YOU FOR DAMAGES, INCLUDING ANY
// GENERAL, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES ARISING OUT OF THE
// USE OR INABILITY TO USE THE PROGRAM (INCLUDING BUT NOT LIMITED TO LOSS OF
// DATA OR DATA BEING RENDERED INACCURATE OR LOSSES SUSTAINED BY YOU OR THIRD
// PARTIES OR A FAILURE OF THE PROGRAM TO OPERATE WITH ANY OTHER PROGRAMS),
// EVEN IF SUCH HOLDER OR OTHER PARTY HAS BEEN ADVISED OF THE POSSIBILITY OF
// SUCH DAMAGES.

// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <iostream>
#include <fstream>
#include <ios>
#include <random>
#include <sstream>
#include <string>
#include <chrono>
#include <thread>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include "fun.hpp"

#ifdef _WIN32
       #include <windows.h>
#endif

extern int scene;
extern int lineWidth;
extern bool thankLuna;

void saveGame(int currentScene) {
    scene = currentScene;
    std::ofstream file("save.dat", std::ios::binary);

    file.write((char*)&currentScene, sizeof(int));
    file.write((char*)&thankLuna, sizeof(bool));
    file.close();
}

bool loadGame(int& currentScene) {
    std::ifstream file("save.dat", std::ios::binary);

    if (!file) return false;

    file.read((char*)&currentScene, sizeof(int));

    file.close();
    return true;
}

bool saveExists() {
    std::ifstream file("save.dat");
    return file.good();
}

void enter() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}


int choice(std::string options[]) {
    for (int i = 0; i < 3; ++i) {
        std::cout << options[i] << "\n";
    }

    std::string input;
    int userChoice;
    printW("Choose :");

    while (true) {
        std::getline(std::cin, input);
        std::stringstream ss(input);

        if (ss >> userChoice && ss.eof()) {
            if (userChoice >= 1 && userChoice <= 3) {
                return userChoice;
            } else {
                printW("Enter an integer between 1–3 :");
            }
        } else {
            printW("Enter an integer between 1-3 :");
        }
    }

}

bool keyChoice(std::string options[]) {
    for (int i = 0; i < 2; ++i) {
        std::cout << options[i] << "\n";
    }

    std::string input;
    int userChoice;
    printW("Choose :");

    while (true) {
        std::getline(std::cin, input);
        std::stringstream ss(input);

        if (ss >> userChoice && ss.eof()) {
            if (userChoice == 1 or userChoice == 2) {
                return userChoice == 1;
            } else {
                printW("Enter either 1 or 2 :");
            }
        } else {
            printW("Enter either 1 or 2 :");
        }
    }
}

bool action(int length) {
    const std::string characters = "0123456789abcdefghijklmnopqrstuvwxyz";

    std::string rndString;
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> dist(0, characters.size()-1);

    for (int i = 0; i < length; ++i) {
        rndString += characters[dist(rng)];
    }

    std::cout << "\nType " << rndString << " in 10 seconds : " << std::flush; 

    fd_set readfds;
    FD_ZERO(&readfds);
    FD_SET(STDIN_FILENO, &readfds);

    struct timeval timeout;
    timeout.tv_sec = 10;
    timeout.tv_usec = 0;

    int result = select(STDIN_FILENO + 1, &readfds, NULL, NULL, &timeout);
    std::string usrString;

    if (result > 0) {
        std::getline(std::cin, usrString);
        return usrString == rndString;
    } else if (result == 0) {
        std::cout << "\nTime's Up!";
        enter();
        return false;
    } else {
        std::cerr << "\nError reading input";
        enter();
        return false;
    }
}

void sceneX(int X) {
    std::string text = "-----> Scene " + std::to_string(X);
    std::cout << "\n";

    for (size_t i = 0; i <= text.length(); i++) {
        std::cout << "\r" << text.substr(0, i);
        for (size_t j = i; j < text.length(); j++) {
            std::cout << char(33 + rand() % 93);
        }
        std::cout << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
    std::cout << "\r" << text;
}

int getConsoleWidth() {
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    return csbi.srWindow.Right - csbi.srWindow.Left + 1;
#else
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    return w.ws_col;
#endif
}

void printW(const std::string& text) {
    std::istringstream stream(text);
    std::string line;
    std::string word;
    std::string textLine;

    while (std::getline(stream, textLine)) {
        std::istringstream words(textLine);
        std::string currentLine;

        while (words >> word) {
            if (word.length() >= lineWidth) {
                if (!currentLine.empty()) {
                    std::cout << "\n" << currentLine;
                    currentLine.clear();
                }

                size_t pos = 0;
                while (pos < word.length()) {
                    size_t breakPoint = std::min(pos + lineWidth - 1, word.length());
                    if (breakPoint < word.length()) {
                        std::cout << word.substr(pos, breakPoint - pos - 1) << "-\n";
                        pos = breakPoint -1;
                    } else {
                        std::cout << word.substr(pos) << "\n";
                        pos = breakPoint;
                                      
                    }
                }
            }
            else if (currentLine.length() + word.length() + 1 > lineWidth) {
                std::cout << "\n" << currentLine;
                currentLine = word;
            } else {
                if (!currentLine.empty()) currentLine += ' ';
                currentLine += word;
            }
        }
        if (!currentLine.empty()) {
            std::cout << "\n" << currentLine;
        }
    }
}
