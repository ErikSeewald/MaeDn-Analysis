package boardVisualizer;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;
import java.util.ArrayList;

import javax.swing.JPanel;

import main.Board;

public class BoardPanel extends JPanel
{
	private static final long serialVersionUID = -458505873926098128L;
	public static final int BOARD_SIZE = 600;
	
	private BoardController boardController;
	
	//PIECES
	private class Piece
	{
		int index;
		int x, y;
		Piece(int index, int x, int y)
		{this.index = index; this.x = x; this.y = y;}
	}
	private Piece selectedPiece;
	private ArrayList<Piece> curPieces;
	private boolean dragging = false;
	
	BoardPanel(BoardController boardController)
	{
		this.boardController = boardController;
		this.setPreferredSize(new Dimension(BOARD_SIZE, BOARD_SIZE));
		this.curPieces = new ArrayList<>();
	}
	
	public void selectPiece(int x, int y)
	{
		selectedPiece = null;
		int grab_buffer = 65;
		
		for (Piece p : curPieces)
		{
			if (x > p.x && x < p.x + grab_buffer && y > p.y && y < p.y + grab_buffer)
			{
				selectedPiece = p;
				return;
			}
		}
	}
	
	public boolean pieceSelected() {return selectedPiece != null;}
	
	public void drag(int x, int y)
	{
		if (selectedPiece == null) {return;}
		
		if (x - 25 > 0 && x < BOARD_SIZE) {selectedPiece.x = x - 25;}
		if (y - 45 > 0 && y - 25 < BOARD_SIZE) {selectedPiece.y = y - 45;}
		dragging = true;
		repaint();
	}
	
	public void releasePiece()
	{
		if (selectedPiece == null) {return;}
		
		//bit of a brute force strategy, but it is not run very often
		//and this is only supposed to be a small visualizer anyway
		int snap_buffer = 20;
		for (int i = 0; i < 48; i++)
		{
			int[] coords = indexToCoords(i);
			if (selectedPiece.x > coords[0] - snap_buffer && selectedPiece.x < coords[0] + snap_buffer
					&& selectedPiece.y > coords[1] - snap_buffer && selectedPiece.y < coords[1]  + snap_buffer)
			{
				//REMOVE OLD POSITION
				boardController.updateBoardBinary(63 - selectedPiece.index);
				
				//ADD NEW POSITION
				boardController.updateBoardBinary(63 - i);
				
				break;
			}
		}
		
		dragging = false;
		updatePieces();
		selectedPiece = null;
	}
	
	private int[] indexToCoords(int index)
	{
		//INDEX: 0-3: waiting | 4-43: board location | 44-47: finished
		int x = 0, y = 0;
		
		if (index >= 0 && index <= 3)
		{
			x = index % 2 == 0 ? 33 : 83;
			y = index < 2 ? 484 : 534;
		}
		
		else if (index >= 4 && index <= 43)
		{
			x = 233;
			y = 533;
			
			//WALK ALONG PATH
			int i = 1;
			while (i < index - 3)
			{
				if (i < 5) {y -= 50;}
				else if (i < 9) {x -= 50;}
				else if (i < 11) {y -= 50;}
				else if (i < 15) {x += 50;}
				else if (i < 19) {y -= 50;}
				else if (i < 21) {x += 50;}
				else if (i < 25) {y += 50;}
				else if (i < 29) {x += 50;}
				else if (i < 31) {y += 50;}
				else if (i < 35) {x -= 50;}
				else if (i < 39) {y += 50;}
				else {x -= 51;}
				i++;
			}
			
		}
		
		else if (index >= 44 && index <= 47)
		{
			x = 283;
			y = 484 - 50*(index - 44);
		}
		
		else {return null;}
		return new int[] {x, y};
	}
	
	private void updatePieces()
	{
		curPieces.removeAll(curPieces);
		String board = boardController.getBoardBinaryString();
		for (int i = 0; i < 64; i++)
		{
			if (board.charAt(i) == '1') 
			{
				int[] coords = indexToCoords(i);
				if (coords != null)
				{curPieces.add(new Piece(i, coords[0], coords[1]));}
			}
		}
	}
	
	public void paint(Graphics g)
	{
		Graphics2D g2D = (Graphics2D) g;
		g2D.drawImage(Board.pngImage, 0, 0, this);

		//PIECES
		if (!dragging) {updatePieces();}
		for (Piece p : curPieces)
		{drawPiece(g2D, p);}
	}
	
	private void drawPiece(Graphics2D g2D, Piece p)
	{		
		//FLOOR
		g2D.setPaint(Color.black);
		g2D.fillOval(p.x, p.y, 33, 33);
		
		//TOP
		g2D.setPaint(Color.DARK_GRAY);
		g2D.fillOval(p.x - 6, p.y - 6, 30, 30);
	}
}
