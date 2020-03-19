import os,sys
import numpy as np
import pandas as pd
import math
import shutil
from callbacks import PartialAutomation,read_single_csv

try:
    flag_chip = sys.argv[1]
except:
    flag_chip = "GPU"

if flag_chip == "CPU":
    os.environ["CUDA_DEVICE_ORDER"] = "PCI_BUS_ID"   # see issue #152
    os.environ["CUDA_VISIBLE_DEVICES"] = "-1"

import tensorflow as tf
from tensorflow import keras
from tensorflow.keras import models, layers, optimizers
from tensorflow.keras import backend as K
	
def build_model(lr,neurons,input_count,output_count,prev_model=None):

    tempModel = models.Sequential()
    
    tempModel.add(layers.Dense(units=neurons, activation='softplus', input_shape=(input_count,),kernel_initializer='random_uniform',bias_initializer='zeros'))
    tempModel.add(layers.Dense(units=output_count))

    # Defining Optimizers
    sgd = optimizers.Adam(lr=lr, beta_1 = 0.9, beta_2 = 0.999, epsilon = 1e-8) # From adama paper
    tempModel.compile(loss = 'mse',optimizer = sgd,metrics = ['mae'])

    if prev_model == None:
        return tempModel
    else:
        tempModel.set_weights(prev_model.get_weights())
        K.set_value(tempModel.optimizer.lr,K.get_value(prev_model.optimizer.lr))
        return tempModel
        
       

def reload_model(model_path):
	try:
		network = models.load_model(model_path)
	except:
		print("Generate model or check if in current directory")
		assert 1 > 2
	
	return network

Reload_Model = False
dir_path = os.getcwd()
stats_path = dir_path + "\\stats"

Path_Train = dir_path + "\\" + "SABR_Train_b1_1d1y.csv"
Path_Test = dir_path + "\\" + "SABR_Test_b1_1d1y.csv"

try:
	df_data_train = pd.read_csv(Path_Train)
	#df_data_test = pd.read_csv(Path_Test)
except:
	print("Generate Train/Test data and place them in current directory")
	assert 1 > 2

# Split into inputs and outputs
number_of_inputs = df_data_train.columns.get_loc("IV_1")
number_of_outputs = len(df_data_train.columns) - df_data_train.columns.get_loc("IV_1")
x_train = df_data_train.iloc[:,0:number_of_inputs]
y_train = df_data_train.iloc[:,number_of_inputs:len(df_data_train.columns)]


# Pre Creating the Neural Network
Hidden_Layer_Nodes = 250
epoch = 1000000000
pd.DataFrame(["go"]).to_csv(stats_path + "\\" + "stop.csv",header=False,index=False) #reset stop
verbose = 1 	# Default setting for keras is 1 - makes it print progress to screen. If you want to remove set to 0.
batch_size = 1000 # Default setting for keras is 32
learning_rate = 0.01
EnableBatchSize = True

model_path = stats_path + "\\" + "NeuralNetwork.ckpt"
if Reload_Model:
	network = reload_model(model_path)
	print("Reloading Model")
else:
    network = build_model(learning_rate,Hidden_Layer_Nodes,number_of_inputs,number_of_outputs)
    pd.DataFrame([batch_size]).to_csv(stats_path + "\\" + "set_batchsize.csv",header=False,index=False) #reset batch_size
    pd.DataFrame([learning_rate]).to_csv(stats_path + "\\" + "set_lr.csv",header=False,index=False) #reset batch_size
    pd.DataFrame([0]).to_csv(stats_path + "\\" + "wait.csv",header=False,index=False) #reset stop
    pd.DataFrame([10]).to_csv(stats_path + "\\" + "patience.csv",header=False,index=False) #
    pd.DataFrame([0.001]).to_csv(stats_path + "\\" + "percent_range.csv",header=False,index=False) #
    pd.DataFrame([-1]).to_csv(stats_path + "\\" + "best_error.csv",header=False,index=False) #reset stop
    print("Creating New Model")


GetCallBack = PartialAutomation(stats_path,EnableBatchSize)
while True:
    # Train neural network
    history = network.fit(x_train, # Features
                      y_train, # Target vector
                      epochs=epoch, # Number of epoch
                      batch_size=read_single_csv(stats_path + "\\" + "set_batchsize.csv","int"),
                      verbose=verbose,
                      callbacks=[GetCallBack])
                      #validation_data=(tensor_x_test, tensor_y_test)) # Data for evaluation
 
    check = read_single_csv(stats_path + "\\" + "stop.csv")
    if (check == "stop"):
        models.save_model(network,model_path,)
        print("Closing program ...")
        sys.exit()
