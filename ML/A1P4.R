## Get paths
path <- paste(getwd(),"data","MNIST-cropped-txt",sep="/")
path_train_x <- paste(path,"MNIST-Train-cropped.txt",sep="/")
path_train_y <- paste(path,"MNIST-Train-Labels-cropped.txt",sep="/")
path_test_x <- paste(path,"MNIST-Test-cropped.txt",sep="/")
path_test_y <- paste(path,"MNIST-Test-Labels-cropped.txt",sep="/")

## Read data - required packages for large data
library(bigmemory)
library(bigtabulate)
library(Rlab)

data_train_x <- read.big.matrix(path_train_x,sep = " ", header = FALSE, type = "double") ## Takes a few seconds
data_test_x <- read.big.matrix(path_test_x,sep = " ", header = FALSE, type = "double")
data_train_y <- read.table(path_train_y,header = FALSE)
data_test_y <- read.table(path_test_y,header = FALSE)

row <- 784
TrainCol <- 10000
TestCol <- 2000
digitRow <- digitCol <- 28

matrix_train_x <- matrix(NA,nrow = row,ncol = TrainCol) ## Convert data strucutre into a matrix
matrix_test_x <- matrix(NA,nrow = row,ncol = TestCol) ## Convert data strucutre into a matrix
### Fill matrix for training data
for (i in 1:TrainCol)
{
  indexStart <- 1 + row*(i-1)
  indexEnd <- row*i
  matrix_train_x[,i] <- data_train_x[indexStart:indexEnd]
}
### Fill matrix for test data
for (i in 1:TestCol)
{
  indexStart <- 1 + row*(i-1)
  indexEnd <- row*i
  matrix_test_x[,i] <- data_test_x[indexStart:indexEnd]
}

#labels for training and test - turn into vectors
vector_train_y <- unlist(data_train_y)
vector_test_y <- unlist(data_test_y)

## remove some the old data structure variables, to preserve space
rm(data_train_x)
rm(data_train_y)
rm(data_test_x)
rm(data_test_y)


## Testing image - m = x data, v = y labels, and x is which column in m that should be plottet
TestImage <- function(m,v,x)
{
  MatrixTest <- matrix(m[,x], nrow =digitRow, ncol = digitCol, byrow = TRUE)
  M <- matrix(0,nrow = digitRow,ncol = digitCol, byrow = TRUE)
  for (i in 1:digitCol) {
    M[,i] <- MatrixTest[,(digitCol+1)-i]
  }
  image(1:digitRow,1:digitCol,M,axes = TRUE,col = grey(seq(0, 1, length = 1000)))
  #titel(paste("Digit:",toString(v[x]),sep = ""))
  text(27,27,paste("Digit:",toString(v[x]),sep = ""),col = "white")
}

### K_NearestNeighbour algo
K_NearestNeighbour <- function(TestPoint,x_data,y_data,k, ReturnVector = FALSE, OddOnly = FALSE)
{
  Temp_Matrix <- x_data - TestPoint
  ## Compute every distance.
  VectorDistance <- diag((t(Temp_Matrix)%*%Temp_Matrix))
  ## Bind distances and y labels together in a matrix
  DistanceAndLabels <- cbind(VectorDistance,y_data) ### So when i sort distance, the y labels follows.
  DistanceAndLabels <- DistanceAndLabels[order(VectorDistance),] ## Sort distances based on distance
  K_GetSum <- matrix(1,nrow=length(DistanceAndLabels[,1]),ncol = max(k)) ## Make upper triangular matrix, with only 1's
  K_GetSum[lower.tri(K_GetSum,diag = FALSE)] <- 0 ## Cumulative sum, with upper tri matrix
  Result_Prediction <- DistanceAndLabels[,2]%*%K_GetSum #cumulative sum of all y labels. Take the sign function around.
  Result_Prediction <- t(sign(Result_Prediction)) #applys sign function - and transpose so it looks like a vector
  
  if (ReturnVector == FALSE) { 
    return (Result_Prediction[k,1])  ## Return a single k
  }
  else
  {
    if (OddOnly == FALSE) { 
      return (Result_Prediction[,1]) ## return results for 1:k
    }
    else  
    {
      return (Result_Prediction[c(TRUE,FALSE),1]) ## return results for 1,3,5,..,k
    }
  }
}

