#include <stdbool.h>
#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>


#define AREA_SIZE_X 28
#define AREA_SIZE_Y 18
#define INIT_BODY_LEN  4
#define BODY_ELEMENT 'x'
#define FOOD_ELEMENT 'o'
#define CLEAR_BLOCK_ASCII  32


typedef struct
{
    char elements[AREA_SIZE_Y][AREA_SIZE_X];
} Area;

Area area;

typedef struct
{
    int x;
    int y;
} Point;

typedef struct
{
    int len;
    bool up;
    bool down;
    bool right;
    bool left;
    Point body[(AREA_SIZE_X - 2) * (AREA_SIZE_Y - 2)];
    bool init;
} Snake;

Snake snake = {
    .len = INIT_BODY_LEN,
    .up = false,
    .down = false,
    .right = false,
    .left = false,
    .init = false,
};

void setOffset()
{
    HANDLE hnd = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD myCord = {
        .X = 0,
        .Y = 3
    };
    SetConsoleCursorPosition(hnd, myCord);
}


void drawArea()
{
    setOffset();
    for (int i = 0; i < AREA_SIZE_Y; ++i)
    {
        for (int j = 0; j < AREA_SIZE_X; ++j)
        {
            printf("%c", area.elements[i][j]);
        }

        printf("\n");
    }
}

void addSnakeToArea()
{
    if (!snake.init)
    {
        int centerX = AREA_SIZE_X / 2;
        int centerY = (AREA_SIZE_Y - snake.len) / 2;

        for (int i = 0; i < snake.len; ++i)
        {
            snake.body[i].x = centerX;
            snake.body[i].y = centerY + i;
        }
        snake.init = true;
    }
    else
    {
        for (int i = 0; i < snake.len; ++i)
        {
            area.elements[snake.body[i].y][snake.body[i].x] = 'x';
        }
    }
}

void generateBaseArea(Area* a)
{
    for (int i = 0; i < AREA_SIZE_Y; ++i)
    {
        for (int j = 0; j < AREA_SIZE_X; ++j)
        {
            if (i == 0 || i == AREA_SIZE_Y - 1)
            {
                a->elements[i][j] = '*';
            }
            else
            {
                if (j == 0) a->elements[i][j] = '*';
                else if (j == AREA_SIZE_X - 1) a->elements[i][j] = '*';
                else a->elements[i][j] = CLEAR_BLOCK_ASCII;
            }
        }
    }
    addSnakeToArea();
}

void gameOver()
{
    printf("Game Over");
    exit(0);
}

void hideConsoleCursor()
{
    HANDLE hndl = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 1;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hndl, &cursorInfo);
}


void checkCollision(int snHead)
{
    for (int j = 1; j < snake.len; ++j)
    {
        if (snake.body[j].x == snake.body[snHead].x
            && snake.body[j].y == snake.body[snHead].y)
            gameOver();
    }
}

void isWithFoodPoint(bool* hasFd, int tailIdx, int headIdx)
{
    if (area.elements[snake.body[headIdx].y][snake.body[headIdx].x] == FOOD_ELEMENT)
    {
        area.elements[snake.body[0].y][snake.body[0].x] = CLEAR_BLOCK_ASCII;
        snake.body[tailIdx + 1].y = snake.body[tailIdx].y;
        snake.body[tailIdx + 1].x = snake.body[tailIdx].x;
        snake.len++;
        *hasFd = false;
    }
}

