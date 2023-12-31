package main.gui;

import java.awt.Graphics;
import java.awt.Graphics2D;
import javax.swing.JPanel;
import main.BoardController;
import main.VisualizerFrame;

public abstract class BoardGUI extends JPanel
{
	private static final long serialVersionUID = 7780227371179851565L;

	protected BoardController boardController;
	
	BoardGUI(BoardController boardController)
	{this.boardController = boardController;}
	
	public void paint(Graphics g)
	{
		Graphics2D g2D = (Graphics2D) g;
		
		g2D.setPaint(VisualizerFrame.background_color);
		g2D.fillRect(0, 0, this.getWidth(), this.getHeight());
	}
}
