#include <iostream>
#include<fstream>
#include<stdlib.h>
#include<windows.h>
#include<math.h>
#include<ctime>
#include<conio.h>



#define capacity 8
using namespace std;
const int pawn = 100;
const int knight = 305;
const int rook = 300;
const int bishop = 500;
const int queen = 900;
const int king = 2000;
int board[capacity][capacity];
const int startingboard[capacity][capacity] = { rook, knight, bishop, queen, king, bishop, knight, rook, pawn, pawn,pawn,pawn,pawn,pawn,pawn, pawn,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
-pawn, -pawn, -pawn, -pawn, -pawn, -pawn, -pawn, -pawn, -rook, -knight, -bishop, -queen, -king, -bishop, -knight, -rook };
void initialize(int  b[capacity][capacity], int& turn)
{
	turn = 0;
	for (int ri = 0; ri < capacity; ri++)
	{
		for (int ci = 0; ci < capacity; ci++)
		{
			board[ri][ci] = startingboard[ri][ci];
		}
	}
}
void print(int board[capacity][capacity], int turn)
{
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	system("cls");
	char piece;
	SetConsoleTextAttribute(h, 16);
	cout << endl << "	Player 1" << endl<<endl;
	for (int ri = 0; ri < capacity; ri++)
	{
		for (int ci = 0; ci < capacity; ci++)
		{
			switch (board[ri][ci])
			{
			case 0:
				piece = '-';
				break;
			
			case -pawn:
				piece = 'p';
				break;
			case -rook:
				piece = 'r';
				break;
			case -queen:
				piece = 'q';
				break;
			case -knight:
				piece = 'n';
				break;
			case -bishop:
				piece = 'b';
				break;
			case -king:
				piece = 'k';
				break;
			case pawn:
				piece = 'P';
				break;
			case knight:
				piece = 'N';
				break;
			case queen:
				piece = 'Q';
				break;
			case king:
				piece = 'K';
				break;
			case bishop:
				piece = 'B';
				break;
			case rook:
				piece = 'R';
				break;
			
			}
			SetConsoleTextAttribute(h, 11);
			cout << " " << piece << " ";

		}
		cout << endl;
	}
	SetConsoleTextAttribute(h, 16);
	cout  <<endl<< "	Player 2" << endl;
	SetConsoleTextAttribute(h, 10);
	cout << endl  << "Player " << turn + 1 << "'s turn" << endl << endl;

}
int absolute(int n)
{
	if (n < 0)
		return n * -1;
	return n;
}
int turnchange(int& turn)
{
	turn = (turn + 1) % 2;
	return turn;
}
void getRowColbyLeftClick(int& rpos, int& cpos)
{
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD Events;
	INPUT_RECORD InputRecord;
	SetConsoleMode(hInput, ENABLE_PROCESSED_INPUT | ENABLE_MOUSE_INPUT | ENABLE_EXTENDED_FLAGS);
	FlushConsoleInputBuffer(hInput);
	do
	{
		ReadConsoleInput(hInput, &InputRecord, 1, &Events);
		if (InputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			cpos = InputRecord.Event.MouseEvent.dwMousePosition.X;
			rpos = InputRecord.Event.MouseEvent.dwMousePosition.Y;
			break;
		}
	} while (true);
}
void destinationselect(int& ori, int& oci, int& dri, int& dci)
{
	getRowColbyLeftClick(ori, oci);
	getRowColbyLeftClick(dri, dci);
	oci = oci / 3;
	dci = dci / 3;
	ori = ori - 3;
	dri = dri - 3;

}
void updateboard(int ori, int oci, int board[capacity][capacity], int dri, int dci)
{
	board[dri][dci] = board[ori][oci];
	board[ori][oci] = 0;
}
bool straightcheck(int board[][capacity], int ori, int oci, int dri, int dci)
{
	if (oci == dci)
	{
		for (int i = 1; ori + i < dri; i++)
			if (board[ori + i][oci] != 0)
				return false;
		for (int i = 1; dri + i < ori; i++)
			if (board[dri + i][oci] != 0)
				return false;
	}
	else if (ori == dri)
	{
		for (int i = 1; ori + i < dri; i++)
			if (board[ori + i][oci] != 0)
				return false;
		for (int i = 1; dri + i < ori; i++)
			if (board[dri + i][oci] != 0)
				return false;
	}
	return true;
}

