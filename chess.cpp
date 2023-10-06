#include "chess.hpp"
#include <iostream>

void ChessGame::swap_pieces() {
    board[to_be_eaten.row][to_be_eaten.column].piece_type = to_be_moved.piece_type;
    board[to_be_eaten.row][to_be_eaten.column].color = to_be_moved.color;
    board[to_be_eaten.row][to_be_eaten.column].sprite.setTexture(*to_be_moved.sprite.getTexture());
    board[to_be_moved.row][to_be_moved.column].piece_type = Empty;
    board[to_be_moved.row][to_be_moved.column].color = Null;
    board[to_be_moved.row][to_be_moved.column].sprite.setScale(0, 0);
    board[to_be_eaten.row][to_be_eaten.column].sprite.setScale(1, 1);
}

void ChessGame::unswap_pieces() {
    board[to_be_eaten.row][to_be_eaten.column].piece_type = to_be_eaten.piece_type;
    board[to_be_eaten.row][to_be_eaten.column].color = to_be_eaten.color;
    board[to_be_eaten.row][to_be_eaten.column].sprite.setTexture(*to_be_eaten.sprite.getTexture());
    board[to_be_moved.row][to_be_moved.column].piece_type = to_be_moved.piece_type;
    board[to_be_moved.row][to_be_moved.column].color = to_be_moved.color;
    board[to_be_eaten.row][to_be_eaten.column].sprite.setTexture(*to_be_eaten.sprite.getTexture());
     if(board[to_be_eaten.row][to_be_eaten.column].piece_type == Empty)
        board[to_be_eaten.row][to_be_eaten.column].sprite.setScale(0, 0);
    board[to_be_moved.row][to_be_moved.column].sprite.setScale(1, 1);
}

void ChessGame::switch_player() {
    turn_color = (turn_color == Black)? White: Black;
}

bool ChessGame::is_movement_possible() {
    if(not is_moved_valid())
        return false;
    if(to_be_moved.piece_type == Pawn) {
        if(to_be_eaten.column == to_be_moved.column)
            if(to_be_moved.color == Black)
                return to_be_eaten.row - to_be_moved.row == 1 or (to_be_eaten.row == 3 and to_be_moved.row == 1);
            else return to_be_eaten.row - to_be_moved.row == -1 or (to_be_eaten.row == 4 and to_be_moved.row == 6);
        if(to_be_eaten.column - to_be_moved.column == 1 or to_be_eaten.column - to_be_moved.column == -1)
            if(to_be_moved.color == Black)
                return to_be_eaten.row - to_be_moved.row == 1;
            else return to_be_eaten.row - to_be_moved.row == -1;
        return false;
    }

    if(to_be_moved.piece_type == Rook) {
        return to_be_eaten.column == to_be_moved.column or
               to_be_eaten.row == to_be_moved.row;
    }

    if(to_be_moved.piece_type == Bishop) {
        return (to_be_eaten.column - to_be_moved.column) == (to_be_eaten.row - to_be_moved.row) or
               (to_be_eaten.column - to_be_moved.column) == -(to_be_eaten.row - to_be_moved.row);
    }

    if(to_be_moved.piece_type == Knight) {
        return (to_be_eaten.column - to_be_moved.column) * (to_be_eaten.column - to_be_moved.column) +
               (to_be_eaten.row - to_be_moved.row) * (to_be_eaten.row - to_be_moved.row) == 5;
    }

    if(to_be_moved.piece_type == Queen) {
        bool c1 = to_be_eaten.column == to_be_moved.column or
                  to_be_eaten.row == to_be_moved.row;
        bool c2 = (to_be_eaten.column - to_be_moved.column) == (to_be_eaten.row - to_be_moved.row) or
                  (to_be_eaten.column - to_be_moved.column) == -(to_be_eaten.row - to_be_moved.row);
        bool condition = c1 or c2;
        return condition;
    }

    if(to_be_moved.piece_type == King) {
        return (to_be_eaten.column - to_be_moved.column) * (to_be_eaten.column - to_be_moved.column) +
               (to_be_eaten.row - to_be_moved.row) * (to_be_eaten.row - to_be_moved.row) < 3;
    }
}

bool ChessGame::is_movement_valid() {
    if(not is_moved_valid() or not  is_eaten_valid() or not is_movement_possible())
        return false;

    if(to_be_moved.piece_type != Knight and to_be_moved.piece_type != Pawn) {
        int i = to_be_moved.row, j = to_be_moved.column;
        while(i != to_be_eaten.row or j != to_be_eaten.column) {
            if(to_be_eaten.row > to_be_moved.row)
                ++ i;
            else if(to_be_eaten.row < to_be_moved.row)
                -- i;

            if(to_be_eaten.column > to_be_moved.column)
                ++ j;
            else if(to_be_eaten.column < to_be_moved.column)
                -- j;

            if(board[i][j].piece_type != Empty and (i != to_be_eaten.row or j != to_be_eaten.column))
                return false;
        }
        return true;
    }
    if(to_be_moved.piece_type == Pawn) {
        if(to_be_eaten.column == to_be_moved.column and to_be_eaten.piece_type != Empty)
            return false;
        else if(to_be_eaten.column != to_be_moved.column and to_be_eaten.piece_type == Empty)
            return false;
        return true;
    }
    return true;
}

bool ChessGame::is_moved_valid() {
    return to_be_moved.color == turn_color and
           to_be_moved.row >= 0 and to_be_moved.row <= 7 and to_be_moved.column >= 0 and to_be_moved.column <= 7;
}

bool ChessGame::is_eaten_valid() {
    return to_be_eaten.color != to_be_moved.color and
           to_be_eaten.row >= 0 and to_be_eaten.row <= 7 and to_be_eaten.column >= 0 and to_be_eaten.column <= 7;
}

ChessGame::Piece ChessGame::find_king() {
    for(int i = 0; i < 8; ++ i)
        for(int j = 0; j < 8; ++ j)
            if(board[i][j].piece_type == King and board[i][j].color == turn_color) {
                return board[i][j];
            }
}

bool ChessGame::is_in_check() {
    Piece moved_temp = to_be_moved, eaten_temp = to_be_eaten;
    to_be_eaten = find_king();
    switch_player();
    for(int i = 0; i < 8; ++ i)
        for(int j = 0; j < 8; ++ j) {
            to_be_moved = board[i][j];
            if(is_movement_valid()) {
                switch_player();
                to_be_moved = moved_temp;
                to_be_eaten = eaten_temp;
                return true;
            }
        }
    switch_player();
    to_be_moved = moved_temp;
    to_be_eaten = eaten_temp;
    return false;
}

ChessGame::Game_state ChessGame::check_game_over() {
    Piece moved_temp = to_be_moved, eaten_temp = to_be_eaten;
    for(int i = 0; i < 8; ++ i)
        for(int j = 0; j < 8; ++ j)
            for(int k = 0; k < 8; ++ k)
                for(int l = 0; l < 8; ++ l) {
                    to_be_eaten = board[i][j];
                    to_be_moved = board[k][l];
                    if(is_movement_valid() and to_be_eaten.piece_type != King) {
                        swap_pieces();
                        if (not is_in_check()) {
                            unswap_pieces();
                            to_be_moved = moved_temp;
                            to_be_eaten = eaten_temp;
                            return Running;
                        }
                        unswap_pieces();
                    }
                }
    to_be_moved = moved_temp;
    to_be_eaten = eaten_temp;
    return (is_in_check()? Checkmate: Stalemate);
}
