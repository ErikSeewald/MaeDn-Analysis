package boardVisualizer.gui;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
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
		
		//TEXT
		g2D.setPaint(Color.white);
		
		g2D.setFont(new Font("", Font.BOLD, 26));
		g2D.drawString("The board is 10x10", 10, 50);
		g2D.drawString("thousand units.", 10, 80);
		g2D.drawString("The last 16 bits", 10, 110);
		g2D.drawString("represent the amount", 10, 140);
		g2D.drawString("of units moved so far.", 10, 170);
		
		g2D.setFont(new Font("", Font.BOLD, 30));
		g2D.drawString("Units moved:", 30, PANEL_HEIGHT - 60);
		
		g2D.setFont(new Font("", Font.PLAIN, 20));
		g2D.drawString("(Last 16 bits - unsigned)", 30, PANEL_HEIGHT - 35);
		
		g2D.setPaint(Color.black);
		g2D.setFont(new Font("", Font.BOLD, 20));
		g2D.drawString(""+boardController.getUnitsMoved(), 30, PANEL_HEIGHT - 10);
	}
}