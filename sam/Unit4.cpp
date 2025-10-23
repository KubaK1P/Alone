//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit4.h"
#include "Unit1.h"
#include <Vcl.Forms.hpp>
#include <System.SysUtils.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm4 *Form4;
//---------------------------------------------------------------------------
__fastcall TForm4::TForm4(TComponent* Owner)
	: TForm(Owner)
{
	this->cSize = 108;
	this->cMargin = 5;
	this->pieceCount = -1;

	Timer1->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TForm4::Nowa1Click(TObject *Sender)
{
	// open a prompt window asking about the board
	// type and the starting empty spot position
	std::unique_ptr<TPromptStart> dlg(new TPromptStart(this));

	if (dlg->ShowModal() == mrOk) {
	   int x, y;

	   if (!TryStrToInt(dlg->xInput->Text, x) || !TryStrToInt(dlg->yInput->Text, y)) {
		ShowMessage("WprowadŸ koordynaty pola pocz¹tkowego od 1 do " + String(BOARD_SIZE) + " (x, y)");
		return;
	   }

	   if ((x > BOARD_SIZE || x < 1 ) || (y < 1 || y > BOARD_SIZE)) {
		ShowMessage("WprowadŸ koordynaty pola pocz¹tkowego od 1 do " + String(BOARD_SIZE) + " (x, y)");
		return;
	   }

	   this->emptySquareX = x;
	   this->emptySquareY = y;
	   this->boardType = (dlg->RadioButton1->Checked)? 1 : 0;

//	   ShowMessage("Pusta pozycja: (" + String(this->emptySquareX) + ", " + String(this->emptySquareY) + "), Typ planszy: " + String(this->boardType));

	   if (this->CreateBoard(this->boardType, this->emptySquareX, this->emptySquareY) == 1) {
		   return;
	   }
	   this->DrawBoard();
	   this->elapsedTimeMs = 0;
	   Timer1->Enabled = true;
	   TimerDisplay->Font->Color = clBlack;

	} else {
		ShowMessage("Nie to nie, ja mam ca³y dzieñ...");
	}

}
//---------------------------------------------------------------------------
int TForm4::CreateBoard(short bt, int x, int y) {
	const int CLASSIC_TEMPLATE[BOARD_SIZE][BOARD_SIZE] = {
		{-1, -1,  1,  1,  1, -1, -1},
		{-1, -1,  1,  1,  1, -1, -1},
		{ 1,  1,  1,  1,  1,  1,  1},
		{ 1,  1,  1,  1,  1,  1,  1},
		{ 1,  1,  1,  1,  1,  1,  1},
		{-1, -1,  1,  1,  1, -1, -1},
		{-1, -1,  1,  1,  1, -1, -1}
	};

	const int NEW_TEMPLATE[BOARD_SIZE][BOARD_SIZE] = {
		{-1, -1,  1,  1,  1, -1, -1},
		{-1,  1,  1,  1,  1,  1, -1},
		{ 1,  1,  1,  1,  1,  1,  1},
		{ 1,  1,  1,  1,  1,  1,  1},
		{ 1,  1,  1,  1,  1,  1,  1},
		{-1,  1,  1,  1,  1,  1, -1},
		{-1, -1,  1,  1,  1, -1, -1}
	};

	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			this->board[row][col] = (bt != 1)? NEW_TEMPLATE[row][col] : CLASSIC_TEMPLATE[row][col];
		}
	}

	if (x >= 1 && x <= BOARD_SIZE && y >= 1 && y <= BOARD_SIZE)
	{
		int &cell = this->board[y - 1][x - 1];

		if(cell == -1) {
			ShowMessage("Koordynaty (" + String(x) + ", " + String(y) + ") s¹ poza wybran¹ plansz¹");
			return 1;
		}

		cell = 0;

	} else
	{
		ShowMessage("WprowadŸ poprawne wspó³rzêdne w zakresie 1–" + String(BOARD_SIZE));
		return 1;
	}

	this->gameOver = false;
	this->UpdateRemainingPieces();


	// just for debbuging
//		String msg;
//	for (int r = 0; r < BOARD_SIZE; ++r)
//	{
//		for (int c = 0; c < BOARD_SIZE; ++c)
//		{
//			if (board[r][c] == -1)
//				msg += " . ";
//			else
//				msg += String(board[r][c]) + " ";
//		}
//		msg += "\n";
//	}
//	ShowMessage(msg);
return 0;
}
//--------------------------------------------------------------------------------------------------------------
//refreshes the board for the user (maybe) (occasionally) edit: (just so happens to be after every flocking click causing the user three brain tumors)