## Testing image - m = x data, v = y labels, and x is which column in m that should be plottet
TestImage <- function(m,v,x)
{
  MatrixTest <- matrix(m[,x], nrow =digitRow, ncol = digitCol, byrow = TRUE)
  M <- matrix(0,nrow = digitRow,ncol = digitCol, byrow = TRUE)
  for (i in 1:digitCol) {
    M[,i] <- MatrixTest[,(digitCol+1)-i]
  }
  image(1:digitRow,1:digitCol,M,axes = TRUE,col = grey(seq(0, 1, length = 1000)))
  #titel(paste("Digit:",toString(v[x]),sep = ""))
  text(27,27,paste("Digit:",toString(v[x]),sep = ""),col = "white")
}




## All the above is data handling. Now we can choose digits, and then use NearestNeighbour etc.
## So choose digits and then run code from here

######### Task 1 ############

FirstDigit <- 0
SecondDigit <- 1
# Pick the images of the corresponding digit from MNIST-Train-cropped file for training.
to_train_y <- vector_train_y[vector_train_y == FirstDigit | vector_train_y == SecondDigit]
to_train_x <- matrix_train_x[,which(  vector_train_y == FirstDigit | vector_train_y == SecondDigit)]
# Used instead oflossfunction
to_train_y[to_train_y == FirstDigit] <- -1
to_train_y[to_train_y == SecondDigit] <- 1
# Pick the images of the corresponding digit from MNIST-Train-cropped file for training.Pick the images of the corresponding digit from MNIST-Test-cropped.txt for the test set.
to_test_y <- vector_test_y[vector_test_y==FirstDigit | vector_test_y==SecondDigit]
to_test_x <- matrix_test_x[,which(vector_test_y==FirstDigit | vector_test_y==SecondDigit)]
# Used instead oflossfunction
to_test_y[to_test_y == FirstDigit] <- -1
to_test_y[to_test_y == SecondDigit] <- 1
# Use the first 80% of the training images for training and the last 20% of the images for calculating the validation error.
train_x_set <- to_train_x[,1:floor(0.8*ncol(to_train_x))]
valid_x_set <- to_train_x[,ceiling(0.8*ncol(to_train_x)):ncol(to_train_x)]
train_y_set <- to_train_y[1:floor(0.8*length(to_train_y))]
valid_y_set <- to_train_y[ceiling(0.8*length(to_train_y)):length(to_train_y)]

# TestImage(to_train_x,to_train_y,7) Displays an example of an image 

k <- 33
valid_error <- rep(0,(k/2+1))
weight <- seq(1,k,2)

for (i in 1:length(valid_y_set))
{
  TestPoint <- valid_x_set[,i]
  Output <- K_NearestNeighbour(TestPoint,train_x_set,train_y_set,k,ReturnVector = TRUE, OddOnly = TRUE)
  valid_error <- valid_error + (abs(Output - valid_y_set[i])/2)/length(valid_y_set)
}

test_error <- rep(0,(k/2+1))
for (i in 1:length(to_test_y))
{
  TestPoint <- to_test_x[,i]
  Output <- K_NearestNeighbour(TestPoint,train_x_set,train_y_set,k,ReturnVector = TRUE, OddOnly = TRUE)
  test_error <- test_error + (abs(Output - to_test_y[i])/2)/length(to_test_y)
}

Task1ValidError <- valid_error
Task1TestError <- test_error
# Plot

plot(weight,Task1ValidError,ylab="Error",xlab="K",col="red", pch=16, sub = "Figure 5")
points(weight,Task1TestError, col="green", pch=17)
legend(1, 0.0023, col=c("red","green"), c("Validation","Test"), pch=c(16,17), bty="o", cex=.8)
title("Task 1")


######### Task 2 ############

FirstDigit <- 0
SecondDigit <- 8
# Pick the images of the corresponding digit from MNIST-Train-cropped file for training.
to_train_y <- vector_train_y[vector_train_y == FirstDigit | vector_train_y == SecondDigit]
to_train_x <- matrix_train_x[,which(  vector_train_y == FirstDigit | vector_train_y == SecondDigit)]
# Used instead oflossfunction
to_train_y[to_train_y == FirstDigit] <- -1
to_train_y[to_train_y == SecondDigit] <- 1
# Pick the images of the corresponding digit from MNIST-Train-cropped file for training.Pick the images of the corresponding digit from MNIST-Test-cropped.txt for the test set.
to_test_y <- vector_test_y[vector_test_y==FirstDigit | vector_test_y==SecondDigit]
to_test_x <- matrix_test_x[,which(vector_test_y==FirstDigit | vector_test_y==SecondDigit)]
# Used instead oflossfunction
to_test_y[to_test_y == FirstDigit] <- -1
to_test_y[to_test_y == SecondDigit] <- 1
# Use the first 80% of the training images for training and the last 20% of the images for calculating the validation error.
train_x_set <- to_train_x[,1:floor(0.8*ncol(to_train_x))]
valid_x_set <- to_train_x[,ceiling(0.8*ncol(to_train_x)):ncol(to_train_x)]
train_y_set <- to_train_y[1:floor(0.8*length(to_train_y))]
valid_y_set <- to_train_y[ceiling(0.8*length(to_train_y)):length(to_train_y)]


