package main.gui;

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Font;
import java.awt.Graphics;
import java.awt.Graphics2D;
import main.BoardController;

public class GUI_South extends BoardGUI
{
	private static final long serialVersionUID = -2993256464263352137L;
	public static final int PANEL_WIDTH = 1200, PANEL_HEIGHT = 200;
	
	// BUFFERS FOR WHERE TO RENDER BINARY DIGITS
	private static final int x_buffer = 50;
	private static final int x_spacing = 35;
	private static final int y_buffer_first = 75;
	private static final int y_buffer_second = 155;

	public GUI_South(BoardController boardController)
	{
		super(boardController);
		this.setPreferredSize(new Dimension(PANEL_WIDTH, PANEL_HEIGHT));
	}
	
	public void clickedDigit(int x, int y)
	{
		if (x < x_buffer || x > PANEL_WIDTH - x_buffer) {return;}
		int digit = (x-x_buffer) / x_spacing;

		if (y > y_buffer_first - 5 && y < y_buffer_first + 30)
		{digit = 63 - digit;}
		else if (y > y_buffer_second - 5 && y < y_buffer_second + 30)
		{digit = 31 - digit;}
		else {return;}
		
		boardController.switchBoardDigit(digit);
	}
	
	@Override
	public void paint(Graphics g)
	{
		super.paint(g);
		Graphics2D g2D = (Graphics2D) g;
		
		//BOARD IN BINARY:
		g2D.setFont(new Font("", Font.PLAIN, 15));
		
		//Indices
		for (int i = 0; i < 32; i++)
		{
			g2D.setPaint(Color.white);
			g2D.setFont(new Font("", Font.PLAIN, 15));
			g2D.drawString("" + (63 - i) , 50 + 35 * i, 50);
			
			if (i > 15) //Last 16 bits
			{
				g2D.setFont(new Font("", Font.BOLD, 15)); 
				g2D.setPaint(Color.black);
			}
			g2D.drawString("" + (63 - (i+32)) , 50 + 35 * i, 130);
		}
		
		//Binary digits
		String binary = boardController.getBoardBinaryString();
		g2D.setFont(new Font("", Font.BOLD, 18));
		for (int i = 0; i < 32; i++)
		{
			g2D.setPaint(binary.charAt(i) == '1' ? Color.black : Color.white);
			g2D.drawString(""+binary.charAt(i), x_buffer + 35 * i, y_buffer_first);
			
			g2D.setPaint(binary.charAt(i+32) == '1' ? Color.black : Color.white);
			g2D.drawString(""+binary.charAt(i+32), x_buffer + 35 * i, y_buffer_second);
		}
	}
}
