compile: 
		clang++ -g -std=c++14 Code/CompileJob.cpp Code/JobSystem.cpp Code/JobWorkerThread.cpp Code/RenderJob.cpp Code/main.cpp Code/RenderJob.cpp -o output
		./output
automated: 
		clang++ -g -std=c++14 ./CompileJobs/Automated.cpp -o auto_out
		./auto_out
test: 
	clang++ -g -std=c++14 ./CompileJobs/TestCompile.cpp -o test_out 
	./test_out