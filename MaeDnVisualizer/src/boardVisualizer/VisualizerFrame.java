package boardVisualizer;
import java.awt.BorderLayout;
import java.awt.Color;
import javax.swing.JFrame;
import boardVisualizer.gui.GUI_East;
import boardVisualizer.gui.GUI_South;
import boardVisualizer.gui.GUI_West;
import boardVisualizer.gui.MouseHandler;

public class VisualizerFrame extends JFrame
{
	private static final long serialVersionUID = 2269971701250845501L;
	
	private BoardPanel boardPanel;
	private GUI_South guiSouth;
	private GUI_West guiWest;
	private GUI_East guiEast;
	private BoardController boardController;
	private MouseHandler mouseHandler;
	
	public static final Color background_color = new Color(120, 90, 60);
	
	public VisualizerFrame()
	{
		boardController = new BoardController();
		boardPanel = new BoardPanel(boardController);
		guiSouth = new GUI_South(boardController);
		guiWest = new GUI_West(boardController);
		guiEast = new GUI_East(boardController);
		
		mouseHandler = new MouseHandler(this, guiSouth, boardPanel);
		this.addMouseListener(mouseHandler.new ClickListener());
		this.addMouseMotionListener(mouseHandler.new DragListener());
		this.addMouseListener(mouseHandler.new ReleaseListener());	
		
		this.setTitle("Mensch Ärgere Dich Nicht");
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setResizable(false);
		
		//LAYOUT
		this.add(boardPanel, BorderLayout.CENTER);
		this.add(guiWest, BorderLayout.WEST);
		this.add(guiEast, BorderLayout.EAST);
		this.add(guiSouth, BorderLayout.SOUTH);
		this.pack();
		this.setVisible(true);
	}
	
	public void update()
	{
		guiSouth.repaint();
		guiWest.repaint();
		guiEast.repaint();
		boardPanel.repaint();
	}
}
