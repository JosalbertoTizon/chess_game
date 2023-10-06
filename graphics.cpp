#include "chess.hpp"

ChessGame::ChessGame() {
    textures[Pawn_Black].loadFromFile("Piece_Sprites/Pawn_Black.png");
    textures[Rook_Black].loadFromFile("Piece_Sprites/Rook_Black.png");
    textures[Bishop_Black].loadFromFile("Piece_Sprites/Bishop_Black.png");
    textures[Knight_Black].loadFromFile("Piece_Sprites/Knight_Black.png");
    textures[Queen_Black].loadFromFile("Piece_Sprites/Queen_Black.png");
    textures[King_Black].loadFromFile("Piece_Sprites/King_Black.png");
    textures[Pawn_White].loadFromFile("Piece_Sprites/Pawn_White.png");
    textures[Rook_White].loadFromFile("Piece_Sprites/Rook_White.png");
    textures[Bishop_White].loadFromFile("Piece_Sprites/Bishop_White.png");
    textures[Knight_White].loadFromFile("Piece_Sprites/Knight_White.png");
    textures[Queen_White].loadFromFile("Piece_Sprites/Queen_White.png");
    textures[King_White].loadFromFile("Piece_Sprites/King_White.png");

    sprites[Pawn_Black].setTexture(textures[Pawn_Black]);
    sprites[Rook_Black].setTexture(textures[Rook_Black]);
    sprites[Bishop_Black].setTexture(textures[Bishop_Black]);
    sprites[Knight_Black].setTexture(textures[Knight_Black]);
    sprites[Queen_Black].setTexture(textures[Queen_Black]);
    sprites[King_Black].setTexture(textures[King_Black]);
    sprites[Pawn_White].setTexture(textures[Pawn_White]);
    sprites[Rook_White].setTexture(textures[Rook_White]);
    sprites[Bishop_White].setTexture(textures[Bishop_White]);
    sprites[Knight_White].setTexture(textures[Knight_White]);
    sprites[Queen_White].setTexture(textures[Queen_White]);
    sprites[King_White].setTexture(textures[King_White]);

    for (int i = 0; i < 8; ++ i)
        for (int j = 0; j < 8; ++ j) {
            table[i][j].setPosition(sf::Vector2f(80 * j, 80 * i));
            table[i][j].setSize(sf::Vector2f(80, 80));
            ((i + j) % 2 == 0) ? table[i][j].setFillColor(sf::Color(238, 238, 210)) : table[j][i].setFillColor(sf::Color(118, 150, 86));
        }

    window.setFramerateLimit(60);

    for (int i = 0; i < 8; ++ i)
        for (int j = 0; j < 8; ++ j) {
            board[i][j].row = i;
            board[i][j].column = j;
        }
    for(int i = 0; i < 2; ++ i)
        for(int j = 0; j < 8; ++ j) {
            board[i][j].color = Black;
            if(i == 1) {
                board[i][j].piece_type = Pawn;
                board[i][j].sprite = sprites[Pawn_Black];
            }
        }
    for(int i = 6; i < 8; ++ i)
        for(int j = 0; j < 8; ++ j) {
            board[i][j].color = White;
            if(i == 6) {
                board[i][j].piece_type = Pawn;
                board[i][j].sprite = sprites[Pawn_White];
            }
        }
    for(int i = 2; i < 6; ++ i)
        for(int j = 0; j < 8; ++ j) {
            board[i][j].color = Null;
            board[i][j].piece_type = Empty;
        }

    board[0][0].piece_type = Rook; board[7][0].piece_type = Rook;
    board[0][0].sprite = sprites[Rook_Black]; board[7][0].sprite = sprites[Rook_White];
    board[0][1].piece_type = Knight; board[7][1].piece_type = Knight;
    board[0][1].sprite = sprites[Knight_Black]; board[7][1].sprite = sprites[Knight_White];
    board[0][2].piece_type = Bishop; board[7][2].piece_type = Bishop;
    board[0][2].sprite = sprites[Bishop_Black]; board[7][2].sprite = sprites[Bishop_White];
    board[0][3].piece_type = Queen; board[7][3].piece_type = Queen;
    board[0][3].sprite = sprites[Queen_Black]; board[7][3].sprite = sprites[Queen_White];
    board[0][4].piece_type = King; board[7][4].piece_type = King;
    board[0][4].sprite = sprites[King_Black]; board[7][4].sprite = sprites[King_White];
    board[0][5].piece_type = Bishop; board[7][5].piece_type = Bishop;
    board[0][5].sprite = sprites[Bishop_Black]; board[7][5].sprite = sprites[Bishop_White];
    board[0][6].piece_type = Knight; board[7][6].piece_type = Knight;
    board[0][6].sprite = sprites[Knight_Black]; board[7][6].sprite = sprites[Knight_White];
    board[0][7].piece_type = Rook; board[7][7].piece_type = Rook;
    board[0][7].sprite = sprites[Rook_Black]; board[7][7].sprite = sprites[Rook_White];

    for(int i = 0; i < 8; ++ i)
        for(int j = 0; j < 8; ++ j) {
            board[i][j].sprite.setPosition(80 * j + 40 - 45 / 2, 80 * i + 40 - 45 / 2);
        }
}

