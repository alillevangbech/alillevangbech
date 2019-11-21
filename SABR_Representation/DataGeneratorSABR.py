## Data generator - SABR

import sys, os
import numpy as np
import pandas as pd
import math

# left,right,mid excel function to python
def left(s, amount):
    return s[:amount]

def right(s, amount):
    return s[-amount:]

def mid(s, offset, amount):
    return s[offset:offset+amount]


# replicate dates i.e. find_date("1d") = 1 day maturity
# Assmues 365.25 days in a year
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


# dynamic vol formula
def volofvol_range(mat):

	volofvol_1M_min = 0.05;
	volofvol_1M_max = 4;
	date_1M = find_date("1m");
	
	if (mat <= date_1M):
		max = volofvol_1M_max / math.sqrt(mat/date_1M)
		min = volofvol_1M_min / math.sqrt(mat/date_1M)
	else:
		max = volofvol_1M_max * math.sqrt(date_1M/mat)
		min = volofvol_1M_min * math.sqrt(date_1M/mat)
	
	return min,max
	
# For equidistant volofvol datapoints
def volofvol_equi(mat,n): 
	MinMax = volofvol_range(mat)
	return np.linspace(MinMax[0],MinMax[1],n) # Linspace creates a array with a min and max, split in n equidistant pieces.
	
# For uniformly randon distributed volofvol datapoints
def volofvol_random(mat): 
	MinMax = volofvol_range(mat)
	return np.random.uniform(MinMax[0],MinMax[1])
	
	
	
### Generating Stikes according to mcghee ###
def terminal_inst_vol(mat,init_vol,volofvol,std_dev):
	return init_vol*math.exp(-1.0*0.5*(volofvol**2.0)*mat + volofvol*std_dev*math.sqrt(mat))

def gen_strike(mat,init_vol,corr,volofvol,eta,std_dev):

	term = -0.5*((init_vol** 2.0) / (volofvol** 2.0))*(math.exp((volofvol** 2.0)*mat) - 1.0)
	term = term + (corr / volofvol)*(terminal_inst_vol(mat,init_vol,volofvol,std_dev) - init_vol)
	term = term + eta*(init_vol / volofvol)*((math.exp((volofvol** 2.0)*mat) - 1.0)** 0.5)
	return math.exp(term)
### Generating Stikes according to mcghee ###


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


####### Define initial parameters #######

#Loops through the entire program to create Train dataset and Test dataset
TrainData = [True,False]

