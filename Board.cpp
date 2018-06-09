#include "Board.h"
#include <iostream>

Board::Board() : filled(nullptr), size(3), lastplayer(true)
{
    this->boardsize = size * size;
    this->filled = new char[boardsize];
    fillBoard();
}

Board::Board(const Board& other) : filled(nullptr), size(other.size), boardsize(other.boardsize), lastplayer(other.lastplayer)
{
    this->filled = new char[other.boardsize];
    for(int i = 0; i< boardsize; ++i)
        this->filled[i] = other.filled[i];
}

Board& Board::operator=(const Board & other)
{
    if(this != &other)
    {
        clearBoard();
        this->size = other.size;
        this->boardsize = other.boardsize;
        this->lastplayer = other.lastplayer;
        this->filled = new char[this->boardsize];
        for(int i = 0; i< boardsize; ++i)
            this->filled[i] = other.filled[i];
    }
    return *this;
}

Board::~Board()
{
    clearBoard();
}

void Board::play()
{
    int x, y;
    while(!hasWinner() && !boardFull())
    {
        printBoard();
        if(!lastplayer)
        {
            std::cout<< "Enter coordinates for X (x: 0 - 2; y: 0 - 2): ";
            std::cin >> x >> y;
            if(x >= this->size || y >= this->size)
            {
                std::cout<< "Invalid move. Try again!\n";
                continue;
            }
            if(!setX(y, x))
                std::cout<< "Invalid move. Try again!\n";
        }
        else
        {
            std::cout<< "Enter coordinates for O: (x: 0 - 2; y: 0 - 2): ";
            std::cin >> x >> y;
            if(x >= this->size || y >= this->size)
            {
                std::cout<< "Invalid move. Try again!\n";
                continue;
            }
            if(!setO(y, x))
                std::cout<< "Invalid move. Try again!\n";
        }
    }
    std::cout << "Winner: ";
    printWinner();
}

void Board::playvscpu()
{
    int x, y;
    while(!hasWinner() && !boardFull())
    {
        printBoard();
        if(!lastplayer)
        {
            minmax(true);
            filled[this->i1] = 'X';
            lastplayer = !lastplayer;
            
        }
        else
        {
            std::cout<< "Enter coordinates for O: (x: 0 - 2; y: 0 - 2): ";
            std::cin >> x >> y;
            if(x >= this->size || y >= this->size)
            {
                std::cout<< "Invalid move. Try again!\n";
                continue;
            }
            if(!setO(y, x))
                std::cout<< "Invalid move. Try again!\n";
        }
    }
    std::cout << "Winner: ";
    char tmp = this->whoWon();
    if(tmp == 'X')
        std::cout<< "CPU"<<std::endl;
    else if(tmp == 'O')
        std::cout<< "Player"<<std::endl;
    else
        std::cout<< "Draw!"<<std::endl;
}

bool Board::setX(const int x, const int y)
{
    if(!isValid(x, y) || this->lastplayer == true)
        return false;
    this->filled[y * size + x] = 'X';
    this->lastplayer = !this->lastplayer;
    return true;
}

bool Board::setO(const int x, const int y)
{
    if(!isValid(x, y) || this->lastplayer == false)
        return false;
    this->filled[y * size + x] = 'O';
    this->lastplayer = !this->lastplayer;
    return true;
}

void Board::printBoard()
{
    std::cout<<"h(state) = "<< this->calcH()<<std::endl;
    std::cout << " ";
    for(int i = 0; i< this->boardsize + this->size -1; ++i)
        std::cout<<"-";
    std::cout<<"  y:\n| ";
    
    for(int i = 0; i< this->size; ++i)
    {
        for(int j = 0; j< this->size; ++j)
        {
            std::cout << this->getXY(i, j);
            std::cout << " | ";
        }
        std::cout << i;
        if((i+1) != this->size)
        {
            std::cout << "\n|";
            for(int i = 0; i< this->boardsize + this->size - 1; ++i)
                std::cout<<"-";
            std::cout<<"|\n| ";
        }

    }
    
    std::cout<<"\n ";
    for(int i = 0; i< this->boardsize + this->size -1; ++i)
        std::cout<<"-";
    std::cout<<"\nx";
    for(int i=0; i< this->size; ++i)
        std::cout<<" "<<i<<"  ";
    std::cout<<"\n";
}

void Board::printWinner()
{
    bool found = false;
    for(int i = 0; i< this->size; ++i)
    {
        if(isWinningRow(i))
        {
            std::cout << getXY(i, 0) << "\n";
            found = true;
        }
        else if(isWinningCol(i))
        {
            std::cout << getXY(0, i) << "\n";
            found = true;
        }
        if(found)
            return;
    }

    
    if(isMainDiagWin())
        std::cout << getXY(0, 0) << "\n";
    
    else if(isOtDiagWin())
        std::cout << getXY(0, size - 1) << "\n";
    else
        std::cout << "Draw!" << "\n";
}

char Board::whoWon()
{
    for(int i = 0; i< this->size; ++i)
    {
        if(isWinningRow(i))
            return getXY(i, 0);
        
        else if(isWinningCol(i))
            return getXY(0, i);
    }
    
    if(isMainDiagWin())
       return getXY(0, 0);
    
    else if(isOtDiagWin())
       return getXY(0, size - 1);
    
    return ' ';
}

