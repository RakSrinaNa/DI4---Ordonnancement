package fr.mrcraftcod.polytech.solutioncalculator;

import javafx.util.Pair;
import java.io.File;
import java.io.IOException;
import java.util.*;
import java.util.stream.Collectors;

/**
 * Created by Thomas Couchoud (MrCraftCod - zerderr@gmail.com) on 10/12/2017.
 *
 * @author Thomas Couchoud
 * @since 2017-12-10
 */
public class Main
{
	private static final String distanceType = "DUE";
	
	public static void main(String[] args) throws IOException
	{
		Instance instance = null;
		Solution solution = null;
		
		LinkedList<String> arguments = new LinkedList<>(Arrays.asList(args));
		String arg;
		while((arg = arguments.poll()) != null)
		{
			switch(arg)
			{
				case "--instance":
				case "--i":
					if((arg = arguments.poll()) == null)
					{
						System.out.println("Invalid options format!");
						System.exit(1);
					}
					instance = Instance.parse(new File(".", arg));
					break;
				case "--solution":
				case "--s":
					if((arg = arguments.poll()) == null)
					{
						System.out.println("Invalid options format!");
						System.exit(1);
					}
					solution = Solution.parse(new File(".", arg));
					break;
				default:
					System.out.println("Invalid options format!");
					System.exit(1);
			}
		}
		
		if(solution == null || instance == null)
		{
			System.out.println("Invalid options format!");
			System.exit(1);
		}
		
		System.out.println(calculate(instance, solution));
	}
	
	private static int calculate(Instance instance, Solution solution)
	{
		updateReadyTasks(instance, solution);
		return calculateDelay(instance, solution);
	}
	
	private static int calculateDelay(Instance instance, Solution solution)
	{
		int delay = 0;
		int readyTruck = 0;
		for(Integer pack : solution.getDeliveries().keySet().stream().sorted().collect(Collectors.toList()))
		{
			int readyPack = solution.getDeliveries().get(pack).stream().map(instance::getTask).mapToInt(Task::getReadyTime).max().orElse(0);
			readyTruck = Math.max(readyTruck, readyPack);
			Pair<Integer, Integer> res = calculateDeliveryTime(instance, solution.getDeliveries().get(pack), readyTruck);
			readyTruck += res.getKey();
			delay += res.getValue();
		}
		return delay;
	}
	
	private static Pair<Integer, Integer> calculateDeliveryTime(Instance instance, List<Integer> integers, int startTime)
	{
		int duration = 0;
		int delay = 0;
		int currTruck = instance.getTaskCount();
		switch(distanceType)
		{
			default:
			case "DUE":
				for(Task t : integers.stream().map(instance::getTask).sorted(Comparator.comparingInt(Task::getDue)).collect(Collectors.toList()))
				{
					duration += instance.getDistance(currTruck, t.getID());
					delay += Math.max(0, (startTime + duration) - t.getDue());
					currTruck = t.getID();
				}
				duration += instance.getDistance(currTruck, instance.getTaskCount());
				break;
			case "CLOSEST":
				break;
		}
		return new Pair<>(duration, delay);
	}
	
	private static void updateReadyTasks(Instance instance, Solution solution)
	{
		HashMap<Integer, Integer> machines = new HashMap<>();
		
		for(int ID : solution.getProduction())
		{
			Task t = instance.getTask(ID);
			for(int machine = 0; machine < instance.getMachineCount(); machine++)
			{
				int endTime = Math.max(machines.getOrDefault(machine, 0), t.getReadyTime()) + t.getDuration(machine);
				t.setReadyTime(endTime);
				machines.put(machine, endTime);
			}
		}
	}
}
