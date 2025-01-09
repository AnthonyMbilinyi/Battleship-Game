import random

class Ship:
    def __init__(self, size):
        self.size = size
        self.hits = 0

    def hit(self):
        self.hits += 1

    def is_sunk(self):
        return self.hits == self.size

class Board:
    def __init__(self, size=10):
        self.size = size
        self.board = [['O' for _ in range(size)] for _ in range(size)]
        self.ships = []

    def place_ship(self, ship, row, col, orientation):
        if orientation == 'horizontal':
            if col + ship.size > self.size:
                return False
            for i in range(ship.size):
                if self.board[row][col + i] != 'O':
                    return False
            for i in range(ship.size):
                self.board[row][col + i] = 'S'
        elif orientation == 'vertical':
            if row + ship.size > self.size:
                return False
            for i in range(ship.size):
                if self.board[row + i][col] != 'O':
                    return False
            for i in range(ship.size):
                self.board[row + i][col] = 'S'
        self.ships.append(ship)
        return True

    def attack(self, row, col):
        if not (0 <= row < self.size and 0 <= col < self.size):
            return False
        if self.board[row][col] == 'O' or self.board[row][col] == 'S':
            self.board[row][col] = 'X'
            for ship in self.ships:
                if row == ship.row and col == ship.col:
                    ship.hit()
            return True
        else:
            return False

    def is_game_over(self):
        for ship in self.ships:
            if not ship.is_sunk():
                return False
        return True

    def print_board(self):
        print("   ", end="")
        for i in range(self.size):
            print(f" {i}", end="")
        print()
        for i in range(self.size):
            print(f"{i} |", end="")
            for j in range(self.size):
                print(f" {self.board[i][j]}", end="")
            print("|")
        print()

class Player:
    def __init__(self, name, board):
        self.name = name
        self.board = board

    def make_move(self):
        while True:
            try:
                row = int(input(f"{self.name}, enter row (0-{self.board.size-1}): "))
                col = int(input(f"{self.name}, enter column (0-{self.board.size-1}): "))
                if self.board.attack(row, col):
                    break
                else:
                    print("Invalid move. Try again.")
            except ValueError:
                print("Invalid input. Please enter numbers.")

class AI:
    def __init__(self, name, board):
        self.name = name
        self.board = board

    def make_move(self):
        row = random.randint(0, self.board.size - 1)
        col = random.randint(0, self.board.size - 1)
        while not self.board.attack(row, col):
            row = random.randint(0, self.board.size - 1)
            col = random.randint(0, self.board.size - 1)
        print(f"{self.name} attacks ({row}, {col})")

def create_ships(board):
    ships = []
    ships.append(Ship(5))  # Carrier
    ships.append(Ship(4))  # Battleship
    ships.append(Ship(3))  # Cruiser
    ships.append(Ship(3))  # Submarine
    ships.append(Ship(2))  # Destroyer
    return ships

def place_ships_randomly(board, ships):
    for ship in ships:
        while True:
            row = random.randint(0, board.size - 1)
            col = random.randint(0, board.size - 1)
            orientation = random.choice(['horizontal', 'vertical'])
            if board.place_ship(ship, row, col, orientation):
                break

def play_game():
    player_board = Board()
    player_ships = create_ships(player_board)
    place_ships_randomly(player_board, player_ships)

    ai_board = Board()
    ai_ships = create_ships(ai_board)
    place_ships_randomly(ai_board, ai_ships)

    player = Player("Player", ai_board)
    ai = AI("AI", player_board)

    while True:
        player.make_move()
        if ai_board.is_game_over():
            print("Player wins!")
            break

        ai.make_move()
        if player_board.is_game_over():
            print("AI wins!")
            break

if __name__ == "__main__":
    play_game()
