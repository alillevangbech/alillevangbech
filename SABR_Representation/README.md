## **Representing SABR through a Neural Network** #

In this folder is the current iteration of my testing setup for my masters thesis. Let it be noted that this is subject to constant changes, as I try to make the network converge.

 The general idea behind the code can be found in the paper:    
        McGhee, William A, An Artificial Neural Network Representation of the SABR Stochastic Volatility Model (November 21, 2018).

Currently, the project include
1. DataGeneratorSABR.py     
    Generates the needed data and saves it to the current directory in a csv format.
2. go.py, callbacks.py  
    My implementation of a neural network in Tensorflow. As I constatly try and tweak this setup alot of the parameters will be send to csv files in the stats folder, so I can follow the evolution when training.
    The callbacks file is my custom implementation of a callback, which just makes it easier to tweak the network.
3. CompareModel.py  
    Easy way to generate graphs for my current iteration of model. Using a cubic spline - it is comparable to page 15-16 of the McGhee paper. 

