package boardVisualizer.gui;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Graphics;
import java.awt.Graphics2D;

import boardVisualizer.BoardController;

public class GUI_East extends BoardGUI
{
	private static final long serialVersionUID = -129325664263352137L;
	public static final int PANEL_WIDTH = 300, PANEL_HEIGHT = 600;
	
	public GUI_East(BoardController boardController)
	{
		super(boardController);
		this.setPreferredSize(new Dimension(PANEL_WIDTH, PANEL_HEIGHT));
	}

	@Override
	public void paint(Graphics g)
	{
		super.paint(g);
		Graphics2D g2D = (Graphics2D) g;
		
		//SOUTH BORDER
		g2D.setPaint(Color.black);
		g2D.fillRect(0, this.getHeight() - 5, this.getWidth(), this.getHeight());
	}
}