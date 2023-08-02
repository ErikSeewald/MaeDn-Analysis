package boardVisualizer;
import java.awt.Dimension;
import java.awt.Graphics;
import javax.swing.JPanel;
import main.Board;

public class BoardPanel extends JPanel
{
	private static final long serialVersionUID = -458505873926098128L;
	public static final int BOARD_SIZE = 600;
	
	BoardPanel()
	{
		this.setPreferredSize(new Dimension(BOARD_SIZE, BOARD_SIZE));
	}
	
	public void paint(Graphics g)
	{
		g.drawImage(Board.pngImage, 0, 0, this);
	}
}
