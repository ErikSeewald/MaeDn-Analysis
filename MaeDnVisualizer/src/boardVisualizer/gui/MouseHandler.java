package boardVisualizer.gui;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;

import boardVisualizer.BoardPanel;
import boardVisualizer.VisualizerFrame;

public class MouseHandler 
{
	private VisualizerFrame frame;
	private GUI_South guiSouth;
	private BoardPanel boardPanel;
	
	public MouseHandler(VisualizerFrame frame, GUI_South guiSouth, BoardPanel boardPanel)
	{
		this.frame = frame;
		this.guiSouth = guiSouth;
		this.boardPanel = boardPanel;
	}

	public class ClickListener extends MouseAdapter
	{
		public void mousePressed(MouseEvent e) 
		{
			int x = e.getX(), y = e.getY();
			
			//INSIDE GUI_SOUTH
			if (y > BoardPanel.BOARD_SIZE)
			{guiSouth.clickedDigit(x, y - BoardPanel.BOARD_SIZE);}
			
			frame.update();
		}
	}

	public class DragListener extends MouseMotionAdapter
	{
		public void mouseDragged(MouseEvent e) 
		{
			int x = e.getX(), y = e.getY();
			
			if (!boardPanel.pieceSelected()) {boardPanel.selectPiece(x - GUI_West.PANEL_WIDTH, y);}
			boardPanel.drag(x - GUI_West.PANEL_WIDTH, y);		
		}
	}

	public class ReleaseListener extends MouseAdapter
	{
		public void mouseReleased(MouseEvent e) 
		{
			boardPanel.releasePiece();	
			frame.update();
		}
	}
}