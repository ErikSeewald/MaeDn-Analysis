package boardVisualizer;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.awt.event.MouseMotionAdapter;

import boardVisualizer.gui.GUI_South;

public class MouseHandler 
{
	private VisualizerFrame frame;
	private GUI_South guiSouth;
	
	MouseHandler(VisualizerFrame frame, GUI_South guiSouth)
	{
		this.frame = frame;
		this.guiSouth = guiSouth;
	}

	public class ClickListener extends MouseAdapter
	{
		public void mousePressed(MouseEvent e) 
		{
			int x = e.getX(), y = e.getY();
			
			//INSIDE GUI_SOUTH
			if (y > BoardPanel.BOARD_SIZE)
			{guiSouth.clickedDigit(x, y - BoardPanel.BOARD_SIZE);}
		}
	}

	public class DragListener extends MouseMotionAdapter
	{
		public void mouseDragged(MouseEvent e) 
		{
		
		}
	}

	public class ReleaseListener extends MouseAdapter
	{
		public void mouseReleased(MouseEvent e) 
		{
		
		}
	}
}