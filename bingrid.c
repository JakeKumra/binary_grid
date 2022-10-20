#include "bingrid.h"

int get_grid_len(char str[]);
bool check_oxo(board* brd, int r, int c);
bool check_pair(board* brd, int r, int c);
void check_counting(board* brd);
bool is_str_valid(char str_in[]);
bool are_same(char c1, char c2);
bool is_UNK(char u);
char flip(char g); 

bool str2board(board* brd, char* str) {  
    if (!is_str_valid(str)) {
        return false;
    }

    int grid_len = get_grid_len(str);
    brd->sz = grid_len;
   
    int cnt = 0;
    for (int i=0; i<grid_len; i++) {
        for (int j=0; j<grid_len; j++) {
            brd->b2d[i][j] = str[cnt];
            cnt++;
        }
    }
    return true;
}

void board2str(char* str, board* brd) {

    int grid_len = brd->sz;
    int max_str_size = (grid_len * grid_len);
   
    int cnt = 0;
    for (int i=0; i<grid_len; i++) {
        for (int j=0; j<grid_len; j++) {
            str[cnt] = brd->b2d[i][j];
            cnt++;
        }
    }
    str[max_str_size] = '\0';
}

bool solve_board(board* brd) {   
    
    int grid_len = brd->sz;
        
    for (int i=0; i<MAX*MAX; i++) { 
        for (int j=0; j<grid_len; j++) {
            for (int k=0; k<grid_len; k++) {
                check_oxo(brd, j, k);
                check_pair(brd, j, k);
                check_counting(brd);
            }
        }
    }
    
    // returns false is grid contains any UNK i.e. unsolvable 
    for (int i=0; i<brd->sz; i++) {
        for (int j=0; j<brd->sz; j++) {
            if(is_UNK(brd->b2d[i][j])) {
                return false;
            }
        }
    }
    return true;
}

void printboard(board* brd) {
    int grid_len = brd->sz;
    for (int i=0; i<grid_len; i++) {
        for (int j=0; j<grid_len; j++) {
            printf("%c", brd->b2d[i][j]);
        }
        printf("\n");
    }
}

bool is_str_valid(char str_in[]) {
    if (strlen(str_in) <= 0 || strlen(str_in) % 2 != 0) {
        return false;
    }
    return true;
}

int get_grid_len(char str[]) {
    int str_len = strlen(str);
    int grid_len = sqrt(str_len);
    return grid_len;
}

bool check_oxo(board* brd, int r, int c) {
    int last_column = brd->sz - 1;
    bool amended = false;

    if (is_UNK(brd->b2d[r][c])) {
        // prevents index from going out of bounds
        if (c != 0 && c != last_column) {
            if ( are_same(brd->b2d[r][c-1], brd->b2d[r][c+1]) && !is_UNK(brd->b2d[r][c-1]) ){
                brd->b2d[r][c] = flip(brd->b2d[r][c-1]);
                amended = true;
            }
        }
        
        // prevens index from going out of bounds
        if (r != 0 && r != last_column) {
            if ( are_same(brd->b2d[r-1][c], brd->b2d[r+1][c]) && !is_UNK(brd->b2d[r-1][c]) ){
                brd->b2d[r][c] = flip(brd->b2d[r-1][c]);
                amended = true;
            }
        }
    }
    return amended;
}

bool check_pair(board* brd, int r, int c) {

    int grid_len = brd->sz;
    bool amended = false;
    
    if (is_UNK(brd->b2d[r][c])) {
        
        // check horizontal pair looking right w/ boundary checking
        if (c < grid_len - 2) {                                   
            if (are_same(brd->b2d[r][c+1], brd->b2d[r][c+2]) && !is_UNK(brd->b2d[r][c+1])) {
                brd->b2d[r][c] = flip(brd->b2d[r][c+1]);
                amended = true;
            }
        }

        // check horizontal pair looking left w/ boundary checking
        if (c > 1) {
            if (are_same(brd->b2d[r][c-1], brd->b2d[r][c-2]) && !is_UNK(brd->b2d[r][c-1])) {
                brd->b2d[r][c] = flip(brd->b2d[r][c-2]);
                amended = true;
            }
        }

        // check vertical pair looking down w/ boundary checking
        if (r < grid_len - 2) {
            if (are_same(brd->b2d[r+1][c], brd->b2d[r+2][c]) && !is_UNK(brd->b2d[r+1][c])) {
                brd->b2d[r][c] = flip(brd->b2d[r+1][c]);
                amended = true;
            }
        }

        // check vertical pair looking up w/ boundary checking
        if (r > 1) {
            if (are_same(brd->b2d[r-1][c], brd->b2d[r-2][c]) && !is_UNK(brd->b2d[r-1][c])) {
                brd->b2d[r][c] = flip(brd->b2d[r-1][c]);
                amended = true;
            }
        }
    }
    return amended;
}

