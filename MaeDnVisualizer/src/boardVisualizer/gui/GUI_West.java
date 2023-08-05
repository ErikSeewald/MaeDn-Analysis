package boardVisualizer.gui;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;

import boardVisualizer.BoardController;

public class GUI_West extends BoardGUI
{
	private static final long serialVersionUID = -4993256464263352137L;
	public static final int PANEL_WIDTH = 300, PANEL_HEIGHT = 600;

	
	public GUI_West(BoardController boardController)
	{
		super(boardController);
		this.setPreferredSize(new Dimension(PANEL_WIDTH, PANEL_HEIGHT));
	}
	
	@Override
	public void paint(Graphics g)
	{
		super.paint(g);
		Graphics2D g2D = (Graphics2D) g;
		
		//TEXT
		g2D.setPaint(Color.white);
		
		g2D.setFont(new Font("", Font.BOLD, 26));
		g2D.drawString("Move the pieces by", 10, 50);
		g2D.drawString("dragging your mouse", 10, 80);
		g2D.drawString("or by adjusting", 10, 110);
		g2D.drawString("the boards binary", 10, 140);
		g2D.drawString("representation!", 10, 170);
		
		g2D.drawString("You can also paste a", 10, 250);
		g2D.drawString("binary board number", 10, 280);
		g2D.drawString("with CTRL-V", 10, 310);
		
		g2D.setFont(new Font("", Font.BOLD, 30));
		g2D.drawString("Board in binary:", 30, PANEL_HEIGHT - 30);
		g2D.drawString("Board in decimal:", 30, PANEL_HEIGHT - 100);
		
		g2D.setFont(new Font("", Font.PLAIN, 20));
		g2D.drawString("(Click to adjust digits)", 30, PANEL_HEIGHT - 5);
		
		g2D.setPaint(Color.black);
		g2D.setFont(new Font("", Font.BOLD, 20));
		g2D.drawString(""+boardController.getBoardBinary(), 30, PANEL_HEIGHT - 70);
	}
}
