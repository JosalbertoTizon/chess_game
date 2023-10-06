#include <string>
#include <array>
#include <SFML/Graphics.hpp>

class ChessGame{
public:
    ChessGame();
    void start();

public:
    enum Piece_type {Pawn, Rook, Bishop, Knight, Queen, King, Empty};
    enum Sprite {
        Pawn_Black, Rook_Black, Bishop_Black, Knight_Black, Queen_Black, King_Black,
        Pawn_White, Rook_White, Bishop_White, Knight_White, Queen_White, King_White,
    };
    enum Color {Black, White, Null};
    enum Game_state {Running, Draw, Stalemate, Checkmate};

    class Piece {
    public:
        int row, column;
        Color color = Null;
        Piece_type piece_type = Empty;
        sf::Sprite sprite;
    };

    std::map<Sprite, sf::Texture> textures;
    std::map<Sprite, sf::Sprite> sprites;

    sf::RenderWindow window{sf::VideoMode(640, 640), "White Turn"};
    sf::RectangleShape table[8][8];

    std::array<std::array<Piece, 8>, 8> board;

    Color turn_color = White;

    Piece to_be_moved, to_be_eaten;

    bool is_moved_valid();
    bool is_eaten_valid();

    bool is_movement_possible();
    bool is_movement_valid();

    void swap_pieces();
    void unswap_pieces();
    void switch_player();

    Piece find_king();
    bool is_in_check();

    Game_state state = Running;
    Game_state check_game_over();
};