package fr.mrcraftcod.polytech.solutioncalculator;

import java.io.File;
import java.nio.file.Path;
import java.util.Objects;
import java.util.concurrent.Callable;
import java.util.regex.Pattern;

/**
 * Created by Thomas Couchoud (MrCraftCod - zerderr@gmail.com) on 08/01/2018.
 *
 * @author Thomas Couchoud
 * @since 2018-01-08
 */
public class ProgCall implements Callable<SolutionCalc>
{
	private final Path executable;
	private final SolutionCalc.Program program;
	private final Instance instance;
	
	public ProgCall(Path executable, SolutionCalc.Program program, Instance instance)
	{
		this.executable = executable;
		this.program = program;
		this.instance = instance;
	}
	
	@Override
	public SolutionCalc call() throws Exception
	{
		System.out.printf("Starting %s on %s\n", program.name(), instance);
		switch(program)
		{
			case C:
				Main.startCommand(executable.getParent(), executable.toAbsolutePath().normalize().toString().replace("/", isWindows() ? "\\" : "/") + " " + instance.getSource().toAbsolutePath().normalize().toString().replace("\\", "/"));
				break;
			case PYTHON:
				Main.startCommand(executable.getParent(), "python3 " + executable.toAbsolutePath().normalize().toString().replace("/", isWindows() ? "\\" : "/") + " " + instance.getSource().toAbsolutePath().normalize().toString().replace("\\", "/"));
				break;
		}
		Solution solution = Solution.parse(findLog(executable.getParent().resolve("log"), instance));
		int score = Main.calculate(instance, solution, false);
		return new SolutionCalc(program, instance, solution, score);
	}
	
	private static boolean isWindows()
	{
		return Main.OS.contains("win");
	}
	
	private static Path findLog(Path folder, Instance instance)
	{
		Pattern p = Pattern.compile("solution_" + instance.getSource().toFile().getName().replace(".", "\\.") + "_\\d+\\.txt");
		for(File f : Objects.requireNonNull(folder.toFile().listFiles()))
			if(f.isFile() && p.matcher(f.getName()).matches())
				return folder.resolve(f.getName());
		return null;
	}
}
