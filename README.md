Project Overview
PlotaGame is a two‑player online board‑game platform focused on classic tabletop games. The project aims to deliver interactive gameplay with a clean UI, synchronized moves, and strict rule enforcement for each game.

  Key Features 🔑:
  
  Online two‑player gameplay with real‑time socket communication/
    Move validation (no overlapping pieces and only valid board positions)/
  Game‑specific rule enforcement/
    Real‑time move synchronization for both players/
  Smooth and understandable user interface
  
  Implemented Games 🎮:
  Plota (main project game)/
  Connect Four
  
Technologies & Tools 🔨:
  Language/Framework: Qt / C++
Networking: Socket
  Version Control: Git / GitHub
Running the Project

  Prerequisites :
Qt and a compatible compiler/
  Git/
Local Run Steps/
  Clone the repository/
Open the project file in Qt Creator/
  Build and Run

Project Structure (High‑Level)🧱:

Client: UI and player interaction/
  Server: Synchronization and move validation/
Game Logic: Rules and win/lose conditions

  Rules & Mechanics 📐:
Game starts based on host settings/
  Pieces can only be placed in valid positions/
If a time limit is enabled, each turn is timed/
Moves are shown simultaneously for both players

Team Contributions
Mahdi Habibi — Server Part /
AmirMohamad Mofateh — Client Part
