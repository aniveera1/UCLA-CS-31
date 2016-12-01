// rats.cpp

#include <iostream>
#include <string>
#include <random>
#include <utility>
#include <cstdlib>
#include <cctype>
using namespace std;

///////////////////////////////////////////////////////////////////////////
// Manifest constants
///////////////////////////////////////////////////////////////////////////

const int MAXROWS = 20;            // max number of rows in the arena
const int MAXCOLS = 20;            // max number of columns in the arena
const int MAXRATS = 100;           // max number of rats allowed

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY      = 0;
const int HAS_POISON = 1;

const int RAT_FINE = 0;             // Rat has eaten no pellets
const int RAT_SLOW = 1;             // Rat has eaten one pellet
const int RAT_DEAD = 2;             // Rat has eaten two pellets

const int YES_ATTEMPT = 0;          // A rat that ate one pellet did not move last turn
const int NO_ATTEMPT  = 1;          // A rat that ate one pellet moved last turn

///////////////////////////////////////////////////////////////////////////
// Type definitions
///////////////////////////////////////////////////////////////////////////

class Arena;

class Rat
{
public:
    // Constructor
    Rat(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;
    
    // Mutators
    void move();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_ratHealth;
    int    m_moveLastTurn;
};

class Player
{
public:
    // Constructor
    Player(Arena* ap, int r, int c);
    
    // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;
    
    // Mutators
    string dropPoisonPellet();
    string move(int dir);
    void   setDead();
    
private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    bool   m_dead;
};

class Arena
{
public:
    // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();
    
    // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     ratCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfRatsAt(int r, int c) const;
    void    display(string msg) const;
    
    // Mutators
    void setCellStatus(int r, int c, int status);
    bool addRat(int r, int c);
    bool addPlayer(int r, int c);
    void moveRats();
    
private:
    int     m_grid[MAXROWS][MAXCOLS];
    int     m_rows;
    int     m_cols;
    Player* m_player;
    Rat*    m_rats[MAXRATS];
    int     m_nRats;
    int     m_turns;
    
    // Helper functions
    void checkPos(int r, int c) const;
    bool isPosInBounds(int r, int c) const;
};

class Game
{
public:
    // Constructor/destructor
    Game(int rows, int cols, int nRats);
    ~Game();
    
    // Mutators
    void play();
    
private:
    Arena* m_arena;
    
