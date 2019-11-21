import tensorflow as tf
import numpy as np
import pandas as pd

def Average(lst): 
    return sum(lst) / len(lst)

def read_single_csv(file_path,type = None):
    df = pd.read_csv(file_path)
    if type == 'int':
        r = int(df.columns[0])
    elif type == 'float':
        r = float(df.columns[0])
    else:
        r = df.columns[0]
    return r
        

class PartialAutomation(tf.keras.callbacks.Callback):
    def __init__(self,dir_path="",ChangeBatchSize=False):
        self.dir_path = dir_path
        self.batch_path = dir_path + "\\" + "set_batchsize.csv"
        self.lr_path = dir_path + "\\" + "set_lr.csv"
        self.stop_path = dir_path + "\\" + "stop.csv"
        self.best_error_path = dir_path + "\\" + "best_error.csv"
        self.wait_path = dir_path + "\\" + "wait.csv"
        self.patience_path = dir_path + "\\" + "patience.csv"
        self.percent_range_path = dir_path + "\\" + "percent_range.csv"
        self.save_model_path = self.dir_path + "\\" + "NeuralNetwork.ckpt"
        self.loss_type = 'loss'
        self.ChangeBatchSize = ChangeBatchSize
        self.epoch = []
        self.history = {}

    def on_train_begin(self, logs=None):

        self.batch_size = read_single_csv(self.batch_path, 'int')        
        ## Insert learning rate here
        
        temp_csv = read_single_csv(self.best_error_path,'float')
        if not temp_csv == -1:
            self.best_error = temp_csv
        else:
            self.best_error = 9999999
        
        temp_csv = read_single_csv(self.wait_path, 'int')
        if not temp_csv == 0:
            self.wait = temp_csv
        else:
            self.wait = 0
        
        self.patience = read_single_csv(self.patience_path,'int')
        self.percent_range = read_single_csv(self.percent_range_path,'float')    
        self.last_error = 0
        self.skip_first = 10
    
    def on_epoch_end(self,epoch,logs=None):
        logs = logs or {}
        self.epoch.append(epoch)
        for k, v in logs.items():
            self.history.setdefault(k, []).append(v)
        ###############################################
        stop_or_go = read_single_csv(self.stop_path)
        if (stop_or_go == "stop"):
            self.stop()
        ###############################################
        if self.ChangeBatchSize == True:
            if epoch == 5:
                if self.batch_size - 1 <= 1:
                    self.ChangeBatchSize = False
                    self.batch_size = 1
                    self.set_batchsize(1)
                    
                else:
                    self.batch_size = self.batch_size - 1
                    self.set_batchsize(self.batch_size)                
        ###############################################
        #     pd.DataFrame([-1]).to_csv(dir_path + "\\" + "best_error.csv",header=False,index=False) #reset stop
        temp_length = len(self.history[self.loss_type])
        if temp_length >= self.skip_first:
            self.last_error = self.history[self.loss_type][len(self.history[self.loss_type])-1]
            if self.last_error <= self.best_error:
                self.wait = 0
                self.wait_last_error = 0
                self.best_error = self.last_error
                self.model.save(self.save_model_path, overwrite=True)
                temp_meanerror = self.history['mae'][len(self.history['mae'])-1]
                pd.DataFrame([self.best_error,temp_meanerror]).to_csv(self.best_error_path,header=False,index=False)
            else:
                if self.wait >= self.patience:
                    temp_data = self.history[self.loss_type][temp_length-self.patience:temp_length]
                    self.last_avg = Average(temp_data)
                    self.RunProfiling(temp_data,self.last_avg)
                    self.wait = 0
                    self.history = {}
                    #self.patience = self.patience + 2500
                else:
                    self.wait = self.wait + 1
                
        ################################################
        #temp = len(self.history['loss'])
        #self.RunProfiling(self.history['loss'][temp-self.patience:temp])
    
    def RunProfiling(self,sample,last_avg): ## Here is what needs to happen if patience has been shot. 
        increase = 0
        decrease = 0
        for x in sample:
            if (last_avg - last_avg*self.percent_range) <= x <= (last_avg + last_avg*self.percent_range):
                increase = increase + 1
            else:
                decrease = decrease + 1
        if increase >= decrease:
            self.set_lr_percent(0.05,1)
        else:
            self.set_lr_percent(0.05,-1)
        self.add_patience(1)
        #self.add_percent_threshold(-0.0001)

    def set_lr_percent(self,percent,plus_minus):
        lr = plus_minus*tf.keras.backend.get_value(self.model.optimizer.lr)*percent + tf.keras.backend.get_value(self.model.optimizer.lr)
        print('\n##############################################')
        if plus_minus == 1:
            print("INCREASE LEARNING RATE")
        else:
            print("DECREASE LEARNING RATE")
        print(lr)
        print(lr)
        print(lr)
        print('##############################################')
        tf.keras.backend.set_value(self.model.optimizer.lr,lr)    
        pd.DataFrame([lr]).to_csv(self.lr_path,header=False,index=False)
    
    def add_patience(self,patience):
        self.patience = self.patience + patience
        pd.DataFrame([self.patience]).to_csv(self.patience_path,header=False,index=False)
    
    def add_percent_threshold(self,percent_range):
        self.percent_range = self.percent_range + percent_range
        pd.DataFrame([self.percent_range]).to_csv(self.percent_range_path,header=False,index=False)
        
    def set_batchsize(self,batch_size): #
        pd.DataFrame([batch_size]).to_csv(self.batch_path,header=False,index=False)
        pd.DataFrame([self.wait]).to_csv(self.wait_path,header=False,index=False)
        
        self.model.stop_training = True
    def stop(self): # Stop completly
        pd.DataFrame(["stop"]).to_csv(self.stop_path,header=False,index=False)
        pd.DataFrame([self.wait]).to_csv(self.wait_path,header=False,index=False)
        self.model.stop_training = True
        