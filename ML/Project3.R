
## Input data paths here

path <- paste(getwd(),"/data/",sep = "")
path_train <- paste(path,"IrisTrainML.dt.txt",sep="")
path_test <- paste(path,"IrisTestML.dt.txt",sep="")


SteepGradiant <- function(y,x,eta,TimeSteps = 1000, w = rnorm(ncol(x),mean = 0,sd = 0.01)) 
{
  if(!is.vector(y) && !is.matrix(x) && length(y) != nrow(x)) {return (NA)}
  ColDim <- ncol(x); RowDim <- nrow(x)

  t <-  1
  constant <- -(1/RowDim)
  while (t < TimeSteps)
  {
    gradiant <- rep(0,ColDim)
    for (i in 1:RowDim)
    {
    gradiant <- gradiant + constant*(y[i]*x[i,])/(1+exp(y[i]*(c(t(w)%*%x[i,]))))
    }
    w <- w - eta*gradiant
    t <- t + 1
  }
  return (w)
}



loss_fun <- function(y_predict,y_label) ## 0-1 loss function
{
  if (length(y_predict) != length(y_label)) {return (NA)}
  Return_Vector <- rep(NA,length(y_predict))
  
  for (i in 1:length(y_predict))
  {
    if (y_predict[i] == y_label[i])
    {
      Return_Vector[i] <- 0
    }
    else
    {
      Return_Vector[i] <- 1
    }
  }
  
  return (Return_Vector)
}

theta_fun <- function(x) {return (1/(1+exp(-x)))}
avg <- function(x) {return (sum(x)/length(x))}




## yDgata handling start


DataTrain <- read.table(path_train,header = FALSE)
DataTest <- read.table(path_test,header = FALSE)

DataTrain <- DataTrain[!(DataTrain$V3 == 2),] ## Remove rows with 2's
DataTrain[DataTrain$V3 == 0,3] <- -1 ## replace 0 with -1

DataTest <- DataTest[!(DataTest$V3 == 2),] ## Remove rows with 2's
DataTest[DataTest$V3 == 0,3] <- -1 ## replace 0 with -1

train_y <- DataTrain[,3] ## insert in vector form
train_x <- matrix(1, nrow = nrow(DataTrain), ncol = 3, byrow = TRUE) ## insert to matrix form
train_x[,1] <- DataTrain[,1]
train_x[,2] <- DataTrain[,2]

test_y <- DataTest[,3] ## insert in vector form
test_x <- matrix(1, nrow = nrow(DataTest), ncol = 3, byrow = TRUE) ## insert to matrix form
test_x[,1] <- DataTest[,1]
test_x[,2] <- DataTest[,2]

## Data handling stop
## We have test_x, train_x matrix's
## We have test_y, train_y vector's


LearningRate <- 0.001
w <- SteepGradiant(train_y,train_x,LearningRate, TimeSteps = 10000)


### Try on data
predict_y_train <- rep(NA,nrow(train_x))
for (i in 1:nrow(train_x))
{
temp <- t(w[1:2])%*%train_x[i,1:2] + w[3]
if (sign(temp) == (-1))
{
predict_y_train[i] <- -1
} else
{
  predict_y_train[i] <- 1
}
}
results_train <- loss_fun(predict_y_train,train_y)
plot(results_train, xlab = "Training data index", ylab = "0/1 Loss Frequency",sub = "Figure 4")
abline(h = avg(results_train))
text(20,y = (avg(results_train)+0.02), paste("Average Frequency of guessing wrong:",format(avg(results_train),digits = 2)))
title("Train Data")

## test
predict_y_test <- rep(NA,nrow(test_x))
for (i in 1:nrow(test_x))
{
  temp <- t(w[1:2])%*%test_x[i,1:2] + w[3]
  if (sign(temp) == (-1))
  {
    predict_y_test[i] <- -1
  } else
  {
    predict_y_test[i] <- 1
  }
}
results_test <- loss_fun(predict_y_test,test_y)
plot(results_test, xlab = "Testing data index", ylab = "0/1 Loss Frequency",sub = "Figure 4")
abline(h = avg(results_test))
text(10,y = (avg(results_test)+0.02), paste("Average Frequency of guessing wrong:",format(avg(results_test),digits = 2)))
title("Test Data")




w <- matrix(w,nrow = 1, ncol =3)
colnames(w) <- c("w_1","w_2","b")
w