    // Helper functions
    string takePlayerTurn();
};

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int  randInt(int min, int max);
bool decodeDirection(char ch, int& dir);
bool attemptMove(const Arena& a, int dir, int& r, int& c);
bool checkMove(const Arena& a, int dir, int r, int c);
int  checkRats(const Arena& a, int dir, int r, int c);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

///////////////////////////////////////////////////////////////////////////
//  Rat implementation
///////////////////////////////////////////////////////////////////////////

Rat::Rat(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** A rat must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "***** Rat created with invalid coordinates (" << r << ","
        << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_ratHealth = RAT_FINE;
    m_moveLastTurn = YES_ATTEMPT;
}

int Rat::row() const
{
    return m_row;
}

int Rat::col() const
{
    return m_col;
}

bool Rat::isDead() const
{
    if(m_ratHealth == RAT_DEAD)
        return true;
    return false;
}

void Rat::move()
{
    //  Generates a random position to move in
    int dir = randInt(NORTH, WEST);
    
    // Determines whether rat should move or not based on whether it already ate a pellet
    // and, if applicable, whether it attempted to move on the last turn
    if (m_ratHealth == RAT_SLOW)
    {
        if (m_moveLastTurn == YES_ATTEMPT)
        {
            attemptMove(*m_arena, dir, m_row, m_col);
            m_moveLastTurn = NO_ATTEMPT;
        }
        else if (m_moveLastTurn == NO_ATTEMPT)
            m_moveLastTurn = YES_ATTEMPT;
    }
    else if (m_ratHealth == RAT_FINE)
        attemptMove(*m_arena, dir, m_row, m_col);
    
    // Checks if pellet is at position and if so, eats it and clears the pellet
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
    {
        if (m_ratHealth == RAT_FINE)
            m_moveLastTurn = NO_ATTEMPT;
        m_ratHealth++;
        m_arena->setCellStatus(m_row, m_col, EMPTY);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Player implementation
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
        << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

string Player::dropPoisonPellet()
{
    if (m_arena->getCellStatus(m_row, m_col) == HAS_POISON)
        return "There's already a poison pellet at this spot.";
    m_arena->setCellStatus(m_row, m_col, HAS_POISON);
    return "A poison pellet has been dropped.";
}

string Player::move(int dir)
{
    if (!attemptMove(*m_arena, dir, m_row, m_col))
        return "Player couldn't move; player stands.";
    else if (m_arena->numberOfRatsAt(m_row, m_col) > 0)
    {
        setDead();
        return "Player walked into a rat and died.";
    }
    else if (dir == NORTH)
        return "Player moved north.";
    else if (dir == EAST)
        return "Player moved east.";
    else if (dir == SOUTH)
        return "Player moved south.";
    else if (dir == WEST)
        return "Player moved west.";
    else
        return "Player couldn't move; player stands.";
}

bool Player::isDead() const
{
    if (m_dead == true)
        return true;
    return false;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Arena implementation
///////////////////////////////////////////////////////////////////////////

Arena::Arena(int nRows, int nCols)
{
    if (nRows <= 0  ||  nCols <= 0  ||  nRows > MAXROWS  ||  nCols > MAXCOLS)
    {
        cout << "***** Arena created with invalid size " << nRows << " by "
        << nCols << "!" << endl;
        exit(1);
    }
    m_rows = nRows;
    m_cols = nCols;
    m_player = nullptr;
    m_nRats = 0;
    m_turns = 0;
    for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
            setCellStatus(r, c, EMPTY);
}

Arena::~Arena()
{
    delete m_player;
    for (int i = 0; i < ratCount(); i++)
        delete m_rats[i];
}

int Arena::rows() const
{
    return m_rows;
}

int Arena::cols() const
{
    return m_cols;
}

Player* Arena::player() const
{
    return m_player;
}

int Arena::ratCount() const
{
    return m_nRats;
}

int Arena::getCellStatus(int r, int c) const
{
    checkPos(r, c);
    return m_grid[r-1][c-1];
}

int Arena::numberOfRatsAt(int r, int c) const
{
    int count = 0;
    for (int i = 0; i < ratCount(); i++)
    {
        if (m_rats[i]->row() == r && m_rats[i]->col() == c && ! m_rats[i]->isDead())
            count++;
    }
    return count;
}

void Arena::display(string msg) const
{
    char displayGrid[MAXROWS][MAXCOLS];
    int r, c;
    
    // Fill displayGrid with dots (empty) and stars (poison pellets)
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            displayGrid[r-1][c-1] = (getCellStatus(r,c) == EMPTY ? '.' : '*');
    
    // Indicate each rat's position
    for (r = 1; r <= rows(); r++)
        for (c = 1; c <= cols(); c++)
            if(numberOfRatsAt(r, c) != 0)
                switch(numberOfRatsAt(r, c))
                {
                    case 1: displayGrid[r-1][c-1] = 'R'; break;
                    case 2: displayGrid[r-1][c-1] = '2'; break;
                    case 3: displayGrid[r-1][c-1] = '3'; break;
                    case 4: displayGrid[r-1][c-1] = '4'; break;
                    case 5: displayGrid[r-1][c-1] = '5'; break;
                    case 6: displayGrid[r-1][c-1] = '6'; break;
                    case 7: displayGrid[r-1][c-1] = '7'; break;
                    case 8: displayGrid[r-1][c-1] = '8'; break;
                    default: displayGrid[r-1][c-1] = '9';
                }
    
    // Indicate player's position
    if (m_player != nullptr)
        displayGrid[m_player->row()-1][m_player->col()-1] = (m_player->isDead() ? 'X' : '@');
    
    // Draw the grid
    clearScreen();
    for (r = 1; r <= rows(); r++)
    {
        for (c = 1; c <= cols(); c++)
            cout << displayGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;
    
    // Write message, rat, and player info
    if (msg != "")
        cout << msg << endl;
    cout << "There are " << ratCount() << " rats remaining." << endl;
    if (m_player == nullptr)
        cout << "There is no player!" << endl;
    else if (m_player->isDead())
        cout << "The player is dead." << endl;
    cout << m_turns << " turns have been taken." << endl;
}

void Arena::setCellStatus(int r, int c, int status)
{
    checkPos(r, c);
    m_grid[r-1][c-1] = status;
}

bool Arena::addRat(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;
    
    // Don't add a rat on a spot with a poison pellet
    if (getCellStatus(r, c) != EMPTY)
        return false;
    
    // Don't add a rat on a spot with a player
    if (m_player != nullptr  &&  m_player->row() == r  &&  m_player->col() == c)
        return false;
    
    // Don't add more rats than the maximum
    if(ratCount() == MAXRATS)
        return false;
    
    m_rats[ratCount()] = new Rat(this, r, c);
    m_nRats++;
    return true;
}

bool Arena::addPlayer(int r, int c)
{
    if (! isPosInBounds(r, c))
        return false;
    
    // Don't add a player if one already exists
    if (m_player != nullptr)
        return false;
    
    // Don't add a player on a spot with a rat
    if (numberOfRatsAt(r, c) > 0)
        return false;
    
    m_player = new Player(this, r, c);
    return true;
}

void Arena::moveRats()
{
    // Randomly moves each rat
    for (int i = 0; i < ratCount(); i++)
        m_rats[i]->move();
    
    // Checks if any rats moved onto the player's position and if so, kills the player
    if (numberOfRatsAt(m_player->row(), m_player->col()) > 0)
        m_player->setDead();
    
    // Save number of rats for future use
    const int counter = m_nRats;
    
    // Checks if any rats died, and if so sets the last element of the m_rats array
    // pointing to that rat, so that it can be deleted
    int temp = ratCount() - 1;
    for (int i = 0; i < ratCount(); i++)
    {
        Rat* holder = nullptr;
        if (m_rats[i]->isDead())
        {
            holder = m_rats[i];
            m_rats[i] = m_rats[temp];
            m_rats[temp] = holder;
            holder = nullptr;
            temp--;
            m_nRats--;
            i--;
        }
    }
    
    // Deletes all nullptrs
    for (int a = counter - 1; a > m_nRats - 1; a--)
        delete m_rats[a];

    // Another turn has been taken
    m_turns++;
}

bool Arena::isPosInBounds(int r, int c) const
{
    return (r >= 1  &&  r <= m_rows  &&  c >= 1  &&  c <= m_cols);
}

void Arena::checkPos(int r, int c) const
{
    if (r < 1  ||  r > m_rows  ||  c < 1  ||  c > m_cols)
    {
        cout << "***** " << "Invalid arena position (" << r << ","
        << c << ")" << endl;
        exit(1);
    }
}

///////////////////////////////////////////////////////////////////////////
//  Game implementation
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nRats)
{
    if (nRats < 0)
    {
        cout << "***** Cannot create Game with negative number of rats!" << endl;
        exit(1);
    }
    if (nRats > MAXRATS)
    {
        cout << "***** Trying to create Game with " << nRats
        << " rats; only " << MAXRATS << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1  &&  cols == 1  &&  nRats > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the rats!" << endl;
        exit(1);
    }
    
    // Create arena
    m_arena = new Arena(rows, cols);
    
    // Add player
    int rPlayer;
    int cPlayer;
    do
    {
        rPlayer = randInt(1, rows);
        cPlayer = randInt(1, cols);
    } while (m_arena->getCellStatus(rPlayer, cPlayer) != EMPTY);
    m_arena->addPlayer(rPlayer, cPlayer);
    
    // Populate with rats
    while (nRats > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
        if (r == rPlayer && c == cPlayer)
            continue;
        m_arena->addRat(r, c);
        nRats--;
    }
}

Game::~Game()
{
    delete m_arena;
}

string Game::takePlayerTurn()
{
    for (;;)
    {
        cout << "Your move (n/e/s/w/x or nothing): ";
        string playerMove;
        getline(cin, playerMove);
        
        Player* player = m_arena->player();
        int dir;
        
        if (playerMove.size() == 0)
        {
            if (recommendMove(*m_arena, player->row(), player->col(), dir))
                return player->move(dir);
            else
                return player->dropPoisonPellet();
        }
        else if (playerMove.size() == 1)
        {
            if (tolower(playerMove[0]) == 'x')
                return player->dropPoisonPellet();
            else if (decodeDirection(playerMove[0], dir))
                return player->move(dir);
        }
        cout << "Player move must be nothing, or 1 character n/e/s/w/x." << endl;
    }
}

void Game::play()
{
    m_arena->display("");
    while ( ! m_arena->player()->isDead()  &&  m_arena->ratCount() > 0)
    {
        string msg = takePlayerTurn();
        Player* player = m_arena->player();
        if (player->isDead())
        {
            cout << msg << endl;
            break;
        }
        m_arena->moveRats();
        m_arena->display(msg);
    }
    if (m_arena->player()->isDead())
        cout << "You lose." << endl;
    else
        cout << "You win." << endl;
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementation
///////////////////////////////////////////////////////////////////////////

// Return a uniformly distributed random int from min to max, inclusive
int randInt(int min, int max)
{
    if (max < min)
        swap(max, min);
    static random_device rd;
    static mt19937 generator(rd());
    uniform_int_distribution<> distro(min, max);
    return distro(generator);
}

// Returns true if the indicated direction is valid and sets an integer to that direction
bool decodeDirection(char ch, int& dir)
{
    switch (tolower(ch))
    {
        default:  return false;
        case 'n': dir = NORTH; break;
        case 'e': dir = EAST;  break;
        case 's': dir = SOUTH; break;
        case 'w': dir = WEST;  break;
    }
    return true;
}

// Return false without changing anything if moving one step from (r,c)
// in the indicated direction would run off the edge of the arena.
// Otherwise, update r and c to the position resulting from the move and
// return true.
bool attemptMove(const Arena& a, int dir, int& r, int& c)
{
    if (dir == NORTH)
    {
        r--;
        if(!(r >= 1  &&  r <= a.rows()  &&  c >= 1  &&  c <= a.cols()))
        {
            r++;
            return false;
        }
        return true;
    }
    else if (dir == EAST)
    {
        c++;
        if(!(r >= 1  &&  r <= a.rows()  &&  c >= 1  &&  c <= a.cols()))
        {
            c--;
            return false;
        }
        return true;
    }
    else if (dir == WEST)
    {
        c--;
        if(!(r >= 1  &&  r <= a.rows()  &&  c >= 1  &&  c <= a.cols()))
        {
            c++;
            return false;
        }
        return true;
    }
    else if (dir == SOUTH)
    {
        r++;
        if(!(r >= 1  &&  r <= a.rows()  &&  c >= 1  &&  c <= a.cols()))
        {
            r--;
            return false;
        }
        return true;
    }
    return false;
}

// Determines whether an indicated direction would move the player off
// the edge of the arena and returns true if the direction if valid or
// false if the direction is invalid.
bool checkMove(const Arena& a, int dir, int r, int c)
{
    if (dir == NORTH)
    {
        r--;
        if(!(r >= 1  &&  r <= a.rows()  &&  c >= 1  &&  c <= a.cols()))
        {
            r++;
            return false;
        }
        return true;
        
    }
    else if (dir == EAST)
    {
        c++;
        if(!(r >= 1  &&  r <= a.rows()  &&  c >= 1  &&  c <= a.cols()))
        {
            c--;
            return false;
        }
        return true;
    }
    else if (dir == WEST)
    {
        c--;
        if(!(r >= 1  &&  r <= a.rows()  &&  c >= 1  &&  c <= a.cols()))
        {
            c++;
            return false;
        }
        return true;
    }
    else if (dir == SOUTH)
    {
        r++;
        if(!(r >= 1  &&  r <= a.rows()  &&  c >= 1  &&  c <= a.cols()))
        {
            r--;
            return false;
        }
        return true;
    }
    return false;
}

// Determines the number of rats next to the player if the player were
// to move in a hypothetical direction.
int checkRats(const Arena& a, int dir, int r, int c)
{
    int ratCounter = MAXRATS;
    if (dir == NORTH)
    {
        r--;
        ratCounter = a.numberOfRatsAt(r - 1, c) + a.numberOfRatsAt(r, c + 1) + a.numberOfRatsAt(r, c - 1);
    }
    else if (dir == EAST)
    {
        c++;
        ratCounter = a.numberOfRatsAt(r, c + 1) + a.numberOfRatsAt(r + 1, c) + a.numberOfRatsAt(r - 1, c);
    }
    else if (dir == WEST)
    {
        c--;
        ratCounter = a.numberOfRatsAt(r, c - 1) + a.numberOfRatsAt(r + 1, c) + a.numberOfRatsAt(r - 1, c);
    }
    else if (dir == SOUTH)
    {
        r++;
        ratCounter = a.numberOfRatsAt(r + 1, c) + a.numberOfRatsAt(r, c + 1) + a.numberOfRatsAt(r, c - 1);
    }
    return ratCounter;
}

// Recommend a move for a player at (r,c):  A false return means the
// recommendation is that the player should drop a poison pellet and not
// move; otherwise, this function sets bestDir to the recommended
// direction to move and returns true.
bool recommendMove(const Arena& a, int r, int c, int& bestDir)
{
    // This array keeps track of how many rats the player would be next to
    // if he or she were to hypothetically move in one of the four directions.
    int ratTracker[NUMDIRS] = {
        MAXRATS, MAXRATS, MAXRATS, MAXRATS
    };
    
    // This boolean will be changed to true if there is at least one safer
    // position to move to.
    bool oneSafeMove = false;
    
    // Determines whether there are any rats directly next to the player in
    // any direction
    
    // In case there is rat directly South of the player
    if (a.numberOfRatsAt(r + 1, c) != 0) // South
    {
        // Checks whether it is possible to move in each direction, and how
        // many rats the player would be next to if he or she were to move in
        // that direction.
        if (checkMove(a, NORTH, r, c) == true && a.numberOfRatsAt(r - 1, c) == 0)
        {
            ratTracker[NORTH] = checkRats(a, NORTH, r, c);
            oneSafeMove = true;
        }
        if (checkMove(a, WEST, r, c) == true && a.numberOfRatsAt(r, c - 1) == 0)
        {
            ratTracker[WEST] = checkRats(a, WEST, r, c);
            oneSafeMove = true;
        }
        if (checkMove(a, EAST, r, c) == true && a.numberOfRatsAt(r, c + 1) == 0)
        {
            ratTracker[EAST] = checkRats(a, EAST, r, c);
            oneSafeMove = true;
        }
        if (oneSafeMove == false)
            return false;
        
        // This code finds which direction will expose the player to the least amount
        // of rats based on the ratTracker array, and sets bestDir to that direction.
        int ratCount = MAXRATS;
        int safestDir = 0;
        for (int i = 0; i < NUMDIRS; i++)
        {
            if (ratTracker[i] < ratCount)
            {
                safestDir = i;
                ratCount = ratTracker[i];
            }
        }
        bestDir = safestDir;
        return true;
    }
    
    // In case there is rat directly North of the player
    if (a.numberOfRatsAt(r - 1, c) != 0) // North
    {
        if (checkMove(a, SOUTH, r, c) == true && a.numberOfRatsAt(r + 1, c) == 0)
        {
            ratTracker[SOUTH] = checkRats(a, SOUTH, r, c);
            oneSafeMove = true;
        }
        if (checkMove(a, WEST, r, c) == true && a.numberOfRatsAt(r, c - 1) == 0)
        {
            ratTracker[WEST] = checkRats(a, WEST, r, c);
            oneSafeMove = true;
        }
        if (checkMove(a, EAST, r, c) == true && a.numberOfRatsAt(r, c + 1) == 0)
        {
            ratTracker[EAST] = checkRats(a, EAST, r, c);
            oneSafeMove = true;
        }
        if (oneSafeMove == false)
            return false;
        
        int ratCount = MAXRATS;
        int safestDir = 0;
        for (int i = 0; i < NUMDIRS; i++)
        {
            if (ratTracker[i] < ratCount)
            {
                safestDir = i;
                ratCount = ratTracker[i];
            }
        }
        bestDir = safestDir;
        return true;
    }
    
    // In case there is rat directly East of the player
    if (a.numberOfRatsAt(r, c + 1) != 0) // East
    {
        if (checkMove(a, WEST, r, c) == true && a.numberOfRatsAt(r, c - 1) == 0)
        {
            ratTracker[WEST] = checkRats(a, WEST, r, c);
            oneSafeMove = true;
        }
        if (checkMove(a, SOUTH, r, c) == true && a.numberOfRatsAt(r + 1, c) == 0)
        {
            ratTracker[SOUTH] = checkRats(a, SOUTH, r, c);
            oneSafeMove = true;
        }
        if (checkMove(a, NORTH, r, c) == true && a.numberOfRatsAt(r - 1, c) == 0)
        {
            ratTracker[NORTH] = checkRats(a, NORTH, r, c);
            oneSafeMove = true;
        }
        if (oneSafeMove == false)
            return false;
        
        int ratCount = MAXRATS;
        int safestDir = 0;
        for (int i = 0; i < NUMDIRS; i++)
        {
            if (ratTracker[i] < ratCount)
            {
                safestDir = i;
                ratCount = ratTracker[i];
            }
        }
        bestDir = safestDir;
        return true;
    }
    
    // In case there is rat directly West of the player
    if (a.numberOfRatsAt(r, c - 1) != 0) // West
    {
        if (checkMove(a, EAST, r, c) == true && a.numberOfRatsAt(r, c + 1) == 0)
        {
            ratTracker[EAST] = checkRats(a, EAST, r, c);
            oneSafeMove = true;
        }
        if (checkMove(a, SOUTH, r, c) == true && a.numberOfRatsAt(r + 1, c) == 0)
        {
            ratTracker[SOUTH] = checkRats(a, SOUTH, r, c);
            oneSafeMove = true;
        }
        if (checkMove(a, NORTH, r, c) == true && a.numberOfRatsAt(r - 1, c) == 0)
        {
            ratTracker[NORTH] = checkRats(a, NORTH, r, c);
            oneSafeMove = true;
        }
        if (oneSafeMove == false)
            return false;
        
        int ratCount = MAXRATS;
        int safestDir = 0;
        for (int i = 0; i < NUMDIRS; i++)
        {
            if (ratTracker[i] < ratCount)
            {
                safestDir = i;
                ratCount = ratTracker[i];
            }
        }
        bestDir = safestDir;
        return true;
    }
    
    // If there are no rats around the player and the player's current position
    // has a poison pellet on it, it will determine if there is another safe
    // position to move to without a poison pellet on it and move the player
    // there.
    if (a.getCellStatus(r, c) == HAS_POISON)
    {
        if (checkMove(a, WEST, r, c) == true && a.numberOfRatsAt(r, c - 1) == 0)
        {
            ratTracker[WEST] = checkRats(a, WEST, r, c);
        }
        if (checkMove(a, EAST, r, c) == true && a.numberOfRatsAt(r, c + 1) == 0)
        {
            ratTracker[EAST] = checkRats(a, EAST, r, c);
        }
        if (checkMove(a, SOUTH, r, c) == true && a.numberOfRatsAt(r + 1, c) == 0)
        {
            ratTracker[SOUTH] = checkRats(a, SOUTH, r, c);
        }
        if (checkMove(a, NORTH, r, c) == true && a.numberOfRatsAt(r - 1, c) == 0)
        {
            ratTracker[NORTH] = checkRats(a, NORTH, r, c);
        }
        
        // Finds a position to move the player to that will expose the player to zero
        // rats and where the cell is empty.
        for (int i = 0; i < NUMDIRS; i++)
            if (ratTracker[i] == 0)
                switch(i)
            {
                case NORTH:
                    if (checkMove(a, NORTH, r, c) == true)
                    {
                        if (a.getCellStatus(r - 1, c) == EMPTY)
                        {
                            bestDir = i;
                            return true;
                        }
                    }
                    break;
                case EAST:
                    if (checkMove(a, EAST, r, c) == true)
                    {
                        if (a.getCellStatus(r, c + 1) == EMPTY)
                        {
                            bestDir = i;
                            return true;
                        }
                    }
                    break;
                case SOUTH:
                    if (checkMove(a, SOUTH, r, c) == true)
                    {
                        if (a.getCellStatus(r + 1, c) == EMPTY)
                        {
                            bestDir = i;
                            return true;
                        }
                    }
                    break;
                case WEST:
                    if (checkMove(a, WEST, r, c) == true)
                    {
                        if (a.getCellStatus(r, c - 1) == EMPTY)
                        {
                            bestDir = i;
                            return true;
                        }
                    }
                    break;
            }
        return false;
    }
    return false;
}

///////////////////////////////////////////////////////////////////////////
// main()
///////////////////////////////////////////////////////////////////////////
int main()
{
}

///////////////////////////////////////////////////////////////////////////
//  clearScreen implementation
///////////////////////////////////////////////////////////////////////////

// DO NOT MODIFY OR REMOVE ANY CODE BETWEEN HERE AND THE END OF THE FILE!!!
// THE CODE IS SUITABLE FOR VISUAL C++, XCODE, AND g++ UNDER LINUX.

// Note to Xcode users:  clearScreen() will just write a newline instead
// of clearing the window if you launch your program from within Xcode.
// That's acceptable.  (The Xcode output window doesn't have the capability
// of being cleared.)

#ifdef _MSC_VER  //  Microsoft Visual C++

#pragma warning(disable : 4005)
#include <windows.h>

void clearScreen()
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    DWORD dwConSize = csbi.dwSize.X * csbi.dwSize.Y;
    COORD upperLeft = { 0, 0 };
    DWORD dwCharsWritten;
    FillConsoleOutputCharacter(hConsole, TCHAR(' '), dwConSize, upperLeft,
                               &dwCharsWritten);
    SetConsoleCursorPosition(hConsole, upperLeft);
}

#else  // not Microsoft Visual C++, so assume UNIX interface

#include <iostream>
#include <cstring>
#include <cstdlib>

void clearScreen()  // will just write a newline in an Xcode output window
{
    static const char* term = getenv("TERM");
    if (term == nullptr  ||  strcmp(term, "dumb") == 0)
        cout << endl;
    else
    {
        static const char* ESC_SEQ = "\x1B[";  // ANSI Terminal esc seq:  ESC [
        cout << ESC_SEQ << "2J" << ESC_SEQ << "H" << flush;
    }
}

#endif
