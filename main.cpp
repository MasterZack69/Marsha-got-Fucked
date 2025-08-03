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
#include <string>
#include "story.hpp"
#include "fun.hpp"

int lineWidth;
std::string menuChoice;
extern int scene;

void mainMenu() {
    printW("1. New Game \n2. Continue \nEnter :");

    while (true) {
        std::cin >> menuChoice;

        
        if (menuChoice == "1" or menuChoice == "2") {
            break;
        } else {
            printW("Enter a number between 1 and 2 :");
        }
    }
}

int main() {

    lineWidth = getConsoleWidth();

    printW("Welcome to Marsha got Fucked!");

    mainMenu();
    
   while (true) {
       if (menuChoice == "2" && saveExists()) {
           loadGame(scene);
           resumeGame(scene);
           break;
       } else if (menuChoice == "2" && saveExists() == false) {
           printW("No save found... \nStarting the narrative from the beginning.");
           enter();
           resumeGame(0);
           break;
       } else if (menuChoice == "1" && saveExists()) {
           printW("You already have a save file, playing a new game will overwrite it. Are you sure you want to play a new game? (y/n) :");
           std::string yesno;
           std::cin >> yesno;

           if (yesno == "y" or yesno == "Y") {
               resumeGame(0);
               break;
           } else if (yesno == "n" or yesno == "N") {
               mainMenu();
           } else {
               printW("I will take that as a no...");
               enter();
               mainMenu();
           }
       } else {
           resumeGame(0);
           break;
       }
   }

    return 0;
}
