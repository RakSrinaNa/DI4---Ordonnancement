package fr.mrcraftcod.polytech.solutioncalculator;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.LinkedList;

/**
 * Created by Thomas Couchoud (MrCraftCod - zerderr@gmail.com) on 10/12/2017.
 *
 * @author Thomas Couchoud
 * @since 2017-12-10
 */
public class Instance
{
	private int machineCount;
	private int taskCount;
	private ArrayList<Task> tasks;
	private HashMap<Integer, HashMap<Integer, Integer>> times;
	
	private Instance()
	{
		tasks = new ArrayList<>();
		times = new HashMap<>();
	}
	
	public static Instance parse(File file) throws IOException
	{
		Instance instance = new Instance();
		LinkedList<String> lines = new LinkedList<>(Files.readAllLines(Paths.get(file.toURI())));
		
		String[] infos = lines.poll().split(" {2}");
		instance.setMachineCount(Integer.parseInt(infos[0]));
		instance.setTaskCount(Integer.parseInt(infos[1]));
		
		for(int i = 0; i < instance.getMachineCount(); i++)
		{
			infos = lines.poll().split("\t");
			for(int j = 0; j < instance.getTaskCount(); j++)
				instance.setTaskLength(j, i, Integer.parseInt(infos[j]));
		}
		
		infos = lines.poll().split("\t");
		for(int i = 0; i < instance.getTaskCount(); i++)
		{
			instance.getTask(i).setDue(Integer.parseInt(infos[i]));
		}
		
		
		
		for(int i = 0; i < instance.getTaskCount() + 1; i++)
		{
			infos = lines.poll().split("\t");
			for(int j = 0; j < instance.getTaskCount() + 1; j++)
				instance.setTravelTime(i, j, Integer.parseInt(infos[j]));
		}
		
		return instance;
	}
	
	private void setTravelTime(int i, int j, int time)
	{
		if(!times.containsKey(i))
			times.put(i, new HashMap<>());
		times.get(i).put(j, time);
	}
	
	private void setTaskLength(int task, int machine, int duration)
	{
		Task t = getTask(task);
		if(t == null)
			addTask((t = new Task(task)));
		t.setDuration(machine, duration);
	}
	
	private void addTask(Task task)
	{
		tasks.add(task);
	}
	
	Task getTask(int task)
	{
		return tasks.stream().filter(taskk -> taskk.getID() == task).findFirst().orElse(null);
	}
	
	public int getDistance(int i, int j)
	{
		return times.get(i).get(j);
	}
	
	public int getMachineCount()
	{
		return machineCount;
	}
	
	public int getTaskCount()
	{
		return taskCount;
	}
	
	public void setMachineCount(int machineCount)
	{
		this.machineCount = machineCount;
	}
	
	public void setTaskCount(int taskCount)
	{
		this.taskCount = taskCount;
	}
}