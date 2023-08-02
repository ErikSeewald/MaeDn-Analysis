package boardVisualizer.gui;

import java.awt.Color;
import java.awt.Graphics;
import java.awt.Graphics2D;

import javax.swing.JPanel;

import boardVisualizer.BoardController;
import boardVisualizer.VisualizerFrame;

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
		
		//SOUTH BORDER
		g2D.setPaint(Color.black);
		g2D.fillRect(0, this.getHeight() - 5, this.getWidth(), this.getHeight());
	}
}