void check_counting(board* brd) {
    
    int grid_len = brd->sz;

    int ONE_cnt = 0;
    int ZERO_cnt = 0;

    // applies counting rule horizontally 
    for (int i=0; i<grid_len;i++) {
        for (int j=0; j<grid_len; j++) {
            if (brd->b2d[i][j] == ONE) {
                ONE_cnt++;
            } else if (brd->b2d[i][j] == ZERO) {
                ZERO_cnt++;
            }

            // fills in the rest of row if the counting condition is met
            if (ONE_cnt == grid_len / 2) {
                for (int j=0; j<grid_len; j++) {
                    if(is_UNK(brd->b2d[i][j])) {
                        brd->b2d[i][j] = ZERO;
                    }
                }
            }

            if (ZERO_cnt == grid_len / 2) {
                for (int j=0; j<grid_len; j++) {
                    if(is_UNK(brd->b2d[i][j])) {
                        brd->b2d[i][j] = ONE;
                    }
                }
            }
        }
        ONE_cnt = 0;
        ZERO_cnt = 0;
    }

    // applies counting rule vertically
    for (int j=0; j<grid_len;j++) {
        for (int i=0; i<grid_len; i++) {
            if (brd->b2d[i][j] == ONE) {
                ONE_cnt++;
            } else if (brd->b2d[i][j] == ZERO) {
                ZERO_cnt++;
            }

            // fills in the rest of row if the counting condition is met
            if (ONE_cnt == grid_len / 2) {
                for (int i=0; i<grid_len; i++) {
                    if(is_UNK(brd->b2d[i][j])) {
                        brd->b2d[i][j] = ZERO;
                    }
                }
            }

            if (ZERO_cnt == grid_len / 2) {
                for (int i=0; i<grid_len; i++) {
                    if(is_UNK(brd->b2d[i][j])) {
                        brd->b2d[i][j] = ONE;
                    }
                }
            }
        }
        ONE_cnt = 0;
        ZERO_cnt = 0;
    }
}


char flip(char g) {
    if (g == ZERO) {
        return ONE;
    } 
    return ZERO;
}

bool are_same(char c1, char c2) {
    if (c1 == c2) {
        return true;
    }
    return false;
}

bool is_UNK(char u) {    
   if (u == UNK) {
    return true;
   } 
   return false;
}


void test(void){

    assert(is_str_valid("..") == true);
    assert(is_str_valid(".0.1") == true);
    assert(is_str_valid("") == false );
    assert(is_str_valid("...") == false );

    assert(get_grid_len("....0...1.....11...0................") == 6);
    assert(get_grid_len("0.............0.00...1.....00.......0.....0..1.......00.........") == 8);
    assert(get_grid_len(".0.1") == 2);

    assert(flip(ZERO) == ONE);
    assert(flip(ONE) == ZERO);

    assert(are_same(ZERO, ZERO) == true);
    assert(are_same(ONE, ONE) == true);
    assert(are_same(ONE, ZERO) == false);

    assert(is_UNK(UNK) == true);
    assert(is_UNK(ZERO) == false);
    assert(is_UNK(ONE) == false);

    board b;
    str2board(&b, "0.0.00.......1.1\n\n"); 
    check_oxo(&b, 0, 1);
    assert(b.b2d[0][1] == ONE);
    check_oxo(&b, 3, 2);
    assert(b.b2d[3][2] == ZERO);
    check_pair(&b, 1, 2);
    assert(b.b2d[1][2] == ONE);

    str2board(&b, "11.........0...0\n\n"); 
    check_pair(&b, 0, 2);
    assert(b.b2d[0][2] == ZERO);
    check_pair(&b, 1, 3);
    assert(b.b2d[1][3] == ONE);
    
    str2board(&b, "....0...1...1.11...0..........00..0.");  
    check_counting(&b);
    assert(b.b2d[2][1] == ZERO);
    assert(b.b2d[2][4] == ZERO);
    assert(b.b2d[2][5] == ZERO);
    assert(b.b2d[5][2] == ONE);
    assert(b.b2d[5][3] == ONE);
    assert(b.b2d[5][5] == ONE);

    str2board(&b, "1...1......0...0");  
    check_counting(&b);
    assert(b.b2d[2][0] == ZERO);
    assert(b.b2d[3][0] == ZERO);
    assert(b.b2d[0][3] == ONE);
    assert(b.b2d[1][3] == ONE);
}