void moveSnake(bool* pHasFood)
{
    int headIndex = 0;
    int tailIndex = snake.len - 1;
    if (snake.up)
    {
        for (int i = tailIndex; i >= headIndex; i--)
        {
            if (snake.body[headIndex].y == 1) gameOver();
            if (i == 0)
            {
                snake.body[i].y = snake.body[i].y - 1;
                checkCollision(headIndex);
                isWithFoodPoint(pHasFood, tailIndex, headIndex);
            }
            if (i > 0)
            {
                area.elements[snake.body[tailIndex].y][snake.body[tailIndex].x] = CLEAR_BLOCK_ASCII;
                snake.body[i].y = snake.body[i - 1].y;
                snake.body[i].x = snake.body[i - 1].x;
            }
        }
    }
    if (snake.right)
    {
        for (int i = tailIndex; i >= headIndex; i--)
        {
            if (snake.body[headIndex].x == AREA_SIZE_X - 2) gameOver();
            if (i == 0)
            {
                snake.body[i].x = snake.body[i].x + 1;
                checkCollision(headIndex);
                isWithFoodPoint(pHasFood, tailIndex, headIndex);
            }
            if (i > 0)
            {
                area.elements[snake.body[tailIndex].y][snake.body[tailIndex].x] = CLEAR_BLOCK_ASCII;
                snake.body[i].y = snake.body[i - 1].y;
                snake.body[i].x = snake.body[i - 1].x;
            }
        }
    }
    if (snake.left)
    {
        for (int i = tailIndex; i >= headIndex; i--)
        {
            if (snake.body[headIndex].x == 1) gameOver();
            if (i == 0)
            {
                snake.body[i].x = snake.body[i].x - 1;
                checkCollision(headIndex);
                isWithFoodPoint(pHasFood, tailIndex, headIndex);
            }
            if (i > 0)
            {
                area.elements[snake.body[tailIndex].y][snake.body[tailIndex].x] = CLEAR_BLOCK_ASCII;
                snake.body[i].y = snake.body[i - 1].y;
                snake.body[i].x = snake.body[i - 1].x;
            }
        }
    }
    if (snake.down)
    {
        for (int i = tailIndex; i >= headIndex; i--)
        {
            if (snake.body[headIndex].y == AREA_SIZE_Y - 2) gameOver();
            if (i == 0)
            {
                snake.body[i].y = snake.body[i].y + 1;
                checkCollision(headIndex);
                isWithFoodPoint(pHasFood, tailIndex, headIndex);
            }
            if (i > 0)
            {
                area.elements[snake.body[tailIndex].y][snake.body[tailIndex].x] = CLEAR_BLOCK_ASCII;
                snake.body[i].y = snake.body[i - 1].y;
                snake.body[i].x = snake.body[i - 1].x;
            }
        }
    }
}


void getFreePoints(Point* outArr, int* outArrSize)
{
    int counter = 0;
    for (int i = 0; i < AREA_SIZE_Y; ++i)
    {
        for (int j = 0; j < AREA_SIZE_X; ++j)
        {
            if (area.elements[i][j] == CLEAR_BLOCK_ASCII)
            {
                outArr[counter].x = j;
                outArr[counter].y = i;
                counter++;
            }
        }
    }
    *outArrSize = counter;
}

bool hasFood = false;

void spawnFood(bool* hasFd)
{
    if (*hasFd) return;
    int sizeFreePoints = 0;
    Point freePoints[AREA_SIZE_Y * AREA_SIZE_X] = {0};
    // memset(freePoints, 0, sizeof(freePoints));
    getFreePoints(freePoints, &sizeFreePoints);
    int r = rand() % sizeFreePoints;
    area.elements[freePoints[r].y][freePoints[r].x] = FOOD_ELEMENT;
    *hasFd = true;
}


bool* prevPath;
bool hasPrevPath = false;


void changeMovePath(bool* newPath)
{
    if (hasPrevPath)
    {
        *prevPath = false;
    }
    prevPath = newPath;
    *prevPath = true;
    if (!hasPrevPath) hasPrevPath = true;
}


int main(int argc, char* argv[])
{
    srand(time(NULL));
    hideConsoleCursor();
    generateBaseArea(&area);
    while (1)
    {
        moveSnake(&hasFood);
        addSnakeToArea();
        spawnFood(&hasFood);
        drawArea();
        Sleep(140);

        if (_kbhit())
        {
            int k = _getch();

            if (k == 'w' && !snake.down)
            {
                changeMovePath(&snake.up);
            }
            if (k == 'a' && !snake.right)
            {
                changeMovePath(&snake.left);
            }
            if (k == 's' && !snake.up)
            {
                changeMovePath(&snake.down);
            }
            if (k == 'd' && !snake.left)
            {
                changeMovePath(&snake.right);
            }
        }
    }
};
