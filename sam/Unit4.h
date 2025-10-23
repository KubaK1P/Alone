//---------------------------------------------------------------------------

#ifndef Unit4H
#define Unit4H

#define BOARD_SIZE 7
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TForm4 : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *Gra1;
	TMenuItem *Nowa1;
	TPanel *Layout;
	TPanel *InfoPanel;
	TPanel *GamePanel;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *CounterDisplay;
	TLabel *TimerDisplay;
	TLabel *Label6;
	TLabel *Label7;
	TTimer *Timer1;
	TButton *Button1;
	void __fastcall Nowa1Click(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
	int emptySquareX;
	int emptySquareY;
	short boardType;
	int board[BOARD_SIZE][BOARD_SIZE];
	int cSize;
	int cMargin;
	TPanel *selectedCell = nullptr;
	bool isSelectingMove = true;
	bool gameOver = false;
	int pieceCount;
	int elapsedTimeMs = 0;
public:		// User declarations
	__fastcall TForm4(TComponent* Owner);
	int CreateBoard(short bt, int x, int y);
	void DrawBoard();
	void __fastcall CellClick(TObject *Sender);
	bool HasValidMoves();
	int CountPieces();
	void UpdateRemainingPieces();
    void UpdateTimerDisplay(String timestr);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm4 *Form4;
//---------------------------------------------------------------------------
#endif
