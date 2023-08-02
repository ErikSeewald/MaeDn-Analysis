package main;
import java.awt.image.BufferedImage;
import java.io.IOException;
import java.net.URL;

import javax.imageio.ImageIO;

public class Board 
{
	//BOARD IMAGE
	public static final BufferedImage pngImage;
	static 
	{
		BufferedImage image = new BufferedImage(100, 100, BufferedImage.TYPE_INT_ARGB);
		try 
		{		    
		    URL input = Main.class.getClassLoader().getResource("Mensch_ärgere_dich_nicht_4.svg.png");
			
			if (input != null) 
			{
				image = ImageIO.read(input);
			}
		} 
		catch (IOException e) 
		{System.out.println("Failed to read board image. Replacing with default image.");}

		pngImage = image;
	}
}
