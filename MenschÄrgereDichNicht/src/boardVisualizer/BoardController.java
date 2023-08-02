package boardVisualizer;

public class BoardController 
{
	private long boardBinary = 344;
	
	public void setBoardBinary(long boardBinary) {this.boardBinary = boardBinary;}
	
	public long getBoardBinary() {return boardBinary;}
	
	public String getBoardBinaryString()
	{return String.format("%64s", Long.toBinaryString(boardBinary)).replace(' ', '0');}
}
