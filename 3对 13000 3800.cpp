#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <math.h>

#define WIDE 10
#define HIGH 20

int board[HIGH][WIDE] = {0};
int map[HIGH][WIDE] = {0};
int tempmap[HIGH][WIDE] = {0};

int now_shape = -1;    
int next_shape = -1;  
int angle = 0;   
int start_x = 0;     
int start_y = 0;   

// ·½¿é¶¨Òå£ºI T O J L S Z X
int shapes[9][4][4][4] = {
    // I
    {{{0,0,0,0},{2,2,2,2},{0,0,0,0},{0,0,0,0}},
     {{2,0,0,0},{2,0,0,0},{2,0,0,0},{2,0,0,0}},
     {{0,0,0,0},{2,2,2,2},{0,0,0,0},{0,0,0,0}},
     {{2,0,0,0},{2,0,0,0},{2,0,0,0},{2,0,0,0}}},

    // T
    {{{0,2,0,0},{2,2,2,0},{0,0,0,0},{0,0,0,0}},
     {{2,0,0,0},{2,2,0,0},{2,0,0,0},{0,0,0,0}},
     {{0,0,0,0},{2,2,2,0},{0,2,0,0},{0,0,0,0}},
     {{0,2,0,0},{2,2,0,0},{0,2,0,0},{0,0,0,0}}},

    // O
    {{{2,2,0,0},{2,2,0,0},{0,0,0,0},{0,0,0,0}},
     {{2,2,0,0},{2,2,0,0},{0,0,0,0},{0,0,0,0}},
     {{2,2,0,0},{2,2,0,0},{0,0,0,0},{0,0,0,0}},
     {{2,2,0,0},{2,2,0,0},{0,0,0,0},{0,0,0,0}}},

    // J
    {{{2,0,0,0},{2,2,2,0},{0,0,0,0},{0,0,0,0}},
     {{2,2,0,0},{2,0,0,0},{2,0,0,0},{0,0,0,0}},
     {{0,0,2,0},{2,2,2,0},{0,0,0,0},{0,0,0,0}},
     {{0,2,0,0},{0,2,0,0},{2,2,0,0},{0,0,0,0}}},

    // L
    {{{0,0,2,0},{2,2,2,0},{0,0,0,0},{0,0,0,0}},
     {{2,0,0,0},{2,0,0,0},{2,2,0,0},{0,0,0,0}},
     {{0,0,0,0},{2,2,2,0},{2,0,0,0},{0,0,0,0}},
     {{2,2,0,0},{0,2,0,0},{0,2,0,0},{0,0,0,0}}},

    // S
    {{{0,0,0,0},{0,2,2,0},{2,2,0,0},{0,0,0,0}},
     {{2,0,0,0},{2,2,0,0},{0,2,0,0},{0,0,0,0}},
     {{0,0,0,0},{0,2,2,0},{2,2,0,0},{0,0,0,0}},
     {{2,0,0,0},{2,2,0,0},{0,2,0,0},{0,0,0,0}}},

    // Z
    {{{0,0,0,0},{2,2,0,0},{0,2,2,0},{0,0,0,0}},
     {{0,2,0,0},{2,2,0,0},{2,0,0,0},{0,0,0,0}},
     {{0,0,0,0},{2,2,0,0},{0,2,2,0},{0,0,0,0}},
     {{0,2,0,0},{2,2,0,0},{2,0,0,0},{0,0,0,0}}},

    // X/E
    {{{0}}}
};

void Makemap() {
    for(int i=0;i<HIGH;i++){
		for(int j=0;j<WIDE;j++){
			map[i][j]=board[i][j];
		}
    }
}

void Maketempmap() {
    for (int i = 0; i < HIGH; i++)
        memcpy(tempmap[i], map[i], sizeof(int) * WIDE);
}

