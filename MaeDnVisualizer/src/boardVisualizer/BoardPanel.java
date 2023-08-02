package boardVisualizer;
import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;

import javax.swing.JPanel;

import main.Board;

public class BoardPanel extends JPanel
{
	private static final long serialVersionUID = -458505873926098128L;
	public static final int BOARD_SIZE = 600;
	
	private BoardController boardController;
	
	BoardPanel(BoardController boardController)
	{
		this.boardController = boardController;
		this.setPreferredSize(new Dimension(BOARD_SIZE, BOARD_SIZE));
	}
	
	public void paint(Graphics g)
	{
		Graphics2D g2D = (Graphics2D) g;
		g2D.drawImage(Board.pngImage, 0, 0, this);

		//PIECES
		String board = boardController.getBoardBinaryString();
		for (int i = 0; i < 64; i++)
		{
			if (board.charAt(i) == '1') {drawPiece(g2D, i);}
		}
	}
	
	private void drawPiece(Graphics2D g2D, int index)
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
		
		else {return;}
		
		drawPieceXY(g2D, x, y);
	}
	
	private void drawPieceXY(Graphics2D g2D, int x, int y)
	{		
		//FLOOR
		g2D.setPaint(Color.black);
		g2D.fillOval(x, y, 33, 33);
		
		//TOP
		g2D.setPaint(Color.DARK_GRAY);
		g2D.fillOval(x - 6, y - 6, 30, 30);
	}
}
