#include <ncurses.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "zone.h"
#include "slider.h"
#include "puck.h"
#include "key.h"
using namespace std;

const int WINNING_SCORE = 3;
const int OBSTACLE_WIDTH = 3;
const std::string SAVE_FILE = "./saves/save_best_10.game";

void displayStartScreen(int& numLevels, int& goalWidth, int& sliderSize, int& difficulty);
void updateScore(int player1Score, int player2Score, int player1Levels, int player2Levels);
void displayGameOverScreen(int player1Levels, int player2Levels);
void saveBestScores(int score);
void displayBestScores(); 

int main() {
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(0);
    timeout(0);  


    start_color();
    init_pair(1, COLOR_BLACK, COLOR_WHITE);  
    init_pair(2, COLOR_RED, COLOR_WHITE);    
    init_pair(3, COLOR_WHITE, COLOR_WHITE);  
    init_pair(4, COLOR_BLUE, COLOR_WHITE);   
    init_pair(5, COLOR_BLACK, COLOR_WHITE);  
    init_pair(6, COLOR_BLACK, COLOR_WHITE);  

    int numLevels, goalWidth, sliderSize, difficulty;
    displayStartScreen(numLevels, goalWidth, sliderSize, difficulty);

   
    int fieldHeight = LINES;
    int fieldWidth = fieldHeight;
    int fieldStartX = (COLS - fieldWidth) / 2;
    int fieldStartY = 0;

   
    int player1Score = 0;
    int player2Score = 0;
    int player1Levels = 0;
    int player2Levels = 0;

    
    int ballDelay;
    switch (difficulty) {
        case 1: 
            ballDelay = 60000;
            break;
        case 2: 
            ballDelay = 50000;
            break;
        case 3: 
            ballDelay = 41666;
            break;
        default:
            ballDelay = 50000;
            break;
    }

    
    while (player1Levels < numLevels && player2Levels < numLevels) {
        
        Zone gameZone(fieldWidth, fieldHeight, fieldStartX, fieldStartY, goalWidth);
        Slider player1(fieldStartX + fieldWidth / 4, fieldStartY + fieldHeight - 3, sliderSize);
        Slider player2(fieldStartX + fieldWidth / 4, fieldStartY + 2, sliderSize);
        Puck gamePuck(fieldStartX + fieldWidth / 2, fieldStartY + fieldHeight / 2);

        
        Slider obstacle1(fieldStartX + fieldWidth / 4, fieldStartY + fieldHeight / 3, OBSTACLE_WIDTH);
        Slider obstacle2(fieldStartX + 3 * fieldWidth / 4, fieldStartY + 2 * fieldHeight / 3, OBSTACLE_WIDTH);

        
        bool keyA = false;
        bool keyD = false;
        bool keyW = false;
        bool keyS = false;

        
        player1Score = 0;
        player2Score = 0;

        
        while (player1Score < WINNING_SCORE && player2Score < WINNING_SCORE) {
            int ch1 = readInput(true);
            int ch2 = readInput(false);

            if (ch1 == 'q' || ch1 == 'Q' || ch2 == 'q' || ch2 == 'Q') {
		abort();    
                break;
            }

            
            switch (ch2) {
                case 'a':
                case 'A':
                    keyA = true;
                    break;
                case 'd':
                case 'D':
                    keyD = true;
                    break;
                case 'w':
                case 'W':
                    keyW = true;
                    break;
                case 's':
                case 'S':
                    keyS = true;
                    break;
                default:
                    keyA = false;
                    keyD = false;
                    keyW = false;
                    keyS = false;
                    break;
            }

           
            player1.move(ch1, fieldStartX, fieldStartY, fieldWidth, fieldHeight);
            if (keyA) {
                player2.move('a', fieldStartX, fieldStartY, fieldWidth, fieldHeight);
            }
            if (keyD) {
                player2.move('d', fieldStartX, fieldStartY, fieldWidth, fieldHeight);
            }
            if (keyW) {
                player2.move('w', fieldStartX, fieldStartY, fieldWidth, fieldHeight);
            }
            if (keyS) {
                player2.move('s', fieldStartX, fieldStartY, fieldWidth, fieldHeight);
            }

            
            gamePuck.move(fieldStartX, fieldStartY, fieldWidth, fieldHeight);

            
            gamePuck.checkSliderCollision(player1);
            gamePuck.checkSliderCollision(player2);
            gamePuck.checkWallCollision(fieldStartX, fieldStartY, fieldWidth, fieldHeight);

            
            if (difficulty == 3) {
                gamePuck.checkSliderCollision(obstacle1);
                gamePuck.checkSliderCollision(obstacle2);
            }

            
            int goalStartX = fieldStartX + (fieldWidth - goalWidth) / 2;
            int goalEndX = goalStartX + goalWidth;
            if (gamePuck.getY() <= fieldStartY + 1 && gamePuck.getX() >= goalStartX && gamePuck.getX() < goalEndX) {
                player2Score++;
                gamePuck.reset(fieldStartX + fieldWidth / 2, fieldStartY + fieldHeight / 2);
            } else if (gamePuck.getY() >= fieldStartY + fieldHeight - 2 && gamePuck.getX() >= goalStartX && gamePuck.getX() < goalEndX) {
                player1Score++;
                gamePuck.reset(fieldStartX + fieldWidth / 2, fieldStartY + fieldHeight / 2);
            }

           
            clear();

            
            bkgd(COLOR_PAIR(3));

            
            gameZone.draw();
            player1.draw();
            player2.draw();
            gamePuck.draw();

            
            if (difficulty == 3) {
                obstacle1.draw();
                obstacle2.draw();
            }

            
            updateScore(player1Score, player2Score, player1Levels, player2Levels);

            
            refresh();

            
            usleep(ballDelay);
        }

        
        if (player1Score == WINNING_SCORE) {
            player1Levels++;
            saveBestScores(player1Score);
        } else if (player2Score == WINNING_SCORE) {
            player2Levels++;
            saveBestScores(player2Score);
        }
    }


    
    displayGameOverScreen(player1Levels, player2Levels);

    displayBestScores();

    
    endwin();
    return 0;
}