void TForm4::DrawBoard() {
	const int cellSize = this->cSize;
	const int margin = this->cMargin;

	for (int i = GamePanel->ControlCount - 1; i >= 0; i--)
	{
		TControl *ctrl = GamePanel->Controls[i];
		delete ctrl;     // pretty self explanatory
	}


	// create the TPanels xd actually
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {

			if (board[row][col] == -1) {
				continue;
			}

			TPanel *cell = new TPanel(GamePanel);
			cell->Parent = GamePanel;

			cell->Width = cellSize;
			cell->Height = cellSize;

            HRGN rgn = CreateRoundRectRgn(0, 0, cell->Width, cell->Height, 15, 15); // rounds corners
			SetWindowRgn(cell->Handle, rgn, TRUE);


			cell->Left = col * (cellSize + margin) + margin;
			cell->Top = row * (cellSize + margin) + margin;

			cell->Tag = row * BOARD_SIZE + col;

            cell->ParentBackground = false;
			cell->Color = (board[row][col] == 1) ? clWhite : (board[row][col] == 0 ? clBlue : clWhite);

			// Highlight selected tile
			if (selectedCell != nullptr && cell->Tag == selectedCell->Tag) {
				cell->Color = clBlack; // or clYellow for selection
			}


			cell->OnClick = this->CellClick; // test
		}
	}

}

//-----------------------------------------------------------------------------
		   // handle the clih       please
void __fastcall TForm4::CellClick(TObject *Sender)
{

	if (this->gameOver) {
    selectedCell = nullptr; // extra safety
    return;
}

    TPanel *cell = dynamic_cast<TPanel*>(Sender);
    if (!cell) return;

    int row = cell->Tag / BOARD_SIZE;
    int col = cell->Tag % BOARD_SIZE;

    if (board[row][col] != 0) {
        // Selecting a piece to move
        selectedCell = cell;
    } else if (selectedCell != nullptr) {
        // Selecting target empty cell
        int srcRow = selectedCell->Tag / BOARD_SIZE;
        int srcCol = selectedCell->Tag % BOARD_SIZE;

        int dRow = row - srcRow;
        int dCol = col - srcCol;

        // Must jump exactly 2 spaces horizontally or vertically
        if ((abs(dRow) == 2 && dCol == 0) || (abs(dCol) == 2 && dRow == 0)) {
            // Check that the middle cell has a piece
            int midRow = srcRow + dRow / 2;
            int midCol = srcCol + dCol / 2;

            if (board[midRow][midCol] != 0) {
                // Execute move
                board[row][col] = board[srcRow][srcCol]; // move piece
                board[srcRow][srcCol] = 0;              // empty source
                board[midRow][midCol] = 0;              // remove jumped piece
                selectedCell = nullptr;
            } else {
				ShowMessage("Nie ma nad czym przeskoczyæ!");
			}
        } else {
			ShowMessage("Mo¿esz skakaæ dok³adnie dwa miejsca od czarnego kloca!");
        }
    }

	this->DrawBoard(); // redraw with highlights
	this->UpdateRemainingPieces();

	if (!HasValidMoves()) {
        int remainingPieces = CountPieces();
		gameOver = true; // <-- stop further moves
		 selectedCell = nullptr; // clear any selection
		DrawBoard(); // redraw without highlights
		Timer1->Enabled = false;
		TimerDisplay->Font->Color = clRed;
		ShowMessage("Koniec gry, zosta³o: " + String(remainingPieces)+ " klocek...");
    }
}

//----------------------------------------------------------------------------

bool TForm4::HasValidMoves() {
    for (int r = 0; r < BOARD_SIZE; r++) {
        for (int c = 0; c < BOARD_SIZE; c++) {
			if (board[r][c] == 1) { // only actual pieces
				// Up
				if (r >= 2 && board[r-1][c] == 1 && board[r-2][c] == 0) return true;
				// Down
				if (r <= BOARD_SIZE-3 && board[r+1][c] == 1 && board[r+2][c] == 0) return true;
				// Left
				if (c >= 2 && board[r][c-1] == 1 && board[r][c-2] == 0) return true;
				// Right
				if (c <= BOARD_SIZE-3 && board[r][c+1] == 1 && board[r][c+2] == 0) return true;
			}
		}
	}
    return false; // no valid moves left
}

//------------------------------------------------------------------------------

int TForm4::CountPieces() {
	int count = 0;
	for (int row = 0; row < BOARD_SIZE; row++) {
		for (int col = 0; col < BOARD_SIZE; col++) {
			if(this->board[row][col] == 1) {
				count++;
            }
		}
	}

    return count;
}
  //------------------------------------------------------------------------------

void TForm4::UpdateRemainingPieces() {          // cool little utility method --
	this->pieceCount = this->CountPieces();     // i like it more than i want to admit hihihi :3
	CounterDisplay->Caption = this->pieceCount;
}

//------------------------------------------------------------------------------
void __fastcall TForm4::Timer1Timer(TObject *Sender)
{
	elapsedTimeMs += 100;

    int totalSeconds = elapsedTimeMs / 1000;
    int minutes = totalSeconds / 60;
    int seconds = totalSeconds % 60;

	int tenths = (elapsedTimeMs % 1000) / 100;

	String timeStr = Format("%2.2d:%2.2d,%d", ARRAYOFCONST((minutes, seconds, tenths)));

	this->UpdateTimerDisplay(timeStr);
}
//---------------------------------------------------------------------------

void TForm4::UpdateTimerDisplay(String timestr) {
  TimerDisplay->Caption = timestr;
}
