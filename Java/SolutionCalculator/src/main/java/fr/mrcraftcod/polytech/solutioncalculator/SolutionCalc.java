package fr.mrcraftcod.polytech.solutioncalculator;

/**
 * Created by Thomas Couchoud (MrCraftCod - zerderr@gmail.com) on 08/01/2018.
 *
 * @author Thomas Couchoud
 * @since 2018-01-08
 */
public class SolutionCalc
{
	private final Instance instance;
	
	public enum Program
	{
		PYTHON, C
	}
	
	private final Program program;
	private final Solution solution;
	private final int calcScore;
	
	public SolutionCalc(Program program, Instance instance, Solution solution, int calcScore)
	{
		this.program = program;
		this.instance = instance;
		this.solution = solution;
		this.calcScore = calcScore;
	}
	
	public Program getProgram()
	{
		return program;
	}
	
	public Solution getSolution()
	{
		return solution;
	}
	
	public int getCalcScore()
	{
		return calcScore;
	}
	
	public Instance getInstance()
	{
		return instance;
	}
	
	@Override
	public String toString()
	{
		return String.format("%s: %d", getProgram().name(), getCalcScore());
	}
}
