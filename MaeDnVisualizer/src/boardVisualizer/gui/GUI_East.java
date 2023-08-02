package boardVisualizer.gui;

import java.awt.Dimension;

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

}