void ChessGame::start() {
    bool isDragging = false;
    sf::Vector2f offset;
    sf::Texture game_over_texture;
    sf::Sprite game_over_sprite;

    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
            else if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            else if(state == Running and event.type == sf::Event::MouseButtonPressed and event.mouseButton.button == sf::Mouse::Left) {
                // iterate through all the board and find the clicked piece
                // make this piece the piece to_be_moved
                for(int i = 0; i < 8; ++ i)
                    for(int j = 0; j < 8; ++ j)
                        if(table[i][j].getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y))) {
                            to_be_moved = board[i][j];
                            if(is_moved_valid()) {
                                isDragging = true;
                                offset = board[i][j].sprite.getPosition() - sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                            }
                        }
            }
            else if(state == Running and event.type == sf::Event::MouseButtonReleased and event.mouseButton.button == sf::Mouse::Left and isDragging) {
                isDragging = false;
                // put moving sprite back to its original position
                board[to_be_moved.row][to_be_moved.column].sprite.setPosition(to_be_moved.sprite.getPosition());

                // iterate through all the board and find the piece where mouse was released
                // make this piece the piece to_be_eaten
                for(int i = 0; i < 8; ++ i)
                    for(int j = 0; j < 8; ++ j)
                        if(table[i][j].getGlobalBounds().contains(sf::Vector2f(event.mouseButton.x, event.mouseButton.y)))
                            to_be_eaten = board[i][j];

                // check if the movement to_be_moved ---> to_be_eaten is valid
                if(is_movement_valid() and to_be_eaten.piece_type != King) {
                    swap_pieces();
                    // if movement is valid but moving player is in check after that move, undo movement
                    if(is_in_check()) {
                        unswap_pieces();
                    }
                    else {
                        switch_player();
                        if(check_game_over() != Running)
                        {
                            state = check_game_over();
                            if(state == Checkmate)
                                if(turn_color == Black)
                                    game_over_texture.loadFromFile("white_wins.png");
                                else game_over_texture.loadFromFile("black_wins.png");
                            else game_over_texture.loadFromFile("draw.png");
                            game_over_sprite.setTexture(game_over_texture);
                            game_over_sprite.setPosition(320 - 100, 320 - 75);}
                    }
                }
            }
        }

        // while mouse is being dragged, make moving sprite follow it
        if(isDragging) {
            sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
            board[to_be_moved.row][to_be_moved.column].sprite.setPosition(mousePosition.x + offset.x, mousePosition.y + offset.y);
        }
        if(turn_color == Black)
            window.setTitle("Black Turn");
        else window.setTitle("White turn");

        window.clear();

        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                window.draw(table[i][j]);
        for (int i = 0; i < 8; ++i)
            for (int j = 0; j < 8; ++j)
                window.draw(board[i][j].sprite);
        if(state != Running)
            window.draw(game_over_sprite);

        window.display();
    }
}