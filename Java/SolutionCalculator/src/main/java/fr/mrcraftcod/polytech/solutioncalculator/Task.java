package fr.mrcraftcod.polytech.solutioncalculator;

import java.util.HashMap;

/**
 * Created by Thomas Couchoud (MrCraftCod - zerderr@gmail.com) on 10/12/2017.
 *
 * @author Thomas Couchoud
 * @since 2017-12-10
 */
public class Task
{
	private int ID;
	private HashMap<Integer, Integer> durations;
	private int readyTime;
	private int due;
	
	public Task(int task)
	{
		this.ID = task;
		durations = new HashMap<>();
	}
	
	public void setDuration(int machine, int duration)
	{
		durations.put(machine, duration);
	}
	
	public int getDuration(int machine)
	{
		return durations.get(machine);
	}
	
	public int getDue()
	{
		return due;
	}
	
	public int getID()
	{
		return ID;
	}
	
	public int getReadyTime()
	{
		return readyTime;
	}
	
	public void setDue(int due)
	{
		this.due = due;
	}
	
	public void setReadyTime(int readyTime)
	{
		this.readyTime = readyTime;
	}
	
	@Override
	public String toString()
	{
		return "Task{" + "ID=" + ID + ", readyTime=" + readyTime + ", due=" + due + '}';
	}
}