# TestImage(to_train_x,to_train_y,7) Displays an example of an image 

k <- 33
valid_error <- rep(0,(k/2+1))
weight <- seq(1,k,2)

for (i in 1:length(valid_y_set))
{
  TestPoint <- valid_x_set[,i]
  Output <- K_NearestNeighbour(TestPoint,train_x_set,train_y_set,k,ReturnVector = TRUE, OddOnly = TRUE)
  valid_error <- valid_error + (abs(Output - valid_y_set[i])/2)/length(valid_y_set)
}

test_error <- rep(0,(k/2+1))
for (i in 1:length(to_test_y))
{
  TestPoint <- to_test_x[,i]
  Output <- K_NearestNeighbour(TestPoint,train_x_set,train_y_set,k,ReturnVector = TRUE, OddOnly = TRUE)
  test_error <- test_error + (abs(Output - to_test_y[i])/2)/length(to_test_y)
}

Task2ValidError <- valid_error
Task2TestError <- test_error
# Plot

plot(weight,Task2TestError,ylab="Error",xlab="K",col="green", pch=17, ylim = c(0,0.02), sub = "Figure 6")
points(weight,Task2ValidError,col="red", pch=16)
legend(1, 0.019, col=c("red","green"), c("Validation","Test"), pch=c(16,17), bty="o", cex=.8)
title("Task 2")

######### Task 3 ############

FirstDigit <- 5
SecondDigit <- 6
# Pick the images of the corresponding digit from MNIST-Train-cropped file for training.
to_train_y <- vector_train_y[vector_train_y == FirstDigit | vector_train_y == SecondDigit]
to_train_x <- matrix_train_x[,which(  vector_train_y == FirstDigit | vector_train_y == SecondDigit)]
# Used instead oflossfunction
to_train_y[to_train_y == FirstDigit] <- -1
to_train_y[to_train_y == SecondDigit] <- 1
# Pick the images of the corresponding digit from MNIST-Train-cropped file for training.Pick the images of the corresponding digit from MNIST-Test-cropped.txt for the test set.
to_test_y <- vector_test_y[vector_test_y==FirstDigit | vector_test_y==SecondDigit]
to_test_x <- matrix_test_x[,which(vector_test_y==FirstDigit | vector_test_y==SecondDigit)]
# Used instead oflossfunction
to_test_y[to_test_y == FirstDigit] <- -1
to_test_y[to_test_y == SecondDigit] <- 1
# Use the first 80% of the training images for training and the last 20% of the images for calculating the validation error.
train_x_set <- to_train_x[,1:floor(0.8*ncol(to_train_x))]
valid_x_set <- to_train_x[,ceiling(0.8*ncol(to_train_x)):ncol(to_train_x)]
train_y_set <- to_train_y[1:floor(0.8*length(to_train_y))]
valid_y_set <- to_train_y[ceiling(0.8*length(to_train_y)):length(to_train_y)]

# TestImage(to_train_x,to_train_y,7) Displays an example of an image 

k <- 33
valid_error <- rep(0,(k/2+1))
weight <- seq(1,k,2)

for (i in 1:length(valid_y_set))
{
  TestPoint <- valid_x_set[,i]
  Output <- K_NearestNeighbour(TestPoint,train_x_set,train_y_set,k,ReturnVector = TRUE, OddOnly = TRUE)
  valid_error <- valid_error + (abs(Output - valid_y_set[i])/2)/length(valid_y_set)
}

test_error <- rep(0,(k/2+1))
for (i in 1:length(to_test_y))
{
  TestPoint <- to_test_x[,i]
  Output <- K_NearestNeighbour(TestPoint,train_x_set,train_y_set,k,ReturnVector = TRUE, OddOnly = TRUE)
  test_error <- test_error + (abs(Output - to_test_y[i])/2)/length(to_test_y)
}

Task3ValidError <- valid_error
Task3TestError <- test_error
# Plot


plot(weight,Task3ValidError,ylim = c(0,0.06),xlim=c(1,33),ylab="Error",xlab="K",col="green", pch=17, sub = "Figure 7")
legend(1, 0.06, col=c("red","green"), c("Validation","Test"), pch=c(16,17), bty="o", cex=.8)
points(weight,Task3TestError,col="red", pch=16)
title("Task 3")