void NewBlock(int now_shape, int rot) {
    for (int i = 0; i < 4; i++) {
        int y = start_y + i;
        if (y < 0 || y >= HIGH) continue;
        for (int j = 0; j < 4; j++) {
            int x = start_x + j;
            if (x < 0 || x >= WIDE) continue;
            if (shapes[now_shape][rot][i][j]) {
                board[y][x] = 2;
            }
        }
    }
}

void tempNewBlock(int now_shape, int rot) {
    for (int i = 0; i < 4; i++) {
        int y = start_y + i;
        if (y < 0 || y >= HIGH) continue;
        for (int j = 0; j < 4; j++) {
            int x = start_x + j;
            if (x < 0 || x >= WIDE) continue;
            if (shapes[now_shape][rot][i][j]) {
                tempmap[y][x] = 2;
            }
        }
    }
}

void ClearBlock(int now_shape, int rot) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shapes[now_shape][rot][i][j]) {
                int y = start_y + i;
                int x = start_x + j;
                if (y >= 0 && y < HIGH && x >= 0 && x < WIDE)
                    board[y][x] = 0;
            }
        }
    }
}

void tempClearBlock(int now_shape, int rot) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (shapes[now_shape][rot][i][j]) {
                int y = start_y + i;
                int x = start_x + j;
                if (y >= 0 && y < HIGH && x >= 0 && x < WIDE)
                    tempmap[y][x] = 0;
            }
        }
    }
}

int GetWidth(int now_shape, int rot) {
    int width = 0;
    for(int j=0; j<4; j++) {
        for(int i=0; i<4; i++) {
            if(shapes[now_shape][rot][i][j]) {
                if(j+1 > width) width = j+1;
                break;
            }
        }
    }
    return width;
}

int Ifdown(int now_shape, int rot) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int x = start_x + j;
            int y = start_y + i;
            if (shapes[now_shape][rot][i][j]) {
                if (x < 0 || x >= WIDE || y >= HIGH) return 1;
                if (y == HIGH - 1 || (y < HIGH-1 && map[y + 1][x] > 0))
                    return 1;
            }
        }
    }
    return 0;
}


typedef struct {
    int count;
    int depth;
    int connected;
    int blocked;
} HoleMetrics;

HoleMetrics CalculateDetailedHoles() {
    HoleMetrics metrics = {0, 0, 0, 0};
    int prev_holes[WIDE] = {0};
    
    for (int j = 0; j < WIDE; j++) {
        int block_above = 0;
        int current_depth = 0;
        int holes_in_column = 0;
        
        for (int i = 0; i < HIGH; i++) {
            if (tempmap[i][j] > 0) {
                block_above = 1;
                if (current_depth > 0) {
                    metrics.blocked++;
                }
            } else if (block_above) {
                metrics.count++;
                metrics.depth += i;
                current_depth++;
                holes_in_column++;
                if (j > 0 && prev_holes[j-1] > 0) {
                    metrics.connected++;
                }
            }
        }
        prev_holes[j] = holes_in_column;
    }
    
    return metrics;
}

float CalculateHolesScore() {
    HoleMetrics metrics = CalculateDetailedHoles();
    
    // Weight different aspects of holes
    float hole_score = metrics.count * 4.0 +
                      metrics.depth * 0.5 +
                      metrics.connected * 2.0 +
                      metrics.blocked * 3.0;
                      
    return hole_score;
}

int CalculateBumpiness() {
    int heights[WIDE];
    for (int j = 0; j < WIDE; j++) {
        heights[j] = 0;
        for (int i = 0; i < HIGH; i++) {
            if (tempmap[i][j]) {
                heights[j] = HIGH - i;
                break;
            }
        }
    }

    int bumpiness = 0;
    for (int j = 1; j < WIDE; j++)
        bumpiness+=abs(heights[j]-heights[j-1]);
    return bumpiness;
}

int CalculateMaxHeight() {
    int max_height = 0;
    for (int j = 0; j < WIDE; j++) {
        for (int i = 0; i < HIGH; i++) {
            if (tempmap[i][j]) {
                int h = HIGH - i;
                if (h > max_height) max_height = h;
                break;
            }
        }
    }
    return max_height;
}