bool RightDiagonalCheck(int Board[][capacity], int ori, int oci, int dri, int dci)
{
	if (oci != dci && ori != dri)
	{
		for (int i = 1; ori + i < dri && oci + i < dci; i++)
		{
			if (Board[ori + i][oci + i] != 0)
				return false;
		}
		for (int i = 1; ori - i > dri && oci + i < dci; i++)
		{
			if (Board[ori - i][oci + i] != 0)
				return false;
		}
		return true;
	}
}
bool leftDiagonalCheck(int Board[][capacity], int ori, int oci, int dri, int dci)
{
	if (oci != dci && ori != dri)
	{
		for (int i = 1; ori + i < dri && oci - i > dci; i++)
		{
			if (Board[ori + i][oci - i] != 0)
				return false;
		}
		for (int i = 1; ori - i > dri && oci - i > dci; i++)
		{
			if (Board[ori - i][oci - i] != 0)
				return false;
		}
		return true;
	}
}
bool rookmove(int board[][capacity], int ori, int oci, int  dri, int dci)
{
	if (ori == dri && oci != dci)
		return true && straightcheck(board, ori, oci, dri, dci);
	if (oci == dci && ori != dri)
		return true && straightcheck(board, ori, oci, dri, dci);
	return false;
}
bool pawnmove(int board[][capacity], int ori, int oci, int dri, int dci)
{
	if (board[ori][oci] > 0)
	{
		if (dri - ori == 1 && absolute(dci - oci) == 0)
			return true;
		if (ori == 1 && dri - ori == 2 && absolute(dci - oci) == 0 && board[dri][dci] == 0)
			return true;
		if (dri - ori == 1 && absolute(dci - oci) == 1 && board[dri][dci] < 0)
			return true;
	}
	else if (board[ori][oci] < 0)
	{
		if (dri - ori == -1 && absolute(dci - oci) == 0)
			return true;
		if (ori == 6 && dri - ori == -2 && absolute(dci - oci) == 0 && board[dri][dci] == 0)
			return true;
		if (dri - ori == -1 && absolute(dci - oci) == 1 && board[dri][dci] > 0)
			return true;
	}
	return false;
}
bool bishopmove(int board[][capacity], int ori, int oci, int dri, int dci)
{
	bool temp;
	if (absolute(ori - dri) == absolute(oci - dci) && absolute(ori - dri) != 0 && absolute(oci - dci) != 0)
		temp = true;
	else
		return false;
	if (oci > dci)
		return temp && leftDiagonalCheck(board, ori, oci, dri, dci);
	else if (oci < dci)
		return temp && RightDiagonalCheck(board, ori, oci, dri, dci);
	return false;
}
bool queenmove(int board[][capacity], int ori, int oci, int dri, int dci)
{
	return(rookmove(board, ori, oci, dri, dci) || bishopmove(board, ori, oci, dri, dci));
}
bool knightmove(int board[][capacity], int ori, int oci, int dri, int dci)
{
	return(absolute(ori - dri) == 2 && absolute(oci - dci) == 1 || absolute(oci - dci) == 2 && absolute(ori - dri) == 1);
}
bool kingmove(int board[][capacity], int ori, int oci, int dri, int dci)
{
	return(absolute(ori - dri) <= 1 && absolute(oci - dci) <= 1);
}
bool rookkill(int board[][capacity], int ori, int oci, int dri, int dci)
{
	while (rookmove(board, ori, oci, dri, dci) == true)
	{
		for (int ri = 0; ri < capacity; ri++)
		{
			for (int ci = 0; ci < capacity; ci++)
			{
				if (board[ri][ci] != '-')
					return true;
			}
		}
	}
	return false;
}
bool pawnkill(int board[][capacity], int ori, int oci, int dri, int dci)
{
	while (pawnmove(board, ori, oci, dri, dci) == true)
	{
		for (int ri = 0; ri < capacity; ri++)
		{
			for (int ci = 0; ci < capacity; ci++)
			{
				if (board[ri][ci] != '-')
					return true;
			}
		}
	}
	return false;
}

bool bishopkill(int board[][capacity], int ori, int oci, int dri, int dci)
{
	while (bishopmove(board, ori, oci, dri, dci) == true)
	{
		for (int ri = 0; ri < capacity; ri++)
		{
			for (int ci = 0; ci < capacity; ci++)
			{
				if (board[ri][ci] != '-')
					return true;
			}
		}
	}
	return false;
}
bool kingkill(int board[][capacity], int ori, int oci, int dri, int dci)
{
	while (kingmove(board, ori, oci, dri, dci) == true)
	{
		for (int ri = 0; ri < capacity; ri++)
		{
			for (int ci = 0; ci < capacity; ci++)
			{
				if (board[ri][ci] != '-')
					return true;
			}
		}
	}
	return false;
}
bool knightkill(int board[][capacity], int ori, int oci, int dri, int dci)
{
	while (knightmove(board, ori, oci, dri, dci) == true)
	{
		for (int ri = 0; ri < capacity; ri++)
		{
			for (int ci = 0; ci < capacity; ci++)
			{
				if (board[ri][ci] != '-')
					return true;
			}
		}
	}
	return false;
}

