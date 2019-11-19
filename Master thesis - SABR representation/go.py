import os,sys
import numpy as np
import pandas as pd
#from sklearn.utils import shuffle
import math
import datetime
#import time
from callbacks import PartialAutomation
#import CompareModel
import shutil

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
        
        
        
def get_datefile():
    x = datetime.datetime.now()
    get_date = str(x.year) + "_" + str(x.month) +"_"+ str(x.day)
    return get_date
        
def new_id(file):
    d = pd.read_csv(file)
    id_ = d.loc[len(d)-1]['ID'] + 1
    d.loc[len(d)] = [id_] + [get_datefile()]
    d.to_csv(file, index = False)
    return int(id_)


def reload_model(model_path):
	try:
		network = models.load_model(model_path)
	except:
		print("Generate model or check if in current directory")
		assert 1 > 2
	
	return network

def checkpoint(file_path):
	df = pd.read_csv(file_path)
	test = df.columns[0]
	return test

def get_batch_size(file_path):
	df = pd.read_csv(file_path)
	print(int(df.columns[0]))
	return int(df.columns[0])
	
    
Reload_Model = False

base_path = "C:\\Users\\alexa\\Desktop\\FileSharingForSpeciale\\Code\\TilRolf"
dir_path = os.getcwd()
stats_path = os.getcwd() + "\\stats"

if Reload_Model == True and base_path != dir_path:
    1+1
    # Reload with current settings
elif Reload_Model == False and base_path != dir_path:
    raise Exception("Please only start new models from the base directoy: " + base_path)
    sys.exit()
elif Reload_Model == True and base_path == dir_path:
    raise Exception("Please only reload models from the subdirectories")
    sys.exit()
elif Reload_Model == False and base_path == dir_path:
    id_ = new_id(dir_path + "\\" + "Model_ID.csv")
    dir_path = base_path + "\\Models\\" + "ID" + str(id_) + "_" + get_datefile()
    os.mkdir(dir_path)
    files = [   "go.py",
            "callbacks.py",
            "set_lr.csv",
            "set_batchsize.csv",
            "stop.csv",
            "best_error.csv",
            "patience.csv",
            "percent_range.csv"
            ]
    for file in files:
        shutil.copy(file,dir_path)
        
    # Create new subfolder with this id.
    # Load with current settings.


Path_Train = base_path + "\\" + "SABR_Train_b1_1d1y.csv"
Path_Test = base_path + "\\" + "SABR_Test_b1_1d1y.csv"

try:
	df_data_train = pd.read_csv(Path_Train)
	#df_data_test = pd.read_csv(Path_Test)
except:
	print("Generate Train/Test data and place them in current directory")
	assert 1 > 2

#df_data_train = df_data_train.iloc[0:200000,:]
# Split into inputs and outputs
number_of_inputs = df_data_train.columns.get_loc("IV_1")
number_of_outputs = len(df_data_train.columns) - df_data_train.columns.get_loc("IV_1")
x_train = df_data_train.iloc[:,0:number_of_inputs]
y_train = df_data_train.iloc[:,number_of_inputs:len(df_data_train.columns)]
#x_test = df_data_test.iloc[:,0:number_of_inputs]
#y_test = df_data_test.iloc[:,number_of_inputs:len(df_data_test.columns)]

# Pre Creating the Neural Network


Hidden_Layer_Nodes = 5000
epoch = 1000000000
pd.DataFrame(["go"]).to_csv(dir_path + "\\" + "stop.csv",header=False,index=False) #reset stop
verbose = 1 	# Default setting for keras is 1 - makes it print progress to screen. If you want to remove set to 0.
# Callbacks parameter is a custom setting - you can write your own routine to implement in the training - like early_stopping etc.
batch_size = 1000 # Default setting for keras is 32
learning_rate = 0.01
EnableBatchSize = False

model_path = dir_path + "\\" + "NeuralNetwork.ckpt"
if Reload_Model:
	network = reload_model(model_path)
	print("Reloading Model")
else:
    network = build_model(learning_rate,Hidden_Layer_Nodes,number_of_inputs,number_of_outputs)
    pd.DataFrame([batch_size]).to_csv(dir_path + "\\" + "set_batchsize.csv",header=False,index=False) #reset batch_size
    pd.DataFrame([learning_rate]).to_csv(dir_path + "\\" + "set_lr.csv",header=False,index=False) #reset batch_size
    pd.DataFrame([0]).to_csv(dir_path + "\\" + "wait.csv",header=False,index=False) #reset stop
    pd.DataFrame([10]).to_csv(dir_path + "\\" + "patience.csv",header=False,index=False) #
    pd.DataFrame([0.001]).to_csv(dir_path + "\\" + "percent_range.csv",header=False,index=False) #
    pd.DataFrame([-1]).to_csv(dir_path + "\\" + "best_error.csv",header=False,index=False) #reset stop
    print("Creating New Model")


GetCallBack = PartialAutomation(dir_path,EnableBatchSize)
while True:
    # Train neural network
    history = network.fit(x_train, # Features
                      y_train, # Target vector
                      epochs=epoch, # Number of epoch
                      batch_size=get_batch_size(dir_path + "\\" + "set_batchsize.csv"),
                      verbose=verbose,
                      callbacks=[GetCallBack])#[newCallBack()],
                      #validation_data=(tensor_x_test, tensor_y_test)) # Data for evaluation
 
    check = checkpoint(dir_path + "\\" + "stop.csv")
    if (check == "stop"):
        models.save_model(network,model_path,)
        print("Closing program ...")
        sys.exit()