void displayStartScreen(int& numLevels, int& goalWidth, int& sliderSize, int& difficulty) {
    clear();
    mvprintw(LINES / 2 - 12, COLS / 2 - 10, "Air Hockey Game");
    mvprintw(LINES / 2 - 10, COLS / 2 - 15, "Scoring System:");
    mvprintw(LINES / 2 - 9, COLS / 2 - 15, "- Each player scores a point when the puck enters the opponent's goal.");
    mvprintw(LINES / 2 - 8, COLS / 2 - 15, "- The player who wins the most levels wins the game.");
    mvprintw(LINES / 2 - 6, COLS / 2 - 15, "Player Controls:");
    mvprintw(LINES / 2 - 5, COLS / 2 - 15, "- Player 1 (bottom): Arrow keys");
    mvprintw(LINES / 2 - 4, COLS / 2 - 15, "- Player 2 (top): W, A, S, D keys");

    
    difficulty = 0;
    while (difficulty < 1 || difficulty > 3) {
        mvprintw(LINES / 2 - 2, COLS / 2 - 20, "Select difficulty level (1-3):");
        mvprintw(LINES / 2 - 1, COLS / 2 - 20, "1. Easy");
        mvprintw(LINES / 2, COLS / 2 - 20, "2. Medium");
        mvprintw(LINES / 2 + 1, COLS / 2 - 20, "3. Hard");
        refresh();
        echo();
        char input[10];
        getstr(input);
        difficulty = atoi(input);
        noecho();
    }

    clear();
    mvprintw(LINES / 2, COLS / 2 - 15, "Press 't' to start the game");
    refresh();

    int ch;
    while ((ch = getch()) != 't' && ch != 'T') {
        
    }

    clear();

    
    numLevels = 0;
    while (numLevels <= 0) {
        mvprintw(LINES / 2 - 2, COLS / 2 - 20, "Enter the number of levels: ");
        refresh();
        echo();
        char input[10];
        getstr(input);
        numLevels = atoi(input);
        noecho();
    }

    clear();



  
    goalWidth = 0;
    int maxGoalWidth = 9; 
    while (goalWidth <= 0 || goalWidth > maxGoalWidth) {
        mvprintw(LINES / 2, COLS / 2 - 20, "Enter the width of the goal area (1-%d): ", maxGoalWidth);
        refresh();
        echo();
        char input[10];
        getstr(input);
        goalWidth = atoi(input);
        noecho();
    }

    clear();

    
    sliderSize = 0;
    while (sliderSize < 4 || sliderSize > 7) {
        mvprintw(LINES / 2 + 2, COLS / 2 - 20, "Enter the size of the slider (4-7): ");
        refresh();
        echo();
        char input[10];
        getstr(input);
        sliderSize = atoi(input);
        noecho();
    }

    clear();
}