bool queenkill(int board[][capacity], int ori, int oci, int dri, int dci)
{
	while (queenmove(board, ori, oci, dri, dci) == true)
	{
		for (int ri = 0; ri < capacity; ri++)
		{
			for (int ci = 0; ci < capacity; ci++)
			{
				if (board[ri][ci] != '-')
					return true;
			}
		}
	}
	return false;

}
void kingpos(int board[capacity][capacity], int& kri, int& kci, int turn)
{
	for (int ri = 0; ri < capacity; ri++)
	{
		for (int ci = 0; ci < capacity; ci++)
		{
			switch (turn)
			{
			case 0:
				if (board[ri][ci] == -2000)
				{
					kri = ri;
					kci = ci;
					return;
				}
			case 1:
				if (board[ri][ci] == 2000)
				{
					kri = ri;
					kci = ci;
					return;
				}
			}
		}
	}
}
int checkside(int num)
{
	if (num == 0)
		return 0;
	if (num > 0)
		return 1;
	else
		return 2;
}

bool legalmove(int board[capacity][capacity], int ori, int oci, int dri, int dci, int& turn)
//white is 0 and black is 1
{

	if (ori < 0 || ori > 8 || oci < 0 || oci > 8 || dri < 0 || dri > 8 || dci < 0 || dci > 8)
		return false;
	if (checkside(board[ori][oci]) - 1 != 0 && checkside(board[ori][oci]) - 1 != 1)
		return false;
	if (checkside(board[ori][oci]) == checkside(board[dri][dci]))
		return false;
	if (checkside(board[ori][oci]) - 1 != turn)
		return false;
	switch (abs(board[ori][oci]))
	{
	case 100: return (pawnmove(board, ori, oci, dri, dci));
	case 305: return knightmove(board, ori, oci, dri, dci);
	case 300: return rookmove(board, ori, oci, dri, dci);
	case 500: return bishopmove(board, ori, oci, dri, dci);
	case 900:  return queenmove(board, ori, oci, dri, dci);
	case 2000: return kingmove(board, ori, oci, dri, dci);
	}
	return false;
	turn = turn + 1;
}

bool check(int board[capacity][capacity], int kri, int kci, int turn)
{
	int side = turn;
	turn = turnchange(turn);
	for (int ri = 0; ri < capacity; ri++)
	{
		for (int ci = 0; ci < capacity; ci++)
		{
			if (checkside(board[ri][ci]) - 1 != side && checkside(board[ri][ci]) != 0)
			{
				if (legalmove(board, ri, ci, kri, kci, turn))
				{
					return true;
				}
			}
		}
	}
	return false;
}
bool checkmate(int board[capacity][capacity], int turn, int kri, int kci)
{
	if (check(board, kri, kci, turn) == false)
		return false;
	int k = board[kri][kci];
	int p;
	board[kri][kci] = 0;
	for (int ri = kri - 1; ri <= kri + 1; ri++)
	{
		for (int ci = kci - 1; ci <= kci + 1; ci++)
		{
			if ((kri == ri && ci == kci) == false)
			{
				board[kri][kci] = k;
				if (legalmove(board, kri, kci, ri, ci, turn) == false)
					continue;
				p = board[ri][ci];
				board[ri][ci] = k;
				board[kri][kci] = 0;
				if (check(board, ri, ci, turn) == false)
				{
					board[kri][kci] = k;
					return false;
				}
				board[ri][ci] = p;
			}
		}
	}
	board[kri][kci] = k;
	return  true;
}
bool stalemate(int board[capacity][capacity], int turn, int eri, int eci, int ori, int oci)
{
	while (!legalmove(board, ori, oci, eri, eci, turn) && !checkmate(board, turn, ori, oci))
		return true;
	return false;
}



int main()
{
	int ori, oci, turn;
	bool flag = false;
	initialize(board, turn);
	int dri, dci;
	int kri, kci;
	kingpos(board, kri, kci, turn);
	HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
	do
	{
		print(board, turn);
		if (check(board, kri, kci, turn) == true)
			cout <<endl<< endl << "CHECK!!!" << endl;
		do
		{
			
			SetConsoleTextAttribute(h, 12);
			if (flag)
				cout << "INVALID MOVE!!!!" << endl;
			destinationselect(ori, oci, dri, dci);
			flag = true;
		} while (!legalmove(board, ori, oci, dri, dci, turn));
		flag = false;
		updateboard(ori, oci, board, dri, dci);
		turnchange(turn);
		kingpos(board, kri, kci, turn);

	} while (!checkmate(board, turn, kri, kci) || !stalemate(board, turn, dri, dci, ori, oci));
	if( checkmate(board, turn, kri, kci))
		SetConsoleTextAttribute(h, 13);
		cout << endl << endl << turn + 1 << " IS WINNER!!!!";
}