int tempclearline() {
    int lines = 0;
    for (int i = 0; i < HIGH; i++) {
        int full = 1;
        for (int j = 0; j < WIDE; j++) {
            if (tempmap[i][j]==0) {
                full = 0;
                break;
            }
        }
        if (full) lines++;
    }
    return lines;
}

int clearline() {
    int lines = 0;
    for (int i = HIGH - 1; i >= 0; i--) {
        int full = 1;
        for (int j = 0; j < WIDE; j++) {
            if (!board[i][j]) {
                full = 0;
                break;
            }
        }
        if (full) {
            lines++;
            for (int k = i; k > 0; k--)
            memcpy(board[k], board[k - 1], sizeof(board[k]));
            memset(board[0], 0, sizeof(board[0]));
            i++;
        }
    }
    switch(lines) {
        case 1: return 100;
        case 2: return 300;
        case 3: return 500;
        case 4: return 800;
        default: return 0;
    }
}

int Ifgameover() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < WIDE; j++) {
            if (map[i][j])
                return 1;
        }
    }
    return 0;
}

int InputNow() {
    char c;
    scanf(" %c", &c);
    if(!strchr("ITOJLSZXE", c)) return -1;
    switch(c) {
        case 'I': return 0;
        case 'T': return 1;
        case 'O': return 2;
        case 'J': return 3;
        case 'L': return 4;
        case 'S': return 5;
        case 'Z': return 6;
        case 'X': return 7;
        case 'E': return 8;
        default: return -1;
    }
}

int InputNext() {
    char c;
    scanf(" %c", &c);
    if(!strchr("ITOJLSZXE", c)) return -1;
    switch(c) {
        case 'I': return 0;
        case 'T': return 1;
        case 'O': return 2;
        case 'J': return 3;
        case 'L': return 4;
        case 'S': return 5;
        case 'Z': return 6;
        case 'X': return 7;
        case 'E': return 8;
        default: return -1;
    }
}

void Judge(int now_shape) {
    float best_score = -1e9;
    int best_x = 0, best_rot = 0;

    Maketempmap();

    for (int rot = 0; rot < 4; rot++) {
        int width = GetWidth(now_shape, rot);
        int xmax = WIDE - width;

        for (int x = 0; x <= xmax; x++) {
            start_x = x;
            start_y = 0;

            while (!Ifdown(now_shape, rot)) start_y++;

            tempNewBlock(now_shape, rot);

            int lines = tempclearline();
            int bumpiness = CalculateBumpiness();
            int max_height = CalculateMaxHeight();
            int holes = CalculateHolesScore();

            float score = lines * 1000.0 - holes*1.0 - bumpiness * 2.0 - max_height * 1.5;

            if (score > best_score || (score == best_score && abs(x - 5) < abs(best_x - 5))) {
                best_score = score;
                best_x = x;
                best_rot = rot;
            }

            tempClearBlock(now_shape, rot);
            
        }
        
    }

    angle = best_rot;
    start_x = best_x;
    start_y = 0;
}

int main() {
    int total_score = 0;
    now_shape = InputNow();
    next_shape = InputNext();

    while (!Ifgameover()) {
        Makemap();
        
        Judge(now_shape);
        int safety_counter = 0;
        while (!Ifdown(now_shape, angle) && safety_counter < HIGH) {
            start_y++;
            safety_counter++;
        }
        NewBlock(now_shape, angle);
        int line_score = clearline();
        total_score += line_score;
        Makemap();
        printf("%d %d\n%d\n", angle, start_x, total_score);

        fflush(stdout);

        if (next_shape == 7) break;
        if (next_shape == 8) exit(0);

        now_shape = next_shape;
        next_shape = InputNext();

        angle = 0;
        start_x = 0;
        start_y = 0;
    }
return 0;
}
