import os,sys
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import math
import scipy.integrate as integrate
import scipy.interpolate as interpolate
import tensorflow as tf
from tensorflow.keras import models
import datetime

os.environ["CUDA_DEVICE_ORDER"] = "PCI_BUS_ID"   # see issue #152
os.environ["CUDA_VISIBLE_DEVICES"] = "-1"

def left(s, amount):
    return s[:amount]

def right(s, amount):
    return s[-amount:]

def mid(s, offset, amount):
    return s[offset:offset+amount]

def find_date(date):

    day_unit = 0.002737850
    week_unit = 0.01916496
    month_unit = 0.08333333
    year_unit = 1

    amount = float(left(date,len(date)-1))
    letter = right(date,1)

    if (letter == "d"):
        return amount*day_unit
                   
    elif (letter == "w"):    
        return amount*week_unit
    
    elif (letter == "m"):
        return amount*month_unit
    
    elif (letter == "y"):
        return amount*year_unit
    
    return -1.0


# Generate sabr european option implied volatilties
def iVolSABR(mat,init_vol,corr,volofvol,beta,strike,Forward0):
    result = -1.0
    if (strike == Forward0):
        IVterm1 = ((1.0-beta)**2/24.0)*(init_vol**2/(Forward0)**(2.0-2.0*beta))
        IVterm2 = 0.25*((corr*beta*init_vol*volofvol) / Forward0**(1.0-beta))
        IVterm3 = ((2.0 - 3.0*corr**2) / 24.0)*volofvol**2
        
        result = (init_vol / Forward0**(1.0-beta))*(1.0 + (IVterm1 + IVterm2 + IVterm3)*mat)
        
    elif (beta == 0.0):
        
        zeta = (volofvol / init_vol)*math.sqrt(Forward0*strike)*math.log(Forward0 / strike)
        x_zeta = math.log((math.sqrt(1.0 - 2.0*corr*zeta + zeta**2 - corr + zeta) / (1.0 - corr)))
        
        IVterm1 = init_vol*(math.log(Forward0 / strike)) / (Forward0 - strike)*(zeta / x_zeta)
        IVterm2 = (init_vol**2 / (24.0*Forward0*strike))
        IVterm3 = ((2.0 - 3.0*corr**2 / (24.0))*volofvol**2)

        result = IVterm1*(1.0 + (IVterm2 + IVterm3)*mat)
        
    elif (beta == 1.0):
        zeta = (volofvol / init_vol)*math.log(Forward0 / strike)
        x_zeta = math.log((math.sqrt(1.0 - 2.0*corr*zeta + zeta**2) - corr + zeta) / (1.0 - corr))

        IVterm1 = init_vol*(zeta / x_zeta)
        IVterm2 = 0.25*corr*init_vol*volofvol
        IVterm3 = (1.0 / 24.0)*(2.0 - 3.0*corr**2*volofvol**2)
        result = IVterm1*(1.0 + (IVterm2 + IVterm3)*mat)
    else:
        zeta = (volofvol / init_vol)*((Forward0*strike)**(1.0-beta) / 2.0)*math.log(Forward0 / strike)
        x_zeta = math.log((math.sqrt(1.0 - 2.0*corr*zeta + zeta**2) - corr + zeta) / (1.0 - corr))
        
        IVterm4 = ((1.0 - beta)**2 / 24.0)*((math.log(Forward0 / strike))**2)
        IVterm5 = ((1.0 - beta)**4 / 1920.0)*((math.log(Forward0 / strike))**4)

        IVterm1 = ((1.0 - beta)**2 / 24.0)*(init_vol**2) / ((Forward0*strike)**(1.0-beta))
        IVterm2 = 0.25*((corr*beta*init_vol*volofvol) / ((Forward0*strike)**(1.0-beta)/ 2.0))
        IVterm3 = ((2.0 - 3.0*corr**2) / 24.0)*volofvol**2

        result = init_vol / ((Forward0*strike)**(1.0-beta) / 2.0)*(1.0 + IVterm4 + IVterm5)*(zeta / x_zeta)*(1.0 + (IVterm1 + IVterm2 + IVterm3)*mat)

    return result

def CreateDictSABR(Forward0,sigma0,mat,corr,volofvol,beta,network):
    ParameterDict = dict()
    ParameterDict["Forward0"] = Forward0
    ParameterDict["mat"] = mat
    ParameterDict["init_vol"] = sigma0
    ParameterDict["corr"] = corr
    ParameterDict["volofvol"] = volofvol
    ParameterDict["beta"] = beta
    ParameterDict["network"] = network
    return ParameterDict

def iVolSABRDict(pd,strike): #pd = parameterdict
	#iVolSABR(mat,init_vol,corr,volofvol,beta,strike,Forward0)
    #iVolSABR(Forward0,sigma0,mat,strike,alpha,beta,rho)
    return (iVolSABR(pd["mat"],pd["init_vol"],pd["corr"],pd["volofvol"],pd["beta"],strike,pd["Forward0"]))

