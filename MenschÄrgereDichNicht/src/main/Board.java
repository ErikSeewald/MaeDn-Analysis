package main;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;

public class Board 
{
	//BOARD IMAGE
	public static final BufferedImage pngImage;
	static 
	{
		BufferedImage image = null;
		try 
		{
			String currentDirectory = System.getProperty("user.dir");
			image = ImageIO.read(new File(currentDirectory + File.separator + "\\src\\main\\Mensch_ärgere_dich_nicht_4.svg.png"));
		} 
		catch (IOException e)
		{
			System.out.println("Failed to read board image. Replacing with default image.");
			image = new BufferedImage(100, 100, 1);
		}
		pngImage = image;
	}
	
	
}
