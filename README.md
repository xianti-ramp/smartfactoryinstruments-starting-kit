# smartfactoryinstruments-starting-kit

Starting kit for the "Layout of Smart Factory Instruments" challenge

In this challenge you can design an C++ or a Python solution.

## Running the Python example solution

Install Python 3.8+, for instance using [an installer the conda-forge distribution](https://github.com/conda-forge/miniforge),
then run,
```
python python/solution.py sample/sample.in
```
This parses the `sample/case0.in` computes a solution, and prints it to the standard output.
This is a minimal solution you can submit to the RAMP platform. 

## Running the C++ example solution

To compile the C++ example solution on Linux or Mac OS
```
cc -I./CPP/ CPP/main.cpp -lstdc++ -O3 -o solution
```


Then you can run the resulting executable with,
```
./solution < sample/sample.in
```
which will print the output to the standard output. To redirect the output to a file, you can use,
```
./solution < sample/sample.in > sample.ans
```