# Network Generator
# Assmues 10 strikes and 10 implied vol
# Returns output of the neural network given parameters and strikes
def NeuralNetworkToPredictSABR(pd,strikes):
    nrFeatures = 10
   
    parameter = np.ones(14).reshape((1,14))
    result = np.array(())
    
    parameter[0,0] = pd["mat"]
    parameter[0,1] = pd["init_vol"]
    parameter[0,2] = pd["corr"]
    parameter[0,3] = pd["volofvol"]
    
    parameter[0,4:14] = strikes
    
    result = pd["network"].predict(parameter)[0]
    
    return result


### MAKE SPLINES ###

# When initiated with a dictionary of SABR parameters
# Creates a cubic spline for both approx sabr values and neural network values
# using 10 knot points taken from equidistant points between minimun and maximum strike
class GetSpline:
	def __init__(self,pd,NrPoints):
		self.pd = pd
		self.minStrike = pd["minStrike"]
		self.maxStrike = pd["maxStrike"]
		self.NrPointsNetwork = 10 #knots
		self.StrikesNetwork = np.linspace(self.minStrike,self.maxStrike,self.NrPointsNetwork) # Set strikes 
		
		self.Strikes = np.linspace(self.minStrike,self.maxStrike,NrPoints) # Set strikes 
		# Run through the two member functions, it just creates 10 values of 
		self.approx() 
		self.network()

	def approx(self):
		PredOutput = []
		for i in self.StrikesNetwork:	
			PredOutput.append(iVolSABRDict(self.pd,i))
		tck = interpolate.splrep(self.StrikesNetwork, PredOutput)

		GetSpline.ApproxPred = PredOutput  #Value of knots - so we can plot them if we want
		return interpolate.splev(self.Strikes, tck)

	def network(self):
		PredOutput = NeuralNetworkToPredictSABR(self.pd,self.StrikesNetwork)
		tck = interpolate.splrep(self.StrikesNetwork, PredOutput)
		
		GetSpline.NetworkPred = PredOutput #Value of knots - so we can plot them if we want
		return interpolate.splev(self.Strikes, tck)



# plots the figure
def Build_CompareSmiles(spline,name,title):
	plt.figure()
	plt.title(title)
	plt.xlabel('Strike')
	plt.ylabel('IV')
	plt.grid()
	plt.plot(spline.Strikes,spline.approx(),label = "Approx")
	plt.plot(spline.StrikesNetwork,spline.ApproxPred,'o')
	plt.plot(spline.Strikes,spline.network(), label = "Network")
	plt.plot(spline.StrikesNetwork,spline.NetworkPred,'o')
	plt.legend(bbox_to_anchor=(0.75, 1), loc=2, borderaxespad=0.)
	plt.savefig(name)

dir_path = os.getcwd()
try:
    network = models.load_model(dir_path + "\\" + "NeuralNetwork.ckpt")
except:
    print("Generate model or check if in current directory")
    assert 1 > 2
          

# First Parameter set
Forward0 = 1
init_vol = 0.30
mat = find_date("14d")
volofvol = 1.60
beta = 1
corr = -0.60

pd_1 = CreateDictSABR(Forward0,init_vol,mat,corr,volofvol,beta,network)
pd_1["minStrike"] = 0.65
pd_1["maxStrike"] = 1.2

spline_1 = GetSpline(pd_1,10000) # nr of points to use in graph - taken from the a cubic spline

title = "Figure 2 in McGhee"
file_name = "SABR_ParameterSet1.pdf"
Build_CompareSmiles(spline_1, subdir_path + "\\" + file_name, title)

# Second Parameter set
Forward0 = 1
init_vol = 0.30
mat = find_date("6m")
volofvol = 0.40
beta = 1
corr = 0

pd_2 = CreateDictSABR(Forward0,init_vol,mat,corr,volofvol,beta,network)
pd_2["minStrike"] = 0.4
pd_2["maxStrike"] = 1.2

spline_2 = GetSpline(pd_2,10000) # nr of points to use in graph - taken from the a cubic spline

title = "Figure 3 in McGhee"
file_name = "SABR_ParameterSet2.pdf"
Build_CompareSmiles(spline_2, subdir_path + "\\" + file_name,title)

# Third Parameter set
Forward0 = 1
init_vol = 0.2
mat = find_date("1y")
volofvol = 0.3
beta = 1
corr = 0.3

pd_3 = CreateDictSABR(Forward0,init_vol,mat,corr,volofvol,beta,network)
pd_3["minStrike"] = 0.5
pd_3["maxStrike"] = 2.25

spline_3 = GetSpline(pd_3,10000) # nr of points to use in graph - taken from the a cubic spline

title = "Figure 4 in McGhee"
file_name = "SABR_ParameterSet3.pdf"
Build_CompareSmiles(spline_3, subdir_path + "\\" + file_name,title)


## Save model in subfolder as well
tf.keras.models.save_model(network,subdir_path + "\\" + "NeuralNetwork.ckpt")

