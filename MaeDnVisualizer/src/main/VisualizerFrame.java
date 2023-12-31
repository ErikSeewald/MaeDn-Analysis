package main;

import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Toolkit;
import java.awt.datatransfer.Clipboard;
import java.awt.datatransfer.DataFlavor;
import java.awt.datatransfer.Transferable;
import java.awt.event.KeyAdapter;
import java.awt.event.KeyEvent;
import javax.swing.JFrame;
import main.gui.GUI_East;
import main.gui.GUI_South;
import main.gui.GUI_West;
import main.gui.MouseHandler;

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
		boardController = new BoardController(this);
		boardPanel = new BoardPanel(boardController);
		guiSouth = new GUI_South(boardController);
		guiWest = new GUI_West(boardController);
		guiEast = new GUI_East(boardController);
		
		mouseHandler = new MouseHandler(guiSouth, boardPanel);
		this.addMouseListener(mouseHandler.new ClickListener());
		this.addMouseMotionListener(mouseHandler.new DragListener());
		this.addMouseListener(mouseHandler.new ReleaseListener());	
		
		this.setTitle("Mensch �rgere Dich Nicht");
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setResizable(false);
		
		//LAYOUT
		this.add(boardPanel, BorderLayout.CENTER);
		this.add(guiWest, BorderLayout.WEST);
		this.add(guiEast, BorderLayout.EAST);
		this.add(guiSouth, BorderLayout.SOUTH);
		this.pack();
		this.setVisible(true);
		
		//PASTING BOARDS
		this.addKeyListener(new KeyAdapter() 
		{
            @Override
            public void keyPressed(KeyEvent e) 
            {
            	// GET THE PASTED BINARY NUMBER
            	if (e.isControlDown() && e.getKeyCode() == KeyEvent.VK_V) 
                {
            		Clipboard clipboard = Toolkit.getDefaultToolkit().getSystemClipboard();
            		Transferable transferable = clipboard.getContents(null);

                    if (transferable != null && transferable.isDataFlavorSupported(DataFlavor.stringFlavor)) 
                    {
                    	try 
                    	{
                    		String pasted = (String) transferable.getTransferData(DataFlavor.stringFlavor);

                            if (pasted.length() > 64) {return;}
                    		// MAKE SURE IT IS A BINARY NUMBER
                            for (char c : pasted.toCharArray())
                            {
                            	if (c != '0' && c != '1') 
                            	{return;}
                            }
                            
                            //HANDLE THE 2 LEFT MOST DIGITS SEPERATELY, parseLong BREAKS OTHERWISE
                            if (pasted.length() == 64)
                            {
                            	long mask = Long.parseLong(""+pasted.charAt(0)) << 63;
                                mask |= Long.parseLong(""+pasted.charAt(1)) << 62;
                                boardController.setBoardBinary(mask | Long.parseLong(pasted.substring(2), 2));
                            }
                            
                            else
                            {boardController.setBoardBinary(Long.parseLong(pasted, 2));}
                            
                            
                    	} catch (Exception ex) {ex.printStackTrace();}
                    }
                }
            }
        });
	}
	
	public void update()
	{
		guiSouth.repaint();
		guiWest.repaint();
		guiEast.repaint();
		boardPanel.repaint();
	}
}
