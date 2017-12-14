package fr.mrcraftcod.polytech.solutioncalculator;

import javafx.util.Pair;
import java.io.File;
import java.io.IOException;
import java.util.*;
import java.util.function.Function;
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
		List<Solution> solutions = new ArrayList<>();
		
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
					solutions.add(Solution.parse(new File(".", arg)));
					break;
				default:
					System.out.println("Invalid options format!");
					System.exit(1);
			}
		}
		
		if(instance == null)
		{
			System.out.println("Invalid options format!");
			System.exit(1);
		}
		
		Instance finalInstance = instance;
		switch(solutions.size())
		{
			case 0:
				System.out.println("Please give at least a solution.");
				break;
			case 2:
				Map<Solution, Integer> results = solutions.stream().collect(Collectors.toMap(Function.identity(), s -> calculate(finalInstance, s)));
				if(results.get(solutions.get(0)) > results.get(solutions.get(1)))
					System.exit(42);
				break;
			default:
				solutions.forEach(s -> calculate(finalInstance, s));
		}
	}
	
	private static int calculate(Instance instance, Solution solution)
	{
		System.out.println("Processing solution " + solution);
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
		System.out.printf("(B: %3d | T: %4d | D: %4d)|--", currTruck, startTime, delay);
		switch(distanceType)
		{
			default:
			case "DUE":
				for(Task t : integers.stream().map(instance::getTask).collect(Collectors.toList()))
				{
					int travelTime = instance.getDistance(currTruck, t.getID());
					duration += travelTime;
					delay += Math.max(0, (startTime + duration) - t.getDue());
					currTruck = t.getID();
					System.out.printf("[%3d]-->(B: %3d | T: %4d | D: %4d)|--", travelTime, currTruck, startTime + duration, delay);
				}
				duration += instance.getDistance(currTruck, instance.getTaskCount());
				break;
			case "CLOSEST":
				break;
		}
		System.out.printf("[%3d]-->(B: %3d | T: %4d | D: %4d)\n", instance.getDistance(currTruck, instance.getTaskCount()), instance.getTaskCount(), startTime + duration, delay);
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
			System.out.println(t.getID() + " --> " + machines.toString());
		}
	}
}
