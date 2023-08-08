package main;

public class BoardController 
{
	private long boardBinary = -1152921504606846976L;
	private VisualizerFrame frame;
	
	BoardController(VisualizerFrame frame)
	{this.frame = frame;}
	
	public void update()
	{
		frame.update();
		System.out.println(getBoardBinaryString());
	}
	
	public void switchBoardDigit(long digitIndex)
	{
		boardBinary ^= 1L << digitIndex; // xor with digit at digitIndex
		update();
	}
	
	public void setBoardBinary(long boardBinary) 
	{
		this.boardBinary = boardBinary;
		update();
	}
	
	public long getBoardBinary() {return boardBinary;}
	
	private static final long bitMask48 = 65535L; //0000000000000000000000000000000000000000000000001111111111111111
	public int getUnitsMoved()
	{
		long masked = boardBinary & bitMask48;
		return (int) masked;
	}
	
	public String getBoardBinaryString()
	{return String.format("%64s", Long.toBinaryString(boardBinary)).replace(' ', '0');}
}