for TrainData_ in TrainData:

	n_mat = 100 # How many equidistant points between, fx. 1 day and 1y for the maturity parameter
	n_other = 10 #How many equidistant points between other parameters - i.e intial_vol,correlation,volofvol

	n_equi = int((n_mat*n_other**3)) # In Mcghee we have 100x10x10x10 = 100000 equidistant data points - maturity,intial_vol,correlation,volofvol respectivly.
	n_random = int(n_equi*1.5) 		# Mcghee uses a factor of 1.5 number of uniformly random distributed datapoints. 
	n_strikes = 10	#Number of strikes							  

	# If generate test data we only want 20% of the total datapoints of the training set - and all of it should be randomly generated.
	# We still generate equidistant points if we choose test data - but it only takes a second or so - and we sort them away farther down in the code.
	if not TrainData_:
		n_random = int((n_equi+n_random)*0.20)

	# beta parameter
	beta = 1

	# maturity range
	max_maturity = find_date("1y")
	min_maturity = find_date("1w")

	# intial volatility range
	max_intial_vol = 0.5
	min_initial_vol = 0.05

	# correlation range
	max_corr = 0.90
	min_corr = -0.90


	# eta - speciale case - used to generate strikes
	n_strikes = 10
	max_eta = 3.5
	min_eta = -3.5
	eta_range_equi = np.linspace(min_eta,max_eta,n_strikes) # Create Equidistant eta range for creating strikes


	####### Define initial parameters - END #######


	####### Create Equidistant Dataset #######

	mat_range_equi = np.linspace(min_maturity,max_maturity,n_mat) 				# Create Equidistant maturity range
	intial_vol_range_equi = np.linspace(min_initial_vol,max_intial_vol,n_other) # Create Equidistant intial_volatility range
	corr_range_equi = np.linspace(min_corr,max_corr,n_other) 					# Create Equidistant correlation range

	volofvol_range_equi = np.repeat(0,n_other) 									# Create placeholder for volofvol - needs to be defined dynamically

	# Get unique combinations of the different parameters - volofvol is a speciale case
	unique_combinations_equi = np.array(np.meshgrid(mat_range_equi,
													volofvol_range_equi,
													corr_range_equi,
													intial_vol_range_equi,indexing ="xy")).T.reshape(-1,4)


	# Swap volofvol and inital_vol in the numpy "matrix"
	temp_vofv = np.copy(unique_combinations_equi[:,1])
	temp_init = np.copy(unique_combinations_equi[:,3])
	unique_combinations_equi[:,1] = temp_init
	unique_combinations_equi[:,3] = temp_vofv

	## Replace volofvol with correct values in unique_combinations variable##
	## Assmues volofvol gets changed every line in unique_combinations - i.e that the unique_combinations has the current initialization
	last_line_mat = -1.0
	for line in unique_combinations_equi:
		current_line_mat = line[0]
		
		if(current_line_mat != last_line_mat):
			count = 0
			volofvol = volofvol_equi(current_line_mat,n_other) # Everytime maturity get's changed - create a new equidistant range of volofvol
		
		line[3] = volofvol[count] 
		count = count + 1
		last_line_mat = current_line_mat
		

	####### Create Equidistant Dataset - END #######


	####### Create Uniform Random Dataset #######


	mat_range_random = np.random.uniform(min_maturity,max_maturity,n_random) 				# Create uniformly distributed maturity range
	intial_vol_range_random = np.random.uniform(min_initial_vol,max_intial_vol,n_random)	# Create uniformly distributed initial_vol range
	corr_range_random = np.random.uniform(min_corr,max_corr,n_random) 						# Create uniformly distributed correlation range

	volofvol_range_random = np.empty((0,1), float)
	for datapoint in mat_range_random:
		volofvol_range_random = np.append(volofvol_range_random,volofvol_random(datapoint)) # Foreach maturity datapoint - create a range and then pick a uniform distributed number from that range

	unique_combinations_random = np.array((mat_range_random,intial_vol_range_random,corr_range_random,volofvol_range_random)).T # Gather maturity,inital_vol,correlation,volofvol


	####### Create Uniform Random Dataset - END #######



	## Gather together equidistant and random datapoints - if testset - then just throw away equidistant points
	if TrainData_:
		unique_combinations = np.concatenate((unique_combinations_equi,unique_combinations_random), axis = 0)
	else:
		unique_combinations = unique_combinations_random


	# Create a csv file to save data - just create a empty csv file to start with.
	header = ["Mat","initial_Vol","Correlation","VolofVol","Strike_1","Strike_2","Strike_3","Strike_4","Strike_5","Strike_6","Strike_7","Strike_8","Strike_9","Strike_10","IV_1","IV_2","IV_3","IV_4","IV_5","IV_6","IV_7","IV_8","IV_9","IV_10"]
	dir_path = os.getcwd()
	if TrainData_:
		name = "SABR_Train_b1_1d1y.csv"
	else:
		name = "SABR_Test_b1_1d1y.csv"
	full_path = dir_path + "\\" + name
	CreateFile = np.empty((0,len(header)), float)
	pd.DataFrame(CreateFile).to_csv(full_path,header=header, index=None)


	# Go through entire dataset and and create 10 strikes and 10 implied volatility 
	# Every 10000 datapoints - save to csv file. The program would be a lot slower if we did it all at once
	
	StrikesAndIV = np.empty((0,int(n_strikes*2)),float)
	std_dev = 1.5 # From Mcghee
	Forward0 = 1 # assmume no interest rate
	total = len(unique_combinations) # total datapoints
	count = 0
	temp = 0

	for line in unique_combinations:

		mat = line[0]
		init_vol = line[1]
		corr = line[2]
		volofvol = line[3]
		
		line_strike = np.array(()) # Holds 10 strikes
		line_implied_vol = np.array(()) #Holds 10 Implied vols
		for x in eta_range_equi:
			strike = gen_strike(mat,init_vol,corr,volofvol,x,std_dev) # Calculate strike
			implied_vol = iVolSABR(mat,init_vol,corr,volofvol,beta,strike,Forward0) # Calculate implied vol
			
			line_strike = np.append(line_strike,strike) # gather together 10 strikes
			line_implied_vol = np.append(line_implied_vol,implied_vol) # gather together 10 implied vols
			
		StrikesAndIV = np.vstack((StrikesAndIV,np.append(line_strike,line_implied_vol))) # Gathers together all the strikes and implied vols
		count = count + 1
		print(str(count) + "/" + str(total))
		
		if (count % 10000 == 0 or count == total):
			final_data = np.hstack((unique_combinations[temp:count],StrikesAndIV)) # Gathers together the parameters and strikes and implied vols
			pd.DataFrame(final_data).to_csv(full_path,mode='a',header = None, index=None) # Saves to csv file - does not delete prior data in the csv file
			StrikesAndIV = np.empty((0,int(n_strikes*2)),float) #release data
			final_data = 0 #release data
			temp = count


			
			