void updateScore(int player1Score, int player2Score, int player1Levels, int player2Levels) {
    attron(COLOR_PAIR(6));  
    mvprintw(1, 2, "Player 1: Score: %d", player1Score);
    mvprintw(2, 2, "Player 1: Levels: %d", player1Levels);
    attroff(COLOR_PAIR(6));

    
    attron(COLOR_PAIR(6)); 
    mvprintw(LINES - 3, COLS - 25, "Player 2: Score: %d", player2Score);
    mvprintw(LINES - 2, COLS - 25, "Player 2: Levels: %d", player2Levels);
    attroff(COLOR_PAIR(6));
}

void displayGameOverScreen(int player1Levels, int player2Levels) {
    clear();
    attron(COLOR_PAIR(6));  
    mvprintw(LINES / 2 - 2, COLS / 2 - 10, "Game Over!");
    if (player1Levels > player2Levels) {
        mvprintw(LINES / 2, COLS / 2 - 15, "Player 1 wins the game!");
    } else if (player2Levels > player1Levels) {
        mvprintw(LINES / 2, COLS / 2 - 15, "Player 2 wins the game!");
    } else {
        mvprintw(LINES / 2, COLS / 2 - 10, "It's a tie!");
    }
    mvprintw(LINES / 2 + 2, COLS / 2 - 15, "Press any key to exit");
    attroff(COLOR_PAIR(6));  
    refresh();

    
    timeout(5000); 
    getch();
    timeout(0);  
}

void saveBestScores(int score) {
    std::vector<int> scores;
    std::ifstream file(SAVE_FILE);
    if (file.is_open()) {
        int savedScore;
        while (file >> savedScore) {
            scores.push_back(savedScore);
        }
        file.close();
    } else {
        std::cerr << "Error: Could not open file " << SAVE_FILE << " for reading." << std::endl;
    }

    scores.push_back(score);
    std::sort(scores.rbegin(), scores.rend());

    std::ofstream outFile(SAVE_FILE);
    if (outFile.is_open()) {
        for (int i = 0; i < std::min(10, (int)scores.size()); i++) {
            outFile << scores[i] << "\n";
        }
        outFile.close();
    } else {
        std::cerr << "Error: Could not open file " << SAVE_FILE << " for writing." << std::endl;
    }
}

void displayBestScores() {
    clear();
    attron(COLOR_PAIR(6));  
    mvprintw(LINES / 2 - 6, COLS / 2 - 10, "Best Scores:");
    attroff(COLOR_PAIR(6));  

    std::ifstream file(SAVE_FILE);
    if (file.is_open()) {
        int score;
        int i = 0;
        while (file >> score && i < 10) {
            attron(COLOR_PAIR(6)); 
            mvprintw(LINES / 2 - 4 + i, COLS / 2 - 5, "%d. %d", i + 1, score);
            attroff(COLOR_PAIR(6));  
            i++;
        }
        file.close();
    }

    attron(COLOR_PAIR(6)); 
    mvprintw(LINES / 2 + 6, COLS / 2 - 15, "Press any key to exit");
    attroff(COLOR_PAIR(6)); 
    refresh();

   
    timeout(5000); 
    getch();
    timeout(0); 
}
