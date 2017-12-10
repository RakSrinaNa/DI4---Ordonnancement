package fr.mrcraftcod.polytech.solutioncalculator;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.*;
import java.util.stream.Collectors;

/**
 * Created by Thomas Couchoud (MrCraftCod - zerderr@gmail.com) on 10/12/2017.
 *
 * @author Thomas Couchoud
 * @since 2017-12-10
 */
public class Solution
{
	private final ArrayList<Integer> production;
	private HashMap<Integer, List<Integer>> deliveries;
	
	private Solution()
	{
		production = new ArrayList<>();
		deliveries = new HashMap<>();
	}
	
	public static Solution parse(File file) throws IOException
	{
		Solution solution = new Solution();
		LinkedList<String> lines = new LinkedList<>(Files.readAllLines(Paths.get(file.toURI())));
		
		String line;
		switch(lines.poll())
		{
			case "P":
				while((line = lines.poll()) != null)
				{
					String[] infos = line.split(" ", 2);
					switch(infos[0])
					{
						case "sched=":
							for(String s : infos[1].substring(1, infos[1].length() - 1).split(", "))
								solution.addProduction(Integer.parseInt(s));
							break;
						case "route=":
							int pack = solution.getDeliveries().keySet().stream().mapToInt(i -> i).max().orElse(-1) + 1;
							for(String s : infos[1].substring(1, infos[1].length() - 1).split(", "))
								solution.addDelivery(pack, Integer.parseInt(s));
							break;
					}
				}
				break;
			case "C":
				solution.setProduction(Arrays.stream(lines.poll().split("\t")).mapToInt(Integer::parseInt).boxed().collect(Collectors.toList()));
				
				int i = 0;
				while((line = lines.poll()) != null)
					solution.setDeliveries(i++, Arrays.stream(line.split("\t")).mapToInt(Integer::parseInt).boxed().collect(Collectors.toList()));
				
				break;
		}
		
		return solution;
	}
	
	private void addDelivery(int pack, int i)
	{
		if(!deliveries.containsKey(pack))
			deliveries.put(pack, new LinkedList<>());
		deliveries.get(pack).add(i);
	}
	
	private void addProduction(int i)
	{
		production.add(i);
	}
	
	private void setDeliveries(int index, List<Integer> deliveries)
	{
		this.deliveries.put(index, deliveries);
	}
	
	public HashMap<Integer, List<Integer>> getDeliveries()
	{
		return deliveries;
	}
	
	public ArrayList<Integer> getProduction()
	{
		return production;
	}
	
	public void setProduction(Collection<Integer> production)
	{
		this.production.clear();
		this.production.addAll(production);
	}
}
