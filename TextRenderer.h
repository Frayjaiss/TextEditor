#ifndef TEXTRENDERER_H_INCLUDED
#define TEXTRENDERER_H_INCLUDED

#define max(a,b) (((a) > (b)) ? (a) : (b))

typedef struct {
    int row;
    int col;
}Cursor;

typedef struct{

    int rowStartIndx;
    int rowSize;

}rowInfo;

enum cursorMovement {CURSOR_ABSOLUTE,CURSOR_RELATIVE};


//MoveCursor move = {CURSOR_RELATIVE, 0, CURSOR_RELATIVE, 0};
//update_cursor_pos(move);
typedef struct{

    int colType,colVal,rowType,rowVal,physicalMoveFlag;

}MoveCursor;

void update_cursor_pos(MoveCursor cursorMove);

int setup_renderer();

void store_text(char c);

void render_text();

void remove_text();

#endif // TEXTRENDERER_H_INCLUDED
