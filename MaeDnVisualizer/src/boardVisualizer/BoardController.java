package boardVisualizer;

public class BoardController 
{
	private long boardBinary = -1152921504606846976L;
	
	public void updateBoardBinary(long changeIndex)
	{
		long mask = (long) Math.pow(2, changeIndex);
		if (changeIndex == 63) {mask = Long.parseUnsignedLong("9223372036854775808");}
		
		boardBinary ^= mask;
	}
	
	public void setBoardBinary(long boardBinary) {this.boardBinary = boardBinary;}
	
	public long getBoardBinary() {return boardBinary;}
	
	public String getBoardBinaryString()
	{return String.format("%64s", Long.toBinaryString(boardBinary)).replace(' ', '0');}
}