int Board::minmax(bool flag)
{
    int max_val=-10000,min_val=10000;
    int value = 0;
    char winner = this->whoWon();

    if(winner == 'X')
        return 10;
    else if(winner == 'O')
        return -10;
    else if(boardFull())
        return 0;
    
    int * score = new int[this->boardsize];
    for(int i=0; i< this->boardsize; ++i)
        score[i] = 1;
    

    for(int i=0; i < this->boardsize; ++i)
    {
        if(this->filled[i] == ' ')
        {
            if(min_val > max_val)
            {
                if(flag == true)
                {
                    this->filled[i] = 'X';
                    value = minmax(false);
                }
                else
                {
                    this->filled[i] = 'O';
                    value = minmax(true);
                }
                this->filled[i] = ' ';
                score[i] = value;
            }
        }
    }

    if(flag == true)
    {
        max_val = -10000;
        for(int i=0; i< this->boardsize; ++i)
        {
            if(score[i] > max_val && score[i] != 1)
            {
                max_val = score[i];
                this->i1 = i;
            }
        }
        delete[] score;
        return max_val;
    }
    
    
    min_val = 10000;
    for(int i=0; i< this->boardsize; ++i)
    {
        if(score[i] < min_val && score[i] != 1)
        {
            min_val = score[i];
            this->i1 = i;
        }
    }
    delete[] score;
    return min_val;
}

int Board::calcH()
{
    int score[2 * this->size + 2][2];
    for(int i=0; i< 2 * this->size + 2; ++i)
    {
        score[i][0] = 0;
        score[i][1] = 0;
    }
    
    for(int i=0; i<this->size; ++i)
    {
        for(int j=0; j<this->size; ++j)
        {
            if(getXY(i, j) == 'O')
                ++score[i][0];
            else if(getXY(i, j) == 'X')
                ++score[i][1];
            
            if(getXY(j, i) == 'O')
                ++score[this->size + i][0];
            else if(getXY(j, i) == 'X')
                ++score[this->size + i][1];
        }
        
        if(getXY(i, i) == 'O')
            ++score[2 * this->size][0];
        else if(getXY(i, i) == 'X')
            ++score[2 * this->size][1];
        
        if(getXY(i, this->size - i - 1) == 'O')
            ++score[2 * this->size + 1][0];
        else if(getXY(i, this->size - i - 1) == 'X')
            ++score[2 * this->size + 1][1];
    }
    
    int txzo = 0; //two x zero o
    int oxzo = 0; //one x zero o
    int tozx = 0;
    int oozo = 0;
    
     for(int i=0; i< 2 * this->size + 2; ++i)
     {
         if(score[i][1] == 2 && score[i][0] == 0)
             ++txzo;
         else if(score[i][1] == 1 && score[i][0] == 0)
             ++oxzo;
         else if(score[i][1] == 0 && score[i][0] == 2)
             ++tozx;
         else if(score[i][1] == 0 && score[i][0] == 1)
             ++oozo;
     }
    return (2 * txzo + oxzo) - (2 * tozx + oozo);
}

void Board::fillBoard()
{
    for(int i = 0; i< this->boardsize; ++i)
        this->filled[i] = ' ';
}

bool Board::boardFull()
{
    for(int i = 0; i< this->boardsize; ++i)
        if(this->filled[i] == ' ')
            return false;
    return true;
}

bool Board::isValid(const int x, const int y)
{
    if(getXY(x, y) != ' ')
        return false;
    else if(hasWinner())
        return false;
    return true;
}

bool Board::hasWinner()
{
    bool rc = false;
    
    //Check rows & cols
    for(int i = 0; i< this->size; ++i)
    {
        rc = isWinningRow(i) || isWinningCol(i);
        if(rc)
            return true;
    }
    
    //Check diags
    //Reset rc
    rc = true;
    //Check main diagonal
    for(int i =1; i< this->size; ++i)
    {
        rc = getXY(0, 0) == getXY(i, i)  && rc && getXY(0, 0) != ' ';
    }
    
    if(rc)
        return true;
    //Reset rc
    rc = true;
    //Check other diagonal
    for(int i = 1; i< this->size; ++i)
    {
        rc = getXY(0, size - 1) == getXY(i, size - i - 1)  && rc && getXY(0, size - 1) != ' ';
    }
    return rc;
}

bool Board::isWinningRow(const int row)
{
    bool rc = true;
    for(int i = 1; i< this->size;  ++i)
    {
        rc = getXY(row, 0) != ' ' && getXY(row, 0) == getXY(row, i) && rc;
        if(!rc)
            break;
    }
    return rc;
}

bool Board::isWinningCol(const int col)
{
    bool rc = true;
    for(int i = 0; i< this->size;  ++i)
    {
        rc = getXY(0, col) != ' ' && getXY(0, col) == getXY(i, col) && rc;
        if(!rc)
            break;
    }
    return rc;
}

bool Board::isMainDiagWin()
{
    bool rc = true;
    //Check main diagonal
    for(int i =1; i< this->size; ++i)
    {
        rc = getXY(0, 0) == getXY(i, i)  && rc && getXY(0, 0) != ' ';
    }
    
    if(rc)
        return true;
    return false;
}
bool Board::isOtDiagWin()
{
    bool rc = true;
    for(int i = 1; i< this->size; ++i)
    {
        rc = getXY(0, size - 1) == getXY(i, size - i - 1)  && rc && getXY(0, size - 1) != ' ';
    }
    if(rc)
        return true;
    return false;
}

void Board::clearBoard()
{
    delete[] this->filled;
}

char Board::getXY(const int x, const int y)
{
    return this->filled[y * size + x];
}
