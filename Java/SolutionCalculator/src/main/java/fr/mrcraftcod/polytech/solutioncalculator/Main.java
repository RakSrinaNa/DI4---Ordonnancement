package fr.mrcraftcod.polytech.solutioncalculator;

import javafx.util.Pair;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;
import java.util.function.Function;
import java.util.regex.Pattern;
import java.util.stream.Collectors;

/**
 * Created by Thomas Couchoud (MrCraftCod - zerderr@gmail.com) on 10/12/2017.
 *
 * @author Thomas Couchoud
 * @since 2017-12-10
 */
public class Main
{
	private static final String OS = System.getProperty("os.name").toLowerCase();
	private static final String distanceType = "DUE";
	
	public static void main(String[] args) throws IOException
	{
		boolean calculate = false;
		
		Path cExecutable = null;
		Path pythonExecutable = null;
		List<Instance> instances = new ArrayList<>();
		List<Solution> solutions = new ArrayList<>();
		
		LinkedList<String> arguments = new LinkedList<>(Arrays.asList(args));
		String arg;
		while((arg = arguments.poll()) != null)
		{
			switch(arg)
			{
				case "--calc":
					calculate = true;
					break;
				case "--c":
					if((arg = arguments.poll()) == null)
					{
						System.out.println("Invalid options format for `c`!");
						System.out.flush();
						System.exit(1);
					}
					cExecutable = Paths.get(arg);
					break;
				case "--p":
					if((arg = arguments.poll()) == null)
					{
						System.out.println("Invalid options format for `p`!");
						System.out.flush();
						System.exit(1);
					}
					pythonExecutable = Paths.get(arg);
					break;
				case "--instance":
				case "--i":
					if((arg = arguments.poll()) == null)
					{
						System.out.println("Invalid options format for `i`!");
						System.out.flush();
						System.exit(1);
					}
					instances.add(Instance.parse(Paths.get(arg)));
					break;
				case "--solution":
				case "--s":
					if((arg = arguments.poll()) == null)
					{
						System.out.println("Invalid options format for `s`!");
						System.out.flush();
						System.exit(1);
					}
					solutions.add(Solution.parse(Paths.get(arg)));
					break;
				default:
					System.out.format("Unknown command `%s`!\n", arg);
					System.out.flush();
					System.exit(1);
			}
		}
		
		switch(instances.size())
		{
			case 0:
				System.out.println("Invalid options format, no instances!");
				System.out.flush();
				System.exit(1);
				break;
			default:
				if(calculate)
				{
					Instance finalInstance = instances.get(0);
					switch(solutions.size())
					{
						case 0:
							System.out.println("Please give at least a solution.");
							break;
						case 2:
							Map<Solution, Integer> results = solutions.stream().collect(Collectors.toMap(Function.identity(), s -> calculate(finalInstance, s)));
							if(results.get(solutions.get(0)) > results.get(solutions.get(1)))
							{
								System.out.flush();
								System.exit(42);
							}
							break;
						default:
							solutions.forEach(s -> calculate(finalInstance, s));
					}
				}
				else
				{
					if(cExecutable != null && cExecutable.toFile().exists() && pythonExecutable != null && pythonExecutable.toFile().exists())
					{
						Path finalCExecutable = cExecutable;
						Path finalPythonExecutable = pythonExecutable;
						instances.forEach(i -> {
							try
							{
								compareInstance(i, finalCExecutable, finalPythonExecutable);
							}
							catch(IOException | InterruptedException e)
							{
								e.printStackTrace();
							}
						});
					}
					else
					{
						System.out.println("Please give executables path.");
						System.out.flush();
						System.exit(1);
					}
				}
		}
	}
	
	private static void compareInstance(Instance instance, Path cExecutable, Path pythonExecutable) throws IOException, InterruptedException
	{
		System.out.println("Comparing instance " + instance);
		startPython(instance, pythonExecutable);
		Solution solutionP = Solution.parse(findLog(pythonExecutable.getParent().resolve("log"), instance));
		int rP = calculate(instance, solutionP);
		startC(instance, cExecutable);
		Solution solutionC = Solution.parse(findLog(cExecutable.getParent().resolve("log"), instance));
		int rC = calculate(instance, solutionC);
		System.out.printf("C: %d vs %d :P\n", rC, rP);
		if(rC > rP)
		{
			System.out.flush();
			System.exit(43);
		}
		else
			System.out.println("OK");
	}
	
	private static Path findLog(Path folder, Instance instance)
	{
		Pattern p = Pattern.compile("solution_" + instance.getSource().toFile().getName().replace(".", "\\.") + "_\\d+\\.txt");
		for(File f : folder.toFile().listFiles())
			if(f.isFile() && p.matcher(f.getName()).matches())
				return folder.resolve(f.getName());
		return null;
	}
	
	private static void startC(Instance instance, Path executable) throws InterruptedException, IOException
	{
		startCommand(executable.getParent(), executable.toAbsolutePath().normalize().toString().replace("/", isWindows() ? "\\" : "/") + " " + instance.getSource().toAbsolutePath().normalize().toString().replace("\\", "/"));
	}
	
	private static void startPython(Instance instance, Path executable) throws InterruptedException, IOException
	{
		startCommand(executable.getParent(), "python3 " + executable.toAbsolutePath().normalize().toString().replace("/", isWindows() ? "\\" : "/") + " " + instance.getSource().toAbsolutePath().normalize().toString().replace("\\", "/"));
	}
	
	private static void startCommand(Path workingDir, String command) throws InterruptedException, IOException
	{
		String beginning = "";
		String ending = "";
		if(isWindows())
		{
			beginning = "cmd /c start /wait ";
		}
		else
		{
			ending = "";
		}
		command = beginning + command + ending;
		System.out.println("Starting " + command);
		Process proc = Runtime.getRuntime().exec(command, null, workingDir.toFile());
		System.out.println("Waiting for " + command);
		
		boolean print = false;
		
		BufferedReader stdInput = new BufferedReader(new InputStreamReader(proc.getInputStream()));
		BufferedReader stdError = new BufferedReader(new InputStreamReader(proc.getErrorStream()));
		
		String s;
		while((s = stdInput.readLine()) != null)
			if(print)
				System.out.println(s);
		
		while((s = stdError.readLine()) != null)
			System.out.println(s);
		
		System.out.format("Waiting done with code %d.\n", proc.waitFor());
	}
	
	private static boolean isWindows()
	{
		return OS.contains("win");
	}
	
	private static int calculate(Instance instance, Solution solution)
	{
		System.out.println("Processing solution " + solution);
		updateReadyTasks(instance, solution);
		int delay = calculateDelay(instance, solution);
		System.out.printf("Score %d for %s\n", delay, solution);
		return delay;
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
